#include "mainwindow.h"
#include "paintarea.h"

PaintArea::PaintArea(const QString& pFileName, const QSize& canvasSize,  MainWindow* parent):
    QWidget(parent),
    canvasSize(canvasSize),
    mode(parent->getMode()),
    parent(*parent)
{
    setObjectName("paintArea");
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    resize(canvasSize);
    whiteFill();

    if (!pFileName.isEmpty())
    {
        image->load(pFileName);
        resize(image->size());
        fileName = pFileName;
    }
}

const int PaintArea::calculateTotalSurface() const
{
    int totalSurfaceArea {0};
    for (int i = 0; i < shapeListModel.getShapeListConst().size(); i++)
    {
        totalSurfaceArea += shapeListModel.getShapeListConst().at(i)->calculateSurfaceArea();
    }
    return totalSurfaceArea;
}

void PaintArea::changeMouseCursor()
{
    if (mode == Type_Selection)
    {
        switch (mouseState)
        {
        case idle:
            setCursor(Qt::ArrowCursor);
            break;
        case targetting:
            setCursor(Qt::OpenHandCursor);
            break;
        case dragging:
            setCursor(Qt::ClosedHandCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }
    }
    else
    {
        setCursor(QCursor(QPixmap(":/icons/gfx/mouse_black.png"), 7, 7));
    }
}

const QImage PaintArea::copyImage()
{
    QImage tempImage (*image);
    QPainter painter(&tempImage);
    for (int i = 0; i < shapeListModel.getShapeListConst().size(); i++)
    {
        const BaseShape* const shape = shapeListModel.getShapeListConst().at(i);
        painter.setPen(shape->getPen());
        painter.setBrush(shape->getBrush());
        shape->drawShape(painter);
    }
    return tempImage;
}

void PaintArea::deleteImage()
{
    undoStack->push(new DeleteImageCommand(*image, shapeListModel));
    update();
}

void PaintArea::drawShapesInList()
{
    for (int var = shapeListModel.getShapeListConst().size() - 1; var >= 0; --var)
    {
        BaseShape* shape = shapeListModel.getShapeListConst().at(var);
        painter.begin(this);
        if (parent.getUseAntialiasing())
        {
            painter.setRenderHint(QPainter::Antialiasing);
        }
        else
        {
            painter.setRenderHint(QPainter::Antialiasing, false);
        }
        painter.setPen(shape->getPen());
        painter.setBrush(shape->getBrush());
        shape->drawShape(painter);
        if (shape == selectedShape)
        {
            QPen red (Qt::black);
            red.setWidth(1);
            painter.setOpacity(0.6);
            red.setCapStyle(Qt::SquareCap);
            red.setStyle(Qt::DashLine);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(red);
            shape->drawSelectionMarker(painter);
        }
        painter.end();
    }
}

void PaintArea::drawShapeInProgress()
{
    if (!shapeInProgress.isNull())
    {
        painter.begin(this);
        painter.setPen(shapeInProgress->getPen());
        painter.setBrush(shapeInProgress->getBrush());
        shapeInProgress->drawShape(painter);
        painter.end();
    }
}

QString PaintArea::getFileName() const
{
    return fileName;
}

QImage& PaintArea::getImage() const
{
    return *image;
}

ShapeListModel* PaintArea::getShapeListModel()
{
    return &shapeListModel;
}

BaseShape* PaintArea::getShapeUnderMouse(const QMouseEvent* event)
{
    for (int i = 0; i < shapeListModel.getShapeListConst().size(); i++)
    {
        BaseShape* shape = shapeListModel.getShapeListConst().at(i);
        if (shape->mouseIntersectCheck(event))
            return shape;
    }
    return nullptr;
}

QUndoStack *PaintArea::getUndoStack() const
{
    return undoStack;
}

void PaintArea::keyPressEvent(QKeyEvent *event)
{
    if (mode == Type_Selection)
    {
        if (event->key() == Qt::Key_Up)
        {
            if (!shapeListModel.getShapeListConst().isEmpty() && selectedShape != nullptr)
            {
                selectedShape->nudgeUp();
                update();
            }
        }
        if (event->key() == Qt::Key_Down)
        {

            if (!shapeListModel.getShapeListConst().isEmpty() && selectedShape != nullptr)
            {
                selectedShape->nudgeDown();
                update();
            }
        }
        if (event->key() == Qt::Key_Left)
        {
            if (!shapeListModel.getShapeListConst().isEmpty() && selectedShape != nullptr)
            {
                selectedShape->nudgeLeft();
                update();
            }
        }
        if (event->key() == Qt::Key_Right)
        {
            if (!shapeListModel.getShapeListConst().isEmpty() && selectedShape != nullptr)
            {
                selectedShape->nudgeRight();
                update();
            }
        }
    }
}

bool PaintArea::getIsModified() const
{
    return isModified;
}

void PaintArea::mouseMoveEvent(QMouseEvent* event)
{
    emit mouseMoved(event->pos());
    if (mode == Type_Selection)
    {
        if (mouseState == dragging)
        {
            selectedShape->mouseDrag(event->pos() + mousePressOffset);
        }
        else
        {
            if (getShapeUnderMouse(event))
            {
                mouseState = targetting;
            }
            else
            {
                mouseState = idle;
            }
        }
    }
    else
    {
        if (!shapeInProgress.isNull())
        {
            shapeInProgress->mouseMoveEvent(event);
        }
    }
    changeMouseCursor();
    update();
}

void PaintArea::mousePressEvent(QMouseEvent* event)
{
    isModified = true;
    if (mode == Type_Selection)
    {
        selectedShape = getShapeUnderMouse(event);

        if (selectedShape == nullptr)
        {
            emit selectionChanged(nullptr);
            mouseState = idle;
        }
        else
        {
            selectionChanged(selectedShape);
            mousePressOffset = selectedShape->getTopLeft().toPoint() - event->pos();
            mouseState = dragging;
        }
    }
    else
    {
        if (shapeInProgress.isNull())
        {
            ShapeFactory shapeFactory{};
            shapeInProgress.reset(shapeFactory.createNewShape(parent.getPen(), parent.getBrush(), nameShape(mode) , event->pos(), mode));
        }
        else
        {
            shapeInProgress->mousePressEvent(event);
        }
    }
    changeMouseCursor();
    update();
}

void PaintArea::mouseReleaseEvent(QMouseEvent* event)
{
    if (mode == Type_Selection)
    {
        if (getShapeUnderMouse(event))
        {
            mouseState = targetting;
        }
        else
        {
            mouseState = idle;
        }
    }
    else
    {
        if (!shapeInProgress.isNull())
        {
            shapeInProgress->mouseReleaseEvent(event);
            if (shapeInProgress->getShapeState() == finished)
            {
                undoStack->push(new AddShapeCommand(shapeListModel, shapeInProgress.take()));
                selectedShape = shapeListModel.getShapeListConst().at(0);
                emit selectionChanged(selectedShape);
            }
            else if (shapeInProgress->getShapeState() == canceled)
            {
                shapeInProgress.reset();
            }
        }
    }
    changeMouseCursor();
    update();
}

const QString PaintArea::nameShape(DrawMode type) const
{
    QString newName{};
    int rectangleCount{1}, ellipseCount{1}, polylineCount{1}, freelineCount{1}, lineCount{1};
    for (int i = 0; i < shapeListModel.getShapeListConst().size(); i++)
    {
        BaseShape* shape = shapeListModel.getShapeListConst().at(i);
        switch (shape->getType())
        {
        case Type_Freehand:
            freelineCount++;
            break;
        case Type_Rectangle:
            rectangleCount++;
            break;
        case Type_Ellipse:
            ellipseCount++;
            break;
        case Type_Polygon:
            polylineCount++;
            break;
        case Type_Line:
            lineCount++;
            break;
        default:
            break;
        }
    }
    switch(type)
    {
    case Type_Freehand:
        newName = QString("Slobodna linija %1").arg(freelineCount);
        break;
    case Type_Rectangle:
        newName = QString("Pravokutnik %1").arg(rectangleCount);
        break;
    case Type_Ellipse:
        newName = QString("Kružnica %1").arg(ellipseCount);
        break;
    case Type_Polygon:
        newName = QString("Poligon %1").arg(polylineCount);
        break;
    case Type_Line:
        newName = QString("Pravac %1").arg(lineCount);
        break;
    default:
        break;
    }
    return newName;
}

void PaintArea::paintEvent(QPaintEvent* event)
{
    painter.begin(this);
    if (image)
    {
        painter.drawImage(event->rect(), *image, event->rect());
    }
    painter.end();
    drawShapesInList();
    drawShapeInProgress();
    //CSS-podrška
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PaintArea::pasteImage(const QImage& newImage)
{
    painter.begin(image.data());
    painter.drawImage(0,0, newImage);
    painter.end();
}

void PaintArea::rasterizeAndClearShapes()
{
    QPainter painter(image.data());
    for (int i = 0; i < shapeListModel.getShapeListConst().size(); i++)
    {
        const BaseShape* const shape = shapeListModel.getShapeListConst().at(i);
        painter.setPen(shape->getPen());
        painter.setBrush(shape->getBrush());
        shape->drawShape(painter);
    }
    shapeListModel.getShapeList().clear();
}

void PaintArea::redo()
{
    undoStack->redo();
    update();
}

void PaintArea::resizeImage(const QSize& newSize)
{
    resize(newSize);
    *image = image->scaled(newSize);
}

bool PaintArea::saveImage(const QString &pFileName)
{
    QImage backup = *image;
    QPainter painter(image.data());
    for (int i = 0; i < shapeListModel.getShapeListConst().size(); i++)
    {
        const BaseShape* const shape = shapeListModel.getShapeListConst().at(i);
        painter.setPen(shape->getPen());
        painter.setBrush(shape->getBrush());
        shape->drawShape(painter);
    }
    if (image.data()->save(pFileName))
    {
        painter.drawImage(0, 0, backup);
        fileName = pFileName;
        emit filenameChanged(fileName);
        isModified = false;
        return true;
    }
    else
    {
        return false;
    }
}

void PaintArea::selectShape(const QItemSelection& selected, const QItemSelection& deselected)
{
    if (!selected.isEmpty())
    {
        selectedShape = shapeListModel.getShapeListConst().at(selected.at(0).top());
        selectionChanged(selectedShape);
        update();
    }
    else
    {
        selectedShape = nullptr;
        selectionChanged(nullptr);
        update();
    }
}

void PaintArea::setMode(const DrawMode& newMode)
{
    mode = static_cast<DrawMode>(newMode);
}

void PaintArea::undo()
{
    undoStack->undo();
    update();
}

void PaintArea::whiteFill()
{
    image->fill(qRgba(255, 255, 255, 255));
}
