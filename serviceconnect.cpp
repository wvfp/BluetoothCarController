#include "serviceconnect.h"
#include "ui_serviceconnect.h"

ServiceConnect::ServiceConnect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServiceConnect)
{
    ui->setupUi(this);
    currentItem = nullptr;
}

ServiceConnect::~ServiceConnect()
{
    delete ui;
}

void ServiceConnect::on_pushButton_clicked()
{
    emit this->CloseButtonClicked();
    update();
}


void ServiceConnect::on_pushButton_2_clicked()
{
    emit this->BackButtonClicked();
    update();
}


void ServiceConnect::on_pushButton_3_clicked()
{
    emit this->StratButtonClicked();
    update();
}
void ServiceConnect::addServiceToListWidget(QListWidgetItem* item){
    ui->listWidget->addItem(item);
    update();
}


void ServiceConnect::clearListWidget(){
    ui->listWidget->clear();
    update();
}

void ServiceConnect::on_listWidget_itemActivated(QListWidgetItem *item)
{
    currentItem = item;
    ui->label->setText(item->text());
    emit ItemSelected(item);
    update();
}


void ServiceConnect::on_listWidget_itemClicked(QListWidgetItem *item)
{
    update();
    emit ui->listWidget->itemActivated(item);
}

