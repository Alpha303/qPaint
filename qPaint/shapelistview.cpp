#include "shapelistview.h"

ShapeListView::ShapeListView(QWidget *parent): QListView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);  
    initializeContextMenu();
}

void ShapeListView::initializeContextMenu()
{
    menu_Context->addAction(action_MoveShapeUp);
    menu_Context->addAction(action_MoveShapeDown);
    menu_Context->addAction(action_DeleteShape);
    action_DeleteShape->setStatusTip("Briše trenutni oblik iz dokumenta");
    //Konekcije
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_custom_contextMenu_requested(QPoint)));
    connect(action_MoveShapeUp, SIGNAL(triggered(bool)), this, SLOT(on_action_moveShapeUp_triggered()));
    connect(action_MoveShapeDown, SIGNAL(triggered(bool)), this, SLOT(on_action_moveShapeDown_triggered()));
    connect(action_DeleteShape, SIGNAL(triggered(bool)), this, SLOT(on_action_deleteShape_triggered()));
}


void ShapeListView::on_custom_contextMenu_requested(const QPoint& pos)
{
    itemIndex = indexAt(pos);
    if (itemIndex.isValid())
    {
        selectionModel()->select(itemIndex, QItemSelectionModel::Select);
        menu_Context->popup(mapToGlobal(pos));
    }
}

void ShapeListView::on_action_deleteShape_triggered()
{
    dynamic_cast<ShapeListModel*>(model())->deleteShapeViaContextMenu(itemIndex.row());
    emit shapeChanged();
}

void ShapeListView::on_action_moveShapeUp_triggered()
{
    dynamic_cast<ShapeListModel*>(model())->moveShapeUp(itemIndex.row());
    emit shapeChanged();
}

void ShapeListView::on_action_moveShapeDown_triggered()
{
    dynamic_cast<ShapeListModel*>(model())->moveShapeDown(itemIndex.row());
    emit shapeChanged();
}

void ShapeListView::selectShape(const BaseShape* const newShape)
{
    if (newShape != nullptr)
    {
        int row{0};
        for (int i = 0; i < static_cast<ShapeListModel*>(model())->getShapeListConst().size(); i++)
        {
            if (newShape == static_cast<ShapeListModel*>(model())->getShapeListConst().at(i))
            {
                row = i;
            }
        }
        selectionModel()->select(model()->index(row, 0), QItemSelectionModel::ClearAndSelect);
    }
    else
    {
        selectionModel()->clearSelection();
    }
}

void ShapeListView::updateSelection(const QModelIndex& topLeft, const int& first, const int& last)
{
    selectionModel()->select(model()->index(first, 0, topLeft), QItemSelectionModel::ClearAndSelect);
}
