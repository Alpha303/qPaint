#include "commands.h"

static const int AddShapeCommandId = 1;
static const int ResizeShapeCommandId = 2;
static const int MoveShapeCommandId = 3;

AddShapeCommand::AddShapeCommand(ShapeListModel& shapeListModel, BaseShape* newShape, QUndoCommand* parent):
    QUndoCommand(parent),
    shapeListModel(shapeListModel),
    newShape(newShape)
{
}
void AddShapeCommand::undo()
{
    newShape.reset(shapeListModel.removeShapeAt(0));
}
void AddShapeCommand::redo()
{
    shapeListModel.insertShapeAt(newShape.take(), 0);
}

DeleteShapeCommand::DeleteShapeCommand(ShapeListModel& pShapeListModel, const int& row, QUndoCommand* parent):
    QUndoCommand(parent),
    shapeListModel(pShapeListModel),
    row{row},
    deletedShape{}
{
}
void DeleteShapeCommand::undo()
{
    shapeListModel.insertShapeAt(deletedShape.take(), row);
}
void DeleteShapeCommand::redo()
{
    deletedShape.reset(shapeListModel.removeShapeAt(row));
}



DeleteImageCommand::DeleteImageCommand(QImage &pImage, ShapeListModel &pModel):
    image(pImage),
    oldImage(pImage),
    model(pModel)
{
}

DeleteImageCommand::~DeleteImageCommand()
{
    if (!storedShapes.isEmpty())
    {
        qDeleteAll(storedShapes);
    }
}

void DeleteImageCommand::undo()
{
    image = oldImage;
    model.swapList(storedShapes);
}

void DeleteImageCommand::redo()
{
    image.fill(Qt::white);
    model.swapList(storedShapes);
}
