cmake_minimum_required(VERSION 3.16)

project(BLE_CAR_CTR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets )

set(PROJECT_SOURCES
        main.cpp
        joystick.cpp
        joystick.h
        widget.ui
)
set(ANDROID_PACKAGE_SOURCE_DIR "$$PWD/android_sources")

find_package(Qt6 REQUIRED COMPONENTS Bluetooth)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BLE_CAR_CTR
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
        maininterface.h maininterface.cpp

        ble_connect.h ble_connect.cpp ble_connect.ui
        characteristicinfo.cpp characteristicinfo.h device.cpp device.h deviceinfo.cpp deviceinfo.h serviceinfo.cpp serviceinfo.h
        serviceconnect.h serviceconnect.cpp serviceconnect.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BLE_CAR_CTR APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BLE_CAR_CTR SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BLE_CAR_CTR
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(BLE_CAR_CTR PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Bluetooth Qt::CorePrivate)

# qt for ios sets macosx_bundle_gui_identifier automatically since qt 6.1.
# if you are developing for ios or macos you should consider setting an
# explicit, fixed bundle identifier manually though.
 # if(${qt_version} version_less 6.1.0)
 #   set(bundle_id_option macosx_bundle_gui_identifier com.example.ble_car_ctr)
 # endif()
set_target_properties(BLE_CAR_CTR PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BLE_CAR_CTR
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BLE_CAR_CTR)
endif()
