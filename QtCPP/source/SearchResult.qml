import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Button {
    property string link: ""

    font.pixelSize: 16
    font.underline: true

    background: Rectangle{
        color: parent.down ? "#888888" : "#AAAAAA"
        opacity: 0.3
    }

    contentItem: ColumnLayout{
        property string color: "black"
        spacing: 0
        Text {
            text: parent.parent.text
            font: parent.parent.font
            color: parent.color
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Text {
            text: parent.parent.link
            font: parent.parent.font

            color: "midnightblue"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }

    onClicked: {
        contentItem.color = "darkviolet";
        Qt.openUrlExternally(link);
    }
}
