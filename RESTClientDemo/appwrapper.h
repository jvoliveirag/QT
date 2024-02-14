#ifndef APPWRAPPER_H
#define APPWRAPPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class AppWrapper : public QObject
{
    Q_OBJECT
public:
    explicit AppWrapper(QObject *parent = nullptr);

    Q_INVOKABLE void fetchPosts ();

    Q_INVOKABLE void removeLast();

    QStringList jokes() const;

signals:

public slots:

private slots:
    void dataReadyRead();
    void dataReadFinished();

private:
    QNetworkAccessManager * mNetManager;
    QNetworkReply * mNetReply;
    QByteArray * mDataBuffer;
    QStringList mJokes;
};


#endif // APPWRAPPER_H
