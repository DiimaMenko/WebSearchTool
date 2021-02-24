import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Web Search Tool")
    Rectangle {
        anchors.fill: parent
        color: "#5BDDE1"
        Image { source: "images/gradient_background.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 1.0}
    }
}
