#include "admin.h"
#include "ui_admin.h"
#include "manager.h"
#include "mainwindow.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
const QString ADMIN_FILE_PATH = "D:/QUSTMAP/QUSTMAP/admin.txt";

Admin::Admin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Admin)
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_login_clicked()
{
    QString inputusername1 =ui->Username->text();
    QString inputpassword1 =ui->Password->text();
    QFile f1(ADMIN_FILE_PATH);
    if(f1.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&f1);
        bool found1=false;
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList admin=line.split(",");
            if(admin.size()==2){
                QString stored_adminusername = admin[0];
                QString stored_adminpassword = admin[1];
                if(inputusername1 == stored_adminusername && inputpassword1 == stored_adminpassword){
                    found1=true;
                    break;
                }
            }
        }
        f1.close();
        if (found1){
            QMessageBox::information(this, "登录成功", "欢迎登录！");
            MainWindow* mainWin = nullptr;
            foreach(QWidget *w, QApplication::topLevelWidgets()) {
                mainWin = qobject_cast<MainWindow*>(w);
                if (mainWin) break;
            }
            schoolmap* schoolMap = nullptr;
            if (mainWin) {
                schoolMap = mainWin->getCurrentSchoolMap();
            }
            if (!schoolMap) {
                // 这里理论上不会走到
                schoolMap = new schoolmap();
            }
            manager* m = new manager(schoolMap);
            this->close();
            m->show();
        }else{
            QMessageBox::warning(this, "登录失败", "用户名或密码错误，请重新输入。");
        }
    }
}


void Admin::on_register_2_clicked()
{
    QString reg_username = ui->Username->text();
    QString reg_password = ui->Password->text();

    if (reg_username.isEmpty() || reg_password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名和密码不能为空，请重新输入。");
        return;
    }
    QFile checkFile(ADMIN_FILE_PATH);
    if (checkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&checkFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() >= 2 && fields[0] == reg_username) {
                QMessageBox::warning(this, "注册失败", "用户名已存在，请选择其他用户名。");
                checkFile.close();
                return;
            }
        }
        checkFile.close();
    }
    QFile file(ADMIN_FILE_PATH);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << reg_username << "," << reg_password << "\n";
        file.close();
        QMessageBox::information(this, "注册成功", "注册成功，请登录。");
        this->close();
    } else {
        QMessageBox::critical(this, "错误", "无法打开用户信息文件进行写入操作。");
    }
}


void Admin::on_exit_clicked()
{
    this->close();
}










