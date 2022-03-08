/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

import QtQuick 2.6
import QtQuick.Controls 2.1 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.13 as Kirigami
import org.kde.rektangles 0.1 as Rektangles

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Rektangles")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    property int counter: 0
    property int grid_spacing: 5

    property Rektangles.PuzzleHandler m_puzzle_handler: Rektangles.PuzzleHandler{}
    property int cell_size: {
        Math.min((gametable.width - grid_spacing - 2 * Kirigami.Units.smallSpacing) / m_puzzle_handler.colSize - grid_spacing,
                 (gametable.height - grid_spacing - 2 * Kirigami.Units.smallSpacing) / m_puzzle_handler.rowSize - grid_spacing)
    }

    Connections {
        target: m_puzzle_handler
        onPuzzleFinished: {
            showPassiveNotification(i18n("You won!"))
        }
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("Rektangles")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile

        actions: [
            Kirigami.Action {
                text: i18n("New Game")
                icon.name: "list-add"
                onTriggered: {
                    m_puzzle_handler.newPuzzle()
                }
            },
            Kirigami.Action {
                text: i18n("Resize")
                icon.name: "image-resize-symbolic"
                onTriggered: {
                    resizeDialog.open()
                }
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "gtk-quit"
                onTriggered: Qt.quit()
            }

        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    Controls.Dialog {
        id: resizeDialog
        modal: true
        focus: true
        x: (page.width - width) / 2
        y: page.height / 2 - height
        title: i18n("Resize table")
        Kirigami.FormLayout {
            Controls.SpinBox {
                id: rowSpin
                focus:true
                Kirigami.FormData.label: i18n("Rows:")
                from: 4
                to: 20
                value: m_puzzle_handler.rowSize
                editable: true
            }
            Controls.SpinBox {
                id: colSpin
                Kirigami.FormData.label: i18n("Columns:")
                from: 4
                to: 20
                value: m_puzzle_handler.colSize
                editable: true
            }
            Controls.Button {
                id: doneButton
                Layout.fillWidth: true
                text: i18n("Done")
                Keys.onEnterPressed: doneButton.clicked()
                Keys.onReturnPressed: doneButton.clicked()
                onClicked: {
                    m_puzzle_handler.setSize(rowSpin.value, colSpin.value)
                    m_puzzle_handler.newPuzzle()
                    resizeDialog.close()
                    page.forceActiveFocus()
                }
            }
        }
    }

    pageStack.initialPage: page

    Kirigami.Page {
        id: page
        Layout.fillWidth: true
        title: i18n("Rektangles")
        focus: true
        Keys.onLeftPressed: {
            if (m_puzzle_handler.selectedCellId % m_puzzle_handler.colSize != 0) {
                m_puzzle_handler.setSelectedCellId(m_puzzle_handler.selectedCellId - 1)
            }
        }
        Keys.onRightPressed: {
            if ((m_puzzle_handler.selectedCellId + 1) % m_puzzle_handler.colSize != 0) {
                m_puzzle_handler.setSelectedCellId(m_puzzle_handler.selectedCellId + 1)
            }
        }
        Keys.onUpPressed: {
            m_puzzle_handler.setSelectedCellId(m_puzzle_handler.selectedCellId - m_puzzle_handler.colSize)
        }
        Keys.onDownPressed: {
            m_puzzle_handler.setSelectedCellId(m_puzzle_handler.selectedCellId + m_puzzle_handler.colSize)
        }
        Keys.onReturnPressed: {
            showPassiveNotification(m_puzzle_handler.selectedCellId)
            m_puzzle_handler.returnPressed();
        }
        Keys.onEnterPressed: {
            m_puzzle_handler.returnPressed();
        }
        Keys.onSpacePressed: {
            m_puzzle_handler.returnPressed();
        }
        Keys.onPressed: {
            if (event.key == Qt.Key_Q) {
                Qt.quit()
            } else if (event.key == Qt.Key_N) {
                m_puzzle_handler.newPuzzle()
            } else if (event.key == Qt.Key_R) {
                resizeDialog.open()
            }
        }

        activeFocusOnTab: true

        actions.main: Kirigami.Action {
            text: i18n("New Game")
            iconName: "list-add"
            tooltip: i18n("Start new game")
            onTriggered: {
                m_puzzle_handler.newPuzzle()
            }
        }

        actions.left: Kirigami.Action {
            text: i18n("Resize")
            iconName: "image-resize-symbolic"
            tooltip: i18n("Resize table")
            onTriggered: {
                resizeDialog.open();
            }
        }

        GameTable {
            id: gametable
            width: page.width
            anchors.fill: parent
        }

    }

}
