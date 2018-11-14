#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matrix.h"
#include <string.h> // wegen memcpy

/*
  Karl Zeilhofer
  14.5.2011
  Animation bzw. Simulation eines Dreifachpendels mit Wagen

  16.6. Simulation funktioniert noch nicht.
	Division durch Null in Fritzs Gauss-Algorithmus

	25.6.2011
	Simulation läuft bereits recht gut.
	Nun geht es noch darum, einen Zustandsregler zu implementieren.
	Dieser benötigt die Lösung der Ricatti-Gleichung
  */

#include <QGraphicsRectItem>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	scene = new QGraphicsScene(this);

	N = 4; // zahl der minimal koordinaten
	initSolver();
	activeMouse = false;

	ui->graphicsView->setScene(scene);
	ui->graphicsView->setRenderHint(QPainter::Antialiasing);
	// Cross Hair
	scene->addLine(-100, 0, 100, 0);
	scene->addLine(0, -100, 0, 100);
	timeLabel = scene->addText("t = 0.0 s", QFont("Arial", 24));
	timeLabel->setPos(-400, -200);

	qLabel = scene->addText("q = [0,0,0,0]", QFont("Arial", 24));
	qLabel->setPos(-400, -150);

	qpLabel = scene->addText("qp = [0,0,0,0]", QFont("Arial", 24));
	qpLabel->setPos(-400, 100);

	matInstancesLabel = scene->addText("", QFont("Arial", 24));
	matInstancesLabel->setPos(-400, -300);

#ifdef LOAD_SIM_DATA
	QFileInfo *info = new QFileInfo("D:\\Karl\\Mechanik\\Inverses Pendel\\matlab\\Dreifachpendel\\simData.txt");
	if(!info->exists())
	{
		QMessageBox::information(this, "Info", "File doesn't exitst");
	}
	if(!info->isReadable())
	{
		QMessageBox::information(this, "Info", "File isn't' readable");
	}

	dataFile = new QFile(info->absoluteFilePath());

	if(!dataFile->open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, "Error", "Cannot open file");
	}
	QByteArray line = dataFile->readLine(1000);
	QList<QByteArray> numberList = line.split(' ');
	if(numberList.size() == 14)
	{
		m1 = numberList.at(0).toDouble();
		m2 = numberList.at(1).toDouble();
		m3 = numberList.at(2).toDouble();
		m4 = numberList.at(3).toDouble();
		J2 = numberList.at(4).toDouble();
		J3 = numberList.at(5).toDouble();
		J4 = numberList.at(6).toDouble();
		s2 = numberList.at(7).toDouble();
		s3 = numberList.at(8).toDouble();
		s4 = numberList.at(9).toDouble();
		l2 = numberList.at(10).toDouble();
		l3 = numberList.at(11).toDouble();
		l4 = numberList.at(12).toDouble();
		g  = numberList.at(13).toDouble();
	}
	else
	{
		QMessageBox::warning(this, "Error", "Error on reading simulation constants");
	}
#endif



	ui->pushButton_restart->animateClick(0);

	l1 = 1;
	QPen pen;
	pen.setWidth(0);
	wagen1 = scene->addRect(-l1/2, -l1/10, l1, l1/5, pen, QBrush(Qt::black));
	wagen1->setTransform(QTransform().scale(M,M));
	pendel2 = scene->addRect(-l2/10, -l2/2, l2/5, l2, pen, QBrush(Qt::darkBlue));
	pendel2->setTransform(QTransform().scale(M,M));
	pendel3 = scene->addRect(-l3/10, -l3/2, l3/5, l3, pen, QBrush(Qt::darkGreen));
	pendel3->setTransform(QTransform().scale(M,M));
	pendel4 = scene->addRect(-l4/10, -l4/2, l4/5, l4, pen, QBrush(Qt::darkRed));
	pendel4->setTransform(QTransform().scale(M,M));

	timer = new QTimer();
	dtFrames = 1.0/50.0;
	timer->setInterval(dtFrames*1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(redraw()));

	timer->start();
	showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initSolver()
{
	// Definition der 4 Körper
	// Bildschirmgröße: 1200x900
	M = 130; // Maßstab

    q0 = Matrix(N,1); // initial coordinates (springs are fixed to this position)
	q_k = Matrix(N,1);
	qp_k = Matrix(N,1);
	Q = Matrix(N,1);
    Q_m = Matrix(N,1); // motor forces, constant during simulation. Set on restart.


	// Anfangswerte:

	q_k(0,0)=0;
	q_k(1,0)=M_PI*1;
	q_k(2,0)=M_PI*1;
	q_k(3,0)=M_PI*1;
	q=q_k;
    q0=q;

	qp_k(0,0)=0;
	qp_k(1,0)=0;
	qp_k(2,0)=0;
	qp_k(3,0)=0;
	qp=qp_k;

	Q.setTo(0);
    Q_m.setTo(0);

	N = 4; // Anzahl der Minimalkoordinaten

	dampings = Matrix(N,1);
	springs = Matrix(N,1);
	qRef = Matrix(N,1);
	regulator = Matrix(1,N);

	g = 9.81; // Gravitation

	dt = 0.001; // Simulations-Zeitschritt
	steps = 0; // Simulations Schritte
	tSim = 0; // Simulationszeit
	xPosCursor = 0; // Maus Cursor Position relativ zum Ursprung und mit M skaliert
}

void MainWindow::redraw()
{
	static double t = 0;
	double q1;
	double q2;
	double q3;
	double q4;

#ifdef LOAD_SIM_DATA
	volatile double tSim;
	QByteArray line = dataFile->readLine(1000);
	if(line.isEmpty()){
		dataFile->close();
		dataFile->open(QIODevice::ReadOnly);
		dataFile->readLine(1000);
		//timer->stop();
		return;
	}
	QList<QByteArray> numberList = line.split(' ');

	if(numberList.size() == 5)
	{
		q1 = numberList.at(0).toDouble();
		q2 = numberList.at(1).toDouble();
		q3 = numberList.at(2).toDouble();
		q4 = numberList.at(3).toDouble();
		tSim = numberList.at(4).toDouble();
	}else{
		QMessageBox::warning(this, "Error", "Error on reading Sim Data");

		//timer->stop();
	}
	static bool init=false;

#else

	static Matrix q;
	QPointF sceneP = ui->graphicsView->mapToScene(ui->graphicsView->cursor().pos());
	xPosCursor = sceneP.x()/M;
	double yPos = sceneP.y()/M - 1; // woher die verschiebung um 2 kommt, weiß ich nicht...

	if(!activeMouse)
	{
		if(fabs(xPosCursor) < 0.1)
		{
			activeMouse=true;
		}
		else{
			xPosCursor = 0;
		}
	}else{
		if(fabs(xPosCursor) > 50)
		{
			xPosCursor=0;
			activeMouse = false;
		}
	}

	if(fabs(yPos)>1.0){
		activeMouse = false;
		xPosCursor=0;
	}

	double timefactor = ui->lineEdit_timefactor->text().toDouble();

	if(fabs(timefactor)<0.01 || fabs(timefactor)>100)
		timefactor=1;

	for(int i=0; i<dtFrames/dt/timefactor; i++)
	{
		q = solverStep();
	}
	q1 = q(0);
	q2 = q(1);
	q3 = q(2);
	q4 = q(3);
#endif

	timeLabel->setPlainText("t = " + QString::number(steps*dt, 'g', 5) + "s");
	qLabel->setPlainText("q = \n" + QString::number(q1) + "\n" + QString::number(q2) + "\n" +
						 QString::number(q3) + "\n" + QString::number(q4));
	qpLabel->setPlainText("dq/dt = \n" + QString::number(qp_k(0,0)) + "\n" + QString::number(qp_k(1,0)) + "\n" +
						 QString::number(qp_k(2,0)) + "\n" + QString::number(qp_k(3,0)));

    matInstancesLabel->setPlainText("matInstances = " + QString::number(q.getActiveInstances()));
    //matInstancesLabel->setPlainText("matInstances = " + QString::number(yPos));

	wagen1->setPos(M*q1,0);
	pendel2->setPos(M*(sin(q2)*l2/2+q1),-M*(-cos(q2)*l2/2));
	pendel2->setRotation(-q2*180/M_PI);
	pendel3->setPos(M*(sin(q3)*l3/2+l2*sin(q2)+q1),-M*(-cos(q3)*l3/2-l2*cos(q2)));
	pendel3->setRotation(-q3*180/M_PI);
	pendel4->setPos(M*(sin(q4)*l4/2+l3*sin(q3)+l2*sin(q2)+q1),-M*(-cos(q4)*l4/2-l3*cos(q3)-l2*cos(q2)));
	pendel4->setRotation(-q4*180/M_PI);

	t+=dtFrames;
}


Matrix MainWindow::solverStep()
{

	qRef(0)=0;qRef(1)=M_PI;qRef(2)=M_PI;qRef(3)=M_PI;
	regulator(0)=0; regulator(1)=2; regulator(2)=-2; regulator(3)=2;

    // Eingeprägte Kräfte
    Q = Q_m; // set to motor forces
    //Q.setTo(0);

    //Q(0) = Q(0) - 10*q(0); // feder vom Wagen zum Ursprung
	//Q = Q -qp*d;
	// Q(1) = Q(1) + 20*cos(3*tSim); // Motor im ersten gelenk



    // auxilary vector, descriping relative rotation in the joints, to calculate joint-dampings, and springs:
    // qp descripes rotation absolute rotation speed
    Matrix delta_qp(N,1);
    delta_qp(0) = qp(0);
    delta_qp(1) = qp(1)-0;
    delta_qp(2) = qp(2)-qp(1);
    delta_qp(3) = qp(3)-qp(2);

    // q describes absolute angle/position
    Matrix delta_q(N,1);
    delta_q(0) = q(0);
    delta_q(1) = (q(1)-0)-(q0(1)-0); // fixed to initial angle of S1
    delta_q(2) = (q(2)-q(1))-(q0(2)-q0(1)); // fixed to initial angle of S1-S2
    delta_q(3) = (q(3)-q(2))-(q0(3)-q0(2)); // fixed to initial angle of S2-S3


    Q = Q - Matrix::dotMul(delta_qp,dampings);
    Q = Q - Matrix::dotMul(delta_q, springs);

	if(ui->checkBox->isChecked()){
		Q(0) = Q(0) + (xPosCursor - q(0))*10000  + -qp(0)*100; // steuerung per Maus mit Feder zum Cursor und Dämpfer gegen den Ursprung
	}else if(ui->checkBox_regulatorOn->isChecked()){
		Q(0) = -100*(regulator*(q-qRef))(0); // proportional regulator
	}

	// EULER-Cauchy Polygonzug:
//	qpp = M_inv(q)*(Q - C_mat(q,qp)*qp - G_vec(q));
//	qp = qp + qpp*dt; // integrieren mit explizitem Eulerverfahren
//	q = q + qp*dt; // integrieren mit explizitem Eulerverfahren


	// Runge-Verfahren
	static Matrix qpp_k;
	static Matrix q_kp1, qp_kp1, qpp_kp1;
	static Matrix K1_q, K1_qp, K2_q, K2_qp;
	qpp_k = M_inv(q)*(Q - C_mat(q,qp)*qp - G_vec(q));
	qp_k = qp;
	q_k = q;
	K1_q = qp_k; // Steigung aus aktueller Ableitung
	K1_qp = qpp_k; // Steigung aus aktueller Ableitung
	qp_kp1 = qp_k + dt*K1_qp; // vorläufige Schätzung für qp(k+1)
	q_kp1 = q_k + dt*K1_q; // vorläufige Schätzung für q(k+1)
	qpp_kp1 = M_inv(q_kp1)*(Q - C_mat(q_kp1,qp_kp1)*qp_kp1 - G_vec(q_kp1)); // vorläufige Schätzung für qpp(k+1)
	K2_qp = qpp_kp1; // Steigung aus Ableitungsschätzung qpp(k+1)
	K2_q = qp_kp1; // Steigung aus Ableitungsschätzung qp(k+1)
	qp_kp1 = qp_k + dt*1/2*(K1_qp+K2_qp); // endgültiger Wert für qp(k+1)
	q_kp1 = q_k + dt*1/2*(K1_q+K2_q); // endgültiger Wert für q(k+1)

	q = q_kp1;
	qp = qp_kp1;
	qpp = qpp_kp1;

	steps++;
	tSim = steps*dt;

	qp_k = qp;
	return q_k = q;
}


Matrix MainWindow::C_mat(Matrix q, Matrix qp)
{
//	C =[0,(-l2*m4-l2*m3)*sin(q2)*q2p,-m3*sin(q3)*q3p*s3-l3*m4*sin(q3)*q3p,-m4*sin(q4)*q4p*s4;...
//	    0,0,(l2*m3*sin(q2)*cos(q3)-l2*m3*cos(q2)*sin(q3))*q3p*s3+(l2*l3*m4*sin(q2)*cos(q3)-l2*l3*m4*cos(q2)*sin(q3))*q3p,(l2*m4*sin(q2)*cos(q4)-l2*m4*cos(q2)*sin(q4))*q4p*s4;...
//	    0,(l2*m3*cos(q2)*q2p*sin(q3)-l2*m3*sin(q2)*q2p*cos(q3))*s3+l2*l3*m4*cos(q2)*q2p*sin(q3)-l2*l3*m4*sin(q2)*q2p*cos(q3),0,(l3*m4*sin(q3)*cos(q4)-l3*m4*cos(q3)*sin(q4))*q4p*s4;...
//	    0,(l2*m4*cos(q2)*q2p*sin(q4)-l2*m4*sin(q2)*q2p*cos(q4))*s4,(l3*m4*cos(q3)*q3p*sin(q4)-l3*m4*sin(q3)*q3p*cos(q4))*s4,0];

	static Matrix C(N,N);

//	double q1 = q(0);
	double q2 = q(1);
	double q3 = q(2);
	double q4 = q(3);

//	double q1p = qp(0);
	double q2p = qp(1);
	double q3p = qp(2);
	double q4p = qp(3);


	C(0,0) = 0;
	C(0,1) = (-l2*m4-l2*m3)*sin(q2)*q2p;
	C(0,2) = -m3*sin(q3)*q3p*s3-l3*m4*sin(q3)*q3p;
	C(0,3) = -m4*sin(q4)*q4p*s4;

	C(1,0) = 0;
	C(1,1) = 0;
	C(1,2) = (l2*m3*sin(q2)*cos(q3)-l2*m3*cos(q2)*sin(q3))*q3p*s3+(l2*l3*m4*sin(q2)*cos(q3)-l2*l3*m4*cos(q2)*sin(q3))*q3p;
	C(1,3) = (l2*m4*sin(q2)*cos(q4)-l2*m4*cos(q2)*sin(q4))*q4p*s4;

	C(2,0) = 0;
	C(2,1) = (l2*m3*cos(q2)*q2p*sin(q3)-l2*m3*sin(q2)*q2p*cos(q3))*s3+l2*l3*m4*cos(q2)*q2p*sin(q3)-l2*l3*m4*sin(q2)*q2p*cos(q3);
	C(2,2) = 0;
	C(2,3) = (l3*m4*sin(q3)*cos(q4)-l3*m4*cos(q3)*sin(q4))*q4p*s4;

	C(3,0) = 0;
	C(3,1) = (l2*m4*cos(q2)*q2p*sin(q4)-l2*m4*sin(q2)*q2p*cos(q4))*s4;
	C(3,2) = (l3*m4*cos(q3)*q3p*sin(q4)-l3*m4*sin(q3)*q3p*cos(q4))*s4;
	C(3,3) = 0;

	return C;
}

Matrix MainWindow::G_vec(Matrix q)
{
//	G = [0;...
//	    g*m2*sin(q2)*s2+(g*l2*m4+g*l2*m3)*sin(q2);...
//	    g*m3*sin(q3)*s3+g*l3*m4*sin(q3);...
//	    g*m4*sin(q4)*s4];

	static Matrix G(N,1);

//	double q1 = q(0);
	double q2 = q(1);
	double q3 = q(2);
	double q4 = q(3);

	G(0) = 0;
	G(1) = g*m2*sin(q2)*s2+(g*l2*m4+g*l2*m3)*sin(q2);
	G(2) = g*m3*sin(q3)*s3+g*l3*m4*sin(q3);
	G(3) = g*m4*sin(q4)*s4;

	return G;
}


Matrix MainWindow::M_inv(Matrix q)
{
//	M = [m4+m3+m1,(l2*m4+l2*m3)*cos(q2),m3*cos(q3)*s3+l3*m4*cos(q3),m4*cos(q4)*s4;...
//	    (l2*m4+l2*m3)*cos(q2),(l2*l2*m4+l2*l2*m3)*sin(q2)*sin(q2)+(l2*l2*m4+l2*l2*m3)*cos(q2)*cos(q2),(l2*m3*sin(q2)*sin(q3)+l2*m3*cos(q2)*cos(q3))*s3+l2*l3*m4*sin(q2)*sin(q3)+l2*l3*m4*cos(q2)*cos(q3),(l2*m4*sin(q2)*sin(q4)+l2*m4*cos(q2)*cos(q4))*s4;...
//	    m3*cos(q3)*s3+l3*m4*cos(q3),(l2*m3*sin(q2)*sin(q3)+l2*m3*cos(q2)*cos(q3))*s3+l2*l3*m4*sin(q2)*sin(q3)+l2*l3*m4*cos(q2)*cos(q3),J3+(m3*sin(q3)*sin(q3)+m3*cos(q3)*cos(q3))*s3*s3+l3*l3*m4*sin(q3)*sin(q3)+l3*l3*m4*cos(q3)*cos(q3),(l3*m4*sin(q3)*sin(q4)+l3*m4*cos(q3)*cos(q4))*s4;...
//	    m4*cos(q4)*s4,(l2*m4*sin(q2)*sin(q4)+l2*m4*cos(q2)*cos(q4))*s4,(l3*m4*sin(q3)*sin(q4)+l3*m4*cos(q3)*cos(q4))*s4,J4+(m4*sin(q4)*sin(q4)+m4*cos(q4)*cos(q4))*s4*s4];

	Matrix M(N,N);

//	double q1 = q(0);
	double q2 = q(1);
	double q3 = q(2);
	double q4 = q(3);

	M(0,0) = m4+m3+m1;
	M(0,1) = (l2*m4+l2*m3)*cos(q2);
	M(0,2) = m3*cos(q3)*s3+l3*m4*cos(q3);
	M(0,3) = m4*cos(q4)*s4;

	M(1,0) = (l2*m4+l2*m3)*cos(q2);
	M(1,1) = (l2*l2*m4+l2*l2*m3)*sin(q2)*sin(q2)+(l2*l2*m4+l2*l2*m3)*cos(q2)*cos(q2);
	M(1,2) = (l2*m3*sin(q2)*sin(q3)+l2*m3*cos(q2)*cos(q3))*s3+l2*l3*m4*sin(q2)*sin(q3)+l2*l3*m4*cos(q2)*cos(q3);
	M(1,3) = (l2*m4*sin(q2)*sin(q4)+l2*m4*cos(q2)*cos(q4))*s4;

	M(2,0) = m3*cos(q3)*s3+l3*m4*cos(q3);
	M(2,1) = (l2*m3*sin(q2)*sin(q3)+l2*m3*cos(q2)*cos(q3))*s3+l2*l3*m4*sin(q2)*sin(q3)+l2*l3*m4*cos(q2)*cos(q3);
	M(2,2) = J3+(m3*sin(q3)*sin(q3)+m3*cos(q3)*cos(q3))*s3*s3+l3*l3*m4*sin(q3)*sin(q3)+l3*l3*m4*cos(q3)*cos(q3);
	M(2,3) = (l3*m4*sin(q3)*sin(q4)+l3*m4*cos(q3)*cos(q4))*s4;

	M(3,0) = m4*cos(q4)*s4;
	M(3,1) = (l2*m4*sin(q2)*sin(q4)+l2*m4*cos(q2)*cos(q4))*s4;
	M(3,2) = (l3*m4*sin(q3)*sin(q4)+l3*m4*cos(q3)*cos(q4))*s4;
	M(3,3) = J4+(m4*sin(q4)*sin(q4)+m4*cos(q4)*cos(q4))*s4*s4;

//	qDebug() << "M: \n" << M.toString().toAscii();

	return M.inv();
}














void MainWindow::on_pushButton_restart_clicked()
{
	QList<double> d = toDouble(ui->lineEdit_q0->text());
    q0(0)=d.at(0);
    q0(1)=d.at(1)/180.0*M_PI;
    q0(2)=d.at(2)/180.0*M_PI;
    q0(3)=d.at(3)/180.0*M_PI;
    q_k = q = q0;

    d = toDouble(ui->lineEdit_motorForces->text());
    Q_m(0)=d.at(0);
    Q_m(1)=d.at(1);
    Q_m(2)=d.at(2);
    Q_m(3)=d.at(3);

	qp.setTo(0);

	d = toDouble(ui->lineEdit_lengths->text());
	l1 = d.at(0);
	l2 = d.at(1);
	l3 = d.at(2);
	l4 = d.at(3);

	s2 = 0.5*l2;
	s3 = 0.5*l3;
	s4 = 0.5*l4;


	wagen1->setRect(-l1/2, -l1/10, l1, l1/5);
	pendel2->setRect(-l2/10, -l2/2, l2/5, l2);
	pendel3->setRect(-l3/10, -l3/2, l3/5, l3);
	pendel4->setRect(-l4/10, -l4/2, l4/5, l4);

	d = toDouble(ui->lineEdit_masses->text());
	m1 = d.at(0);
	m2 = d.at(1);
	m3 = d.at(2);
	m4 = d.at(3);

	d = toDouble(ui->lineEdit_inertia->text());
	J2 = d.at(1);
	J3 = d.at(2);
	J4 = d.at(3);

	d = toDouble(ui->lineEdit_dampings->text());
	dampings(0) = d.at(0);
	dampings(1) = d.at(1);
	dampings(2) = d.at(2);
	dampings(3) = d.at(3);

	d = toDouble(ui->lineEdit_springs->text());
	springs(0) = d.at(0);
	springs(1) = d.at(1);
	springs(2) = d.at(2);
	springs(3) = d.at(3);

	steps = 0; // Simulations Schritte
	tSim = 0; // Simulationszeit
	activeMouse = false;
}


QList<double> MainWindow::toDouble(QString str)
{
	QStringList strList = str.split(QRegExp(","));
	QList<double> dList;

	for(int i=0; i<strList.length(); i++){
		dList.append(strList.at(i).toDouble());
	}

	return dList;
}

// TODO: fertig machen und testen!
Matrix MainWindow::solveLQR(Matrix A, Matrix B, Matrix Q, Matrix R)
{
//	const double DELTA = 0.01;
//	int N = A.rows();
//	int m = B.columns();

//	Matrix P1 = Matrix::ident(N); // Startwert

//	Matrix V1 = A.T()*P + P*A - P*B*R.inv()*B.T()*P + Q; // Ricatti Gleichung
//	Matrix P2 = P1*(1 + DELTA);

//	for(int k=0; k<100; k++){
//		Matrix V2 = A.T()*P + P*A - P*B*R.inv()*B.T()*P + Q;
//		Matrix deltaV_vec = (V2-V1).toColumnVector();
//		Matrix deltaP_vec = (P2-P1).toColumnVector();

//		Matrix J(N*N, N*N);
//		for(int i=0; i<N*N; i++){
//			for(int j=0; j<N*N; j++){
//				J(i,j) = deltaV_vec(i)/deltaP_vec(j);
//			}
//		}

//		V1 = V2;
//		P1 = P2;
//		P2 = (P2.toColumnVector() - J.inv()*V2.toColumnVector()).toMatrix(N,N);
//	}

//	return P;
}

