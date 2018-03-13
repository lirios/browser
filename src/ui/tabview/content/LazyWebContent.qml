import QtQuick 2.0
import ".."

Item {
    id: lazyWebContent

    property var parentTabPage
    property var initProperties
    property Component webContentComponent: Component {
        WebContent {
            tab: parentTabPage.tab
            actionManager: parentTabPage.actionManager
            anchors.fill: parent
        }
    }

    anchors.fill: parent

    Binding {
        target: parentTabPage.tab
        property: "title"
        value: initProperties['title']
        when: lazyCreateConnection.enabled
    }

    Binding {
        target: parentTabPage.tab
        property: "url"
        value: initProperties['url']
        when: lazyCreateConnection.enabled
    }

    Binding {
        target: parentTabPage.tab
        property: "iconUrl"
        value: initProperties['iconUrl']
        when: lazyCreateConnection.enabled
    }

    Connections {
        id: lazyCreateConnection

        function tryCreateContent()
        {
            if (parentTabPage.tab !== parentTabPage.activeTab)
                return;

            if (parentTabPage.loadContent)
            {
                delete initProperties['properties']['anchors.fill'];
                webContentComponent.createObject(contentItem, initProperties['properties']);
                enabled = false;
            }
        }

        target: parentTabPage
        onActiveTabChanged: {
            tryCreateContent();
        }
        onLoadContentChanged: {
            tryCreateContent();
        }

    }
}
