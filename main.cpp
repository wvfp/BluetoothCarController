#include "maininterface.h"
#include <QApplication>
#include <QPermission>
#include <QFuture>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QBluetoothPermission QBP;
    QBP.setCommunicationModes(QBluetoothPermission::Default);
    a.requestPermission(QBP,[](){});
    mainInterface *w=new mainInterface(nullptr);

    w->show();

    return a.exec();
}
// bool checkPermission(const QString &permission)
// {
// #ifdef Q_OS_ANDROID
//     auto r = QtAndroidPrivate::checkPermission(permission);
//     if(r != QtAndroidPrivate::PermissionResult::Granted) {
//         QtAndroidPrivate::requestPermissionsSync( QStringList() << permission );
//         r = QtAndroidPrivate::checkPermission(permission);
//         if(r == QtAndroidPrivate::PermissionResult::Denied) {
//             return false;
//         }
//     }

// #else
//     return true;
// #endif
