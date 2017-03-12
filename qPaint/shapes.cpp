#include "shapes.h"

BaseShape::BaseShape(const QPen &pen, const QBrush &brush, const QString &name, const DrawMode &type):
    pen(pen),
    brush(brush),
    name(name),
    type(type)
{
}

void BaseShape::setName(const QString& value)
{
    name = value;
}

const QPen& BaseShape::getPen() const
{
    return pen;
}

const QBrush& BaseShape::getBrush() const
{
    return brush;
}

ShapeState BaseShape::getShapeState() const
{
    return shapeState;
}

const DrawMode BaseShape::getType() const
{
    return type;
}
const QString& BaseShape::getName() const
{
    return name;
}

//FREEHAND//
Shape_Freehand::Shape_Freehand(const QPen& pen, const QBrush& brush, const QString& name, const QPoint& firstPoint):
    BaseShape(pen, brush, name, Type_Freehand)
{
    points.append(firstPoint);
}
void Shape_Freehand::drawShape(QPainter& painter) const
{
    painter.drawPolyline(points);
}

void Shape_Freehand::drawSelectionMarker(QPainter &painter) const
{
    QPen newPen {this->pen};
    if (pen.width() <= 10)
    {
        newPen.setWidthF(pen.width() + 5);
    }
    else
    {
        newPen.setWidthF(newPen.widthF() * 1.5);
    }
    QColor newColor = Qt::black;
    newColor.setAlpha(150);
    newPen.setStyle(Qt::SolidLine);
    newPen.setColor(newColor);
    painter.setPen(newPen);
    painter.drawPolyline(points);
}

void Shape_Freehand::mousePressEvent(const QMouseEvent* event)
{

}

void Shape_Freehand::mouseMoveEvent(const QMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton)
        points.append(event->pos());
}

void Shape_Freehand::mouseReleaseEvent(const QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        shapeState = finished;
    isMoving = false;
    if (event->button() == Qt::RightButton)
        shapeState = canceled;

}

bool Shape_Freehand::mouseIntersectCheck(const QMouseEvent* event)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (testX(event->pos().x(), points.at(i).x()) && testY(event->pos().y(), points.at(i).y()))
        {
            return true;
        }
    }
    return false;
}

const QPointF Shape_Freehand::getTopLeft() const
{
    qreal lowestX{9999.0};
    qreal lowestY{9999.0};
    for (int i = 0; i < points.size(); i++)
    {
        QPointF currentPoint = points.at(i);
        if (currentPoint.x() < lowestX)
        {
            lowestX = currentPoint.x();
        }
        if (currentPoint.y() < lowestY)
        {
            lowestY = currentPoint.y();
        }
    }
    return QPointF {lowestX, lowestY};
}

void Shape_Freehand::mouseDrag(const QPoint& newPos)
{
    isMoving = true;
    QPointF topLeft = getTopLeft();
    for (int i = 0; i < points.count(); i++)
    {
        QPointF individualPointOffset = points.at(i) - topLeft;
        points[i] = newPos + individualPointOffset;
    }
}

void Shape_Freehand::nudgeUp()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPointF{0, -1};
    }
    //boundingRectangle.translate(0, -1);
}

void Shape_Freehand::nudgeDown()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPointF{0, 1};
    }
    //boundingRectangle.translate(0, 1);
}

void Shape_Freehand::nudgeLeft()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPointF{-1, 0};
    }
    //boundingRectangle.translate(-1, 0);
}

void Shape_Freehand::nudgeRight()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPointF{1, 0};
    }
    //boundingRectangle.translate(1, 0);
}
void Shape_Freehand::addPoint(const QPointF& newPoint)
{
    points.append(newPoint);
    //updateBoundingRectangle();
}

bool Shape_Freehand::testX(const int mousePosX, const int pointX)
{
    if (pen.width() <= 5)
    {
        if (mousePosX <= pointX + 5 && mousePosX >= pointX - 5)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (mousePosX <= pointX + pen.widthF()/2 && mousePosX >= pointX - pen.widthF()/2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool Shape_Freehand::testY(const int mousePosY, const int pointY)
{
    if (pen.width() <= 5)
    {
        if (mousePosY <= pointY + 5 && mousePosY >= pointY - 5)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (mousePosY <= pointY + pen.widthF()/2 && mousePosY >= pointY - pen.widthF()/2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

const int Shape_Freehand::calculateSurfaceArea() const
{
    return 0;
}

const bool Shape_Freehand::getIsMoving() const
{
    return isMoving;
}

/*const QPointF Shape_Freehand::getTopLeft() const
{
    qreal lowestX{9999.0};
    qreal lowestY{9999.0};
    for (int i = 0; i < points.count(); i++)
    {
        if (points.at(i).x() < lowestX)
        {
            lowestX = points.at(i).x();
        }
        if (points.at(i).y() < lowestY)
        {
            lowestY = points.at(i).y();
        }
    }
    return QPointF{lowestX, lowestY};
}

const QPointF Shape_Freehand::getBottomRight() const
{
    qreal highestX{0.0};
    qreal highestY{0.0};
    for (int i = 0; i < points.count(); i++)
    {
        if (points.at(i).x() > highestX)
        {
            highestX = points.at(i).x();
        }
        if (points.at(i).y() > highestY)
        {
            highestY = points.at(i).y();
        }
    }
    return QPointF{highestX, highestY};
}*/



//RECTANGLE
Shape_Rectangle::Shape_Rectangle(const QPen& pen, const QBrush& brush, const QString& name, const QPoint &topLeft):
    BaseShape(pen, brush, name, Type_Rectangle),
    rect(QRect(topLeft, topLeft))
{
}
void Shape_Rectangle::drawShape(QPainter& painter) const
{
    painter.drawRect(rect);
}

void Shape_Rectangle::drawSelectionMarker(QPainter& painter) const
{
    //    painter.drawRect(rect.adjusted(-2 - pen.width()/2, -2 - pen.width()/2, 2 + pen.width()/2, 2 + pen.width()/2));
    QPen newPen {this->pen};
    if (pen.width() <= 10)
    {
        newPen.setWidthF(pen.width() + 5);
    }
    else
    {
        newPen.setWidthF(newPen.widthF() * 1.5);
    }
    QColor newColor = Qt::black;
    newColor.setAlpha(150);
    newPen.setStyle(Qt::SolidLine);
    newPen.setColor(newColor);
    painter.setPen(newPen);
    painter.drawRect(rect);
}

void Shape_Rectangle::mousePressEvent(const QMouseEvent* event)
{

}

void Shape_Rectangle::mouseMoveEvent(const QMouseEvent* event)
{
    rect.setBottomRight(event->pos());
}

void Shape_Rectangle::mouseReleaseEvent(const QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        shapeState = finished;
    else if (event->button() == Qt::RightButton)
        shapeState = canceled;
}

bool Shape_Rectangle::mouseIntersectCheck(const QMouseEvent* event)
{
    if (rect.contains(event->pos()))
    {
        return true;
    }
    return false;
}
void Shape_Rectangle::mouseDrag(const QPoint& newPos)
{
    rect.moveTopLeft(newPos);
}

void Shape_Rectangle::nudgeUp()
{
    rect.translate(0, -1);
}
void Shape_Rectangle::nudgeDown()
{
    rect.translate(0, 1);
}
void Shape_Rectangle::nudgeLeft()
{
    rect.translate(-1, 0);
}
void Shape_Rectangle::nudgeRight()
{
    rect.translate(1, 0);
}

const QPointF Shape_Rectangle::getTopLeft() const
{
    return rect.topLeft();
}

const int Shape_Rectangle::calculateSurfaceArea() const
{
    return rect.width() * rect.height();
}

const bool Shape_Rectangle::getIsMoving() const
{
    return isMoving;
}











//ELLIPSE
Shape_Ellipse::Shape_Ellipse(const QPen& pen, const QBrush& brush, const QString &name, const QPoint& topLeft):
    BaseShape(pen, brush, name, Type_Ellipse),
    rect(QRect(topLeft, topLeft))
{
}

void Shape_Ellipse::drawShape(QPainter &painter) const
{
    painter.drawEllipse(rect);
}

void Shape_Ellipse::drawSelectionMarker(QPainter& painter) const
{
    QPen newPen {this->pen};
    if (pen.width() <= 10)
    {
        newPen.setWidthF(pen.width() + 5);
    }
    else
    {
        newPen.setWidthF(newPen.widthF() * 1.5);
    }
    QColor newColor = Qt::black;
    newColor.setAlpha(150);
    newPen.setStyle(Qt::SolidLine);
    newPen.setColor(newColor);
    painter.setPen(newPen);
    painter.drawEllipse(rect);
}

void Shape_Ellipse::nudgeUp()
{
    rect.translate(0, -1);
}
void Shape_Ellipse::nudgeDown()
{
    rect.translate(0, 1);
}
void Shape_Ellipse::nudgeLeft()
{
    rect.translate(-1, 0);
}
void Shape_Ellipse::nudgeRight()
{
    rect.translate(1, 0);
}

const QPointF Shape_Ellipse::getTopLeft() const
{
    return rect.topLeft();
}

const int Shape_Ellipse::calculateSurfaceArea() const
{
    //Area = Pi * A * B
    return 3.14159265359 * rect.width()/2 * rect.height()/2;
}

const bool Shape_Ellipse::getIsMoving() const
{
    return isMoving;
}
void Shape_Ellipse::mouseDrag(const QPoint& newPos)
{
    rect.moveTopLeft(newPos);
}

void Shape_Ellipse::mousePressEvent(const QMouseEvent* event)
{

}

void Shape_Ellipse::mouseMoveEvent(const QMouseEvent* event)
{
    rect.setBottomRight(event->pos());
}

void Shape_Ellipse::mouseReleaseEvent(const QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        shapeState = finished;
    else if (event->button() == Qt::RightButton)
        shapeState = canceled;
}

bool Shape_Ellipse::mouseIntersectCheck(const QMouseEvent* event)
{
    double powX = pow(event->pos().x() - rect.center().x(), 2);
    double powY = pow(event->pos().y() - rect.center().y(), 2);
    double powLeft = pow(rect.width() / 2, 2);
    double powTop = pow(rect.height() / 2, 2);

    if (powX / powLeft + powY / powTop <= 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}





//LINE//
Shape_Line::Shape_Line(const QPen &pen, const QBrush &brush, const QString& name, const QPoint &firstPoint):
    BaseShape(pen, brush, name, Type_Line),
    line(QLine(firstPoint, firstPoint)),
    boundingRectangle(QRect(firstPoint, firstPoint))
{
}
void Shape_Line::drawShape(QPainter &painter) const
{
    painter.drawLine(line);
}

void Shape_Line::drawSelectionMarker(QPainter& painter) const
{
    QPen newPen {this->pen};
    if (pen.width() <= 10)
    {
        newPen.setWidthF(pen.width() + 5);
    }
    else
    {
        newPen.setWidthF(newPen.widthF() * 1.5);
    }
    QColor newColor = Qt::black;
    newColor.setAlpha(150);
    newPen.setStyle(Qt::SolidLine);
    newPen.setColor(newColor);
    painter.setPen(newPen);
    painter.drawLine(line);
}

void Shape_Line::mousePressEvent(const QMouseEvent* event)
{

}
void Shape_Line::mouseMoveEvent(const QMouseEvent* event)
{
    line.setP2(event->pos());
}

void Shape_Line::mouseReleaseEvent(const QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        boundingRectangle = QRect(line.p1(), line.p2());
        shapeState = finished;
    }
    else if (event->button() == Qt::RightButton)
        shapeState = canceled;
}

bool Shape_Line::mouseIntersectCheck(const QMouseEvent* event)
{
    //y = ax + b
    //ax + b = y
    //b = y - ax

    QPoint mousePos = event->pos();

    int x1 = line.p1().x();
    int y1 = line.p1().y();
    int dY = line.dy();
    int dX = line.dx();
    float a = static_cast<float>(dY)/static_cast<float>(dX);
    int b = y1 - (a * x1);

    if (mousePos.y() >= (a * mousePos.x() + b) - 5 && mousePos.y() <= (a * mousePos.x() +b) + 5)
    {
        if (boundingRectangle.contains(mousePos))
        {
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

void Shape_Line::mouseDrag(const QPoint& newPos)
{
    if (line.p1().x() > newPos.x())
    {
        qreal diffX = line.p1().x() - newPos.x();
        line.setP1(QPoint(line.p1().x() - diffX, line.p1().y()));
        line.setP2(QPoint(line.p2().x() - diffX, line.p2().y()));
    }
    else
    {
        qreal diffX = newPos.x() - line.p1().x();
        line.setP1(QPoint((line.p1().x() + diffX), line.p1().y()));
        line.setP2(QPoint((line.p2().x() + diffX), line.p2().y()));
    }
    if (line.p1().y() > newPos.y())
    {
        qreal diffY = line.p1().y() - newPos.y();
        line.setP1(QPoint(line.p1().x(), line.p1().y() - diffY));
        line.setP2(QPoint(line.p2().x(), line.p2().y() - diffY));
    }
    else
    {
        qreal diffY = newPos.y() - line.p1().y();
        line.setP1(QPoint(line.p1().x(), line.p1().y() + diffY));
        line.setP2(QPoint(line.p2().x(), line.p2().y() + diffY));
    }
    boundingRectangle.moveTopLeft(newPos);
}

void Shape_Line::nudgeUp()
{
    line.setP1(line.p1() += QPoint{0, -1});
    line.setP2(line.p2() += QPoint{0, -1});
}

void Shape_Line::nudgeDown()
{
    line.setP1(line.p1() += QPoint{0, 1});
    line.setP2(line.p2() += QPoint{0, 1});
}

void Shape_Line::nudgeLeft()
{
    line.setP1(line.p1() += QPoint{-1, 0});
    line.setP2(line.p2() += QPoint{-1, 0});
}

void Shape_Line::nudgeRight()
{
    line.setP1(line.p1() += QPoint{1, 0});
    line.setP2(line.p2() += QPoint{1, 0});
}

const QPointF Shape_Line::getTopLeft() const
{
    return line.p1();
}

const int Shape_Line::calculateSurfaceArea() const
{
    return 0;
}

const bool Shape_Line::getIsMoving() const
{
    return isMoving;
}

//POLYGON
Shape_Polygon::Shape_Polygon(const QPen& pen, const QBrush& brush, const QString& name, const QPoint& firstPoint):
    BaseShape(pen, brush, name, Type_Polygon)
{
    points.append(firstPoint);
    points.append(firstPoint);
}

void Shape_Polygon::drawShape(QPainter &painter) const
{
    painter.drawPolygon(points);
}

void Shape_Polygon::drawSelectionMarker(QPainter& painter) const
{
    QPen newPen {this->pen};
    if (pen.width() <= 10)
    {
        newPen.setWidthF(pen.width() + 5);
    }
    else
    {
        newPen.setWidthF(newPen.widthF() * 1.5);
    }
    QColor newColor = Qt::black;
    newColor.setAlpha(150);
    newPen.setStyle(Qt::SolidLine);
    newPen.setColor(newColor);
    painter.setPen(newPen);
    painter.drawPolygon(points);


    //    QVector<int> X {};
    //    QVector<int> Y {};
    //    int xSum {};
    //    int ySum {};
    //    int xCenter {};
    //    int yCenter {};

    //    for (int i = 0; i < points.size(); i++)
    //    {
    //        X.append(points.at(i).x());
    //        xSum += points.at(i).x();
    //        Y.append(points.at(i).y());
    //        ySum += points.at(i).y();
    //    }
    //    xCenter = xSum / X.size();
    //    yCenter = ySum / Y.size();

    //    painter.translate(xCenter, yCenter);
    //    painter.scale(1.2, 1.2);
    //    painter.translate(-xCenter, -yCenter);
    //    painter.drawPolygon(points);
}

void Shape_Polygon::mouseDrag(const QPoint& newPos)
{
    const QPointF topLeft = getTopLeft();
    for (int i = 0; i < points.count(); i++)
    {
        if (points.at(i).x() > newPos.x())
        {
            qreal diffX = topLeft.x() - newPos.x();
            points[i].setX(points.at(i).x() - diffX);
        }
        else
        {
            qreal diffX = newPos.x() - topLeft.x();
            points[i].setX(points.at(i).x() + diffX);
        }
        if (points.at(i).y() > newPos.y())
        {
            qreal diffY = topLeft.y() - newPos.y();
            points[i].setY(points.at(i).y() - diffY);
        }
        else
        {
            qreal diffY = newPos.y() - topLeft.y();
            points[i].setY(points.at(i).y() + diffY);
        }
    }
    //boundingRectangle.moveTopLeft(newPos);
}

void Shape_Polygon::nudgeUp()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPoint{0, -1};
    }
    //boundingRectangle.translate(0, -1);
}

void Shape_Polygon::nudgeDown()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPoint{0, 1};
    }
    //boundingRectangle.translate(0, 1);
}

void Shape_Polygon::nudgeLeft()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPoint{-1, 0};
    }
    //boundingRectangle.translate(-1, 0);
}

void Shape_Polygon::nudgeRight()
{
    for (int i = 0; i < points.count(); i++)
    {
        points[i] += QPoint{1, 0};
    }
    //boundingRectangle.translate(1, 0);
}

void Shape_Polygon::mousePressEvent(const QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        points.append(event->pos());
    }
}

void Shape_Polygon::mouseMoveEvent(const QMouseEvent* event)
{
    points.replace(points.size() - 1, event->pos());
}

void Shape_Polygon::mouseReleaseEvent(const QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        shapeState = finished;
    }
}

bool Shape_Polygon::mouseIntersectCheck(const QMouseEvent* event)
{
    QPolygon innerPolygon{points};
    if (innerPolygon.containsPoint(event->pos(), Qt::OddEvenFill))
    {
        calculateSurfaceArea();
        return true;
    }
    else
    {
        return false;
    }
}

const QPointF Shape_Polygon::getTopLeft() const
{
    qreal lowestX{9999.0};
    qreal lowestY{9999.0};
    for (int i = 0; i < points.size(); i++)
    {
        QPointF currentPoint = points.at(i);
        if (currentPoint.x() < lowestX)
        {
            lowestX = currentPoint.x();
        }
        if (currentPoint.y() < lowestY)
        {
            lowestY = currentPoint.y();
        }
    }
    return QPointF {lowestX, lowestY};
}

const int Shape_Polygon::calculateSurfaceArea() const
{
    QVector<int>pointsX {};
    QVector<int>pointsY {};

    for (int i = points.size() - 1; i >= 0; --i)
    {
        pointsX.append(points.at(i).x());
    }
    for (int i = points.size() - 1; i >= 0; --i)
    {
        pointsY.append(points.at(i).y());
    }

    int area = 0;
    int j = points.size() - 1;
    for (int i = 0; i < points.size(); i++)
    {
        area = area + (pointsX[j] + pointsX[i]) * (pointsY[j] - pointsY[i]);
        j = i;
    }
    return abs(area/2);
}

const bool Shape_Polygon::getIsMoving() const
{
    return isMoving;
}
