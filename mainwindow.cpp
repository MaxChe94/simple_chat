#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

extern bool server_mode;

#ifdef BUILD_FOR_SERVER
const bool isServer = true;
#else
const bool isServer = false;
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->message->installEventFilter(this);
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
    case 0: // соединение открыто, нет собеседников
        ui->startBtn->setDisabled(true);
        ui->stopBtn->setDisabled(false);
        ui->sendBtn->setDisabled(true);
        break;
    case 1: // клиент подключился к серверу
        ui->startBtn->setDisabled(true);
        ui->stopBtn->setDisabled(false);
        ui->sendBtn->setDisabled(false);
        getMessage(QString("Клиент %1 подключился").arg(text));
        break;
    case 2: // лиент отключился от  сервера
        ui->startBtn->setDisabled(true);
        ui->stopBtn->setDisabled(false);
        ui->sendBtn->setDisabled(false);
        getMessage(QString("Клиент %1 отключился").arg(text));
        break;
    case 3: // подключено к серверу
        ui->startBtn->setDisabled(true);
        ui->stopBtn->setDisabled(false);
        ui->sendBtn->setDisabled(false);
        getMessage(QString("Подключено к серверу %1").arg(text));
        break;
    case 4:
        ui->startBtn->setDisabled(false);
        ui->stopBtn->setDisabled(true);
        ui->sendBtn->setDisabled(true);
        getMessage(QString("Сервер %1 отключился").arg(text));
        break;
    default:
        break;
    }
}

void MainWindow::on_sendBtn_clicked()
{
    if (ui->sendBtn->isEnabled())
    {
        QString text = prependDateTime(name, ui->message->toPlainText());
        con->sendMsg(text);
        ui->message->clear();
        ui->history->append(text);
    }
}

void MainWindow::insertNewLine()
{
    if (ui->message->hasFocus()) {
            ui->message->insertPlainText("\n");
        }
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->message && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return && keyEvent->modifiers() == Qt::NoModifier) {
            // Нажата клавиша Enter без модификаторов
            on_sendBtn_clicked(); // Вызываем слот отправки сообщения
            return true; // Мы обработали событие, больше ничего не нужно делать
        }
    }
    // Если мы не обработали событие, передаем его дальше по цепочке обработки
    return QObject::eventFilter(obj, event);
}
