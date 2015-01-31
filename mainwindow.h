#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "matrix.h"

#define LOAD_SIM_DATA
#undef LOAD_SIM_DATA

namespace Ui {
    class MainWindow;
}

class QGraphicsRectItem;
class QFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void redraw();

private slots:
	void on_pushButton_restart_clicked();

private:
	void initSolver();
	Matrix solverStep();
	Matrix solveLQR(Matrix A, Matrix B, Matrix Q, Matrix R);
	Matrix C_mat(Matrix q, Matrix qp);
	Matrix G_vec(Matrix q);
	Matrix M_inv(Matrix q);

	QList<double> toDouble(QString str);



    Ui::MainWindow *ui;

#ifdef LOAD_SIM_DATA
	QFile *dataFile;
#endif

	double xPosCursor;
	bool activeMouse;
	QGraphicsScene *scene;
	QGraphicsRectItem* wagen1;
	QGraphicsRectItem* pendel2;
	QGraphicsRectItem* pendel3;
	QGraphicsRectItem* pendel4;
	QGraphicsTextItem* timeLabel;
	QGraphicsTextItem* qLabel;
	QGraphicsTextItem* qpLabel;
	QGraphicsTextItem* matInstancesLabel;


	QTimer *timer;
	double dtFrames;

	double dt;
	long int steps;
	double tSim;

	double m1;
	double m2;
	double m3;
	double m4;
	double J2;
	double J3;
	double J4;
	double s2;
	double s3;
	double s4;
	double l1;
	double l2;
	double l3;
	double l4;
	double g;
	double M; // Massstab Pixel pro Meter

	int N; // Anzahl der Minimalkoordinaten
	Matrix q_k, qp_k, Q;
    Matrix Q_m; // motor forces
    Matrix q0, q,qp,qpp;
	Matrix dampings, springs;
	Matrix qRef, regulator;
};

#endif // MAINWINDOW_H
