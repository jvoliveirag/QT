#include <QNetworkRequest>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "appwrapper.h"

AppWrapper::AppWrapper(QObject *parent)
    : QObject{parent},
    mNetManager(new QNetworkAccessManager(this)),
    mNetReply(nullptr),
    mDataBuffer(new QByteArray)
{}

void AppWrapper::fetchPosts()
{
    //Clean data before a new request
    mDataBuffer->clear();

    //const QUrl API_ENDPOINT("http://api.icndb.com/jokes/random/"+QString::number(number));
    const QUrl API_ENDPOINT("https://api.chucknorris.io/jokes/random/");

    QNetworkRequest request;
    request.setUrl(API_ENDPOINT);

    mNetReply = mNetManager->get(request);
    connect(mNetReply, &QIODevice::readyRead, this, &AppWrapper::dataReadyRead);
    connect(mNetReply, &QNetworkReply::finished, this, &AppWrapper::dataReadFinished);
}

void AppWrapper::removeLast()
{
    if(!mJokes.isEmpty()){
        mJokes.removeLast();
        resetModel();
    }
}

QStringList AppWrapper::jokes() const
{
    return mJokes;
}

bool AppWrapper::initialize()
{
    mEngine.rootContext()->setContextProperty("Wrapper",this);

    resetModel();

    const QUrl url(u"qrc:/RESTClientDemo/Main.qml"_qs);

    mEngine.load(QUrl(url));
    if (mEngine.rootObjects().isEmpty()){
        return false;
    }else{
        return true;
    }
}

void AppWrapper::dataReadyRead()
{
    mDataBuffer -> append(mNetReply -> readAll());
}

void AppWrapper::dataReadFinished()
{
    //PARSE THE JSON
    if(mNetReply->error()){
        qDebug() << "An error has occured: " << mNetReply -> errorString();
    } else {

        //Turns data into a JSON document
        QJsonDocument doc = QJsonDocument::fromJson(*mDataBuffer);

        // Check if parsing was successful
        if (doc.isNull()) {
            qDebug() << "Error: Failed to parse JSON data. The document is null.";
            return; // Exit the function or handle the error appropriately
        }

        // Check if the JSON document is an object
        if (!doc.isObject()) {
            qDebug() << "Error: JSON data is not an object.";
            return; // Exit the function or handle the error appropriately
        }

        // Gets the root object
        QJsonObject rootObject = doc.object();

        // Check if the root object contains "value" key
        if (!rootObject.contains("value") || !rootObject["value"].isString()) {
            qDebug() << "Error: JSON data format is unexpected.";
            return; // Exit the function or handle the error appropriately
        }

        // Access the joke value
        QString joke = rootObject["value"].toString();

        // Append the joke to the list
        mJokes.append(joke);

        qDebug() << mJokes;

        qDebug() << mJokes.size();

        // EXPOSE THE C++ DATA TO QML
        if(mJokes.size() > 0){
            resetModel();
        }
    }
}

// 'SYNC' DATA WITH QML - reset the conext property passing the new dataset 'mJokes' everytime it is called.
void AppWrapper::resetModel()
{
    mEngine.rootContext()->setContextProperty("myModel", QVariant::fromValue(mJokes)); //in Main.qml the 'model' in 'ListView' has to be 'myModel'
}
