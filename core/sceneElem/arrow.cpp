/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "arrow.h"

#include <math.h>

#include <QPen>
#include <QPainter>
#include <QDebug>

const qreal Pi = 3.14;

//! [0]
Arrow::Arrow(MyTable *startItem, int StartItemElemNum, MyTable *endItem, int endItemElemNum, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setZValue(1);
    myStartItem = startItem;
    myEndItem = endItem;
    myStartItemElemNum = StartItemElemNum;
    myEndItemElemNum = endItemElemNum;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    updatePosition();
}
//! [0]

//! [1]
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
    const qreal scrollAreaWidthOffset = 30.0;

    QPointF pointStart = myStartItem->pos(), pointEnd = myEndItem->pos();
    pointStart.setY(pointStart.y()+ 10 + 17*myStartItemElemNum);
    pointEnd.setY(pointEnd.y()+ 10 + 17*myEndItemElemNum);

    qreal endOfStartItemX = pointStart.x()+myStartItem->size().width();
    qreal endOfEndItemX = pointEnd.x()+myEndItem->size().width();
    if (endOfStartItemX < pointEnd.x() ) {
        pointStart.rx() += myStartItem->size().width() - scrollAreaWidthOffset;
    } else if (endOfEndItemX < pointStart.x()){
        pointEnd.rx() += myEndItem->size().width() - scrollAreaWidthOffset;
    }


    QLineF line(pointStart, pointEnd);
    setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
//    if (myStartItem->collidesWithItem(myEndItem))
//        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 7;
    painter->setPen(myPen);
    painter->setBrush(myColor);
//! [4] //! [5]

//    QLineF centerLine(myStartItem->pos(), myEndItem->pos());
//    QPolygonF endPolygon = myEndItem->polygon();
//    QPointF p1 = endPolygon.first() + myEndItem->pos();
//    QPointF p2;
//    QPointF intersectPoint;
//    QLineF polyLine;
//    for (int i = 1; i < endPolygon.count(); ++i) {
//    p2 = endPolygon.at(i) + myEndItem->pos();
//    polyLine = QLineF(p1, p2);
//    QLineF::IntersectType intersectType =
//        polyLine.intersect(centerLine, &intersectPoint);
//    if (intersectType == QLineF::BoundedIntersection)
//        break;
//        p1 = p2;
//    }

//    setLine(QLineF(intersectPoint, myStartItem->pos()));
////! [5] //! [6]

    QLineF reversedLine(line().p2(), line().p1());
    double angle = ::acos(reversedLine.dx() / reversedLine.length());
    if (reversedLine.dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = reversedLine.p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                        cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = reversedLine.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                        cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << reversedLine.p1() << arrowP1 << arrowP2;
////! [6] //! [7]
//        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
//        if (isSelected()) {
//            painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        painter->drawLine(myLine);
        QRectF myRect( QPointF ( myLine.p1().x() - 3, myLine.p1().y() - 3 ), QSizeF(6, 6));
        painter->drawEllipse(myRect);

//        myLine.translate(0, 4.0);
//        painter->drawLine(myLine);
//        myLine.translate(0,-8.0);
//        painter->drawLine(myLine);
//    }
}
//! [7]