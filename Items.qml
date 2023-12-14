import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import showText 1.0





Rectangle {
    id: display
    property alias displayEditText: displayText.text
    property alias displayColor: display.color
    property alias showText: showText
    property alias mouseAreaIsActive: mouseArea.enabled
    color: "#ffffff"
    radius: 7
    //        width: 100
    //        height: 100

    Text {
        id: displayText
        text: qsTr("T")
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 25



    }

    MouseArea{
        id:mouseArea
        Timer { id: timer }


        ShowText{
            id:showText
            onTextChanged:  {

                text = displayText.text
                console.log(text)
            }
        }


        width: display.width
        height: display.height
        onPressed:  {
            showText.onTextChanged(); //in showText signal

            displayColor = "gray"
            timer.interval = 100  // a short delay, enabled status would change properly.
            timer.triggered.connect(callback)
            timer.start()
        }
        function callback() {
            timer.triggered.disconnect(callback) // don't forget to remove the callback
            displayColor = "white"
        }

    }


}

