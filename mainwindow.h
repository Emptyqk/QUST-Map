#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "schoolmap.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class schoolmap;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    schoolmap* getCurrentSchoolMap() { return currentSchoolMap; }
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_quit_clicked();

    void on_start_clicked();

    void on_admin_clicked();

private:
    Ui::MainWindow *ui;
    schoolmap* currentSchoolMap = nullptr;
};
#endif // MAINWINDOW_H
