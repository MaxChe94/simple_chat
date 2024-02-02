#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

#ifdef BUILD_FOR_SERVER
const bool isServer = false;
#else
const bool isServer = true;
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getConnectState(-1);
    if (isServer)
    {
        ui->stackedWidget->setCurrentIndex(1);
        setWindowTitle("Chat Server");
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        setWindowTitle("Chat Client");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getMessage(QString msg)
{
    ui->history->append(msg);
}


void MainWindow::on_startBtn_clicked()
{
    con = new Connection(isServer);
    connect(con, &Connection::messageReceived, this, &MainWindow::getMessage);
    connect(con, &Connection::gettingNewState, this, &MainWindow::getConnectState);
    if (isServer)
    {
        con->start(ui->serverName->text(), "", ui->serverPort->text().toInt());
        name = ui->serverName->text();
    }
    else
    {
        con->start(ui->clientName->text(), ui->clienIp->text(), ui->clientPort->text().toInt());
        name = ui->clientName->text();
    }
}

void MainWindow::on_stopBtn_clicked()
{
    delete con;
    getConnectState(-1);
}

void MainWindow::getConnectState(int state, QString text)
{
    switch (state){
    case -1:
        ui->startBtn->setDisabled(false);
        ui->stopBtn->setDisabled(true);
        ui->sendBtn->setDisabled(true);
        break;
    case 0:
        ui->startBtn->setDisabled(true);
        ui->stopBtn->setDisabled(false);
        ui->sendBtn->setDisabled(true);
        //getMessage("Собеседник отключен");
        break;
    case 1:
        ui->startBtn->setDisabled(true);
        ui->stopBtn->setDisabled(false);
        ui->sendBtn->setDisabled(false);
        getMessage(QString("Клиент %1 подключился").arg(text));
        break;
    default:
        break;
    }
}

void MainWindow::on_sendBtn_clicked()
{
    QString text = prependDateTime(name, ui->message->toPlainText());
    con->sendMsg(text);
    ui->message->clear();
    ui->history->append(text);
}

QString MainWindow::prependDateTime(const QString& n, const QString& s) {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");
    return QString("%1 [%2] %3").arg(formattedDateTime, n, s);
}

void MainWindow::saveLastSession()
{
    QFile file("last_session.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << ui->history->toPlainText();
        file.close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isServer)
    {
        saveLastSession();
    }
    QMainWindow::closeEvent(event);
}
