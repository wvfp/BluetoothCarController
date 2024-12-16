#ifndef BLE_CONNECT_H
#define BLE_CONNECT_H

#include <QWidget>
#include <QAction>
#include "device.h"
#include <QListWidget>
#include <QDialog>
#include "serviceconnect.h"
namespace Ui {
class BLE_connect;
}

class BLE_connect : public QWidget
{
    Q_OBJECT
private:
    Device* device;
    DeviceInfo* currentDevice;
    ServiceInfo* activeService;
    ServiceConnect* SCPage;
    CharacteristicInfo* m_SendCharacteristicInfo;
    CharacteristicInfo* m_RecvCharacteristicInfo;
    QByteArray recvData;
    QByteArray sendData;
public:
    explicit BLE_connect(QWidget *parent = nullptr);
    ~BLE_connect();
    DeviceInfo* getCurrentDevice(){
        return currentDevice;
    }
    ServiceInfo* getCurrentService(){
        return activeService;
    }
    DeviceInfo** getCurrentDevicePtr(){
        return &currentDevice;
    }
    ServiceInfo** getCurrentServicePtr(){
        return &activeService;
    }
    const QByteArray& getRecvData(){
        return recvData;
    }
    const QByteArray& getSendData(){
        return sendData;
    }
private:
    Ui::BLE_connect *ui;
signals:
    void CloseBleWidget();
    void ServiceConnected();
    void SendDataSuccess();
    void RecvDataSuccess();
    void RecvDataReached(const QLowEnergyCharacteristic&info,const QByteArray& data);
    void DisConnected();
public slots:
    void RecvCharacteristicUpdate(const QLowEnergyCharacteristic&info,const QByteArray& data);
    void ConnectToService();
    void sendMsg(QString&data);
    void recvMsg(QString&data);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void addDeviceToWidget();
    void on_listWidget_itemActivated(QListWidgetItem *item);
    void preparedService();
    void on_pushButton_3_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // BLE_CONNECT_H
