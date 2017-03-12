#ifndef SHAPELISTMODEL
#define SHAPELISTMODEL

#include <QWidget>
#include <QAbstractListModel>
#include <QItemSelection>
#include <QUndoStack>
#include "shapes.h"

class PaintArea;

class ShapeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ShapeListModel(PaintArea* parent = 0);
    void                 insertShapeAt(BaseShape* newShape, int row);
    BaseShape*           removeShapeAt(int row);
    void                 swapList(QList<BaseShape*>& newList);
    void                 moveShapeUp(int row);
    void                 moveShapeDown(int row);
    void                 renameShape(const QString& newName);
    void                 deleteShapeViaContextMenu (int row);
    const QList<BaseShape*>&   getShapeListConst() const;
    QList<BaseShape*>&   getShapeList();
private:
    QVariant             headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant             data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool                 setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool                 insertRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    bool                 removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    int                  rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    Qt::ItemFlags        flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Qt::DropActions      supportedDropActions() const Q_DECL_OVERRIDE;
    QUndoStack&          undoStack;
    QList<BaseShape*>    shapeList {};
};

#endif // SHAPELISTMODEL
