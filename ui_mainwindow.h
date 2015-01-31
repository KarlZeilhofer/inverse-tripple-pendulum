/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_lengths;
    QLabel *label_2;
    QLineEdit *lineEdit_masses;
    QLabel *label_4;
    QLabel *label_3;
    QLineEdit *lineEdit_inertia;
    QLabel *label_5;
    QLineEdit *lineEdit_springs;
    QLabel *label_6;
    QLineEdit *lineEdit_dampings;
    QLabel *label_7;
    QLineEdit *lineEdit_q0;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkBox;
    QPushButton *pushButton_restart;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_8;
    QLineEdit *lineEdit_timefactor;
    QLabel *label_9;
    QLineEdit *lineEdit_kMouse;
    QLabel *label_10;
    QLineEdit *lineEdit_motorForces;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBox_regulatorOn;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1333, 679);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit_lengths = new QLineEdit(centralWidget);
        lineEdit_lengths->setObjectName(QString::fromUtf8("lineEdit_lengths"));

        horizontalLayout->addWidget(lineEdit_lengths);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit_masses = new QLineEdit(centralWidget);
        lineEdit_masses->setObjectName(QString::fromUtf8("lineEdit_masses"));

        horizontalLayout->addWidget(lineEdit_masses);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        lineEdit_inertia = new QLineEdit(centralWidget);
        lineEdit_inertia->setObjectName(QString::fromUtf8("lineEdit_inertia"));

        horizontalLayout->addWidget(lineEdit_inertia);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout->addWidget(label_5);

        lineEdit_springs = new QLineEdit(centralWidget);
        lineEdit_springs->setObjectName(QString::fromUtf8("lineEdit_springs"));

        horizontalLayout->addWidget(lineEdit_springs);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        lineEdit_dampings = new QLineEdit(centralWidget);
        lineEdit_dampings->setObjectName(QString::fromUtf8("lineEdit_dampings"));

        horizontalLayout->addWidget(lineEdit_dampings);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout->addWidget(label_7);

        lineEdit_q0 = new QLineEdit(centralWidget);
        lineEdit_q0->setObjectName(QString::fromUtf8("lineEdit_q0"));

        horizontalLayout->addWidget(lineEdit_q0);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        horizontalLayout->addWidget(checkBox);

        pushButton_restart = new QPushButton(centralWidget);
        pushButton_restart->setObjectName(QString::fromUtf8("pushButton_restart"));

        horizontalLayout->addWidget(pushButton_restart);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_2->addWidget(label_8);

        lineEdit_timefactor = new QLineEdit(centralWidget);
        lineEdit_timefactor->setObjectName(QString::fromUtf8("lineEdit_timefactor"));

        horizontalLayout_2->addWidget(lineEdit_timefactor);

        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_2->addWidget(label_9);

        lineEdit_kMouse = new QLineEdit(centralWidget);
        lineEdit_kMouse->setObjectName(QString::fromUtf8("lineEdit_kMouse"));

        horizontalLayout_2->addWidget(lineEdit_kMouse);

        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_2->addWidget(label_10);

        lineEdit_motorForces = new QLineEdit(centralWidget);
        lineEdit_motorForces->setObjectName(QString::fromUtf8("lineEdit_motorForces"));

        horizontalLayout_2->addWidget(lineEdit_motorForces);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        checkBox_regulatorOn = new QCheckBox(centralWidget);
        checkBox_regulatorOn->setObjectName(QString::fromUtf8("checkBox_regulatorOn"));

        horizontalLayout_2->addWidget(checkBox_regulatorOn);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1333, 18));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "L in m:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_lengths->setToolTip(QApplication::translate("MainWindow", "Lengths", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_lengths->setText(QApplication::translate("MainWindow", "1,1,1,1", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "m in kg:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_masses->setToolTip(QApplication::translate("MainWindow", "Masses", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_masses->setText(QApplication::translate("MainWindow", "1,1,1,1", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "J in kg m^2/s:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        lineEdit_inertia->setToolTip(QApplication::translate("MainWindow", "Torque of Intertia", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_inertia->setText(QApplication::translate("MainWindow", "0,1,1,1", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "k in N/m:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_springs->setToolTip(QApplication::translate("MainWindow", "Springs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_springs->setText(QApplication::translate("MainWindow", "10,0,0,0", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "d in Ns/m:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_dampings->setToolTip(QApplication::translate("MainWindow", "Dampings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_dampings->setText(QApplication::translate("MainWindow", "1,1,1,1", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "q0 in m or \302\260:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_q0->setToolTip(QApplication::translate("MainWindow", "Initial Coordinates", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_q0->setText(QApplication::translate("MainWindow", "0,181,180,180", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "Pull by Mouse", 0, QApplication::UnicodeUTF8));
        pushButton_restart->setText(QApplication::translate("MainWindow", "Restart", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Timefactor:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_timefactor->setToolTip(QApplication::translate("MainWindow", "1 = Realtime, 5 = 5 times slow motion", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_timefactor->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "k_mouse in N/m:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_kMouse->setToolTip(QApplication::translate("MainWindow", "Spring between Cursor and Cart", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_kMouse->setText(QApplication::translate("MainWindow", "5", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "M in N or Nm:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_motorForces->setToolTip(QApplication::translate("MainWindow", "active Forces or Momentum", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_motorForces->setText(QApplication::translate("MainWindow", "0,0,0,0", 0, QApplication::UnicodeUTF8));
        checkBox_regulatorOn->setText(QApplication::translate("MainWindow", "Regulator ON", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
