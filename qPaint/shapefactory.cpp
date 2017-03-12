#include "shapefactory.h"

ShapeFactory::ShapeFactory()
{
}

BaseShape* ShapeFactory::createNewShape(const QPen& pen, const QBrush& brush, const QString& name,
                                        const QPoint& initialPos, const DrawMode& shapeType)
{
    switch (shapeType)
    {
    case Type_Freehand:
        return new Shape_Freehand(pen, brush, name, initialPos);
        break;
    case Type_Line:
        return new Shape_Line(pen, brush, name, initialPos);
        break;
    case Type_Rectangle:
        return new Shape_Rectangle(pen, brush, name, initialPos);
        break;
    case Type_Ellipse:
        return new Shape_Ellipse(pen, brush, name, initialPos);
        break;
    case Type_Polygon:
        return new Shape_Polygon(pen, brush, name, initialPos);
        break;
    default:
        return nullptr;
    }
}
