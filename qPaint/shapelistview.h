#ifndef SHAPELISTVIEW_H
#define SHAPELISTVIEW_H

#include <QMenu>
#include <QListView>
#include "shapes.h"
#include "shapelistmodel.h"

class ShapeListView : public QListView
{
    Q_OBJECT
public:
    ShapeListView(QWidget* parent = 0);
private:
    QMenu*      menu_Context                        {new QMenu(this)};
    QAction*    action_DeleteShape                  {new QAction("Obriši", this)};
    QAction*    action_MoveShapeUp                  {new QAction("Pomakni gore", this)};
    QAction*    action_MoveShapeDown                {new QAction("Pomakni dolje", this)};
    QModelIndex itemIndex                           {QModelIndex()};
    void        initializeContextMenu               ();
public slots:
    void        on_custom_contextMenu_requested     (const QPoint& pos);
    void        on_action_deleteShape_triggered     ();
    void        on_action_moveShapeUp_triggered     ();
    void        on_action_moveShapeDown_triggered   ();
    void        selectShape                         (const BaseShape* const newShape);
    void        updateSelection                     (const QModelIndex& topLeft, const int& first, const int& last);
signals:
    void        shapeChanged                        ();
};

#endif // SHAPELISTVIEW_H


