#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QSignalMapper>

class ResizeDialog : public QDialog
{
    public:
        ResizeDialog(QWidget* parent = 0, const QSize currentSize = QSize(800, 600));
        const QSize getNewSize();
    private:
        QVBoxLayout* mainLayout {new QVBoxLayout};
        QGridLayout* gridLayout {new QGridLayout};
        QHBoxLayout* buttonLayout {new QHBoxLayout};
        QPushButton* button_OK;
        QPushButton* button_Cancel;
        QSpinBox* spinbox_width;
        QSpinBox* spinbox_height;
        QSignalMapper* mapper;
        QSize newSize;
};

#endif // RESIZEDIALOG_H
