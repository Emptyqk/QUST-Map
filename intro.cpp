#include "intro.h"
#include "ui_intro.h"
#include <QDialog>
intro::intro(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::intro)
{
    ui->setupUi(this);
}

intro::~intro()
{
    delete ui;
}

void intro::on_pushButton_clicked()
{
    this->close();
}

