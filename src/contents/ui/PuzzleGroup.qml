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
    id: group_rect
    Layout.alignment: Qt.AlignCenter
    property int group_width
    property int group_height
    antialiasing: true
    radius: 5
}
