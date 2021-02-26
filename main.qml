import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

Window {
    id: root
    width: 800
    height: 600
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Web Search Tool")

    Rectangle {
        id: backgroundImage
        anchors.fill: parent
        color: "#FFFFFF"
        Image { source: "images/gradient_background.png"; fillMode: Image.Stretch; anchors.fill: parent; opacity: 0.7}

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


        Rectangle{
            anchors.margins: 10
            anchors.fill: leftSideRectangle
            layer.enabled: true
            color:"transparent"

            ScrollView{
                id: leftsideColumnScrollView
                anchors.fill: parent
                anchors.margins: 10
                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    Text {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        text: "Enter your search request here"

                        font.pixelSize: 16
                        font.bold: true
                    }

                    Rectangle{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        color:"transparent"
                        border.width: 1
                        border.color: "#AAAAAA"

                        TextInput {
                            anchors.fill: parent
                            id: searchRequestInput
                            text: "Search request"

                            layer.enabled: true
                            selectByMouse: true
                            font.pixelSize: 16
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        text: "Enter starting url here"

                        font.pixelSize: 16
                        font.bold: true
                    }

                    Rectangle{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        color:"transparent"
                        border.width: 1
                        border.color: "#AAAAAA"

                        TextInput {
                            id: startUrlInput
                            text: "Starting url"
                            anchors.fill: parent

                            selectByMouse: true
                            layer.enabled: true
                            font.pixelSize: 16
                        }
                    }

                    RowLayout {
                        id: mainControlButtons

                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        spacing: 5
                        Button {
                            id: startSearchButton
                            Layout.preferredWidth: 80
                            Layout.preferredHeight: 24
                            text: "Start"
                            font.pixelSize: 16
                            font.bold: true
                        }

                        Button {
                            id: pauseSearchButton
                            Layout.preferredWidth: 85
                            Layout.preferredHeight: 24
                            text: "Pause"
                            font.pixelSize: 16
                            font.bold: true
                        }

                        Button {
                            id: stopSearchButton
                            Layout.preferredWidth: 80
                            Layout.preferredHeight: 24
                            text: "Stop"
                            font.pixelSize: 16
                            font.bold: true
                        }
                    }

                    Slider{
                        id: testSlider

                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        value: 0.8
                        //visible: false
                    }

                    ProgressBar {
                        id: progressBar
                        value: testSlider.value

                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        background: Rectangle {
                            implicitWidth: 200
                            color: "#e6e6e6"
                        }

                        contentItem: Item{
                            implicitWidth: 200

                            Rectangle {
                                width: progressBar.visualPosition * parent.width
                                height: parent.height
                                color: "#e6e6e6"
                                Image { source: "images/progress_bar.png"; fillMode: Image.PreserveAspectCrop; horizontalAlignment: Image.AlignLeft ; anchors.fill: parent; opacity: 1.0}
                            }
                        }
                    }

                    Button {
                        id: showHideAdvanced
                        property var advancedSectionVisible: true

                        Layout.fillWidth: true
                        Layout.preferredHeight: 72

                        background: Rectangle{
                            anchors.top: parent.top
                            color: "transparent"
                            Rectangle{
                                color: "transparent"
                                width: 26
                                height: 26

                                Image {
                                    source: "images/triangle.png";
                                    fillMode: Image.Pad;
                                    horizontalAlignment: Image.AlignHCenter;
                                    verticalAlignment: Image.AlignVCenter;
                                    anchors.fill: parent;
                                    sourceSize.width: 16
                                    rotation: showHideAdvanced.advancedSectionVisible ? 90: 0;
                                    opacity: 1.0
                                }
                            }

                            Text {
                                id: advancedSection
                                width: parent.width
                                leftPadding: 26
                                text: "Advanced section (Change this values only when you know what you do)"
                                font.pixelSize: 16
                                font.bold: true
                                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                            }
                        }

                        onClicked: showHideAdvanced.advancedSectionVisible = !showHideAdvanced.advancedSectionVisible
                    }

                    Text {
                        visible: showHideAdvanced.advancedSectionVisible

                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        id: maximumThreadsCount
                        text: "Maximum threads count"
                        font.pixelSize: 16
                    }

                    Text {
                        id: recommendedThreadsCount
                        visible: showHideAdvanced.advancedSectionVisible

                        Layout.fillWidth: true
                        Layout.preferredHeight: 18

                        text: "Recommended for your system is 4"

                        font.pixelSize: 12
                        font.italic: true
                    }

                    RowLayout {
                        visible: showHideAdvanced.advancedSectionVisible
                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        Slider {
                            id: threadsCountSlider
                            Layout.fillHeight: true
                            from: 1
                            to: 2048
                            value: 4
                            stepSize: 1
                        }

                        TextInput {
                            id: threadsCountTextInput
                            text: threadsCountSlider.value
                            selectByMouse: true

                            Layout.preferredWidth: 40
                            Layout.fillHeight: true

                            horizontalAlignment: TextInput.AlignLeft

                            validator: IntValidator{
                                bottom:1
                                top:2048
                            }

                            onTextChanged: threadsCountSlider.value = Number(text)
                        }
                    }

                    Text {
                        id: maximumScanUrlsCount
                        visible: showHideAdvanced.advancedSectionVisible

                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        text: "Maximum scan Urls count"

                        font.pixelSize: 16
                    }

                    Text {
                        id: recommendationAndAttension
                        visible: showHideAdvanced.advancedSectionVisible

                        Layout.fillWidth: true
                        Layout.maximumWidth: parent.width
                        Layout.preferredHeight: 72

                        text: "Bigger number is, more results can be found and longer search will be running. Be aware that using huge number requires a lot of time for search to be finished"

                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        font.pixelSize: 12
                        font.italic: true
                    }


                    RowLayout {
                        visible: showHideAdvanced.advancedSectionVisible

                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        Slider {
                            id: scanUrlsCountSlider

                            Layout.fillHeight: true

                            from: 1
                            to:4096
                            value: 4
                            stepSize: 1
                        }

                        TextInput {
                            id: scanUrlsCountTextInput
                            text: scanUrlsCountSlider.value
                            selectByMouse: true

                            Layout.preferredWidth: 40
                            Layout.fillHeight: true

                            horizontalAlignment: TextInput.AlignLeft

                            validator: IntValidator{
                                bottom:1
                                top:4096
                            }

                            onTextChanged: scanUrlsCountSlider.value = Number(text)
                        }
                    }

                    Button {
                        id: showLogButton
                        visible: showHideAdvanced.advancedSectionVisible

                        Layout.fillWidth: true
                        Layout.preferredHeight: 24

                        property bool showLog: false

                        text: "Show log"
                        font.pixelSize: 16
                        font.bold: true

                        onClicked:{
                            if(text=="Show log") {
                                showLog = true
                                text = "Hide log"
                            }
                            else {
                                showLog = false
                                text="Show log"
                            }
                        }
                    }
                }
            }
        }

        Rectangle{
            anchors.margins: 10
            anchors.fill: rightSideRectangle
            layer.enabled: true
            color:"transparent"

            ColumnLayout{
                id:rightsideColumn
                anchors.fill: rightSideRectangle

                layer.enabled: true

                ScrollView{
                    id: resultsScrollView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredWidth: 400

                    layer.enabled: true

                    ColumnLayout{
                        spacing: 10
                        width: parent.width
                        height: parent.height
                        Button{
                            id: button1
                            text: "button1"
                        }
                        Button{
                            id: button2
                            text: "button2"
                        }
                        Button{
                            id: button3
                            text: "button4"
                        }
                        Button{
                            id: button4
                            text: "button4"
                        }
                        Button{
                            id: button5
                            text: "button5"
                        }
                        Button{
                            id: button6
                            text: "button6"
                        }
                        Button{
                            id: button7
                            text: "button7"
                        }
                        Button{
                            id: button8
                            text: "button8"
                        }
                        Button{
                            id: button9
                            text: "button9"
                        }
                        Button{
                            id: button10
                            text: "button10"
                        }
                        Button{
                            id: button11
                            text: "button11"
                        }
                        Button{
                            id: button12
                            text: "button12"
                        }
                        Button{
                            id: button13
                            text: "button13"
                        }
                        Button{
                            id: button14
                            text: "button14"
                        }
                    }
                }

                ScrollView{
                    id: logsScrollView
                    visible: showLogButton.showLog

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumHeight: 200
                    Layout.minimumHeight: 50

                    layer.enabled: true

                    Rectangle {
                        color: "white"
                        anchors.fill: parent

                        TextEdit{
                            anchors.fill: parent

                            text: "klasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \nklasjdfhlkas fhlasf as\n as;dofy aiosehfl;ash f;lasdfh kl;ajsdhf \n"
                        }
                    }
                }
            }
        }
    }
}
