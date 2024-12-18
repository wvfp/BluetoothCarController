#include "maininterface.h"
#include "ble_connect.h"
#include <QPushButton>
#include <QDateTime>
#include <QString>
#include <cmath>
mainInterface::mainInterface(QWidget*parent)
    :QWidget(parent),ui(new Ui::Widget()),bleInitFlag(false){
    ui->setupUi(this);
    ble=nullptr;
    joystick=new Joystick(ui->widget);
    timer.setInterval(250);
    ui->textEdit->setReadOnly(true);
    connect(joystick,&Joystick::ValueChange,this,&mainInterface::on_joystick_changed);
    connect(joystick,&Joystick::joystickPressed,this,[&](){
        timer.start();
    });
    connect(joystick,&Joystick::joystickReleased,this,[&](){
        this->ui->spinBox->setValue(0);
        this->ui->spinBox_2->setValue(0);
        QString data=QString("R+%1").arg(char(0));
        if(preDataR!=0){
            ble->sendMsg(data);
            preDataR=0;
        }
        data=QString("L+%1").arg(char(0));
        if(preDataL!=0){
            ble->sendMsg(data);
            preDataL=0;
        }
        timer.stop();
    });
    connect(&timer,&QTimer::timeout,this,[&](){
        this->sendPwmLCtr();
        this->sendPwmRCtr();
    });
    connect(ui->pushButton,&QPushButton::clicked,this,
            [=](){
                if(ble==nullptr){
                ble=new BLE_connect();
                connect(ble,&BLE_connect::ServiceConnected,this,[this](){
                    QString str=QString("设备连接成功(%1)").arg(ble->getCurrentDevice()->getName());
                    this->ui->label->setText(str);
                });
                connect(ble,&BLE_connect::CloseBleWidget,this,[&](){
                    this->show();
                    ble->close();
                });
                connect(ble,&BLE_connect::RecvDataSuccess,this,[this](){
                    auto c_color = this->ui->textEdit->textColor();
                    this->ui->textEdit->setTextColor(Qt::blue);
                    QString str=QDateTime::currentDateTime().toString("hh:mm:ss ")+"接收: "+ble->getRecvData();
                    this->ui->textEdit->append(str);
                    this->ui->textEdit->setTextColor(c_color);
                });
                connect(ble,&BLE_connect::SendDataSuccess,this,[this](){
                    auto c_color = this->ui->textEdit->textColor();
                    this->ui->textEdit->setTextColor(Qt::red);
                    QString str=QDateTime::currentDateTime().toString("hh:mm:ss ")+"发送: "+ble->getSendData();
                    this->ui->textEdit->append(str);
                    this->ui->textEdit->setTextColor(c_color);
                });
                connect(ble,&BLE_connect::DisConnected,this,[this]()->void{
                    QMessageBox::information(this,"提示",QString("蓝牙(%1)已断开").arg(ble->getCurrentDevice()->getName()));
                    this->ui->label->setText("设备未连接");
                });
                }
                ble->show();
                this->close();
                update();
    }
    );

}

mainInterface::~mainInterface(){
    delete ui;
}

void mainInterface::on_horizontalSlider_valueChanged(int value)
{

    ui->spinBox->setValue(value);
    update();
}


void mainInterface::on_horizontalSlider_2_valueChanged(int value)
{
    ui->spinBox_2->setValue(value);
    update();

}


void mainInterface::on_spinBox_valueChanged(int arg1)
{   if(arg1!=ui->horizontalSlider->value()){
        ui->horizontalSlider->setValue(abs(arg1));

    }
}


void mainInterface::on_spinBox_2_valueChanged(int arg1)
{
    if(arg1!=ui->horizontalSlider_2->value()){
        ui->horizontalSlider_2->setValue(abs(arg1));

    }

}

void mainInterface::onValueChange(int x,int y){
    std::string str_x="x: "+std::to_string(x);
    std::string str_y="y: "+std::to_string(y);
    ui->label_12->setText(str_x.c_str());
    ui->label_13->setText(str_y.c_str());

}

void mainInterface::on_textEdit_textChanged()
{
    this->ui->textEdit->moveCursor(QTextCursor::End);
    update();
}


void mainInterface::on_pushButton_2_clicked()
{
    QString data=this->ui->lineEdit->text();
    ble->sendMsg(data);
    update();
}


void mainInterface::on_pushButton_3_clicked()
{
    QString str("C:TurnLeft");
    this->ble->sendMsg(str);
}


void mainInterface::on_pushButton_4_clicked()
{
    QString str("C:TurnRight");
    this->ble->sendMsg(str);

}


void mainInterface::on_pushButton_5_clicked()
{
    QString str("C:TurnBack");
    this->ble->sendMsg(str);
}


void mainInterface::on_pushButton_6_clicked()
{
    QString str("C:Stop");
    this->ble->sendMsg(str);
}

void mainInterface::on_joystick_changed(int x,int y){
    //设置摇杆的死区
    y=(abs(y)<30)?0:(abs(y)-30)*y/abs(y);
    x=(abs(x)<30)?0:(abs(x)-30)*x/abs(x);
    int l=abs(y),r=abs(y);
    this->onValueChange(x,y);
    if(y>0){
    this->ui->checkBox->setCheckState(Qt::Checked);
    this->ui->checkBox_5->setCheckState(Qt::Checked);
    }else{
        this->ui->checkBox->setCheckState(Qt::Unchecked);
        this->ui->checkBox_5->setCheckState(Qt::Unchecked);
    }

    l+=x/2;
    r-=x/2;
    if(l>100)
        l=100;
    if(r<0)
        r=0;

    this->ui->spinBox->setValue(l);
    this->ui->spinBox_2->setValue(r);

}

void mainInterface::on_pushButton_7_clicked()
{
    QString str("C:Forward");
    this->ble->sendMsg(str);
}


void mainInterface::on_pushButton_8_clicked()
{
    QString str("C:Backward");
    this->ble->sendMsg(str);

}


void mainInterface::sendPwmLCtr(){
    char c;
    char value=char(this->ui->horizontalSlider->value());
     if(ui->checkBox->isChecked())
        c='-';
    else
        c='+';
    QString data=QString("L%1%2").arg(c).arg(value);
    if(preDataL!=value){
        ble->sendMsg(data);
        preDataL=value;
    }
}
void mainInterface::sendPwmRCtr(){
    char c;
    char value=char(this->ui->horizontalSlider_2->value());
    if(ui->checkBox_5->isChecked())
        c='-';
    else
        c='+';
    QString data=QString("R%1%2").arg(c).arg(value);
    if(preDataR!=value){
        ble->sendMsg(data);
        preDataR=value;
    }
}

void mainInterface::on_horizontalSlider_sliderReleased()
{
    sendPwmLCtr();
    update();
}


void mainInterface::on_horizontalSlider_2_sliderReleased()
{
    sendPwmRCtr();
    update();
}

