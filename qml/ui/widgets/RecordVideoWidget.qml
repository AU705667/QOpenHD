import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import QtQuick.Shapes 1.0

import Qt.labs.settings 1.0

import OpenHD 1.0

import "../elements"

BaseWidget {
    width: 256
    height: 48

    visible: true

    widgetIdentifier: "record_video_widget"

    defaultAlignment: 0
    defaultXOffset: 200
    defaultYOffset: 2
    defaultHCenter: false
    defaultVCenter: false

    hasWidgetDetail: false
    hasWidgetAction: true
    widgetActionWidth: 200
    widgetActionHeight: 190

    property var instanceMavlinkSettingsModel: _airPiSettingsModel

    widgetActionComponent: ScrollView{

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        clip: true

        ColumnLayout{
            id:recID
            width: 400
                Item {
                width: parent.width
                height: 32
                Text {
                    text: qsTr("Enable Recording")
                    color: "white"
                    font.bold: true
                    height: parent.height
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
            }
            ConfirmSlider {

                visible: true
                text_off: qsTr("Record Video")
                onCheckedChanged: {
                    if (checked == true) {
                        widgetInner.rec=instanceMavlinkSettingsModel.try_update_parameter_int(V_AIR_RECORDING,1)
                        //here should also be an update "space-left" action
                        airVideoSpaceLeft.text="2GB" //just a mockup

                    }
                }
            }
            Item {
                width: parent.width
                height: 32
                Text {
                    text: qsTr("Disable Recording")
                    color: "white"
                    font.bold: true
                    height: parent.height
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
            }
            ConfirmSlider {

                visible: true
                text_off: qsTr("Stop Recording")
                onCheckedChanged: {
                    if (checked == true) {
                        widgetInner.rec=instanceMavlinkSettingsModel.try_update_parameter_int(V_AIR_RECORDING,0)
                        //here should also be an update "space-left" action
                        airVideoSpaceLeft.text="2GB" //just a mockup
                    }
                }
            }


           }
    }


    Item {
        id: widgetInner
        property bool rec: false

        anchors.fill: parent
                Text {
                    text: qsTr("Record Video");
                    color: settings.color_text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideNone
                    wrapMode: Text.NoWrap
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 14
                    font.family: settings.font_text
                    style: Text.Outline
                    styleColor: settings.color_glow
                    visible: true
                }
                Text {
                    id:record_status
                    text: {
                        text = qsTr("OFF")
                        if(widgetInner.rec == false)
                            text = qsTr("OFF")
                        if(widgetInner.rec == true)
                            text = qsTr("ON")
                     }
                    color: {
                           color = "red"
                           if(widgetInner.rec == false)
                               color = "red"
                           if(widgetInner.rec == true)
                               color = "green"
                        }
                    anchors.fill: parent
                    anchors.leftMargin: 95
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideNone
                    wrapMode: Text.NoWrap
                    font.pixelSize: 14
                    font.family: settings.font_text
                    style: Text.Outline
                    styleColor: settings.color_glow
                    visible: true
                }
                Text {
                    text: qsTr("Free Space");
                    color: settings.color_text
                    anchors.topMargin: 30
                    anchors.leftMargin: 90
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideNone
                    wrapMode: Text.NoWrap
                    font.pixelSize: 14
                    font.family: settings.font_text
                    style: Text.Outline
                    styleColor: settings.color_glow
                    visible: true
                }
                Text {
                    id:airVideoSpaceLeft
                    text: qsTr("UNKNOWN");
                    color: "green"
                    anchors.fill: parent
                    anchors.leftMargin: 95
                    anchors.topMargin: -30
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideNone
                    wrapMode: Text.NoWrap
                    font.pixelSize: 14
                    font.family: settings.font_text
                    style: Text.Outline
                    styleColor: settings.color_glow
                    visible: true
                }
    }

}
