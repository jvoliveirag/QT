#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "appwrapper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    AppWrapper wrapper;
    QQmlApplicationEngine engine;

    engine.rootContext() -> setContextProperty("Wrapper", &wrapper);

    const QUrl url(u"qrc:/RESTClientDemo/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
