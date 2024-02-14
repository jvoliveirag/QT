import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Button {
        id: mButton
        text: "Fetch"
        onClicked: {
            Wrapper.fetchPosts()
        }
    }
}
