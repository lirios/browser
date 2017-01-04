/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <tim.sueberkrueb@web.de>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
*/

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import Fluid.Core 1.0
import Fluid.Controls 1.0
import core 1.0
import "../.."

TabContent {
    id: content

    function timeString(time) {
        return Qt.formatTime(time, "HH:mm");
    }

    canReload: false
    canGoBack: false
    canGoForward: false
    loading: false
    loadProgress: 100
    iconUrl: Utils.getSourceForIconName("action/settings")
    adaptIconColor: true
    title: "Settings"
    url: "liri://settings"
    hasThemeColor: false

    Flickable {
        anchors {
            fill: parent
            margins: 2 * Units.smallSpacing
        }

        ScrollBar.vertical: ScrollBar {}

        contentHeight: contentItem.childrenRect.height
        interactive: contentHeight > height

        Item {
            id: contentItem
            width: parent.width
            height: childrenRect.height

            Column {
                width: parent.width
                spacing: 2 * Units.smallSpacing

                HeadlineLabel {
                    text: "Settings"
                }

                TitleLabel {
                    text: "Start"
                }

                Label {
                    text: "Start url"
                    font.pixelSize: 16
                }

                GridLayout {
                    columns: 2
                    columnSpacing: 2 * Units.smallSpacing

                    Label {
                        text: "Primary"
                    }

                    TextField {
                        Layout.minimumWidth: 256
                        text: Settings.startConfig.primaryStartUrl
                        onEditingFinished: {
                            if (Settings.startConfig.primaryStartUrl != text) {
                                Settings.startConfig.primaryStartUrl = text;
                                Settings.dirty = true;
                            }
                        }
                    }

                    Label {
                        text: "Dark theme"
                    }

                    TextField {
                        Layout.minimumWidth: 256
                        text: Settings.startConfig.darkStartUrl
                        onEditingFinished: {
                            if (Settings.startConfig.darkStartUrl != text) {
                                Settings.startConfig.darkStartUrl = text;
                                Settings.dirty = true;
                            }
                        }
                    }

                    Label {
                        text: "Incognito"
                    }

                    TextField {
                        Layout.minimumWidth: 256
                        text: Settings.startConfig.incognitoStartUrl
                        onEditingFinished: {
                            if (Settings.startConfig.incognitoStartUrl != text) {
                                Settings.startConfig.incognitoStartUrl = text;
                                Settings.dirty = true;
                            }
                        }
                    }
                }

                TitleLabel {
                    text: "Search"
                }

                ColumnLayout {
                    RadioButton {
                        text: "DuckDuckGo"
                        checked: Settings.searchConfig.searchEngine == SearchConfig.DuckDuckGo
                        onClicked: {
                            if (Settings.searchConfig.searchEngine != SearchConfig.DuckDuckGo) {
                                Settings.searchConfig.searchEngine = SearchConfig.DuckDuckGo;
                                Settings.dirty = true;
                            }
                        }
                    }

                    RadioButton {
                        text: "Google"
                        checked: Settings.searchConfig.searchEngine == SearchConfig.Google
                        onClicked: {
                            if (Settings.searchConfig.searchEngine != SearchConfig.Google) {
                                Settings.searchConfig.searchEngine = SearchConfig.Google;
                                Settings.dirty = true;
                            }
                        }
                    }

                    RadioButton {
                        text: "Bing"
                        checked: Settings.searchConfig.searchEngine == SearchConfig.Bing
                        onClicked: {
                            if (Settings.searchConfig.searchEngine != SearchConfig.Bing) {
                                Settings.searchConfig.searchEngine = SearchConfig.Bing;
                                Settings.dirty = true;
                            }
                        }
                    }

                    RadioButton {
                        text: "Yahoo"
                        checked: Settings.searchConfig.searchEngine == SearchConfig.Yahoo
                        onClicked: {
                            if (Settings.searchConfig.searchEngine != SearchConfig.Yahoo) {
                                Settings.searchConfig.searchEngine = SearchConfig.Yahoo;
                                Settings.dirty = true;
                            }
                        }
                    }

                    ColumnLayout {
                        RowLayout {
                            RadioButton {
                                text: "Custom"
                                checked: Settings.searchConfig.searchEngine == SearchConfig.Custom
                                onClicked: {
                                    if (Settings.searchConfig.searchEngine != SearchConfig.Custom) {
                                        Settings.searchConfig.searchEngine = SearchConfig.Custom;
                                        Settings.dirty = true;
                                    }
                                }
                            }

                            TextField {
                                Layout.minimumWidth: 256
                                enabled: Settings.searchConfig.searchEngine == SearchConfig.Custom
                                text: Settings.searchConfig.customSearchUrl
                                placeholderText: "e.g https://example.com/?q="
                                onEditingFinished: {
                                    if (Settings.searchConfig.customSearchUrl != text) {
                                        Settings.searchConfig.customSearchUrl = text;
                                        Settings.dirty = true;
                                    }
                                }
                            }
                        }
                    }
                }

                TitleLabel {
                    text: "Theme"
                }

                ThemeSelection {
                    width: Math.min(parent.width, 400)
                    title: "Primary theme"
                    selectedName: Settings.themeConfig.primary
                    onSelected: {
                        Settings.themeConfig.primary = selectedName;
                        Settings.dirty = true;
                    }
                }

                RowLayout {
                    CheckBox {
                        id: radioButtonDarkBetween
                        text: "Secondary theme between"
                        checked: Settings.themeConfig.secondaryEnabled && (timeString(Settings.themeConfig.secondaryStartTime)
                                                                          !== timeString(Settings.themeConfig.secondaryEndTime))
                        onClicked: {
                            if (checked) {
                                Settings.themeConfig.secondaryEnabled = true;
                                if (inputStartTime.text == inputEndTime.text) {
                                    inputStartTime.text = "21:00";
                                    inputEndTime.text = "07:00"
                                }
                                Settings.themeConfig.setSecondaryStartTime(inputStartTime.text, "HH:mm");
                                Settings.themeConfig.setSecondaryEndTime(inputEndTime.text, "HH:mm");
                            }
                            else {
                                Settings.themeConfig.secondaryEnabled = false;
                            }
                            SecondaryThemeTimer.update();
                            Settings.dirty = true;
                        }
                    }

                    TextField {
                        id: inputStartTime
                        Layout.maximumWidth: 56
                        text: Qt.formatTime(Settings.themeConfig.secondaryStartTime, "HH:mm")
                        enabled: radioButtonDarkBetween.checked
                        maximumLength: 5
                        validator: RegExpValidator {
                            regExp: /^([0-1][0-9]|2[0-3]):([0-5][0-9])/
                        }
                        onEditingFinished: {
                            Settings.themeConfig.setSecondaryStartTime(text, "HH:mm");
                            SecondaryThemeTimer.update();
                            Settings.dirty = true;
                        }

                    }

                    Label {
                        text: "and"
                    }

                    TextField {
                        id: inputEndTime
                        Layout.maximumWidth: 56
                        text: Qt.formatTime(Settings.themeConfig.secondaryEndTime, "HH:mm")
                        enabled: radioButtonDarkBetween.checked
                        maximumLength: 5
                        validator: RegExpValidator {
                            regExp: /^([0-1][0-9]|2[0-3]):([0-5][0-9])/
                        }
                        onEditingFinished: {
                            Settings.themeConfig.setSecondaryEndTime(text, "HH:mm");
                            SecondaryThemeTimer.update();
                            Settings.dirty = true;
                        }
                    }
                }

                ThemeSelection {
                    enabled: Settings.themeConfig.secondaryEnabled
                    width: Math.min(parent.width, 400)
                    title: "Secondary theme"
                    selectedName: Settings.themeConfig.secondary
                    onSelected: {
                        Settings.themeConfig.secondary = selectedName;
                        Settings.dirty = true;
                    }
                }

                ThemeSelection {
                    width: Math.min(parent.width, 400)
                    title: "Incognito theme"
                    selectedName: Settings.themeConfig.incognito
                    onSelected: {
                        Settings.themeConfig.incognito = selectedName;
                        Settings.dirty = true;
                    }
                }
            }
        }
    }
}
