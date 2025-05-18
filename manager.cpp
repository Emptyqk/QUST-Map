#include "manager.h"
#include "ui_manager.h"
#include "struct.h"
#include "schoolmap.h"
#include <QIntValidator>
#include <QMessageBox>

#define Infinity 540

manager::manager(schoolmap* schoolmapPtr, QWidget *parent) // 修改构造函数
    : QWidget(parent), ui(new Ui::manager), m_schoolmap(schoolmapPtr) {
    ui->setupUi(this);
    ui->nodeIdLineEdit->setValidator(new QIntValidator(1, MaxVertexNum, this));
    ui->startNodeLineEdit->setValidator(new QIntValidator(1, MaxVertexNum, this));
    ui->endNodeLineEdit->setValidator(new QIntValidator(1, MaxVertexNum, this));
    ui->distanceLineEdit->setValidator(new QIntValidator(0, 9999, this));

    ui->enableCheckBox->setChecked(true);
    if (m_schoolmap) {
        setGraph(m_schoolmap->getGraph());
    }
}

manager::~manager()
{
    delete ui;
}
void manager::setGraph(mgraph *graph){
    m_graph = graph;
}

void manager::on_pushButton_clicked()
{
    if (!m_graph) {
        QMessageBox::warning(this, "错误", "图数据未初始化");
        return;
    }

    const int nodeId = ui->nodeIdLineEdit->text().toInt();
    const bool isEnabled = ui->enableCheckBox->isChecked();

    if (nodeId < 1 || nodeId > m_graph->vexnum) {
        QMessageBox::warning(this, "错误", "无效的结点编号");
        return;
    }

    // 更新结点访问状态
    m_graph->vexs[nodeId-1].accessible = isEnabled;

    // 显示操作结果
    const QString status = isEnabled ? "已启用" : "已禁用";
    QMessageBox::information(this, "成功", QString("结点 %1 已%2访问").arg(nodeId).arg(status));

    m_graph->vexs[nodeId-1].accessible = isEnabled;
    if (m_schoolmap) {
        m_schoolmap->updateGraph(); // 触发界面更新
        m_schoolmap->forceRefresh(); // 强制刷新
    }
}


void manager::on_pushButton_2_clicked()
{
    if (!m_graph) {
        QMessageBox::warning(this, "错误", "图数据未初始化");
        return;
    }

    const int startId = ui->startNodeLineEdit->text().toInt();
    const int endId = ui->endNodeLineEdit->text().toInt();
    const int distance = ui->distanceLineEdit->text().toInt();

    if (startId < 1 || startId > m_graph->vexnum ||
        endId < 1 || endId > m_graph->vexnum) {
        QMessageBox::warning(this, "错误", "无效的结点编号");
        return;
    }

    if (startId == endId) {
        QMessageBox::warning(this, "错误", "起始结点与结束结点不能相同");
        return;
    }

    // 检查两个节点之间是否原本就存在连接
    if (m_graph->arcs[startId - 1][endId - 1].adj == Infinity) {
        QMessageBox::warning(this, "错误", "这两个结点之间没有连接，不能修改距离");
        return;
    }

    // 更新边的距离（无向图）
    m_graph->arcs[startId - 1][endId - 1].adj = distance;
    m_graph->arcs[endId - 1][startId - 1].adj = distance;

    QMessageBox::information(this, "成功",
                             QString("结点 %1 到 %2 的距离已更新为 %3").arg(startId).arg(endId).arg(distance));

    if (m_schoolmap) {
        m_schoolmap->updateGraph(); // 触发界面更新
        m_schoolmap->forceRefresh(); // 强制刷新
    }
}
