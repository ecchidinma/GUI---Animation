/*
 * File:   main.cpp : Defines the entry point for the application.
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on June 07, 2015, 7:42 PM
 */
 
#include "anim01.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Anim01 anim01;
    anim01.setWindowTitle(QObject::tr("Clock"));
    anim01.resize(600, 600);
    anim01.show();

    return app.exec();
}// end main()
