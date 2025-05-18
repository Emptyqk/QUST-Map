#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include "struct.h"
#include "schoolmap.h"

namespace Ui {
class manager;
}
class schoolmap;

class manager : public QWidget
{
    Q_OBJECT

public:
    explicit manager(schoolmap* schoolmapPtr, QWidget *parent = nullptr);
    ~manager();
    void setGraph(mgraph* graph);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::manager *ui;
    mgraph* m_graph;
    schoolmap* m_schoolmap;
};

#endif // MANAGER_H
