#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit        AboutDialog(QWidget *parent = 0);
public slots:
    void            on_exitButton_clicked();
private:
    QLabel*         label  {new QLabel("qPaint v0.2, ©2015.", this)};
    QLabel*         label2 {new QLabel("Sva prava pridržana.", this)};
    QLabel*         label3 {new QLabel("Autor: Robert Komljenović", this)};
    QLabel*         label4 {new QLabel("<a href='mailto:rkomljen@foi.hr'>rkomljen@foi.hr</a>", this)};
    QVBoxLayout*    layout {new QVBoxLayout(this)};
    QPushButton*    exitButton {new QPushButton("Izlaz", this)};
};

#endif // ABOUTDIALOG_H
