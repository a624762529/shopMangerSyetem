#include "loadview.h"
#include "ui_loadview.h"

LoadView::LoadView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadView)
{
    ui->setupUi(this);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

LoadView::~LoadView()
{
    delete ui;
}

void LoadView::on_pushButton_clicked()
{
    string count= ui->lineEdit_count->text().toStdString();
    string password= ui->lineEdit_password->text().toStdString();

    if(ui->radioButton_charger->isChecked())
    {
        Load load;
        load.setInfo(count,password);
        cli->writeInfo(reinterpret_cast<char*>(&load),sizeof(load));
        char buf[4096];
        int ret=cli->readInfo(buf,sizeof(buf));
        SendBack *back=reinterpret_cast<SendBack *>(buf);
        if(back->m_tag)
        {
            cout<<"load success"<<endl;
            status ="charger";
            emit this->load();
        }
        else
        {
            QMessageBox::information(NULL, "load fail", "账号密码错误",
                                     QMessageBox::Yes,
                                     QMessageBox::Yes);
            return;
        }
    }
    else if(ui->radioButton_solder->isChecked())
    {
        SolderLoad load;
        load.setInfo(count,password);
        cli->writeInfo(reinterpret_cast<char*>(&load),sizeof(load));
        char buf[4096];
        int ret=cli->readInfo(buf,sizeof(buf));
        SendBack *back=reinterpret_cast<SendBack *>(buf);
        if(back->m_tag)
        {
            status ="solder";
            emit this->load();
        }
        else
        {
            QMessageBox::information(NULL, "load fail", "账号密码错误",
                                     QMessageBox::Yes,
                                     QMessageBox::Yes);
            return;
        }
        emit sig_SolderLoad();
    }
    else
    {
        QMessageBox::information(NULL, "load fail", "选择身份",
                                 QMessageBox::Yes,
                                 QMessageBox::Yes);
        return;
    }
    user_name=count;
}

void LoadView::setSendSer(shared_ptr<SockClient> cli)
{
    this->cli=cli;
}


//注册
void LoadView::on_pushButton_2_clicked()
{
    //发出注册信号
    emit this->logon();
}
