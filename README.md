# BluetoothCarController
This application is built on Android platform, which is used for controlling small car by low energy bluetooth.
## Using Qt Version
- Qt 6.8
## Abort code
- It is coding with C++, No QML code.
- And also using CMake to construct.
- The part of low energy Bluetooth is from
 the example ( low energy scanner ) of Qt6.8
## UI
- **MainInterface**
![UI]("https://github.com/wvfp/BluetoothCarController/master/ui.jpg")

## Notice
* Please use the application after having connected to Bluetooth Device.

* The Application only is used for connecting specific device.
_ Please ensure that all devices are properly connected before proceeding with the operation; otherwise, the application may crash.

* Please ignore bugs of application.

## about sending messages by Bluetooth 
### Standard
- When send a message by application, it will add the ending flag ('\0') to the end of message.(As an end flag to receiving data)
```cpp
void SendMsg(QString &data){
    ...
    data.append(char('\0'));
    ...
}
```
- Data is sent in 1-byte [ `sizeof(char)` ] increments.




