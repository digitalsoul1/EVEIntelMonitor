/*
* Copyright (c) 2024 github.com/digitalsoul1
*
* This program is free software: you can redistribute it and/or modify
        * it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        * General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QIcon>

#define APP_MAJOR_VERSION "0"
#define APP_MINOR_VERSION "2"

int main(int argc, char **argv) {

    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":/images/app-icon.png"));
    auto hiveProtectorEngine = QQmlApplicationEngine();
    QObject::connect(&hiveProtectorEngine, &QQmlApplicationEngine::objectCreationFailed, &app, [](const QUrl &url) {
        qDebug() << "objectCreationFailed" << url;
        QCoreApplication::exit(EXIT_FAILURE);
    }, Qt::QueuedConnection);

    hiveProtectorEngine.rootContext()->setContextProperty("APP_MAJOR_VERSION", APP_MAJOR_VERSION);
    hiveProtectorEngine.rootContext()->setContextProperty("APP_MINOR_VERSION", APP_MINOR_VERSION);
    hiveProtectorEngine.loadFromModule("EVEIntelMonitor", "MainWindow");

    // set app icon

    return QGuiApplication::exec();
}
