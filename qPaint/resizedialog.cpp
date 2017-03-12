#include "resizedialog.h"

ResizeDialog::ResizeDialog(QWidget* parent, const QSize currentSize):
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    newSize{currentSize},
    button_OK(new QPushButton("Primjeni")),
    button_Cancel(new QPushButton("Odustani")),
    spinbox_width(new QSpinBox(this)),
    spinbox_height(new QSpinBox(this)),
    mapper(new QSignalMapper(this))
{
    setWindowTitle("Promjena veličine slike");
    setFixedSize(200, 100);
    
    spinbox_width->setMaximum(9999);
    spinbox_height->setMaximum(9999);
    spinbox_width->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinbox_height->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinbox_width->setValue(newSize.width());
    spinbox_height->setValue(newSize.height());

    
    gridLayout->addWidget(new QLabel("Širina slike:"), 0, 0);
    gridLayout->addWidget(spinbox_width, 0, 1);
    gridLayout->addWidget(new QLabel("px"), 0, 2);
    gridLayout->addWidget(new QLabel("Visina slike:"), 1, 0);
    gridLayout->addWidget(spinbox_height, 1, 1);
    gridLayout->addWidget(new QLabel("px"), 1, 2);

    gridLayout->setColumnMinimumWidth(0, 130);

    buttonLayout->addWidget(button_OK);
    buttonLayout->addWidget(button_Cancel);

    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    
    connect(button_Cancel, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(button_OK, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    mapper->setMapping(button_Cancel, 0);
    mapper->setMapping(button_OK, 1);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(done(int)));
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(done(int)));
}

const QSize ResizeDialog::getNewSize()
{
    int width = spinbox_width->text().toInt();
    int height = spinbox_height->text().toInt();
    QSize sizeToBeSet{width, height};
    return sizeToBeSet;
}

