import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1
import QtQuick.Controls.Material 2.12

import Qt.labs.settings 1.0

import OpenHD 1.0

import "../../../ui" as Ui
import "../../elements"

ColumnLayout {
    // from https://doc.qt.io/qt-6/qml-qtquick-layouts-rowlayout.html
    anchors.fill: parent
    spacing: 2

    property bool m_is_ground: false

    property var m_model: m_is_ground ? _ohdSystemGround : _ohdSystemAir

    property string m_version: m_model.openhd_version
    property string m_last_ping: m_model.last_ping_result_openhd
    property bool m_is_alive: m_model.is_alive

    function get_alive_text(){
        return m_is_alive ? "Yes" : "NOT ALIVE !"
    }
    function get_alive_text_color(){
        //return m_is_alive ? "green" : "red"
        return m_is_alive ? "green" : "black"
    }

    property int rowHeight: 64
    property int text_minHeight: 20


    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("OpenHD Version:")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: m_version
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("Last Ping:")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: m_last_ping
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("Alive: ")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: get_alive_text()
            color: get_alive_text_color()
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
}
