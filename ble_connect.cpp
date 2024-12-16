#include "ble_connect.h"
#include "ui_ble_connect.h"
#include <QMessageBox>


BLE_connect::BLE_connect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BLE_connect)
{
    ui->setupUi(this);
    device=new Device();
    currentDevice=nullptr;
    activeService=nullptr;
    SCPage=nullptr;
    m_SendCharacteristicInfo=nullptr;
    m_RecvCharacteristicInfo=nullptr;
}

BLE_connect::~BLE_connect()
{
    if(activeService)
        delete activeService;
    if(currentDevice)
        delete activeService;
    if(device)
        delete device;
    delete ui;
}

void BLE_connect::on_pushButton_clicked()
{
    device->stopDeviceDiscovery();
    device->startDeviceDiscovery();
    connect(device,&Device::devicesUpdated,this,&BLE_connect::addDeviceToWidget);
    connect(device,&Device::deviceDiscoverDone,this,[this](){
        QMessageBox* MsgBox=new  QMessageBox(this);
        MsgBox->setWindowTitle("Msg:");
        MsgBox->setText("扫描结束");
    });    
}


void BLE_connect::on_pushButton_2_clicked()
{
    if(currentDevice!=nullptr){
        device->connectToService(currentDevice->getAddress());
        device->scanServices(currentDevice->getAddress());
        connect(device,&Device::disconnected,this,&BLE_connect::DisConnected);
        connect(device,&Device::deviceConnectSuccess,this,[this](){
            device->scanServices(currentDevice->getAddress());
            QMessageBox* MB=new QMessageBox(this);
            MB->setWindowTitle("Success");
            MB->setText("设备选择成功");
            MB->show();
        });
        connect(device,&Device::servicesUpdated,this,&BLE_connect::preparedService);
        if(SCPage==nullptr)
            SCPage = new ServiceConnect(this);
        SCPage->show();
        ui->widget->close();
        connect(SCPage,&ServiceConnect::CloseButtonClicked,this,[this](){
            emit BLE_connect::CloseBleWidget();
        });
        connect(SCPage,&ServiceConnect::BackButtonClicked,this,[=](){
            device->disconnectFromDevice();
            ui->widget->show();
            SCPage->close();
        });
        connect(SCPage,&ServiceConnect::StratButtonClicked,this,&BLE_connect::ConnectToService);
    }
}
void BLE_connect::addDeviceToWidget(){
    QList<DeviceInfo*> devices=(device->getDevices()).value<QList<DeviceInfo*>>();
    ui->listWidget->clear();
    for(auto i:devices){
        QListWidgetItem *item=new QListWidgetItem(i->getName()+" \n"+i->getAddress());
        ui->listWidget->addItem(item);
        update();
    }

    update();
}

void BLE_connect::on_listWidget_itemActivated(QListWidgetItem *item)
{
    QList<DeviceInfo*> devices=(device->getDevices()).value<QList<DeviceInfo*>>();
    auto it=std::find_if(devices.begin(),devices.end(),[item](DeviceInfo*info){
        return item->text()==info->getName()+" \n"+info->getAddress();
    });
    if(it==devices.end()){
        QMessageBox* MB=new QMessageBox(this);
        MB->setWindowTitle("Warnning");
        MB->setText("Selected Failue");
        MB->show();
    }else{
        currentDevice = *it;
        ui->label->setText(currentDevice->getName());
    }
    update();

}

void BLE_connect::preparedService(){
    QList<ServiceInfo*> services=device->getServices().value<QList<ServiceInfo*>>();
    this->SCPage->clearListWidget();
    for(auto i:services){
        QListWidgetItem* item=new QListWidgetItem(i->getUuid());
        SCPage->addServiceToListWidget(item);
        update();
    }
}

void BLE_connect::on_pushButton_3_clicked()
{
    update();
    emit CloseBleWidget();
}


void BLE_connect::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit ui->listWidget->itemActivated(item);
}

void BLE_connect::ConnectToService(){
        QList<ServiceInfo*> services=device->getServices().value<QList<ServiceInfo*>>();
        auto it = std::find_if(services.begin(),services.end(),[this](ServiceInfo* info){
            return info->getUuid()==SCPage->getItem()->text();
        });

        QMessageBox* msg=new QMessageBox(this);
        if(it==services.end()){

            msg->setWindowTitle("Warrning");
            msg->setText("Can't find good services");
        }else{
            activeService=*it;
            device->connectToService(activeService->getUuid());
            connect(device,&Device::characteristicsUpdated,this,[this](){
                QList<CharacteristicInfo*> infos=device->getCharacteristics().value<QList<CharacteristicInfo*>>();
                for(auto info:infos){
                    if(info->getUuid()=="0xfff1"){
                        m_RecvCharacteristicInfo=info;
                        QLowEnergyDescriptor notifiction = m_RecvCharacteristicInfo->getCharacteristic().clientCharacteristicConfiguration();
                        QByteArray value = QLowEnergyCharacteristic::CCCDEnableNotification;
                        activeService->service()->writeDescriptor(notifiction,value);
                        connect(activeService->service(),&QLowEnergyService::characteristicChanged,this,&BLE_connect::RecvDataReached);
                        connect(activeService->service(),&QLowEnergyService::characteristicRead,this,&BLE_connect::RecvDataReached);
                        connect(this,&BLE_connect::RecvDataReached,this,&BLE_connect::RecvCharacteristicUpdate);
                    }
                    else if(info->getUuid()=="0xfff2"){
                        m_SendCharacteristicInfo=info;
                    }
                    else
                        break;
                }
            }
            );
            QList<CharacteristicInfo*> infos=device->getCharacteristics().value<QList<CharacteristicInfo*>>();
            connect(device,&Device::serviceSanDone,this,[=](){
                QMessageBox* msgc=new QMessageBox(this);
                msgc->setWindowTitle("Success");
                QString text("Services:");
                for(auto info:infos){
                    text.append("\n");
                    text.append(info->getUuid());
                }
                msgc->setText(text);
            });

            emit ServiceConnected();
        }

        connect(msg,&QMessageBox::buttonClicked,this,[this](QAbstractButton*){
            emit this->CloseBleWidget();
        });
        msg->show();

        update();
}

void BLE_connect::sendMsg(QString&data){
    data.append('\0');
    if(activeService&&m_SendCharacteristicInfo){
        sendData.clear();
        for(auto i:data){
            sendData.append(static_cast<char>(i.unicode()));
        }
        for(int i=0;i<sendData.size();i++){
            QByteArray d;
            d.append(sendData.at(i));
            activeService->service()->writeCharacteristic(m_SendCharacteristicInfo->getCharacteristic(),d);
        }
        emit SendDataSuccess();
    }
}
void BLE_connect::recvMsg(QString&data){

}
void BLE_connect::RecvCharacteristicUpdate(const QLowEnergyCharacteristic&info,const QByteArray& data){
    //if(&info == m_RecvCharacteristicInfo->getCharacteristic())
    recvData =  data;
    emit RecvDataSuccess();
}
