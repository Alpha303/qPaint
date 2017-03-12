#ifndef COMMANDS_H
#define COMMANDS_H

#include <QApplication>
#include <QUndoCommand>
#include <QClipboard>
#include "shapes.h"
#include "shapelistmodel.h"

class AddShapeCommand : public QUndoCommand
{
public:
    AddShapeCommand(ShapeListModel& shapeListModel, BaseShape* newShape, QUndoCommand* parent = 0);
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
private:
    ShapeListModel& shapeListModel;
    QScopedPointer<BaseShape> newShape;
};

class DeleteShapeCommand : public QUndoCommand
{
public:
    DeleteShapeCommand(ShapeListModel& pShapeListModel, const int& row, QUndoCommand* parent = 0);
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
private:
    int row;
    ShapeListModel& shapeListModel;
    QScopedPointer<BaseShape> deletedShape;
};

class DeleteImageCommand : public QUndoCommand
{
public:
    DeleteImageCommand(QImage& pImage, ShapeListModel& pModel);
    ~DeleteImageCommand();
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
private:
    QImage& image;
    QImage oldImage;
    ShapeListModel& model;
    QList<BaseShape*> storedShapes {};
};

#endif // COMMANDS_H
