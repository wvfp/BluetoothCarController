#ifndef SERVICECONNECT_H
#define SERVICECONNECT_H

#include <QWidget>
#include <QListWidget>
namespace Ui {
class ServiceConnect;
}

class ServiceConnect : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceConnect(QWidget *parent = nullptr);
    ~ServiceConnect();
    QListWidgetItem* getItem(){
        return currentItem;
    }
signals:
    void BackButtonClicked();
    void CloseButtonClicked();
    void StratButtonClicked();
    void ItemSelected(QListWidgetItem*item);

public slots:
    void addServiceToListWidget(QListWidgetItem* item);

    void clearListWidget();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();



    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::ServiceConnect *ui;
    QListWidgetItem* currentItem;
};

#endif // SERVICECONNECT_H
