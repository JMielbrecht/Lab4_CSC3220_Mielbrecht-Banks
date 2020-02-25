#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

class httpManager : public QObject
{
    Q_OBJECT
public:
    explicit httpManager(QObject *parent = nullptr);
    ~httpManager();

    void sendImageRequest();
    void sendWeatherRequest(QString zip);

signals:
    void ImageReady(QPixmap *image);
    void WeatherJsonReady(QJsonObject* json);

public slots:
    void ImageDownloadedHandler(QNetworkReply * reply);
    void WeatherDownloadHandler(QNetworkReply *reply);
    void processWeatherJson(QJsonObject* json);

private:
    QNetworkAccessManager * imageDownloadManager;
    QNetworkAccessManager *weatherAPIManager;
    QByteArray downloadedData;

};

#endif // HTTPMANAGER_H
