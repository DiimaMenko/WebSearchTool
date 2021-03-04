#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "qmlinteractions.h"
#include "logger.h"
#include <QIcon>
#include <QtWebEngineCore>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    //QtWebEngine::initialize();

    qmlRegisterType<QmlInteractions>("QmlInteractions.myself", 1, 0, "QmlInteractions");
    qmlRegisterType<Logger>("QmlInteractions.myself", 1, 0, "Logger");

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/Logo.png"));

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
