/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

import QtQuick 2.6
import QtQuick.Controls 2.1 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.13 as Kirigami
import org.kde.rektangles 0.1 as Rektangles

Rectangle {
    id: cell_rect
    property string value: ""
    radius: 5
    border.width: cell_size / 10
    border.color: "transparent"
    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false
    color: Kirigami.Theme.backgroundColor
    Kirigami.Heading{
        anchors.fill: cell_rect
        text: value
        font.pointSize: cell_size / 3
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    states: [
        State {
            name: "selected"
            PropertyChanges {
                target: cell_rect
                border.color: Kirigami.Theme.highlightColor
            }
        }

    ]
}
