#include <QApplication>
#include "mainwindow.h"
#include <stdio.h>

int main(int argc, char *argv[])
{


	//add this at the beginning of your main
    freopen("stdout.txt","w",stdout);
    freopen("stderr.txt","w",stderr);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
