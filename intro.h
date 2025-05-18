#ifndef INTRO_H
#define INTRO_H

#include <QDialog>
namespace Ui {
class intro;
}

class intro : public QWidget
{
    Q_OBJECT

public:
    explicit intro(QWidget *parent = nullptr);
    ~intro();

private slots:
    void on_pushButton_clicked();

private:
    Ui::intro *ui;
};

#endif // INTRO_H
