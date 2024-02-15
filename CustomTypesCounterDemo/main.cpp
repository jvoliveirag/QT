#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "counter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Counter>("jao.test.counter",1,0,"Counter");

    QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    const QUrl url(u"qrc:/CustomTyesCounterDemo/Main.qml"_qs);

    engine.load(QUrl(url));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
