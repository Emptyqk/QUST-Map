#ifndef SCHOOLMAP_H
#define SCHOOLMAP_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMainWindow>
#include "struct.h"
namespace Ui {
class schoolmap;
}

class schoolmap : public QWidget
{
    Q_OBJECT

public:
    explicit schoolmap(QWidget *parent = nullptr);
    ~schoolmap();
    void updateGraph();
    void refresh();
    void forceRefresh();

    void path(mgraph c,int m,int n,int k);
    void allpath(mgraph c, int start, int end);
    void shortdistance(mgraph c, int start, int end);
    void floyd(mgraph c);
    mgraph* getGraph();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_showroute_clicked();
    void on_minroute_clicked();
    void on_find_clicked();
    void on_exit_clicked();

private:
    Ui::schoolmap *ui;
    mgraph* m_campus;

signals:
    void graphUpdated();
};

#endif // SCHOOLMAP_H
