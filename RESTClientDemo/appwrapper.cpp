#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "appwrapper.h"

AppWrapper::AppWrapper(QObject *parent)
    : QObject{parent},
    mNetManager(new QNetworkAccessManager(this)),
    mNetReply(nullptr),
    mDataBuffer(new QByteArray)
{}

void AppWrapper::fetchPosts()
{
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

}

QStringList AppWrapper::jokes() const
{
    return mJokes;
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

        //Gets the "value" array
        QJsonObject data = doc.object();

        QString joke = data["value"].toString();

        qDebug() << joke;
    }
}
