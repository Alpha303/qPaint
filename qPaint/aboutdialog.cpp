#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{

    setWindowTitle("O programu");
    setFixedSize(200, 150);

    label4->setOpenExternalLinks(true);

    layout->addWidget(label);
    layout->addWidget(label2);
    layout->addWidget(label3);
    layout->addWidget(label4);
    layout->addWidget(exitButton);

    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(on_exitButton_clicked()));
}

void AboutDialog::on_exitButton_clicked()
{
    done(0);
}
