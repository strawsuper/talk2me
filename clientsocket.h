#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "constant.h"
#include "QtNetwork/QTcpSocket"
#include "QtNetwork/QtNetwork"

class ClientSocket:public QTcpSocket
{
    Q_OBJECT
private:
    saveStruct save;//save all kinds of message
    qint32 blockSize;

public:
    ClientSocket(QObject *parent = 0);
    ~ClientSocket();

signals:
    //ask Server deal with message
    void sendSignal(const saveStruct &temp);
    //user login
    void userLoginSignal(const UserInformation &user);
    //user disconnect
    void deleteSignal(const QString &acc);
public slots:
    //receive and send Message
    void receiveMessage();
    void sendMessage(const saveStruct &temp);
    void deleteSocket();
};
#endif // CLIENTSOCKET_H
