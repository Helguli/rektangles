/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

import QtQuick 2.6
import QtQuick.Controls 2.1 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.13 as Kirigami
import org.kde.rektangles 0.1 as Rektangles


ColumnLayout {
    id: cica
    Layout.alignment: Qt.AlignCenter
    Layout.margins: 0
    Grid {
        id: gametable
        columns: m_puzzle_handler.colSize
        rows: m_puzzle_handler.rowSize

        spacing: grid_spacing
        Layout.alignment: Qt.AlignCenter
        horizontalItemAlignment: Grid.AlignHCenter
        verticalItemAlignment: Grid.AlignVCenter
        Repeater {
            id: cell_repeater
            model: m_puzzle_handler.puzzleValues;
            delegate: PuzzleCell {
                id: cell_rect
                property int indexOfThis: index
                border.width: cell_size / 10
                width: cell_size
                height: cell_size
                state: {
                    if(m_puzzle_handler.selectedCellId == index)
                        "selected"
                    else
                        ""
                }
                value: model.modelData

            }
        }
    }
    Item {
        anchors.fill: gametable
        Repeater {
            id: group_repeater
            model: m_puzzle_handler.cellGroups;
            delegate: PuzzleGroup {
                id: group_rect
                color: model.modelData.color
                group_width: model.modelData.endCol - model.modelData.startCol
                group_height: model.modelData.endRow - model.modelData.startRow
                x: cell_repeater.itemAt(model.modelData.startCol).x
                y: cell_repeater.itemAt(model.modelData.startRow * m_puzzle_handler.colSize).y
                width: cell_size + ((cell_size + grid_spacing) * (group_width))
                height: cell_size + ((cell_size + grid_spacing) * (group_height))

            }
        }

    }
    MouseArea {
        id: area
        anchors.fill: gametable
        hoverEnabled: false
        onPressed: {
            m_puzzle_handler.mousePressed(gametable.childAt(mouse.x, mouse.y).indexOfThis)
        }
        onReleased: {m_puzzle_handler.mouseReleased()}
        onPositionChanged: {m_puzzle_handler.setSelectedCellId(gametable.childAt(mouse.x, mouse.y).indexOfThis)}
    }
}
