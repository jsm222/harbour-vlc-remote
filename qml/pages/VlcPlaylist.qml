import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.XmlListModel 2.0
import harbour.vlc_remote 1.0
Page {
property string currentId: "0";
    PlaylistModel {
        id: listModel
        password:rootWindow.password
        username:rootWindow.username
        remoteUrl:rootWindow.ip+":"+rootWindow.port
    }

    Timer {
        id: xmlWaitTimer
        interval: 50; running: false; repeat: true
        onTriggered: updateXML()
    }
    Timer {
        id: xmlRefresh
        interval: 50; running: false; repeat: false
        onTriggered: getVLCMedia();
    }
    Timer {
        id:gainFocusTimer
        interval: 10;running:false;repeat: true;
        onTriggered: {listView.headerItem.forceActiveFocus()}
    }

    SilicaListView {
        id: listView
        model:listModel.proxyModel
        anchors.fill: parent
        header: SearchField {
            id:sf
            width: parent.width
            placeholderText: "Search playlist"
            text:""

            onTextChanged: {

                listModel.search=sf.text;
                gainFocusTimer.start();
                if(sf.text==="") {
                    gainFocusTimer.stop()
                }

            }


        }

        PullDownMenu {

            MenuItem{
                text: "Empty Playlist"
                onClicked: passCommands("pl_empty")
            }
            MenuItem {
                text: "Jump to the end"
                onClicked: listView.scrollToBottom()
            }
        }
        PushUpMenu {
            id: pushUpMenu
            spacing: Theme.paddingLarge
            MenuItem {
                text: "Return to Top"
                onClicked: listView.scrollToTop()
            }
        }
        /*        delegate: ListItem {
             width: parent.width; height: 50
           Text {
               color:Theme.primaryColor
               text:name
           }
        }
*/
        delegate: ListItem {
            //           visible: found.indexOf(id) !==-1
            id: delegate
            menu: contextMenu
            function remove() {
                remorseAction("Deleting", function() {
                    passCommands("pl_delete&id=" + id )
                    listModel.update();
                })
            }

            Image{
                id: listIcon
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                opacity: 0.8
                source: id === "-1" ? "image://theme/icon-m-developer-mode" : name === "" ? null : id === currentId ? "icons/icon-cover-play.png" : "image://theme/icon-m-music"
            }

            Text {
                //truncationMode: TruncationMode.Fade
                color: delegate.highlighted || currentId === id ? Theme.highlightColor :  Theme.primaryColor
                text:name
                anchors.left: listIcon.right
                anchors.right: parent.right
                anchors.margins: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: {
                passCommands("pl_play&id=" + id )
                currentId = id;
                listView.forceLayout()

            }

            Component {
                id: contextMenu

                ContextMenu {

                    MenuItem {
                        text: "Remove from Playlist"
                        onClicked: remove()
                    }
                }
            }
        }
        VerticalScrollDecorator {}
    }






}
