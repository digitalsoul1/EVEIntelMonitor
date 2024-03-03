//
// Created by Lukasz Klepacki on 26/09/2023.
//
#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QIcon>

#define APP_MAJOR_VERSION "0"
#define APP_MINOR_VERSION "1"

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
