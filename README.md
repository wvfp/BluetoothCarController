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

* Please ignore bugs of application.

## Abort sending messages by Bluetooth 
### Standard
- When send a message by application, it will add the ending flag ('\0') to the end of message.
```cpp
void SendMsg(QString data){
    ...
    data.append(char('\0'));
    ...
}
```
