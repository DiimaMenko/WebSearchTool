import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("Web Search Tool")

    Rectangle {
        id: backgroundImage
        anchors.fill: parent
        color: "#5BDDE1"
        Image { source: "images/gradient_background.png"; fillMode: Image.Stretch; anchors.fill: parent; opacity: 1.0}

        Rectangle {
            id: leftSideRectangle
            width: 300
            anchors.left: backgroundImage.left
            anchors.top: backgroundImage.top
            anchors.bottom: backgroundImage.bottom
            anchors.margins: 10
            color:"#000000"
            opacity: 0.02
        }

        Rectangle {
            id: rightSideRectangle
            anchors.left: leftSideRectangle.right
            anchors.right: backgroundImage.right
            anchors.top: backgroundImage.top
            anchors.bottom: backgroundImage.bottom
            anchors.margins: 10
            color:"#000000"
            opacity: 0.02
        }


        ScrollView{
            id: scrollView
            anchors.top: leftSideRectangle.top
            anchors.left: leftSideRectangle.left
            anchors.right: leftSideRectangle.right
            anchors.bottom: leftSideRectangle.bottom
            anchors.margins: 10
            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Text {
                    id: enterTextLabel
                    text: "Enter your search request here"
                    height: 24
                    font.pixelSize: 16
                    font.bold: true
                }

                TextInput {
                    id: searchRequestInput
                    text: "Search request"
                    Layout.fillWidth: true
                    font.pixelSize: 16
                }

                Text {
                    id: enterUrlLabel
                    text: "Enter starting url here"
                    height: 24
                    font.pixelSize: 16
                    font.bold: true
                }

                TextInput {
                    id: startUrlInput
                    text: "Starting url"
                    Layout.fillWidth: true
                    font.pixelSize: 16
                }

                RowLayout {
                    id: mainControlButtons
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        id: startSearchButton
                        text: "Start"
                        Layout.preferredWidth: 85
                        Layout.preferredHeight: 24
                        style: ButtonStyle{
                            label: Text{
                                font.pixelSize: 16
                                font.bold: true
                            }
                        }
                    }
                    Button {
                        id: pauseSearchButton
                        text: "Pause"
                        Layout.preferredWidth: 90
                        Layout.preferredHeight: 24
                        style: ButtonStyle{
                            label: Text{
                                font.pixelSize: 16
                                font.bold: true
                            }
                        }
                    }
                    Button {
                        id: stopSearchButton
                        text: "Stop"
                        Layout.preferredWidth: 85
                        Layout.preferredHeight: 24
                        style: ButtonStyle{
                            label: Text{
                                font.pixelSize: 16
                                font.bold: true
                            }
                        }
                    }
                }

                Slider{
                    id: testSlider
                    value: 0.8
                    Layout.preferredHeight: 24
                    Layout.fillWidth: true
                    visible: false
                }

                ProgressBar {
                    id: progressBar
                    value: testSlider.value
                    Layout.preferredHeight: 24
                    Layout.fillWidth: true

                    ProgressBarStyle{
                        background: Rectangle {
                            implicitWidth: 200
                            color: "#e6e6e6"
                        }

                        progress: Item {
                            implicitWidth: 200

                            Rectangle {
                                width: progressBar.visualPosition * parent.width
                                height: parent.height
                                color: "#e6e6e6"
                                Image { source: "images/progress_bar.png"; fillMode: Image.PreserveAspectCrop; horizontalAlignment: Image.AlignLeft ; anchors.fill: parent; opacity: 1.0}

                            }
                        }
                    }
                }

                Text {
                    id: advancedSection
                    text: "Advanced section (Change this values only when you know what you do)"
                    Layout.fillWidth: true
                    font.pixelSize: 16
                    font.bold: true
                    wrapMode: Text.WordWrap
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: 72
                }

                Text {
                    id: maximumThreadsCount
                    text: "Maximum threads count"
                    Layout.fillWidth: true
                    font.pixelSize: 16
                    Layout.preferredHeight: 24
                }

                Text {
                    id: recommendedThreadsCount
                    text: "Recommended for your system is 4"
                    Layout.fillWidth: true
                    font.pixelSize: 12
                    font.italic: true
                    Layout.preferredHeight: 24
                }

                RowLayout {
                    Slider {
                        id: threadsCountSlider
                        minimumValue: 1
                        value: 4
                        stepSize: 1
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 200
                        visible: false
                    }

                    TextInput {
                        id: threadsCountTextInput
                        text: threadsCountSlider.value

                        inputMask: "9"
                        onAccepted: threadsCountSlider.value = Number(text)
                    }
                }

                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 24
                    text: "Show log"
                    style: ButtonStyle{
                        label: Text{
                            color: black
                            font.pixelSize: 16
                            font.bold: true
                        }
                    }
                    onClicked: if(text=="Show log")
                                    text="Hide log"
                                else
                                    text="Show log"
                }
            }
        }
    }
}
