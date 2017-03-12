#ifndef SHAPES

#define SHAPES

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <math.h>
#include "enum.h"

class BaseShape //(abstract)
{
    public:
        virtual void            drawShape(QPainter& painter) const             = 0;
        virtual void            drawSelectionMarker(QPainter& painter) const   = 0;
        virtual void            mousePressEvent(const QMouseEvent* event)      = 0;
        virtual void            mouseMoveEvent(const QMouseEvent* event)       = 0;
        virtual void            mouseReleaseEvent(const QMouseEvent* event)    = 0;
        virtual bool            mouseIntersectCheck(const QMouseEvent* event)  = 0;
        virtual void            mouseDrag(const QPoint& newPos)                = 0;
        virtual void            nudgeUp()                                      = 0;
        virtual void            nudgeDown()                                    = 0;
        virtual void            nudgeLeft()                                    = 0;
        virtual void            nudgeRight()                                   = 0;
        virtual const QPointF   getTopLeft() const                             = 0;
        virtual const int       calculateSurfaceArea() const                   = 0;
        virtual const bool      getIsMoving() const                            = 0;
        const DrawMode          getType() const;
        const QString&          getName() const;
        const QPen&             getPen() const;
        const QBrush&           getBrush() const;
        ShapeState              getShapeState() const;
        void                    setName(const QString& value);
    protected:
        BaseShape               (const QPen &pen, const QBrush &brush, const QString& name, const DrawMode& type);
        ShapeState              shapeState {inProgress};
        QPen                    pen;
        QBrush                  brush;
        QString                 name;
        DrawMode                type;
        bool                    isMoving {false};
};
Q_DECLARE_METATYPE(BaseShape*) //potrebno za QVariant

class Shape_Freehand : public BaseShape
{
    public:
        Shape_Freehand          (const QPen& pen, const QBrush& brush, const QString& name, const QPoint& firstPoint);
        void                    drawShape(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    drawSelectionMarker(QPainter &painter) const;
        void                    mousePressEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseMoveEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseReleaseEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        bool                    mouseIntersectCheck(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseDrag(const QPoint& newPos) Q_DECL_OVERRIDE;
        void                    nudgeUp() Q_DECL_OVERRIDE;
        void                    nudgeDown() Q_DECL_OVERRIDE;
        void                    nudgeLeft() Q_DECL_OVERRIDE;
        void                    nudgeRight() Q_DECL_OVERRIDE;
        const QPointF           getTopLeft() const Q_DECL_OVERRIDE;
        const int               calculateSurfaceArea() const Q_DECL_OVERRIDE;
        const bool              getIsMoving() const Q_DECL_OVERRIDE;
        void                    addPoint(const QPointF& newPoint);
        bool                    testX(const int mousePosX, const int pointX);
        bool                    testY(const int mousePosY, const int pointY);
    private:
        QVector<QPointF>        points {};
};

class Shape_Rectangle : public BaseShape
{
    public:
        Shape_Rectangle         (const QPen& pen, const QBrush& brush, const QString& name, const QPoint& topLeft);
        void                    drawShape(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    drawSelectionMarker(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    mousePressEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseMoveEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseReleaseEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        bool                    mouseIntersectCheck(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseDrag(const QPoint& newPos) Q_DECL_OVERRIDE;
        void                    nudgeUp() Q_DECL_OVERRIDE;
        void                    nudgeDown() Q_DECL_OVERRIDE;
        void                    nudgeLeft() Q_DECL_OVERRIDE;
        void                    nudgeRight() Q_DECL_OVERRIDE;
        const QPointF           getTopLeft() const Q_DECL_OVERRIDE;
        const int               calculateSurfaceArea() const;
        const bool              getIsMoving() const Q_DECL_OVERRIDE;
    private:
        QRect                   rect;
};

class Shape_Ellipse : public BaseShape
{
    public:
        Shape_Ellipse           (const QPen& pen, const QBrush& brush, const QString& name, const QPoint& topLeft);
        void                    drawShape(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    drawSelectionMarker(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    mouseDrag(const QPoint& newPos) Q_DECL_OVERRIDE;
        void                    mousePressEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseMoveEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseReleaseEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        bool                    mouseIntersectCheck(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    nudgeUp() Q_DECL_OVERRIDE;
        void                    nudgeDown() Q_DECL_OVERRIDE;
        void                    nudgeLeft() Q_DECL_OVERRIDE;
        void                    nudgeRight() Q_DECL_OVERRIDE;
        const QPointF           getTopLeft() const Q_DECL_OVERRIDE;
        const int               calculateSurfaceArea() const;
        const bool              getIsMoving() const Q_DECL_OVERRIDE;
    private:
        QRect                   rect;
};

class Shape_Line : public BaseShape
{
    public:
        Shape_Line              (const QPen& pen, const QBrush& brush, const QString& name, const QPoint& firstPoint);
        void                    drawShape(QPainter &painter) const Q_DECL_OVERRIDE;
        void                    drawSelectionMarker(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    mousePressEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseMoveEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseReleaseEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        bool                    mouseIntersectCheck(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseDrag(const QPoint& newPos) Q_DECL_OVERRIDE;
        void                    nudgeUp() Q_DECL_OVERRIDE;
        void                    nudgeDown() Q_DECL_OVERRIDE;
        void                    nudgeLeft() Q_DECL_OVERRIDE;
        void                    nudgeRight() Q_DECL_OVERRIDE;
        const QPointF           getTopLeft() const Q_DECL_OVERRIDE;
        const int               calculateSurfaceArea() const;
        const bool              getIsMoving() const Q_DECL_OVERRIDE;
    private:
        QLine                   line;
        QRect                   boundingRectangle;
};

class Shape_Polygon : public BaseShape
{
    public:
        Shape_Polygon           (const QPen& pen, const QBrush& brush, const QString& name, const QPoint& firstPoint);
        void                    drawShape(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    drawSelectionMarker(QPainter& painter) const Q_DECL_OVERRIDE;
        void                    mousePressEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseMoveEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseReleaseEvent(const QMouseEvent* event) Q_DECL_OVERRIDE;
        bool                    mouseIntersectCheck(const QMouseEvent* event) Q_DECL_OVERRIDE;
        void                    mouseDrag(const QPoint& newPos) Q_DECL_OVERRIDE;
        void                    nudgeUp() Q_DECL_OVERRIDE;
        void                    nudgeDown() Q_DECL_OVERRIDE;
        void                    nudgeLeft() Q_DECL_OVERRIDE;
        void                    nudgeRight() Q_DECL_OVERRIDE;
        const QPointF           getTopLeft() const Q_DECL_OVERRIDE;
        const int               calculateSurfaceArea() const Q_DECL_OVERRIDE;
        const bool              getIsMoving() const Q_DECL_OVERRIDE;
    private:
        QVector<QPoint>         points {};
};

#endif // SHAPES
