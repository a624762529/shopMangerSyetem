#include "chargeview.h"
#include "ui_chargeview.h"

ChargeView::ChargeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChargeView)
{
    ui->setupUi(this);

    ui->addItem->setToolTip(QString("进货"));
    setTableViewHead();
}

ChargeView::~ChargeView()
{
    delete ui;
}

void ChargeView::setTableViewHead()
{

    ui->tableViewShowAll->setModel(&model_ShowAll);
    this->model_ShowAll.setHorizontalHeaderItem(0, new QStandardItem("名字") );
    this->model_ShowAll.setHorizontalHeaderItem(1, new QStandardItem("价格"));
    this->model_ShowAll.setHorizontalHeaderItem(2, new QStandardItem("数目"));
    this->model_ShowAll.setHorizontalHeaderItem(3, new QStandardItem("类型"));


    ui->tableViewFind->setModel(&model_Find);
    this->model_Find.setHorizontalHeaderItem(0, new QStandardItem("名字") );
    this->model_Find.setHorizontalHeaderItem(1, new QStandardItem("价格"));
    this->model_Find.setHorizontalHeaderItem(2, new QStandardItem("数目"));
    this->model_Find.setHorizontalHeaderItem(3, new QStandardItem("类型"));

    ui->tableViewShowAllStore->setModel(&model_ShowAllStore);
    this->model_ShowAllStore.setHorizontalHeaderItem(0, new QStandardItem("仓库名") );


    ui->tableViewCls->setModel(&model_Cls);
    this->model_Cls.setHorizontalHeaderItem(0, new QStandardItem("类型") );
    this->model_Cls.setHorizontalHeaderItem(1, new QStandardItem("数目"));


    ui->tableViewSailCensus->setModel(&model_SailCensus);
    this->model_SailCensus.setHorizontalHeaderItem(0, new QStandardItem("名字") );
    this->model_SailCensus.setHorizontalHeaderItem(1, new QStandardItem("价格"));
    this->model_SailCensus.setHorizontalHeaderItem(2, new QStandardItem("数目"));
    this->model_SailCensus.setHorizontalHeaderItem(3, new QStandardItem("仓库"));


    ui->tableViewEarlyWarn->setModel(&model_EarlyWarn);
    this->model_EarlyWarn.setHorizontalHeaderItem(0, new QStandardItem("名字") );
    this->model_EarlyWarn.setHorizontalHeaderItem(1, new QStandardItem("价格"));
    this->model_EarlyWarn.setHorizontalHeaderItem(2, new QStandardItem("数目"));
    this->model_EarlyWarn.setHorizontalHeaderItem(3, new QStandardItem("类型"));
}


//商品的销售
void ChargeView::on_salButton_clicked()
{

    string goods_name=ui->sal_name->text().toStdString();
    int goods_qua=ui->sal_qua->text().toInt();
    int goods_price=ui->sal_price->text().toInt();
    string goods_store= ui->sal_store->text().toStdString();
    // Goods

    Goods good;
    memset(&good,0,sizeof(good));

    good.setGoods(goods_name,goods_price
                  ,goods_qua,goods_store,"");

    SalItem item(good);
    client->writeInfo(reinterpret_cast<char*>(&item),sizeof(item));

    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
       QMessageBox::question(NULL, "sail success", "交易成功",
                             QMessageBox::Yes , QMessageBox::Yes);
    }
    else
    {
       QMessageBox::question(NULL, "sail fail", "交易失败",
                              QMessageBox::Yes , QMessageBox::Yes);
    }
}


void ChargeView::setSendSer( shared_ptr<SockClient> cli)
{
    this->client=cli;
}

//删除商品
void ChargeView::on_pushButton_deleteting_clicked()
{
    string del_store=ui->delete_store->text().toStdString();
    string del_name=ui->delete_item_name->text().toStdString();
    Goods good;
    memset(&good,0,sizeof(good));
    //setGoods(string name,int price,int qua,string store,string type)
    good.setGoods(del_name,-1,-1,del_store,"");
    DeleteGoods del(good);

    client->writeInfo(reinterpret_cast<char*>(&del),sizeof(del));

    char buf[4096];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        QMessageBox::information(NULL, "删除商品成功", "success",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::information(NULL, "删除商品失败", "fail",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
}

//改变商品
void ChargeView::on_pushButton_change_clicked()
{
    string store=ui->change_store_2->text().toStdString();
    string b_name =ui->change_name_2->text().toStdString();

    Goods before;
    memset(&before,0,sizeof(before));
    before.setGoods(b_name,-1,-1,store,"");

    string a_name =ui->change_to_name_2->text().toStdString();
    int    a_price=ui->change_to_price_2->text().toInt();
    int    a_qua  =ui->change_to_qua_2->text().toInt();
    string a_type= ui->change_to_type_2->text().toStdString();
    Goods after;
    memset(&after,0,sizeof(after));
    after.setGoods(a_name,a_price,a_qua,store,a_type);

    ChangeGoods chg_goods(before,after);
    //讲参数包发送给服务端
    client->writeInfo(reinterpret_cast<char*>(&chg_goods),sizeof(chg_goods));

    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        QMessageBox::information(NULL, "修改成功", "success",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::information(NULL, "修改失败", "fail",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
}

//增加商品
void ChargeView::on_pushButton_addInfo_clicked()
{
    string add_name=ui->add_name->text().toStdString();      //获取要添加的商品的名字
    int    add_price   =ui->add_price->text().toInt();       //获取要添加商品的价格
    int    add_qua   =ui->add_qua->text().toInt();
    string add_store   =ui->add_store->text().toStdString(); //获取要添加商品要存储的位置
    string add_type    = ui->add_type->text().toStdString(); //获取要添加商品的类型

    Goods addItem;
    memset(&addItem,0,sizeof(addItem));
    addItem.setGoods(add_name,add_price,add_qua,add_store,add_type);

    AddGoods add(addItem);
    client->writeInfo(reinterpret_cast<char*>(&add),sizeof(add));

    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        QMessageBox::information(NULL, "增加商品成功", "success",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::information(NULL, "增加商品失败", "fail",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
}

//寻找商品
void ChargeView::on_find_button_clicked()
{
    string search_name= ui->search_name->text().toStdString();
    string search_store= ui->search_store->text().toStdString();

    Goods goods;
    memset(&goods,0,sizeof(goods));
    goods.setGoods(search_name,-1,-1,search_store,"");
    Find fd_sd(goods);
    client->writeInfo(reinterpret_cast<char*>(&fd_sd),sizeof(fd_sd));

    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(&back->m_ch);
        int line=back->m_num;
        setModelLine(model_Find,line);
        for(int i=0;i<line;i++)
        {
            setModelGoods(model_Find,good,i);
            good++;
        }
    }
    else
    {
        //ui->search_show->setText(QString("没有发现"));
        QMessageBox::information(NULL, "find fail", "没有发现",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);

    }
}

//调货
void ChargeView::on_pushButton_2_clicked()
{
    //补货
    string name= ui->pack_name->text().toStdString();
    string store=ui->pack_store->text().toStdString();
    string type=ui->pack_type->text().toStdString();
    int qua=     ui->pack_qua->text().toInt();
    int price=ui->pack_price->text().toInt();

    Goods from_good;
    memset(&from_good,0,sizeof(from_good));
    from_good.setGoods(name,price,qua,store,type);

    string to_name   =name;
    int    to_qua    =qua;
    int    to_price  =price;
    string to_store  =ui->pack_to_store->text().toStdString();

    Goods to_good;
    memset(&to_good,0,sizeof(to_good));
    to_good.setGoods(to_name,to_price,to_qua,to_store,type);

    Replenish req(from_good,to_good);
    client->writeInfo(reinterpret_cast<char*>(&req),sizeof(req));

    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        QMessageBox::information(NULL, "调货成功", "success",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::information(NULL, "调货失败", "fail",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
}

//查询全部仓库
void ChargeView::on_button_showstr_clicked()
{
    FindAll fdall;
    client->writeInfo(reinterpret_cast<char*>(&fdall),sizeof(fdall));
    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        Store *store=reinterpret_cast<Store*>(&back->m_ch);
        int line=back->m_num;
        setModelLine(model_ShowAllStore,line);
        for(int i=0;i<line;i++)
        {
            setModelStore(model_ShowAllStore,store,i);
            store++;
        }
    }
    else
    {
        ui->show_store->setText(QString("没有仓库"));
    }
}

//删除仓库
void ChargeView::on_pushButton_clicked()
{
    DeleteStore de_store;
    string del_store= ui->lineEdit_delstore->text().toStdString();
    de_store.setStore(del_store);
    client->writeInfo(reinterpret_cast<char*>(&de_store),sizeof(de_store));

    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        QMessageBox::information(NULL, "删除仓库成功", "success",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::information(NULL, "删除仓库失败", "fail",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
}

//增加仓库
void ChargeView::on_pushButton_addStre_clicked()
{
    string ad_store_name= ui->line_ed_del_store->text().toStdString();
    AddStore add_store;
    add_store.setStore(ad_store_name);

    client->writeInfo(reinterpret_cast<char*>(&add_store),sizeof(add_store));
    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        QMessageBox::information(NULL, "增加仓库成功", "success",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::information(NULL, "增加仓库失败", "fail",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
}


//分类统计
void ChargeView::on_butt_SalAll_clicked()
{
    string store=ui->lineEdit_FindStore->text().toStdString();
    ClsGoods cls;
    cls.setStore(store);

    client->writeInfo(reinterpret_cast<char*>(&cls),sizeof(cls));
    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        //ui->showSalAll->setText(&back->m_ch);

        RetCls *cls= reinterpret_cast<RetCls*> (&back->m_ch);
        int line=back->m_num;
        setModelLine(model_Cls,line);
        for(int i=0;i<line;i++)
        {
            setModelCls(model_Cls,cls,i);
            cls++;
        }
    }
    else
    {
        QMessageBox::information(NULL, "仓库没有东西", "无",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

//销售统计
void ChargeView::on_buttonClassSalar_clicked()
{
    string mounth= ui->lineEdit_mouth->text().toStdString();
    if(mounth.size()<2)
    {
        mounth="0"+mounth;
    }
    string year=ui->lineEdit_year->text().toStdString();
    string day=ui->lineEdit_day->text().toStdString();
    if(day.size()<2)
    {
        day="0"+day;
    }
    string data=year+"-"+mounth+"-"+day+" ";

    SoldCensus soil;
    soil.setData(data);
    client->writeInfo(reinterpret_cast<char*>(&soil),sizeof(soil));

    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    int all_sail=0;
    if(back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(&back->m_ch);
        int line=back->m_num;
        setModelLine(model_SailCensus,line);
        for(int i=0;i<line;i++)
        {
            all_sail+=good->m_price * good->m_qua;
            setModelSoldCe(model_SailCensus,good,i);
            good++;

        }
        model_SailCensus.setItem(line, 0, new QStandardItem( QString        ("销售总额")  ));
        model_SailCensus.setItem(line, 1, new QStandardItem( QString::number(all_sail)  ));
    }
    else
    {
        QMessageBox::information(NULL, "今天没有销售货物", "无",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

//销售预警
void ChargeView::on_buttonEarlyWarn_clicked()
{
    string store=ui->line_ed_EarlyWarn->text().toStdString();
    int    num  =ui->line_ed_EarlyWarnNum->text().toInt();
    EarlyWarning warn;
    warn.setStore(store,num);

    client->writeInfo(reinterpret_cast<char*>(&warn),sizeof(warn));
    char buf[64*1024];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(&back->m_ch);
        int line=back->m_num;
        setModelLine(model_EarlyWarn,line);
        for(int i=0;i<line;i++)
        {
            setModelGoods(model_EarlyWarn,good,i);
            good++;
        }
    }
    else
    {
        //ui->showWarm->setText("no warning");
        QMessageBox::information(NULL, "没有预警", "无",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    }
}


//查询该仓库的全部商品
void ChargeView::on_buttonClassSalar_2_clicked()
{
    string store= ui->lineEdit_Showstore->text().toStdString();
    ShowStoreAll sw;
    //显示所有的商品

    sw.setStore(store);
    client->writeInfo(reinterpret_cast<char*>(&sw),sizeof(sw));

    char buf[4096];
    int ret=client->readInfo(buf,4096);
    if(ret==-1)
    {
        perror("read error");
    }
    SendBack *back=reinterpret_cast<SendBack*>(buf);
    if(back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(&back->m_ch);
        int line=back->m_num;
        setModelLine(model_ShowAll,line);
        for(int i=0;i<line;i++)
        {
            setModelGoods(model_ShowAll,good,i);
            good++;
        }
    }
    else
    {
        QMessageBox::information(NULL, "find fail", "仓库不存在|没有数据",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
    }
}

void ChargeView::setModelGoods(QStandardItemModel &mode,Goods *good,int line)
{
    mode.setItem(line, 0, new QStandardItem( QString        (good->m_name)  ));
    mode.setItem(line, 1, new QStandardItem( QString::number(good->m_price) ));
    mode.setItem(line, 2, new QStandardItem( QString::number(good->m_qua  ) ));
    mode.setItem(line, 3, new QStandardItem( QString        (good->m_type ) ));
}

void ChargeView::setModelSoldCe(QStandardItemModel &mode,Goods *good,int line)
{
    mode.setItem(line, 0, new QStandardItem( QString        (good->m_name)  ));
    mode.setItem(line, 1, new QStandardItem( QString::number(good->m_price) ));
    mode.setItem(line, 2, new QStandardItem( QString::number(good->m_qua  ) ));
    mode.setItem(line, 3, new QStandardItem( QString        (good->m_store) ));
}

void ChargeView::setModelLine(QStandardItemModel &mode,int line)
{
    mode.setRowCount(line);
    for(int i=0;i<line;i++)
    {
        QString num_line=QString("%1:").arg(i);
        mode.setHeaderData(i,Qt::Vertical, num_line);
    }
}

void ChargeView::setModelStore(QStandardItemModel &mode,Store *store,int line)
{
    mode.setItem(line, 0, new QStandardItem( QString        (store->arry_name)  ));
}

void ChargeView::setModelCls  (QStandardItemModel &mode,RetCls *cls,int line)
{
    mode.setItem(line, 0, new QStandardItem( QString        (cls->type)  ));
    mode.setItem(line, 1, new QStandardItem( QString::number(cls->size)  ));
}

void ChargeView::setModelSoldCent  (QStandardItemModel &mode,Goods *good,int line)
{
    mode.setItem(line, 0, new QStandardItem( QString        (good->m_name)  ));
    mode.setItem(line, 1, new QStandardItem( QString::number(good->m_price) ));
    mode.setItem(line, 2, new QStandardItem( QString::number(good->m_qua  ) ));
    mode.setItem(line, 3, new QStandardItem( QString        (good->m_type ) ));
}
