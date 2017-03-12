#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QBitmap>
#include <QInputEvent>
#include <QStyleOption>
#include <QItemSelection>
#include "enum.h"
#include "shapelistmodel.h"
#include "shapes.h"
#include "shapefactory.h"
#include "commands.h"

class MainWindow;

class PaintArea : public QWidget
{
    Q_OBJECT
signals:
    void                            mouseMoved(const QPoint& mousePos);
    void                            selectionChanged(const BaseShape* const);
    void                            filenameChanged(const QString);
public:
    explicit PaintArea              (const QString& pFileName, const QSize& canvasSize = QSize(800, 600), MainWindow *parent = 0);
    void                            rasterizeAndClearShapes();
    const QImage                    copyImage();
    void                            deleteImage();
    void                            undo();
    void                            redo();
    void                            resizeImage(const QSize& newSize);
    QImage&                         getImage() const;
    void                            pasteImage(const QImage& newImage);
    QString                         getFileName() const;
    ShapeListModel*                 getShapeListModel();
    const int                       calculateTotalSurface() const;
    bool                            saveImage(const QString& pFileName);
    QUndoStack*                     getUndoStack() const;
    void                            whiteFill();
    bool                            getIsModified() const;

protected:
    void                            paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void                            mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void                            mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void                            mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void                            keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
private:
    const MainWindow&               parent;
    bool                            isModified{false};
    QString                         fileName {};
    QSize                           canvasSize {};
    DrawMode                        mode {};
    bool                            isMousePressed {false};
    QUndoStack*                     undoStack {new QUndoStack(this)};
    ShapeListModel                  shapeListModel {this};
    QScopedPointer<QImage>          image {new QImage(canvasSize, QImage::Format_ARGB32_Premultiplied)};
    QPainter                        painter {};
    QScopedPointer<BaseShape>       shapeInProgress {nullptr};
    BaseShape*                      selectedShape {nullptr};
    QPoint                          mousePressOffset {0,0};
    MouseState                      mouseState {idle};
    //Funkcije
    BaseShape*                      getShapeUnderMouse(const QMouseEvent* event);
    void                            changeMouseCursor();
    void                            cursorOnSelectionRelease(QMouseEvent* event);
    void                            drawShapeInProgress();
    void                            drawShapesInList();
    const QString                   nameShape(DrawMode type) const;
public slots:
    void                            setMode(const DrawMode& newMode);
    void                            selectShape(const QItemSelection &selected, const QItemSelection &deselected);
};
#endif // PAINTAREA_H
