#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)// конструктор
{
    ui->setupUi(this);

    ui->pushButton_connect->hide();
    ui->pushButton_create->hide();

    ui->label_adress->hide();
    ui->label_port->hide();
    ui->lineEdit_port->hide();

    ui->lineEdit_adress->hide();
    ui->line->hide();
    ui->textEdit_output->hide();


    ui->label_nick->hide();
    ui->lineEdit_nick->hide();


    hideall();

    // перемещение кнопки
    QPoint point;
    point = ui->pushButton_connect->pos();
    point.setY(310);
    ui->pushButton_connect->move(point);

    point = ui->pushButton_create->pos();
    point.setY(270);
    ui->pushButton_create->move(point);


    timer = new QTimer(this); // экземпляр таймера
    connect(timer,SIGNAL(timeout()),this,SLOT(timer_tick()));


    setTabOrder(ui->lineEdit_send , ui->lineEdit_spam); // изменения порядка табуляции


    for (int i = 0; i < 99 ;i++ )
    {
        idlist[i] = false;
    }

}

MainWindow::~MainWindow() // деструктор
{
    delete ui;
}

void MainWindow::on_radioButton_client_clicked()// режим клиента
{
    if(swtch) ui->pushButton_timed->click(); // остановка спама

    ui->lineEdit_adress->show();
    ui->label_adress->show();

    ui->lineEdit_port->show();
    ui->label_port->show();

    ui->pushButton_connect->show();
    ui->pushButton_create->hide();

    ui->label_nick->show();
    ui->lineEdit_nick->show();


    if(fl_s) // проверка на существование сервера
    {
        TcpServer->disconnect();
        TcpServer->close();
        for (int  i = 0 ; i < 99 ; i++ )
        {
            if(idlist[i] == true )
            {
                TcpSocketlist[i]->disconnect();
                TcpSocketlist[i]->close();

            }
            idlist[i] = false;

        }
        hideall();
        fl_s = false;
        fl_connect = false;
    }

}

void MainWindow::on_radioButton_server_clicked() // режим сервера
{
    if(swtch) ui->pushButton_timed->click(); // остановка спама

    ui->lineEdit_adress->hide();
    ui->label_adress->hide();

    ui->lineEdit_port->show();
    ui->label_port->show();

    ui->pushButton_connect->hide();
    ui->pushButton_create->show();

    ui->label_nick->hide();
    ui->lineEdit_nick->hide();

    if(fl_c) // проверка на существование сокета
    {
        TcpSocket->disconnect();
        TcpSocket->close();
        hideall();
        fl_c = false;
        fl_connect = false;

    }

}

void MainWindow::on_pushButton_create_clicked() // создание сервера
{
    int port;
    bool fl1;
    port = ui->lineEdit_port->text().toInt(&fl1);
    if(port > 0 && fl1)
    {
        if(port != prevport || !fl_s) // проверка используемого порта
        {
            if(fl_s) // проверка существования сервера
            {
                TcpServer->disconnect();
                TcpServer->close();
                //                TcpSocket->disconnect();
                //                TcpSocket->close();
            }

            TcpServer = new QTcpServer(this);
            //TcpSocket = new QTcpSocket(this);

            fl_s = true;
            prevport = port;

            if(TcpServer->listen(QHostAddress::AnyIPv4,quint16(port)))
            {
                connect(TcpServer,SIGNAL(newConnection()),this,SLOT(new_connection()));
                //QMessageBox::information(this,"Внимaние", "Сервер создан", QMessageBox::Ok);
                ui->textBrowser->append("Сервер создан");
                ui->pushButton_disconnect->show();
                nickname = "Сервер";
                ui->lineEdit_nick->hide();
                ui->textBrowser->show();
                ui->pushButton_send->show();
                ui->lineEdit_send->show();
                ui->line->show();
                ui->pushButton_timed->show();
                ui->spinBox_time->show();
                ui->label_spam->show();
                ui->lineEdit_spam->show();
                ui->pushButton_disconnect->show();


            }
            else
            {
                prevport = 0;
                QMessageBox::information(this,"Внимaние", "Ошибка при создании сервера", QMessageBox::Ok);

            }
        }
        else QMessageBox::information(this,"Внимание", "сервер уже создан", QMessageBox::Ok);

    }
    else
    {
        ui->lineEdit_port->clear();
        QMessageBox::information(this,"Внимание", "Неправильный порт", QMessageBox::Ok);
    }
}

void MainWindow::new_connection() // к нам подключились
{
//    id = socketlist.size();

//    socketlist.append( TcpServer->nextPendingConnection() );

//    if(socketlist.at(id)->waitForReadyRead())

//    {
//        QByteArray ba =   socketlist.at(id)->readAll();
//        nicklist.append(QString(ba));

//    }

//    ui->textBrowser->append("Пользователь "+ nicklist.atend +" подключился"); // сообщение о подключении


//    if(fl_connect) // рассылка сообщения
//    {
//        for(int i ; i <nicklist.size() ; i++)
//        {

//            QByteArray ba = "Пользователь "+ nicklist.at(i).toUtf8() +" подключился";
//            TcpSocketlist[j]->write(ba);

//        }
//    }
//    connect(socketlist.at([id]),&QTcpSocket::readyRead,[=]() // сигнал чтения данных
//    {
//        this->read_data_server(id);
//    });

//    connect(socketlist.at(id),&QTcpSocket::disconnected,[=]()  // сигнал отключения
//    {
//        this->discheck_server(id);
//    });

//    fl_connect = true;


    for (int i = 0; i < 99 ;i++) // прокрутка доступных id
    {
        if(idlist[i] == false) // 1й доступный
        {
            TcpSocketlist[i] = new QTcpSocket(this);
            TcpSocketlist[i] = TcpServer->nextPendingConnection();


            if(TcpSocketlist[i]->waitForReadyRead()) // никнейм подключившегося
            {
                QByteArray ba =   TcpSocketlist[i]->readAll();
                nicklist[i] = QString(ba);
            }

            ui->textBrowser->append("Пользователь "+ nicklist[i] +" подключился"); // сообщение о подключении


            if(fl_connect) // рассылка сообщения
            {
                for (int  j = 0 ; j < 99 ; j++ )
                {
                    if(idlist[j] == true )
                    {

                        QByteArray ba = "Пользователь "+ nicklist[i].toUtf8() +" подключился";
                        TcpSocketlist[j]->write(ba);

                    }
                }
            }

            connect(TcpSocketlist[i],&QTcpSocket::readyRead,[=]() // сигнал чтения данных
            {
                this->read_data_server(i);
            });

            connect(TcpSocketlist[i],&QTcpSocket::disconnected,[=]()  // сигнал отключения
            {
                this->discheck_server(i);
            });

            fl_connect = true;

            //QMessageBox::information(this,"Внимание", "Пользователь подключился", QMessageBox::Ok);

            idlist[i] = true; // занимаем id
            break;

        }

    }

}

void MainWindow::read_data() // чтение данных при клиенте
{
    ui->textBrowser->show();
    ui->pushButton_send->show();
    ui->lineEdit_send->show();
    ui->line->show();
    ui->pushButton_timed->show();
    ui->spinBox_time->show();
    ui->label_spam->show();
    ui->lineEdit_spam->show();
    QByteArray ba;
    ba = TcpSocket->readAll();
    QString str(ba);
    ui->textBrowser->append(str);

}

void MainWindow::on_pushButton_connect_clicked() // подключение клиента
{
    QHostAddress  * adress;
    adress = new QHostAddress;

    bool fl1;

    int port;

    port = ui->lineEdit_port->text().toInt(&fl1);

    if(fl1 && port > 0) // проверка порта
    {
        if(adress->setAddress((ui->lineEdit_adress->text())) && !ui->lineEdit_adress->text().isEmpty()) // проверка адреса
        {
            if(!fl_c  || prevportadres !=  adress->toString() + static_cast<char>(port )) // проверка переподключения
            {
                if(fl_c) // поверка существования сокета
                {
                    TcpSocket->disconnect();
                    TcpSocket->close();
                    hideall();
                }

                TcpSocket = new  QTcpSocket(this);
                fl_c = true;

                connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(read_data()));

                adress->setAddress(ui->lineEdit_adress->text());

                connect(TcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error_slot(QAbstractSocket::SocketError))); // Обработчик ошибок

                TcpSocket->connectToHost(*adress,static_cast<unsigned short >(port));

                connect(TcpSocket,SIGNAL(connected()),this,SLOT(check())); // сигнал подключения


                prevportadres = adress->toString() + static_cast<char>(port);
            }
            else QMessageBox::information(this,"Внимaние", "Соединение уже установлено", QMessageBox::Ok);
        }
        else QMessageBox::information(this,"Внимaние", "Неправильный адрес", QMessageBox::Ok);
    }
    else QMessageBox::information(this,"Внимaние", "Неправильный порт", QMessageBox::Ok);
}

void MainWindow::on_pushButton_send_clicked() // отправка данных
{
    if(ui->radioButton_client->isChecked())
    {
        if(fl_connect)
        {
            if(!ui->lineEdit_send->text().isEmpty())
            {
                QByteArray ba = nickname.toUtf8() +":" + ui->lineEdit_send->text().toUtf8();
                TcpSocket->write(ba);
                ui->textBrowser->append(nickname + ":" + ui->lineEdit_send->text());
                ui->lineEdit_send->clear();
            }
        }
        else QMessageBox::information(this,"Сообщение не отправлено", "Ошибка соединения", QMessageBox::Ok);
    }

    if(ui->radioButton_server->isChecked())
    {
        if(fl_connect)
        {
            if(!ui->lineEdit_send->text().isEmpty())
            {
                for (int  i = 0 ; i < 99 ; i++ )
                {
                    if(idlist[i] == true )
                    {

                        QByteArray ba = nickname.toUtf8() +":" + ui->lineEdit_send->text().toUtf8();
                        TcpSocketlist[i]->write(ba);

                    }

                }

                ui->textBrowser->append(nickname + ":" + ui->lineEdit_send->text());
                ui->lineEdit_send->clear();
            }

        }
        else QMessageBox::information(this,"Сообщение не отправлено", "Ошибка соединения", QMessageBox::Ok);
    }

}


void::MainWindow::check() // интерфейс при подключении
{
    QByteArray ba = nickname.toUtf8();
    TcpSocket->write(ba);

    fl_connect = true;
    ui->textBrowser->show();
    ui->pushButton_send->show();
    ui->lineEdit_send->show();
    ui->line->show();
    ui->pushButton_timed->show();
    ui->spinBox_time->show();
    ui->label_spam->show();
    ui->lineEdit_spam->show();
    ui->pushButton_disconnect->show();

}

void MainWindow::on_pushButton_ip_clicked() // кнопка для ip
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost); // установка local host ip
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) // for по  всем адресам
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost && address.toString().section( ".",-1,-1 ) != "1") // выбор ethernet адреса
            ui->label_ip->setText(address.toString());
    }

}

void::MainWindow::discheck() // Обработка отключения от сервера(старая)
{
    if(swtch) ui->pushButton_timed->click(); // остановка спама
    ui->textBrowser->clear();
    fl_connect = false;


    QMessageBox::information(this,"Внимание", "Пользователь отключился", QMessageBox::Ok);

}

void MainWindow::on_pushButton_timed_clicked() // спам
{
    if(fl_connect)
    {
        if(!swtch) swtch = true;
        else swtch = false;

        if(swtch)
        {
            timer->start(1000 / ui->spinBox_time->value()); // запуск таймера(кол-во раз в секунду)
            ui->spinBox_time->setEnabled(false);
        }
        else
        {
            timer->stop();
            ui->spinBox_time->setEnabled(true);

        }
    }
    else
    {
        QMessageBox::information(this,"Ошибка спама", "Ошибка соединения", QMessageBox::Ok);
        timer->stop();
        ui->spinBox_time->setEnabled(true);
    }

}

void MainWindow::timer_tick() // то, что будет вызываться по таймеру
{


    //    if(!ui->lineEdit_spam->text().isEmpty() && fl_connect)
    //    {
    //        QByteArray ba = ui->lineEdit_spam->text().toUtf8();
    //        TcpSocket->write(ba);
    //        ui->textBrowser->append("Отправлено: " + ui->lineEdit_spam->text());
    //    }



    if(ui->radioButton_client->isChecked())
    {
        if(!ui->lineEdit_spam->text().isEmpty() && fl_connect)
        {
            QByteArray ba = nickname.toUtf8() +":" + ui->lineEdit_spam->text().toUtf8();
            TcpSocket->write(ba);
            ui->textBrowser->append(nickname +":" + ui->lineEdit_spam->text());

        }
    }

    if(ui->radioButton_server->isChecked())
    {
        if(!ui->lineEdit_spam->text().isEmpty() && fl_connect)
        {
            for (int  i = 0 ; i < 99 ; i++ )
            {
                if(idlist[i] == true )
                {

                    QByteArray ba = nickname.toUtf8() +":" + ui->lineEdit_spam->text().toUtf8();
                    TcpSocketlist[i]->write(ba);

                }
            }
            ui->textBrowser->append(nickname + ":" + ui->lineEdit_spam->text());

        }
    }

}

void MainWindow::error_slot(QAbstractSocket::SocketError error ) // обработчик ошибок сокета
{
    if(swtch) ui->pushButton_timed->click(); // остановка спама
    hideall();
    fl_c = false;

    if(error == 0) QMessageBox::information(this,"Внимaние", "Подключение отклонено", QMessageBox::Ok);
    else if(error == 1) QMessageBox::information(this,"Внимaние", "Сервер отключен", QMessageBox::Ok);
    else if(error == 2) QMessageBox::information(this,"Внимaние", "Адрес не найден", QMessageBox::Ok);
    else if(error == 5) QMessageBox::information(this,"Внимaние", "Превышено время подключения", QMessageBox::Ok);
    else if(error == 7) QMessageBox::information(this,"Внимaние", "Ошибка сети", QMessageBox::Ok);
    else if(error == 9) QMessageBox::information(this,"Внимaние", "Сокет недоступен", QMessageBox::Ok);
    else if(error == 19) QMessageBox::information(this,"Внимaние", "Ошибка операции", QMessageBox::Ok);
    else QMessageBox::information(this,"Внимaние", "Ошибка", QMessageBox::Ok); // остальные ошибки

}

void MainWindow::hideall() // скрытие интерфейса чата
{
    ui->textBrowser->hide();
    ui->pushButton_send->hide();
    ui->lineEdit_send->hide();
    ui->line->hide();
    ui->pushButton_timed->hide();
    ui->spinBox_time->hide();
    ui->label_spam->hide();
    ui->lineEdit_spam->hide();
    ui->textBrowser->clear();
    ui->pushButton_disconnect->hide();

}

void MainWindow::server_error(QAbstractSocket::SocketError ) // обработчик ошибок сервера
{
    if(swtch) ui->pushButton_timed->click(); // остановка спама
    QMessageBox::information(this,"Внимaние", "Ошибка сервера", QMessageBox::Ok);
}

void MainWindow::on_pushButton_disconnect_clicked() // кнопка отключения
{
    if(swtch) ui->pushButton_timed->click(); // остановка спама

    if(fl_c) // отключение клиента
    {
        TcpSocket->disconnect();
        TcpSocket->close();
        hideall();
        fl_c = false;

    }

    if(fl_s) // отключение сервера
    {
        for (int  i = 0 ; i < 99 ; i++ )
        {
            if(idlist[i] == true )
            {
                TcpSocketlist[i]->disconnect();
                TcpSocketlist[i]->close();
                idlist[i] = false;

            }

        }
        TcpServer->disconnect();
        TcpServer->close();
        hideall();
        fl_s = false;

        ui->lineEdit_nick->show();

    }

}

void MainWindow::on_lineEdit_nick_textChanged(const QString &arg1) // создание никнейма
{
    nickname = arg1;

}

void MainWindow::read_data_server(int id) // прием данных сервера
{

    QByteArray ba;
    ba = TcpSocketlist[id]->readAll();
    QString str(ba);
    ui->textBrowser->append(str);

    if(fl_connect)
    {

        for (int  i = 0 ; i < 99 ; i++ )
        {

            if(idlist[i] == true  && i != id)
            {

                TcpSocketlist[i]->write(ba);

            }

        }

    }

}




void MainWindow::discheck_server(int id) // дисконнект от сервера
{
    //if(swtch) ui->pushButton_timed->click(); // остановка спама
    //ui->textBrowser->clear();
    //fl_connect = false;
    idlist[id] = false;
    TcpSocketlist[id]->disconnect();
    TcpSocketlist[id]->close();

    int k =0;

    for (int i =0; i < 99 ; i++ )
    {
        if(idlist[i] == false) k++;



    }
    if(k == 99) fl_connect = false;

    ui->textBrowser->append("Пользователь "+ nicklist[id] +" отключился");

    if(fl_connect)
    {
        for (int  i = 0 ; i < 99 ; i++ )
        {
            if(idlist[i] == true )
            {

                QByteArray ba = "Пользователь "+ nicklist[id].toUtf8() +" отключился";
                TcpSocketlist[i]->write(ba);


            }

        }
        //QMessageBox::information(this,"Внимание", "Пользователь отключился", QMessageBox::Ok);



    }
}

