#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <connection.h>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void getMessage(QString msg);
    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_sendBtn_clicked();

private:
    Ui::MainWindow *ui;
    Connection *con;
    int port;
    QString ip;
    QString name;
    QString prependDateTime(const QString& n, const QString& s);
};
#endif // MAINWINDOW_H
