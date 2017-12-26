/*
 * File:   anim01.h
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on June 07, 2015, 7:42 PM
 */
 
#ifndef ANIM01_H
#define ANIM01_H

#include <QWidget>
#include <QTimer>

class Anim01 : public QWidget
{
    Q_OBJECT

signals:
    void timeout(); // not used in this program

public slots:
    void animate();

public:
    Anim01(QWidget *parent = 0);
    void draw(QPainter *painter);
     ~Anim01();

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool isPM; // is the time PM?
    unsigned int secondCount; // ranges from 0 to 59
    unsigned int minuteCount; // ranges from 0 to 59
    unsigned int hourCount; // ranges from 0 to 11
    unsigned int numberOf6minutes; // ranges from 0 to 9 and represents the number of 6 minutes within an hour
    QTimer* updateTimer;

    float numToAngle(unsigned int number, bool bInDegrees);

}; // end class Anim01 

#endif // ANIM01_H
