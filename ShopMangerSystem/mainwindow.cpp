#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("首页");//设置窗口名字
    signal(SIGPIPE,SIG_IGN);
    while (true)
    {
        client =shared_ptr<SockClient>(new SockClient("127.0.0.1",8888));
        if(client->connectToHost()==-1)
        {
            //超时  你可以在这里设置重传次数
        }
        else
        {
            break;
        }
    }
//登录界面显示
    client->start();
    m_load.show();
    m_load.setSendSer(client);

    m_chargeViw.setSendSer(client);
//    m_chargeViw.show();
    this->hide();

//注册页面显示
    m_log.setSendSer(client);
//   m_log.show();


    connect(&m_load,&LoadView::load,[=]()
                                    {
                                        m_chargeViw.show();
                                        m_load.hide();
                                    }
            );
    connect(&m_load,&LoadView::logon,[=]()
                                    {
                                        m_log.show();
                                        m_load.hide();
                                    }
            );
    connect(&m_log,&Logon::logonSuccess,[=]()
                                    {
                                        m_log.hide();
                                        m_load.show();
                                    }
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSendSer(shared_ptr<SockClient> sendSer)
{
    client=sendSer;
}
