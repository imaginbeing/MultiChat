#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QTimer>
#include <QList>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioButton_client_clicked();

    void read_data(void);

    void read_data_server(int id);

    void new_connection(void);

    void on_radioButton_server_clicked();

    void on_pushButton_create_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_send_clicked();

    void check(void);

    void discheck(void);

    void discheck_server(int id);

    void on_pushButton_ip_clicked();

    void on_pushButton_timed_clicked();

    void timer_tick(void);

    void error_slot(QAbstractSocket::SocketError error);

    void hideall();

    void server_error(QAbstractSocket::SocketError);


    void on_pushButton_disconnect_clicked();

    void on_lineEdit_nick_textChanged(const QString &arg1);


private:
    Ui::MainWindow *ui;
    QTcpServer * TcpServer;
    QTcpSocket * TcpSocket;

    QTimer * timer; // указатель на таймер
    int count; //счетчик
    bool swtch = false; // переключение спама
    bool fl_s = false ,fl_c = false; // флаги существования сервера и сокета
    bool fl_connect = false;  // флаг существующего подключения


    int prevport =0;
    QString prevportadres;

    QString nickname = "Анон";

    QString  nicklist[99];

    QTcpSocket * TcpSocketlist[99];

    bool idlist[100];

//    int id;




//        QList <QTcpSocket *> socketlist;
//        QList <QString> nicklist;
//        QList <bool> idlist;

};

#endif // MAINWINDOW_H
