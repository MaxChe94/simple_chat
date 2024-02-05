#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <connection.h>
#include <QDateTime>
#include <QFile>
#include <QCloseEvent>
#include <QTextEdit>

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
    void saveLastSession();
    void on_stopBtn_clicked();
    void getConnectState(int state, QString text = "");
    void on_sendBtn_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event);


private:
    Ui::MainWindow *ui;
    Connection *con;
    int port;
    QString ip;
    QString name;
    bool isMessageTextEditActive = false;
    QString prependDateTime(const QString& n, const QString& s);
    void insertNewLine();
};
#endif // MAINWINDOW_H
