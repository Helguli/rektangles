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
    GridView {
        id: gametable
        width: (cell_size + grid_spacing) * m_puzzle_handler.colSize
        height: (cell_size + grid_spacing) * m_puzzle_handler.rowSize
        cellWidth: cell_size + grid_spacing
        cellHeight: cell_size + grid_spacing
        flow: GridView.FlowLeftToRight
        layoutDirection: Qt.LeftToRight
        verticalLayoutDirection: GridView.TopToBottom
        Layout.alignment: Qt.AlignCenter
        model: m_puzzle_handler.puzzleValues;
        delegate: PuzzleCell {}
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
                x: gametable.itemAtIndex(model.modelData.startCol).x
                y: gametable.itemAtIndex(model.modelData.startRow * m_puzzle_handler.colSize).y
                width: cell_size + ((cell_size + grid_spacing) * (group_width))
                height: cell_size + ((cell_size + grid_spacing) * (group_height))

            }
        }
    }
    MouseArea {
        id: area
        anchors.fill: gametable
        hoverEnabled: true
        onPressed: {
            m_puzzle_handler.mousePressed(gametable.indexAt(mouse.x, mouse.y))
        }
        onReleased: {m_puzzle_handler.mouseReleased()}
        onPositionChanged: {
            m_puzzle_handler.setSelectedCellId(gametable.indexAt(mouse.x, mouse.y))
        }
    }
}
