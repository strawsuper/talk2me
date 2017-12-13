#ifndef SERVER_H_
#define SERVER_H_

#include "constant.h"
#include "database.h"
#include "clientsocket.h"
#include <QtNetwork/QtNetwork>
class Server : public QTcpServer
{
	Q_OBJECT
private:
    quint16 ServerPort;
	//网络连接的描述符
	int descriptor;	
	saveStruct temp;
	//连接与操作数据库
	ServerDatabase data;
	//已登录的用户列表
	QMap<QString, ClientSocket*> userMap;

public:
	Server(QObject *parent=0);
	~Server();

	//处理好友请求
	void friendRequest(const saveStruct &save);

protected:
	void incomingConnection(int sockDescriptor);

	public slots:
		//客户端断开连接
		void clientDisconnected(const QString &acc);
		//要求clientSocket发送信息
		void sendMessage(const saveStruct &save); 
};

#endif
