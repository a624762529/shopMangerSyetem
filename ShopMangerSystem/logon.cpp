#include "logon.h"
#include "ui_logon.h"

Logon::Logon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logon)
{
    ui->setupUi(this);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_rpassword->setEchoMode(QLineEdit::Password);
    setWindowTitle("注册");
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());

    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

}

Logon::~Logon()
{
    delete ui;
}

void Logon::on_pushButton_log_clicked()
{
    string count    = ui->lineEdit_count->text().toStdString();
    if(count.size()==0)
    {
        QMessageBox::information(NULL, "error", "没有输入账号",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
        return;
    }
    string password = ui->lineEdit_password->text().toStdString();
    if(password.size()==0)
    {
        QMessageBox::information(NULL, "error", "没有输入密码",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
        return;
    }
    string rpassword= ui->lineEdit_rpassword->text().toStdString();

    if(password!=rpassword)
    {
        QMessageBox::information(NULL, "error", "前后两次密码输入不一致",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
        return;
    }
    else
    {
       //发送给服务端
        if(ui->radioButton_charger->isChecked())
        {
            cout<<"管理员注册"<<endl;
            Login sendlog(count,password);
            client->writeInfo((char *)&sendlog,sizeof(sendlog));
            SendBack *back=reinterpret_cast<SendBack *>(new char[4096]);

            client->readInfo(reinterpret_cast<char*>(back),4096);
            if(back->m_tag)
            {
                QMessageBox::information(NULL, "success", "注册成功",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                return;
            }
            else
            {
                QMessageBox::information(NULL, "注册失败", "用户名相同",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                return;
            }
            delete []back;
        }
        else if(ui->radioButton_solder->isChecked())
        {
            cout<<"销售员注册"<<endl;
            SolderLogin sendlog(count,password);
            client->writeInfo((char *)&sendlog,sizeof(sendlog));
            SendBack *back=reinterpret_cast<SendBack *>(new char[4096]);

            client->readInfo(reinterpret_cast<char*>(back),4096);
            if(back->m_tag)
            {
                QMessageBox::information(NULL, "success", "注册成功",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                return;
            }
            else
            {
                QMessageBox::information(NULL, "注册失败", "用户名相同",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                return;
            }
            delete []back;
        }
        else
        {
            QMessageBox::information(NULL, "load fail", "选择身份",
                                     QMessageBox::Yes,
                                     QMessageBox::Yes);
            return;
        }
     }

}

void Logon::setSendSer(shared_ptr<SockClient> cli)
{
    client=cli;
}

void Logon::on_pushButton_clicked()
{
     emit logonSuccess();
}
