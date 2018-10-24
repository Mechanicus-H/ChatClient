#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QWidget>
#include <QtWidgets>
#include <QtNetwork>

class Client : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket *mSocket;
    QTextEdit *console;
    QLineEdit *txtInput;
    QLineEdit *hostLine;
    quint16 nextBlockSize;

    QString strHost;
    quint16 port;

public:
    Client(QWidget *parent = 0);
    virtual ~Client();

    void setHost(QString sHost);
    void setPort(quint16 iPort);

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotConnect();
};
//-----------------------------------------------
// Utilites
//-----------------------------------------------
QPair<QString, quint16> getAddress(const QString& str);

#endif // CHATCLIENT_H
