#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schoolmap.h"
#include "schoolmap.h"
#include "admin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quit_clicked()
{
    this->close();
}


void MainWindow::on_start_clicked()
{
    if (!currentSchoolMap) {
        currentSchoolMap = new schoolmap();
    }
    currentSchoolMap->show();

}


void MainWindow::on_admin_clicked()
{
    Admin *A1 = new Admin;
    A1->show();
}

