#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QObject>
#include <QWidget>
#include "ui_widget.h"
#include "joystick.h"
#include "ble_connect.h"
#include <QTimer>
QT_BEGIN_NAMESPACE


QT_END_NAMESPACE
class mainInterface:public QWidget
{
    Q_OBJECT
private:
  Ui::Widget *ui;
    Joystick* joystick;
    bool bleInitFlag;
    BLE_connect* ble;
    QTimer timer;
    char preDataL,preDataR;
public:
    mainInterface(QWidget*parent=nullptr);
    ~mainInterface();

private:
    void sendPwmLCtr();
    void sendPwmRCtr();

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_spinBox_valueChanged(int arg1);
    void on_spinBox_2_valueChanged(int arg1);
    void onValueChange(int x,int y);
    void on_textEdit_textChanged();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void on_joystick_changed(int x,int y);
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_2_sliderReleased();
};




#endif // MAININTERFACE_H
