#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "enum.h"
#include "shapes.h"

class ShapeFactory
{
public:
               ShapeFactory();
    BaseShape* createNewShape(const QPen& pen, const QBrush& brush, const QString& name,
                              const QPoint& initialPos, const DrawMode& shapeType);
};

#endif // SHAPEFACTORY_H
