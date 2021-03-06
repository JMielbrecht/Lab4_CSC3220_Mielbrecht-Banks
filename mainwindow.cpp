#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      timer(new QTimer),
      HTTPManager(new httpManager),
      toDoList(new TaskListModel(this))
{
    ui->setupUi(this);
    ui->tableView->setModel(toDoList);
    toDoList->openFile("tasks.csv");
    weatherGrabbed = false;

    connect(timer, SIGNAL(timeout()),
            this, SLOT(setCurrentTime()));

    setCurrentTime();
    timer->start(1000);

    connect(HTTPManager, SIGNAL(ImageReady(QPixmap *)),
            this, SLOT(processImage(QPixmap *)));
    connect(HTTPManager, SIGNAL(WeatherIconReady(QPixmap *)),
            this, SLOT(processWeatherIcon(QPixmap *)));
    connect(HTTPManager, SIGNAL(WeatherJsonReady(QJsonObject* )),
            this, SLOT(processWeatherJson(QJsonObject* )));

    ui->weatherLabel->setText("");
    ui->weatherIconLabel->setText("");
    ui->imageLabel->setText("");
    ui->zipCodeInput->setText("98119");

    //WEATHER GRAB

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCurrentTime()
{
    QTime time = QTime::currentTime();
    QString hour = time.toString("hh");
    QString minute = time.toString("mm");
    QString second = time.toString("ss");

    ui->hourLCD->display(hour);
    ui->minuteLCD->display(minute);
    ui->secondLCD->display(second);

    //LONDON
    QTime london_time = QTime::currentTime().addSecs(28800);
    QString london_hour = london_time.toString("hh");
    QString london_minute = london_time.toString("mm");
    QString london_second = london_time.toString("ss");

    ui->london_HourLCD->display(london_hour);
    ui->london_MinuteLCD->display(london_minute);
    ui->london_SecondLCD->display(london_second);

    //PARIS
    QTime paris_time = QTime::currentTime().addSecs(32400);
    QString paris_hour = paris_time.toString("hh");
    QString paris_minute = paris_time.toString("mm");
    QString paris_second = paris_time.toString("ss");

    ui->paris_HourLCD->display(paris_hour);
    ui->paris_MinuteLCD->display(paris_minute);
    ui->paris_SecondLCD->display(paris_second);

    //REFRESH WEATHER EVERY 5 SECONDS
    if(time.elapsed() % 5000 == 0){
        refreshWeather();
    }
}

void MainWindow::processImage(QPixmap *image)
{
    ui->imageLabel->setPixmap(*image);
}

void MainWindow::processWeatherIcon(QPixmap *image)
{
    ui->weatherIconLabel->setPixmap(*image);
}

void MainWindow::processWeatherJson(QJsonObject *json)
{
    qDebug() << "Json ready";
    QString weather = json->value("weather").toArray()[0].toObject()["main"].toString();
    QString weatherDesc = json->value("weather").toArray()[0].toObject()["description"].toString();
    double temp = json->value("main").toObject()["temp"].toDouble();
    double temp_min = json->value("main").toObject()["temp_min"].toDouble();
    double temp_max = json->value("main").toObject()["temp_max"].toDouble();

    qDebug() << weather;
    qDebug() << weatherDesc;
    qDebug() << temp;
    qDebug() << temp_min;
    qDebug() << temp_max;

    ui->weatherLabel->setText("Current weather: " + weather + ", temp: " + QString::number(temp));

    //GRAB WEATHER ICON
    icon = json->value("weather").toArray()[0].toObject()["icon"].toString();
    HTTPManager->sendWeatherIconRequest(icon);

    //Success -- weather was grabbed
    weatherGrabbed = true;
    /*
     * {
     * "coord":{"lon":-122.36,"lat":47.64},
     * "weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04n"}],
     * "base":"stations",
     * "main":{"temp":46.22,"feels_like":42.55,"temp_min":42.01,"temp_max":50,"pressure":1033,"humidity":81},
     * "visibility":16093,
     * "wind":{"speed":2.84,"deg":149},
     * "clouds":{"all":90},"dt":1582771378,
     * "sys":{"type":1,"id":2674,"country":"US","sunrise":1582728994,
     * "sunset":1582768114},
     * "timezone":-28800,
     * "id":0,
     * "name":"Seattle",
     * "cod":200
     * }
     */
}

void MainWindow::on_imageDownloadButton_clicked()
{
    QString zip = ui->zipCodeInput->text();
    qDebug() << zip;
    HTTPManager->sendImageRequest(zip);
}

void MainWindow::on_weatherDownloadButton_clicked()
{
    QString zip = ui->zipCodeInput->text();
    qDebug() << zip;
    HTTPManager->sendWeatherRequest(zip);
}

void MainWindow::refreshWeather()
{
    if(weatherGrabbed==true){
        QString zip = ui->zipCodeInput->text();
         HTTPManager->sendWeatherIconRequest(icon);
         HTTPManager->sendWeatherRequest(zip);
    }
}

