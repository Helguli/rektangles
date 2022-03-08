/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

#include <KLocalizedContext>
#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>
#include <iostream>
#include <cstdlib>
#include "puzzlehandler.h"
#include "cellgroup.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("KDE");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication::setApplicationName("Rektangles");
    qmlRegisterType<PuzzleHandler>("org.kde.rektangles", 0, 1, "PuzzleHandler");
    qmlRegisterType<CellGroup>("org.kde.rektangles", 0, 1, "CellGroup");
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.addImportPath("qrc:/");
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    QTextStream out(stdout);

    return app.exec();
}
