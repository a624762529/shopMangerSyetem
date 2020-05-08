#include "chargeview.h"
#include "ui_chargeview.h"

ChargeView::ChargeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChargeView)
{
    ui->setupUi(this);

    //ui->addItem->setToolTip(QString("进货"));
    setTableViewHead();

    m_pointout.insert(pair<string,int>("零食",30));
    m_pointout.insert(pair<string,int>("书籍",30));
    m_pointout.insert(pair<string,int>("蔬菜",30));
    m_pointout.insert(pair<string,int>("水果",3));
    m_pointout.insert(pair<string,int>("饮料",30));
    m_pointout.insert(pair<string,int>("烟",30));
}

bool ChargeView::cmpPointOut(string type,int leftval)
{
    auto ret=m_pointout.find(type);
    if(ret==m_pointout.end())
    {
        return !(leftval<40);
    }
    if(ret->second>leftval)
    {
        return false;
    }
    else
    {
        return true;
    }
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
    int ret=QMessageBox::question(NULL, "question", "确定要销售吗",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret==QMessageBox::Yes)
    {
        string goods_name=ui->sal_name->text().toStdString();
        int    goods_qua=ui->sal_qua->text().toInt();
        string goods_store= ui->sal_store->text().toStdString();

        Goods good;
        memset(&good,0,sizeof(good));
        good.setGoods(goods_name,-1
                      ,goods_qua,goods_store,"");

        SalItem item(good);
        //使用阻塞式的写入操作
        client->writeInfo(reinterpret_cast<char*>(&item),sizeof(item));

        //读取首部信息
        int len_head=sizeof(SendBack)-3;
        char*head=stableReadInfo(len_head);
        SendBack*back=reinterpret_cast<SendBack*>(head);
        int message_len=back->m_len-len_head;
        char*info=stableReadInfo(message_len);

        if(back->m_tag)
        {
            SalItemRetBack *data=reinterpret_cast<SalItemRetBack *>
                    (info);
            int leftover=data->left_qua - goods_qua;
            if(cmpPointOut((char*)data->arry,leftover))
            {
                QMessageBox::question(NULL, "sail success", "交易成功",
                                      QMessageBox::Yes , QMessageBox::Yes);
            }
            else
            {
                QString info=QString("剩下的的数目是:%1").arg(QString::number(leftover));
                QMessageBox::question(NULL,"WARNING",info,
                                      QMessageBox::Yes , QMessageBox::Yes);
            }
            return;
        }
        else
        {

                 QMessageBox::question(NULL, "sail fail", "交易失败",
                                  QMessageBox::Yes , QMessageBox::Yes);
        }
        free(head);
        free(info);
    }
}


void ChargeView::setSendSer( shared_ptr<SockClient> cli)
{
    this->client=cli;
}

//删除商品
void ChargeView::on_pushButton_deleteting_clicked()
{
    int ret=QMessageBox::question(NULL, "question", "确定要删除吗", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret==QMessageBox::Yes)
    {
        string del_store=ui->delete_store->text().toStdString();
        string del_name=ui->delete_item_name->text().toStdString();
        Goods good;
        memset(&good,0,sizeof(good));
        good.setGoods(del_name,-1,-1,del_store,"");
        DeleteGoods del(good);
        client->writeInfo(reinterpret_cast<char*>(&del),sizeof(del));


        int len_head=sizeof(SendBack)-3;
        char*head=stableReadInfo(len_head);
        SendBack*back=reinterpret_cast<SendBack*>(head);
        int message_len=back->m_len-len_head;
        char*info=stableReadInfo(message_len);



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

        free(head);
        free(info);
    }
}

//改变商品
void ChargeView::on_pushButton_change_clicked()
{
    int ret=QMessageBox::question(NULL, "question", "确定要更改吗", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret==QMessageBox::Yes)
    {
        string store=ui->change_store_2->text().toStdString();
        string b_name =ui->change_name_2->text().toStdString();

        Goods before;
        memset(&before,0,sizeof(before));
        before.setGoods(b_name,-1,-1,store,"");

        string a_name =ui->change_to_name_2->text().toStdString();
        int    a_price=ui->change_to_price_2->text().toInt();
        int    a_qua  =ui->change_to_qua_2->text().toInt();
        string a_type= ui->change_itemtype->currentText().toStdString();

        Goods after;
        memset(&after,0,sizeof(after));
        after.setGoods(a_name,a_price,a_qua,store,a_type);

        ChangeGoods chg_goods(before,after);
        //讲参数包发送给服务端
        client->writeInfo(reinterpret_cast<char*>(&chg_goods),sizeof(chg_goods));

        int len_head=sizeof(SendBack)-3;
        char*head=stableReadInfo(len_head);
        SendBack*back=reinterpret_cast<SendBack*>(head);
        int message_len=back->m_len-len_head;
        char*info=stableReadInfo(message_len);

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

        free(info);free(head);
    }
}

//增加商品
void ChargeView::on_pushButton_addInfo_clicked()
{

    int ret=QMessageBox::question(NULL, "question", "确定要添加吗", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret==QMessageBox::Yes)
    {
        string add_name=ui->add_name->text().toStdString();      //获取要添加的商品的名字
        int    add_price   =ui->add_price->text().toInt();       //获取要添加商品的价格
        int    add_qua   =ui->add_qua->text().toInt();
        string add_store   =ui->add_store->text().toStdString(); //获取要添加商品要存储的位置
        string add_type    = ui->add_itemtype->currentText().toStdString(); //获取要添加商品的类型

        Goods addItem;
        memset(&addItem,0,sizeof(addItem));
        addItem.setGoods(add_name,add_price,add_qua,add_store,add_type);

        AddGoods add(addItem);
        client->writeInfo(reinterpret_cast<char*>(&add),sizeof(add));

        int len_head=sizeof(SendBack)-3;
        char*head=stableReadInfo(len_head);
        SendBack*back=reinterpret_cast<SendBack*>(head);
        int message_len=back->m_len-len_head;
        char*info=stableReadInfo(message_len);

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
        free(head);
        free(info);
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

    int len_head=sizeof(SendBack)-3;
    char*head=stableReadInfo(len_head);
    SendBack*back=reinterpret_cast<SendBack*>(head);
    int message_len=back->m_len-len_head;
    char*info=stableReadInfo(message_len);



    if(back&&back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(info);
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
    free(head);
    free(info);
}

//调货
void ChargeView::on_pushButton_2_clicked()
{
    //补货
    int ret=QMessageBox::question(NULL, "question", "确定要补货吗", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret==QMessageBox::Yes)
    {
    string name= ui->pack_name->text().toStdString();
    string store=ui->pack_store->text().toStdString();
    string type=ui->packing_itemtype->currentText().toStdString();
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

    int len_head=sizeof(SendBack)-3;
    char*head=stableReadInfo(len_head);
    SendBack*back=reinterpret_cast<SendBack*>(head);
    int message_len=back->m_len-len_head;
    char*info=stableReadInfo(message_len);

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
    free(head);
    free(info);
    }
}

//查询全部仓库
void ChargeView::on_button_showstr_clicked()
{
    FindAll fdall;
    client->writeInfo(reinterpret_cast<char*>(&fdall),sizeof(fdall));

    int len_head=sizeof(SendBack)-3;
    char*head=stableReadInfo(len_head);
    SendBack*back=reinterpret_cast<SendBack*>(head);
    int message_len=back->m_len-len_head;
    char*info=stableReadInfo(message_len);

    if(back->m_tag)
    {
        Store *store=reinterpret_cast<Store*>(info);
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
        QMessageBox::question(NULL, "err", "没有仓库",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    free(head);free(info);
}

//删除仓库
void ChargeView::on_pushButton_clicked()
{
    int ret=QMessageBox::question(NULL, "question", "确定要删除仓库吗", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret==QMessageBox::Yes)
    {
        DeleteStore de_store;
        string del_store= ui->lineEdit_delstore->text().toStdString();
        de_store.setStore(del_store);
        client->writeInfo(reinterpret_cast<char*>(&de_store),sizeof(de_store));

        int len_head=sizeof(SendBack)-3;
        char*head=stableReadInfo(len_head);
        SendBack*back=reinterpret_cast<SendBack*>(head);
        int message_len=back->m_len-len_head;
        char*info=stableReadInfo(message_len);

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

        free(head);free(info);
    }
}

//增加仓库
void ChargeView::on_pushButton_addStre_clicked()
{
    int ret=QMessageBox::question(NULL, "question", "确定要添加仓库吗", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret==QMessageBox::Yes)
    {
        string ad_store_name= ui->line_ed_del_store->text().toStdString();
        AddStore add_store;
        add_store.setStore(ad_store_name);

        client->writeInfo(reinterpret_cast<char*>(&add_store),sizeof(add_store));

        int len_head=sizeof(SendBack)-3;
        char*head=stableReadInfo(len_head);
        SendBack*back=reinterpret_cast<SendBack*>(head);
        int message_len=back->m_len-len_head;
        char*info=stableReadInfo(message_len);
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
        free(head);free(info);
    }
}


//分类统计
void ChargeView::on_butt_SalAll_clicked()
{
    string store=ui->lineEdit_FindStore->text().toStdString();
    ClsGoods cls;
    cls.setStore(store);

    client->writeInfo(reinterpret_cast<char*>(&cls),sizeof(cls));

    int len_head=sizeof(SendBack)-3;
    char*head=stableReadInfo(len_head);
    SendBack*back=reinterpret_cast<SendBack*>(head);
    int message_len=back->m_len-len_head;
    char*info=stableReadInfo(message_len);

    if(back->m_tag)
    {
        RetCls *cls= reinterpret_cast<RetCls*> (info);
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
    free(head);free(info);
}
void func_gettimer(char *local_time)
{

    char Year[6]  = {0};
    char Month[4] = {0};
    char Day[4]   = {0};
    char Hour[4]  = {0};
    char Min[4]   = {0};
    char Sec[4]   = {0};
    time_t current_time;
    struct tm* now_time;
    time(&current_time);
    now_time = localtime(&current_time);
    strftime(Year, sizeof(Year), "%Y-", now_time);
    strftime(Month, sizeof(Month), "%m-", now_time);
    strftime(Day, sizeof(Day), "%d ", now_time);
    strftime(Hour, sizeof(Hour), "%H:", now_time);
    strftime(Min, sizeof(Min), "%M:", now_time);
    strftime(Sec, sizeof(Sec), "%S", now_time);
    strncat(local_time, Year, 5);
    strncat(local_time, Month, 3);
    strncat(local_time, Day, 3);
    strncat(local_time, Hour, 3);
    strncat(local_time, Min, 3);
    strncat(local_time, Sec, 3);

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

    int len_head=sizeof(SendBack)-3;
    char*head=stableReadInfo(len_head);
    SendBack*back=reinterpret_cast<SendBack*>(head);
    int message_len=back->m_len-len_head;
    char*info=stableReadInfo(message_len);

    int all_sail=0;

    char arry[20]{0};
    func_gettimer(arry);
    QString file_path= QString("./FileOutPut/SailCentos%1").arg(arry);
    File file(file_path.toStdString());

    stringstream in_head;
    in_head<<data.c_str()<<endl;
    in_head<<setw(20)<<"商品名字"<<setw(20)<<"价格"<<setw(20)<<"数目"<<setw(20)<<"仓库"<<endl;
    file.Write((char*)in_head.str().c_str(),in_head.str().size());

    if(back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(info);
        int line=back->m_num;
        setModelLine(model_SailCensus,line);

        for(int i=0;i<line;i++)
        {
            all_sail+=good->m_price * good->m_qua;
            setModelSoldCe(model_SailCensus,good,i);

            stringstream in_item;
            in_item<<setw(20)<<good->m_name
              <<setw(20)<<good->m_price
              <<setw(20)<<good->m_qua
              <<setw(20)<<good->m_store<<endl;

            file.Write((char*)in_item.str().c_str(),in_item.str().size());
            good++;

        }

        stringstream all_price;
        model_SailCensus.setItem(line, 0, new QStandardItem( QString        ("销售总额")  ));
        model_SailCensus.setItem(line, 1, new QStandardItem( QString::number(all_sail)  ));
        all_price<<setw(20)<<"销售总额"<<setw(20)<< all_sail<<endl;
        file.Write((char*)all_price.str().c_str(),all_price.str().size());
    }
    else
    {
        QMessageBox::information(NULL, "今天没有销售货物", "无",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    free(head);free(info);
}

//销售预警
void ChargeView::on_buttonEarlyWarn_clicked()
{
    string store=ui->line_ed_EarlyWarn->text().toStdString();
    int    num  =ui->line_ed_EarlyWarnNum->text().toInt();
    EarlyWarning warn;
    warn.setStore(store,num);

    client->writeInfo(reinterpret_cast<char*>(&warn),sizeof(warn));

    int len_head=sizeof(SendBack)-3;
    char*head=stableReadInfo(len_head);
    SendBack*back=reinterpret_cast<SendBack*>(head);
    int message_len=back->m_len-len_head;
    char*info=stableReadInfo(message_len);

    if(back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(info);
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
    free(head);free(info);
}


//查询该仓库的全部商品
void ChargeView::on_buttonClassSalar_2_clicked()
{
    string store= ui->lineEdit_Showstore->text().toStdString();
    ShowStoreAll sw;

    //显示所有的商品

    sw.setStore(store);
    client->writeInfo(reinterpret_cast<char*>(&sw),sizeof(sw));

    int len_head=sizeof(SendBack)-3;
    char*head=stableReadInfo(len_head);
    SendBack*back=reinterpret_cast<SendBack*>(head);
    int message_len=back->m_len-len_head;
    char*info=stableReadInfo(message_len);

    if(back->m_tag)
    {
        Goods *good=reinterpret_cast<Goods*>(info);
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
    free(head);free(info);
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

void ChargeView::on_pushButton_3_clicked()
{
    on_pushButton_2_clicked();
}

void ChargeView::on_pushButton_4_clicked()
{
}


void ChargeView::stableReadHead(SendBack *back)
{
    if(back==nullptr)
    {
        perror("arg error");
        throw bad_alloc();
        return;
    }
    else
    {
        int len1=sizeof(SendBack);
        int len=sizeof(SendBack)-1;
        int ret=client->stableRecv
                (reinterpret_cast<char*>(back),len);
        cout<<ret<<endl;
    }
}

char* ChargeView::stableReadInfo(int back_len)
{
    char *info_buf=reinterpret_cast<char*>(malloc(back_len));
    //读取错误  吐出异常  程序直接挂掉
    client->stableRecv(info_buf,back_len);
    return info_buf;
}


void ChargeView::solderLoad()
{

    ui->toolBox_3->removeItem(2);
    ui->toolBox_3->removeItem(2);
    ui->toolBox_3->removeItem(2);
    ui->toolBox_3->removeItem(3);
    ui->toolBox_3->removeItem(3);
    ui->toolBox_3->removeItem(3);
    ui->toolBox_3->removeItem(3);

    ui->toolBox_2->hide();
    ui->ChargeStore->hide();
}
