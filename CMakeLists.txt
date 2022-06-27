# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>

cmake_minimum_required(VERSION 3.16)

project(rektangles)

include(FeatureSummary)

set(QT5_MIN_VERSION 5.12)
set(KF5_MIN_VERSION 5.65)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)

find_package(Qt5 ${QT5_MIN_VERSION} REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Svg)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Kirigami2 I18n)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
add_subdirectory(src)
add_subdirectory(icons)

install(PROGRAMS org.kde.rektangles.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.rektangles.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
