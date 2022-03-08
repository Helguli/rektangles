# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>

add_executable(rektangles main.cpp puzzlehandler.h puzzlehandler.cpp cellgroup.h cellgroup.cpp resources.qrc)
target_link_libraries(rektangles
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
    KF5::I18n)
install(TARGETS rektangles ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
