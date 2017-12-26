/*
 * File:   anim01.cpp
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on June 07, 2015, 7:42 PM
 */
 
#include <QtGui>
#include <sstream>
#include <iomanip>
#include "anim01.h"

const double PI = 3.14159265789;
const unsigned int N0 = 7;     //number of different vertices in arrow
const int NumClockSpacings = 60;
const double DegreesPerSpacing = 360/NumClockSpacings; // (360/60)
const int UpdateInterval = 1; // in seconds

QPoint secondHandBeginPt(1, 0); QPoint secondHandEndPt(40, 0);

int arrowMinuteHandCoords[N0][2] = { // in anti-clockwise order
   {2, -2}, {2, 2}, {25, 2}, {25, 4}, {30, 0}, {25, -4}, {25, -2}
};

int arrowHourHandCoords[N0][2] = { // in anti-clockwise order
    {3, -3}, {3, 3}, {20, 3}, {20, 5}, {25, 0}, {20, -5}, {20, -3}
};

Anim01::Anim01(QWidget *parent): QWidget(parent)
{
    QTime presentTime = QTime::currentTime(); // obtain current time of the day
    isPM = false; // is the time PM?
    secondCount = presentTime.second(); // ranges from 0 to 59
    minuteCount = presentTime.minute(); // ranges from 0 to 59

    hourCount = presentTime.hour(); // hourCount ranges from 0 to 23
    if(hourCount > 11)
    {
        hourCount = hourCount - 12; // hourCount now ranges from 0 to 23
        isPM = true;
    }

    numberOf6minutes = minuteCount/6; // ranges from 0 to 9 and represents the number of 6 minutes within an hour

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(animate())); // call Anim01 widget's update() on time out
    updateTimer->start(UpdateInterval * 1000); // start timer

    QFont font;
    font.setPointSize(8);
    setFont(font);
    //update(); // superfluous since the constructor calls paintEvent() implicitly
}// end constructor

void Anim01::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    int side = qMin(width(), height());

    painter.setViewport((width() - side)/2, (height() - side)/2, side, side);
    painter.setWindow(-50, -50, 100, 100);

    draw(&painter);
}// end paintEvent()

void Anim01::draw(QPainter* painter)
{
    //1. DRAW OUTER CIRCLE
    painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    painter->setBrush(QBrush(Qt::magenta, Qt::SolidPattern));
    painter->drawEllipse(-46, -46, 92, 92); // draw outer circle

    //2. DRAW INNER CIRCLE
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    painter->drawEllipse(-28, -28, 56, 56); // draw inner circle

    //3. DRAW CLOCK WITHOUT HAND (ARROW)
    QPen thickPen(palette().foreground(), 1.5);
    QPen thinPen(palette().foreground(), 0.5);

    for (int i = 0; i < NumClockSpacings; ++i)
    {
        painter->save(); // save transformation matrix M0
        painter->rotate(i*DegreesPerSpacing); // rotate coordinate system -i*DegreesPerSpacing degrees

        if (i % 5 == 0)
        {
            painter->setPen(thickPen);
            painter->drawLine(0, -41, 0, -44);
            painter->drawText(-15, -41, 30, 30, Qt::AlignHCenter|Qt::AlignTop, QString::number(i==0 ? 12 : i/5));
        }
        else
        {
            painter->setPen(thinPen);
            painter->drawLine(0, -42, 0, -44);
        }
        painter->restore(); // restore transformation matrix M0
    }// end for

    //4. DRAW CLOCK SECOND HAND (A LINE)
    painter->save(); // save transformation matrix M0

    float theta01 = numToAngle((secondCount==0 ? 60 : secondCount), true);
    painter->rotate(theta01); // rotate coordinate system -theta01 degrees

    painter->setPen(QPen(Qt::red, 2, Qt::SolidLine));
    painter->drawLine(secondHandBeginPt, secondHandEndPt);

    painter->restore(); //restore transformation matrix M0

    //5. DRAW CLOCK MINUTE HAND (AN ARROW - A RE-ENTRACT POLYGON)
    painter->save(); // save transformation matrix M0

    float theta02 = numToAngle((minuteCount==0 ? 60 : minuteCount), true);
    painter->rotate(theta02); // rotate coordinate system -theta02 degrees

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    painter->drawPolygon(QPolygon(7, arrowMinuteHandCoords[0]));

    painter->restore(); //restore transformation matrix M0

    //6. DRAW CLOCK HOUR HAND (AN ARROW - A RE-ENTRACT POLYGON)
    painter->save(); // save transformation matrix M0

    //float theta03 = numToAngle((hourCount==0 ? 60 : hourCount*5), true) + numberOf12minutes*DegreesPerSpacing;
    float theta03 = numToAngle((hourCount==0 ? 60 : hourCount*5), true) + numberOf6minutes*(DegreesPerSpacing/2);
    painter->rotate(theta03); // rotate coordinate system -theta03 degrees

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter->drawPolygon(QPolygon(7, arrowHourHandCoords[0]));

    painter->restore(); //restore transformation matrix M0

    //7. DRAW THE CENTRAL CIRCLE
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
    painter->drawEllipse(-5, -5, 10, 10); // draw central circle

    //8. DRAW DIGITAL TIME
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
    QRect rect(-45, -45, 20, 4);
    painter->drawRect(rect);

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hourCount << ":" <<
           std::setfill('0') << std::setw(2) << minuteCount << ":" <<
           std::setfill('0') << std::setw(2) << secondCount << ":" <<
           (isPM ? "PM" : "AM")  << std::ends;

     painter->setPen(QPen(Qt::white, 0.5));
     painter->setFont(QFont ("Helvetica [Cronyx]", 2, -1, true)); //family, size, weight, italic=true
     painter->drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, QString((oss.str()).data()));

} // end draw()

// deduce the angles from the +ve x-axis to the clock number moving clockwise
float Anim01::numToAngle(unsigned int number, bool bInDegrees)
{
    float theta = (number <= 14 ? (6*number + 270) : (6*number - 90)); // theta in degrees
    return (bInDegrees ? theta : theta * PI/180); // theta in degrees or radians depending on bInDegrees
}// end numToAngle()

void Anim01::animate() // called every 1 second when updateTimer times out
{
    secondCount++; // ranges from 0 to 59
    if(secondCount > 59)
    {
        secondCount = 0;
        minuteCount++; // ranges from 0 to 59
    }

    if(minuteCount > 59)
    {
        minuteCount = 0;
        hourCount++;  // ranges from 0 to 11
    }

    numberOf6minutes = minuteCount/6; // ranges from 0 to 9 and represents the number of 6 minutes within an hour

    if(hourCount > 11)
    {
        hourCount = 0;
        isPM = isPM ? false : true; // toggle isPM
    }

    update(); // re-display
}// end  animate()

Anim01::~Anim01()
{

}// end destructor
