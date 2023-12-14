import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.0
import showText 1.0
import QtQuick.Controls 1.2
import QtQuick.VirtualKeyboard 1.2
Window {
    id: window
    visible: true
    color: "#f1ecec"
    title: qsTr("App")
    minimumWidth: 300
    minimumHeight: 300
    width: 450
    height: 450
    property int currentIndex: showText.text.length

    function undoClicked(){


        currentIndex=showText.text.length
        --currentIndex
        showText.isButtomClicked = true
        if(undo.mouseAreaIsActive === true){
            redo.mouseAreaIsActive = true
            showTextCpp.text = showTextCpp.undoHistorySlots()
            showText.text =showTextCpp.text
            undo.mouseAreaIsActive = true
            if(showTextCpp.undoHistoryCheck() === false){
                undo.mouseAreaIsActive = false
            }
            showText.cursorPosition = currentIndex
            console.log("undo : Shortcut activated.")
        }else{
            console.log("undo : Shortcut disable.")
        }
    }
    function redoClicked()
    {
        currentIndex=showText.text.length
        ++currentIndex
        showText.isButtomClicked = true
        if(redo.mouseAreaIsActive === true){
            showTextCpp.text = showTextCpp.redoHistorySlots()
            showText.text =showTextCpp.text
            undo.mouseAreaIsActive = true
            if(showTextCpp.redoHistoryCheck() === false){
                redo.mouseAreaIsActive = false
            }
            showText.cursorPosition = currentIndex
            console.log("redo : Shortcut activated.")
        }else{

            console.log("redo : Shortcut disable.")
        }
    }




    Shortcut {
        context: Qt.WindowShortcut
        sequences: ["Ctrl+Z"]
        onActivated:undoClicked()
    }
    Shortcut {
        context: Qt.ApplicationShortcut
        sequences: ["Shift+Ctrl+Z"]
        onActivated: redoClicked()
    }


    Column{
        id:column
        width: window.width
        height: window.height
        Rectangle {
            id: result
            color: "white"
            width:column.width
            height: column.height/4

            Text {
                id: resultText
                text: qsTr("Result")
                anchors.fill: parent
                font.pixelSize: 25
            }
        }
        Rectangle {

            id: show
            color: "lightgray"
            width:column.width
            height:column.height /4

            ShowText{
                id:showTextCpp



            }
            //            Rectangle {

            //                id: show
            //                color: "lightgray"
            //                width:column.width
            //                height:column.height /4

            TextInput {
                id: showText
                width: show.width
                height: show.height
                text: showText.text
                anchors.margins: 10
                onAccepted: { showText.isButtomClicked = true
                    resultText.text =  showTextCpp.mResult(showText.text)}






                onSelectedTextChanged: { currentIndex = selectionStart

                    cursorPosition = currentIndex
                }










                Keys.onPressed: {


                    if (event.key === Qt.Key_Backspace){
                        if( showText.text.length == 0){
                            console.log("Empty")
                        }else{
                            showText.isBackSpaceClicked = true
                            console.log(showText.text)
                             showTextCpp.appendHistory(showText.text,true)
                            redo.mouseAreaIsActive = false
                            undo.mouseAreaIsActive = true

                    }
                    }



                    if (event.matches(StandardKey.Undo)) {
                        undoClicked()
                    }
                    if (event.matches(StandardKey.Redo)) {
                        redoClicked()
                    }


                }










                property bool isButtomClicked: false
                property bool isBackSpaceClicked: false

                function changeInShowText(){
                    ++currentIndex
                    showText.text =showTextCpp.text
                    undo.mouseAreaIsActive = true
                    showText.cursorPosition = currentIndex

                }


                onTextChanged: {//keyBoard Task

                   if(isBackSpaceClicked == true){

                       showTextCpp.text = showText.text
                       console.log(showText.text)


}
                    if(isButtomClicked == false){
                        //                        showTextCpp.text +=displayEditText //change another Show text

                        showTextCpp.text= showText.text
                        showTextCpp.appendHistory(showText.text);
                        redo.mouseAreaIsActive = false
                        undo.mouseAreaIsActive = true
                        ++currentIndex


                    }
                    isButtomClicked = false
                    isBackSpaceClicked = false
                }
                font.pixelSize: 25
                //                      onSetText: console.log(x + "3")




            }

        }
        Rectangle {
            id: page
            color: "lightgray"
            width:column.width
            height:column.height/2

            function appendTextToDisplay(id,bool){
                if(bool === true){///display buttom
                    showTextCpp.text +=id.displayEditText //change another Show text
                }


            }

            Grid{
                id: numberpick
                rows: 4; columns: 4; spacing: 3

                Items { id:buttom7;displayEditText: "7" ; width: page.width/6 ; height: page.height/4;showText.onTextChanged:{
                        showText.isButtomClicked = true
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text

                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()




                    }}
                Items { id:buttom8;displayEditText: "8" ;  width: page.width/6 -3; height: page.height/4;showText.onTextChanged:{ if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttom9;displayEditText: "9" ;  width: page.width/6 -3; height: page.height/4;showText.onTextChanged:{ if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttomSlash;displayEditText: "/" ;  width: page.width/6 -3; height: page.height/4;showText.onTextChanged:{  if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttom4;displayEditText: "4" ;  width: page.width/6 ; height: page.height/4 -3;showText.onTextChanged:{ if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttom5;displayEditText: "5" ;  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{ if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items{ id:buttom6;displayEditText: "6" ;  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttomx;displayEditText: "x" ;  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttom1;displayEditText: "1" ; width: page.width/6 ; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttom2;displayEditText: "2" ; width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttom3;displayEditText: "3";  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttomline;displayEditText: "-";  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttom0;displayEditText: "0";  width: page.width/6 ; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttomPoint;displayEditText: "." ;  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttomPercent;displayEditText: "%" ;  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items { id:buttomSum;displayEditText: "+" ;  width: page.width/6 -3; height: page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || currentIndex === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{


                            showTextCpp.getDisplayText(currentIndex,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
            }


            Rectangle{
                id:sum
                width: page.width/3 -3 ; height:  page.height/4  -3

                anchors.left: numberpick.right
                anchors.bottom: numberpick.bottom
                radius: 7
                anchors.leftMargin: 3
                Items{displayEditText: "=" ; width: sum.width ; height:  sum.height ; showText.onTextChanged: {
                        showText.isButtomClicked = true
                        resultText.text =  showTextCpp.mResult(showText.text)
                    }}




            }

            Grid{
                id: numberpick2
                anchors.bottom : sum.top; anchors.left: numberpick.right
                anchors.leftMargin: 3
                anchors.rightMargin: 3
                anchors.bottomMargin: 3
                ;rows: 3;columns: 2;spacing: 3
                Items{id:undo;displayEditText: "<-" ; width: page.width/6 -3 ; height:  page.height/4


                    mouseAreaIsActive: false;showText.onTextChanged:undoClicked()}
                Items{id:redo;displayEditText: "->"; width: page.width/6 -3 ; height:  page.height/4;

                    mouseAreaIsActive: false;showText.onTextChanged:redoClicked()}
                Items{id:openPar;displayEditText: "(";width: page.width/6 -3 ; height:  page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || showText.selectionStart === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{

                            showTextCpp.getDisplayText(showText.selectionStart,displayEditText)
                        }//change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items{id:closePar;displayEditText: ")"; width: page.width/6 -3 ; height:  page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || showText.selectionStart === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{

                            showTextCpp.getDisplayText(showText.selectionStart,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items{id:buttomX2;displayEditText: "x²"; width: page.width/6 -3 ; height:  page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || showText.selectionStart === showText.text.length){
                            showTextCpp.text +="²"
                        }else{

                            showTextCpp.getDisplayText(showText.selectionStart,"²")
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()}}
                Items{id:buttomRad;displayEditText: "√";width: page.width/6 -3  ; height:  page.height/4 -3;showText.onTextChanged:{
                        if(showText.selectionStart === "" || showText.selectionStart === showText.text.length){
                            showTextCpp.text +=displayEditText
                        }else{

                            showTextCpp.getDisplayText(showText.selectionStart,displayEditText)
                        } //change another Show text
                        showText.isButtomClicked = true
                        showTextCpp.appendHistory(showTextCpp.text);
                        redo.mouseAreaIsActive = false
                        showText.changeInShowText()

                    }}

            }

        }

    }







}








/*##^##
Designer {
    D{i:2;anchors_x:37;anchors_y:18}D{i:1;anchors_x:57}D{i:3;anchors_x:134}D{i:49;anchors_height:78;anchors_width:590;anchors_x:0;anchors_y:120}
}
##^##*/
