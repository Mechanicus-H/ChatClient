#include "chatclient.h"

Client::Client(QWidget *parent)
    : QWidget(parent), port(2424), strHost("localhost"),
      nextBlockSize(0)
{
    setWindowTitle("Octopus chat");
    mSocket=new QTcpSocket(this);

    console=new QTextEdit;
    txtInput=new QLineEdit;
    hostLine=new QLineEdit;

    console->setReadOnly(true);

    QPushButton *pcmd=new QPushButton("&Send");
    QPushButton *con=new QPushButton("Connect");
    connect(con, SIGNAL(clicked()),
            this, SLOT(slotConnect()));
    connect(pcmd, SIGNAL(clicked()),
            this, SLOT(slotSendToServer()));
    connect(txtInput, SIGNAL(returnPressed()),
            this, SLOT(slotSendToServer()));

    QHBoxLayout *line1=new QHBoxLayout;
    QHBoxLayout *line2=new QHBoxLayout;
    QVBoxLayout *box=new QVBoxLayout;

    line1->addWidget(hostLine);
    line1->addWidget(con);

    line2->addWidget(txtInput);
    line2->addWidget(pcmd);

    box->addWidget(new QLabel("<H1>Client</H1>"));
    box->addLayout(line1);
    box->addWidget(console);
    box->addLayout(line2);
    setLayout(box);
}

Client::~Client()
{

}
//-----------------------------------------------
void Client::setHost(QString sHost)
{
    strHost=sHost;
}
//-----------------------------------------------
void Client::setPort(quint16 iPort)
{
    port=iPort;
}
//-----------------------------------------------
void Client::slotConnect()
{
    if(!hostLine->text().isEmpty())
    {
        auto pair=getAddress(hostLine->text());
        strHost=pair.first;
        port=pair.second;
    }


    mSocket->connectToHost(strHost, port);
    connect(mSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));
}
//-----------------------------------------------
void Client::slotReadyRead()
{
    QDataStream in(mSocket);
    in.setVersion(QDataStream::Qt_5_2);

    for(;;)
    {
        if(!nextBlockSize)
        {
            if(mSocket->bytesAvailable()<sizeof(quint16)) break;
            in >>nextBlockSize;
            qDebug() << nextBlockSize << " " << mSocket->bytesAvailable();
        }

        if(mSocket->bytesAvailable() < nextBlockSize) break;

        QTime time;
        QString str;
        in >> time >> str;
        console->append(time.toString()+" "+str);
        nextBlockSize=0;
    }
}
//-----------------------------------------------
void Client::slotError(QAbstractSocket::SocketError err)
{
    QString str=
            "Error: "+(err == QAbstractSocket::HostNotFoundError?
                           "The host was not found." :
                           err== QAbstractSocket::RemoteHostClosedError ?
                               "The remote host is closed." :
                               err== QAbstractSocket::ConnectionRefusedError ?
                                   "The connection was refused." :
                                   QString(mSocket->errorString()));
    console->append(str);
}
//-----------------------------------------------
void Client::slotSendToServer()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    console->append(QTime::currentTime().toString()+" "+
                    txtInput->text());

    out << quint16(0) << QTime::currentTime() << txtInput->text();
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(quint16));

    mSocket->write(block);
    txtInput->setText("");
}
//-----------------------------------------------
void Client::slotConnected()
{
    console->append("Connected to server");
}
//-----------------------------------------------


// Utilites
//-----------------------
QPair<QString, quint16> getAddress(const QString& str)
{
    auto list=str.split(':');

    return QPair<QString, quint16>{list[0], list[1].toUShort()};
}







