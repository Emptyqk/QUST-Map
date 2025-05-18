#include "schoolmap.h"
#include "ui_schoolmap.h"
#include "intro.h"
#include "manager.h"
#include "mainwindow.h"
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDialog>
#include <QDebug>
#include <QGraphicsView>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QLabel>
#include <QApplication>

#define Infinity 540
#define MAX 40

mgraph initgraph();
int locatevex(mgraph m,int v);
int allpath(mgraph m,int start,int end);
void path(mgraph m,int p,int n,int k);
QString transtring;

int d[30];
int visited[30];
int D[MaxVertexNum][MaxVertexNum];
int pathh[MaxVertexNum][MaxVertexNum];


schoolmap::schoolmap(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::schoolmap)
{
    m_campus = new mgraph;      // 先分配空间
    *m_campus = initgraph();    // 再用 initgraph() 的内容赋值
    ui->setupUi(this);
    QRegularExpression regx("\\d{2}");//它只对那种由两个数字组成的字符串感兴趣
    QValidator* validator=new QRegularExpressionValidator(regx,this);//
//这行代码创建了一个验证器validator，它会使用刚才那个小侦探regx来工作。以后它就负责检查输入的内容是不是符合两个数字的规则。
    ui->endpoint->setValidator(validator);
    ui->startpoint->setValidator(validator);
}

schoolmap::~schoolmap()
{
    delete m_campus;
    delete ui;
}

mgraph* schoolmap::getGraph()
{
    return m_campus;
}

void schoolmap::updateGraph() {
    emit graphUpdated(); // 触发更新
}

void schoolmap::on_find_clicked()
{
    QString inputText = ui->input->text().trimmed(); // 获取输入并去除空格
    bool ok;
    int num = inputText.toInt(&ok); // 转换为整数

    // 输入验证：非数字或超出范围
    if (!ok || num < 1 || num > 15) {
        QMessageBox::warning(this, "输入错误", "请输入有效的景点序号（1-15）");
        return;
    }

    // 根据序号匹配景点信息
    QString name, description, imagePath;
    switch (num) {
    case 1:
        name = "大门";
        description = "青岛科技大学的大门";
        imagePath = ":/images/1.png"; // 假设图片路径
        break;
    case 2:
        name = "图书馆";
        description = "图书馆，可以在这里读书、自习。借书记得带校园卡。";
        imagePath = ":/images/2.png";
        break;
    case 3:
        name = "湖";
        description = "景色宜人，生态和谐。之前好像有人在这里钓鱼。";
        imagePath = ":/images/3.png";
        break;
    case 4:
        name = "行政楼";
        description = "老师们用来行政办公的场所。";
        imagePath = ":/images/4.png";
        break;
    case 5:
        name = "弘毅楼";
        description = "教学楼，有ABCDE五个区。";
        imagePath = ":/images/5.png";
        break;
    case 6:
        name = "明德楼";
        description = "教学楼，新奇的环形建筑。";
        imagePath = ":/images/6.png";
        break;
    case 7:
        name = "篮球足球排球场";
        description = "可以在这里进行各种球类活动。";
        imagePath = ":/images/7.png";
        break;
    case 8:
        name = "操场";
        description = "平时进行体育活动的地方。";
        imagePath = ":/images/8.png";
        break;
    case 9:
        name = "南四宿舍";
        description = "我们的宿舍楼。";
        imagePath = ":/images/9.png";
        break;
    case 10:
        name = "南苑食堂";
        description = "有三层，饭菜都很好吃。";
        imagePath = ":/images/10.png";
        break;
    case 11:
        name = "南十对面快递站";
        description = "可以在这里拿快递。";
        imagePath = ":/images/11.png";
        break;
    case 12:
        name = "颐海苑";
        description = "含学生澡堂、超市、快递站。";
        imagePath = ":/images/12.png";
        break;
    case 13:
        name = "信院楼";
        description = "我们的学院楼。";
        imagePath = ":/images/13.png";
        break;
    case 14:
        name = "北苑食堂";
        description = "食堂，有三层。";
        imagePath = ":/images/14.png";
        break;
    case 15:
        name = "体育馆";
        description = "可以在这里进行室内活动。";
        imagePath = ":/images/15.png";
        break;
    default:
        return;
    }

    // 创建弹窗并设置内容
    intro *messageWindow = new intro(this);
    QTextBrowser *textBrowser = messageWindow->findChild<QTextBrowser*>("textBrowser");
    QTextBrowser *textBrowser_2 = messageWindow->findChild<QTextBrowser*>("textBrowser_2");
    QLabel *label = messageWindow->findChild<QLabel*>("label");

    QPixmap img;
    if (!img.load(imagePath)) {
        img.load(":/images/default.png"); // 加载失败则使用默认图片
    }else{
        img.load(imagePath);
    }
    label->setPixmap(img.scaled(400, 400, Qt::KeepAspectRatio)); // 调整图片大小
    textBrowser_2->setText(name);
    textBrowser->setText(description);
    messageWindow->show();
}

void schoolmap::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);

    QRect target(0,0,662,662);
    QPixmap umap(":/images/map_2.png");

    painter.drawPixmap(target,umap);

    for (int i = 0; i < m_campus->vexnum; ++i) {
        if (m_campus->vexs[i].accessible)
            painter.setBrush(Qt::green);
        else
            painter.setBrush(Qt::red);
        // painter.drawEllipse(...); // 画结点
    }
}

/*1大门*/
void schoolmap::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        int x=this->mapFromGlobal(QCursor().pos()).x();
        int y=this->mapFromGlobal(QCursor().pos()).y();

        if((x>543 && x<591) &&(y>368 && y<418)){
            intro *messageWindow=new intro(this);

            QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
            QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");

            QPixmap *img=new QPixmap;
            img->load(":/images/1.png");
            QLabel *label=messageWindow->findChild<QLabel *>("label");
            label->setPixmap(*img);
            textBrowser_2->setText(tr("大门"));
            textBrowser->setText(tr("青岛科技大学的大门"));

            messageWindow->show();
        }
    /*2图书馆*/
    else if((x>520 && x<543) &&(y>368 && y<418)){
        intro *messageWindow=new intro(this);

        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/2.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");

        label->setPixmap(*img);
        textBrowser_2->setText(tr("图书馆"));
        textBrowser->setText(tr("图书馆，可以在这里读书、自习。借书记得带校园卡。"));
        messageWindow->show();
    }/*3湖*/
    else if((x>543 && x<591) &&(y>487 && y<560)){
        intro *messageWindow=new intro(this);

        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/3.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("湖"));
        textBrowser->setText(tr("景色宜人，生态和谐。之前好像有人在这里钓鱼。"));
        messageWindow->show();
    }/*4行政楼*/
    else if((x>474 && x<520) &&(y>510 && y<537)){
        intro *messageWindow=new intro(this);

        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/4.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("综合楼"));
        textBrowser->setText(tr("老师们用来行政办公的场所。"));
        messageWindow->show();
    }/*5弘毅楼*/
    else if((x>318 && x<474) &&(y>368 && y<417)){
        intro *messageWindow=new intro(this);

        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/5.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("弘毅楼"));
        textBrowser->setText(tr("教学楼，有ABCDE五个区。"));
        messageWindow->show();
    }/*6明德楼*/
    else if((x>474 && x<520) &&(y>367 && y<432)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/6.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("明德楼"));
        textBrowser->setText(tr("教学楼，新奇的环形建筑。"));

        messageWindow->show();
    }/*7篮球足球排球场*/
    else if((x>350 && x<445) &&(y>487 && y<537)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/7.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("篮球足球排球场"));
        textBrowser->setText(tr("可以在这里进行各种球类活动。"));
        messageWindow->show();
    }/*8操场*/
    else if((x>298 && x<350) &&(y>487 && y<600)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/8.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("操场"));
        textBrowser->setText(tr("平时进行体育活动的地方。"));
        messageWindow->show();
    }/*9南四宿舍*/
    else if((x>255 && x<298) &&(y>487 && y<520)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/9.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("南四宿舍"));
        textBrowser->setText(tr("我们的宿舍楼。"));
        messageWindow->show();
    }/*10南苑食堂*/
    else if((x>255 && x<298) &&(y>416 && y<472)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/10.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("南苑食堂"));
        textBrowser->setText(tr("有三层，饭菜都很好吃。"));
        messageWindow->show();
    }/*11南十对面快递站*/
    else if((x>125 && x<160) &&(y>487 && y<520)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/11.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("南十对面快递站"));
        textBrowser->setText(tr("可以在这里拿快递。"));

        messageWindow->show();
    }/*12颐海苑*/
    else if((x>142 && x<212) &&(y>560 && y<600)){
       intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/12.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("颐海苑"));
        textBrowser->setText(tr("含学生澡堂、超市、快递站。"));
        messageWindow->show();
    } /*13信院楼*/
    else if((x>543 && x<591) &&(y>235 && y<295)){
       intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/13.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("信息科学技术学院"));
        textBrowser->setText(tr("我们的学院楼。"));
        messageWindow->show();
    }/*14北苑食堂*/
    else if((x>460 && x<520) &&(y>145 && y<180)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/14.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("北苑食堂"));
        textBrowser->setText(tr("食堂，有三层。"));
        messageWindow->show();
    }/*15体育馆*/
    else if((x>460 && x<520) &&(y>120 && y<145)){
        intro *messageWindow=new intro(this);
        QTextBrowser *textBrowser=messageWindow->findChild<QTextBrowser *>("textBrowser");
        QTextBrowser *textBrowser_2=messageWindow->findChild<QTextBrowser *>("textBrowser_2");
        QPixmap *img=new QPixmap;
        img->load(":/images/15.png");
        QLabel *label=messageWindow->findChild<QLabel *>("label");
        label->setPixmap(*img);
        textBrowser_2->setText(tr("体育馆"));
        textBrowser->setText(tr("可以在这里进行室内活动。"));
        messageWindow->show();
    }
    }
}




void schoolmap::on_exit_clicked()
{
    this->close();
}

mgraph initgraph(){
    int i,j;
    mgraph m;
    m.vexnum=15;
    m.arcnum=18;
    for(i=0;i<15;i++){
        m.vexs[i].position=i+1;
        m.vexs[i].accessible = true;
    }
    strcpy(m.vexs[0].name,"大门");
    strcpy(m.vexs[1].name,"图书馆");
    strcpy(m.vexs[2].name,"湖");
    strcpy(m.vexs[3].name,"行政楼");
    strcpy(m.vexs[4].name,"弘毅楼");
    strcpy(m.vexs[5].name,"明德楼");
    strcpy(m.vexs[6].name,"篮球足球排球场");
    strcpy(m.vexs[7].name,"操场");
    strcpy(m.vexs[8].name,"南四宿舍");
    strcpy(m.vexs[9].name,"南苑食堂");
    strcpy(m.vexs[10].name,"南十对面快递站");
    strcpy(m.vexs[11].name,"颐海苑");
    strcpy(m.vexs[12].name,"信院楼");
    strcpy(m.vexs[13].name,"北苑食堂");
    strcpy(m.vexs[14].name,"体育馆");
    for(i=0;i<15;i++){
        for(j=0;j<15;j++){
            m.arcs[i][j].adj=Infinity;
        }
    }
    m.arcs[0][1].adj=10;
    m.arcs[0][12].adj=40;
    m.arcs[0][2].adj=30;
    m.arcs[1][5].adj=10;
    m.arcs[2][3].adj=20;
    m.arcs[8][3].adj=90;
    m.arcs[8][7].adj=10;
    m.arcs[8][9].adj=10;
    m.arcs[8][10].adj=40;
    m.arcs[8][11].adj=40;
    m.arcs[6][7].adj=20;
    m.arcs[7][9].adj=30;
    m.arcs[10][11].adj=30;
    m.arcs[4][5].adj=30;
    m.arcs[4][9].adj=30;
    m.arcs[5][9].adj=50;
    m.arcs[12][13].adj=40;
    m.arcs[13][14].adj=10;
    for(i=0;i<15;i++){
        for(j=0;j<15;j++){
            m.arcs[j][i].adj=m.arcs[i][j].adj;
        }
    }
    return m;
}

int locatevex(mgraph m,int v)
{
    int i;
    for(i=0;i<m.vexnum;i++){
        if(v==m.vexs[i].position){
            return i;
        }
    }
    return -1;
}

void schoolmap::path(mgraph m, int p, int n, int k)
{
    int s, length = 0;
    if (d[k] == n) {
        // 检查路径中是否有不可访问结点
        for (int i = 0; i <= k; ++i) {
            if (!m.vexs[d[i]].accessible) {
                QMessageBox::warning(this, "不可访问", "路径中包含不可访问结点！");
                return;
            }
        }
        for (int i = 0; i < k; i++) {
            length += m.arcs[d[i]][d[i+1]].adj;
        }
        if (length > 0) {
            QString transtring;
            for (int i = 0; i <= k; i++) {
                transtring.append(QString::number(m.vexs[d[i]].position) + "->");
            }
            transtring.chop(2); //移除末尾的"->"
            ui->textBrowser->append(transtring);
            ui->textBrowser->append("该条线路长为" + QString::number(length));
        }
    } else {
        for (s = 0; s < m.vexnum; s++) {
            if (m.arcs[d[k]][s].adj < Infinity && !visited[s]) {
                visited[s] = 1;
                d[k+1] = s;
                path(m, p, n, k+1);
                visited[s] = 0;
            }
        }
    }
}

void schoolmap::allpath(mgraph m,int start,int end){
    int k,p,n;
    p=locatevex(m,start);
    n=locatevex(m,end);
    d[0]=p;
    for(k=0;k<15;k++){
        visited[k]=0;
    }
    visited[p]=1;
    path(m,p,n,0);
}

void schoolmap::shortdistance(mgraph m, int start, int end)
{
    int i = locatevex(m, start);
    int j = locatevex(m, end);
    floyd(m); // 调用floyd算法
    int cur = i;
    bool hasInaccessible = false;
    QStringList pathNodes;
    pathNodes << QString::number(m.vexs[cur].position);
    while (pathh[cur][j] != j) {
        int next = pathh[cur][j];
        if (!m.vexs[next].accessible) {
            hasInaccessible = true;
            break;
        }
        pathNodes << QString::number(m.vexs[next].position);
        cur = next;
    }
    if (!hasInaccessible && !m.vexs[j].accessible) {
        hasInaccessible = true;
    }
    if (hasInaccessible) {
        QMessageBox::warning(this, "不可访问", "最短路径中包含不可访问结点！");
        return;
    }
    transtring.clear();
    transtring = pathNodes.join("->") + "->" + QString::number(m.vexs[j].position);
    ui->textBrowser_2->append(transtring);
    ui->textBrowser_2->append("最短路线长为" + QString::number(D[i][j]));
    transtring.clear();
}

void schoolmap::floyd(mgraph m){
    int i,j,k;
    for(i=0;i<15;i++){
        for(j=0;j<15;j++){
            D[i][j]=m.arcs[i][j].adj;
            if(D[i][j]<Infinity && i!=j){
                pathh[i][j]=j;
            }else{
                pathh[i][j]=-1;
            }
        }
    }
    for(k=0;k<15;k++){
        for(i=0;i<15;i++){
            for(j=0;j<15;j++){
                if(D[i][k]+D[k][j]<D[i][j]){
                    D[i][j]=D[i][k]+D[k][j];
                    pathh[i][j]=pathh[i][k];
                }
            }
        }
    }
}

void schoolmap::on_showroute_clicked()
{
    int start = ui->startpoint->text().toInt();
    int end = ui->endpoint->text().toInt();
    ui->textBrowser->clear();
    if ((start >= 1 && start <= 15) && (end >= 1 && end <= 15)) {
        if (!m_campus->vexs[start-1].accessible) {
            QMessageBox::warning(this, "不可访问", QString("起点 %1 不可访问！").arg(start));
            return;
        }
        if (!m_campus->vexs[end-1].accessible) {
            QMessageBox::warning(this, "不可访问", QString("终点 %1 不可访问！").arg(end));
            return;
        }
        allpath(*m_campus, start, end);
    }
}

void schoolmap::on_minroute_clicked()
{
    int end = ui->endpoint->text().toInt();
    int start = ui->startpoint->text().toInt();
    ui->textBrowser_2->clear();
    if ((start >= 1 && start <= 15) && (end >= 1 && end <= 15)) {
        if (!m_campus->vexs[start-1].accessible) {
            QMessageBox::warning(this, "不可访问", QString("起点 %1 不可访问！").arg(start));
            return;
        }
        if (!m_campus->vexs[end-1].accessible) {
            QMessageBox::warning(this, "不可访问", QString("终点 %1 不可访问！").arg(end));
            return;
        }
        shortdistance(*m_campus, start, end);
    }
}

void schoolmap::refresh() {
    update(); // 触发重绘
}

void schoolmap::forceRefresh() {
    update();
}

