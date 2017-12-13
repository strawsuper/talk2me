#include "server.h"
#include <QtWidgets>
Server::Server(QObject *parent)
: QTcpServer(parent)
{
    quint16 ServerPort=6666;
    if( !this->listen(QHostAddress::Any,ServerPort) )
		//if( !listen() )
	{
		QMessageBox::about(NULL, "Error", "Can't connected");
		exit(2);
	}
}

Server::~Server()
{

}

void Server::incomingConnection(int sockDescriptor)
{
	//建立新连接，并连接相应的信号/糟
	ClientSocket *clientSocket = new ClientSocket(this);
	connect(clientSocket, SIGNAL(deleteSignal(const QString &)),
		this, SLOT(clientDisconnected(const QString &)));
	connect(clientSocket, SIGNAL(sendSignal(const saveStruct &)),
		this, SLOT(sendMessage(const saveStruct &)));
	clientSocket->setSocketDescriptor(sockDescriptor);
}

void Server::clientDisconnected(const QString &acc)
{
	//某个客户端断开连接时，就更新该用户的状态
	userMap.remove(acc);
	data.quitRequest(acc);
}

void Server::sendMessage(const saveStruct &save)
{
	temp.requestKind = save.requestKind;
	//根据save.kind来处理各种信息
	if(LOGIN == temp.requestKind)
	{
		temp.logInf = save.logInf;
		temp.clientSocket = save.clientSocket;
		temp.replyKind = data.loginRequest(temp.logInf, temp.friendsVec);
		QMap<QString, ClientSocket*>::iterator iter;
		save.clientSocket->sendMessage(temp);
	}
	else if(REGISTER == temp.requestKind)
	{
		temp.userInf = save.userInf;
		temp.replyKind = data.registerRequest(temp.userInf);
		QMap<QString, ClientSocket*>::iterator iter;
		save.clientSocket->sendMessage(temp);
	}

	else if(CHECK_MESSAGE == temp.requestKind)
	{
		temp.myAccount = save.myAccount;
		temp.replyKind = data.checkRequest(temp.myAccount, temp.messageVec);
		temp.clientSocket = save.clientSocket;
		userMap.insert(temp.myAccount, temp.clientSocket);
		save.clientSocket->sendMessage(temp);
	}
	else if(TALK == temp.requestKind)
	{
		temp.message = save.message;
		if(TALK_MESSAGE == temp.message.kind)
		{
			temp.replyKind = TALK;
			QMap<QString, ClientSocket*>::iterator iter;
			iter = userMap.find(temp.message.receiver);
			if(userMap.end() == iter)
				data.messageRequest(save.message);
			else
				iter.value()->sendMessage(temp);
		}
	}


    else if(GROUPTALK == temp.requestKind)
    {
        temp.groupmessage = save.groupmessage;
        temp.replyKind = GROUPTALK;
        QVector< QMap<QString, ClientSocket*>::iterator > iters;
        for(int i=0;i<temp.groupmessage.receiver.size();i++){
            iters.push_back(userMap.find(temp.groupmessage.receiver[i]));
        }
        for(int i=0;i<iters.size();i++){
            if(userMap.end() == iters[i])
                data.groupmessageRequest(save.groupmessage,i);
            else
                iters[i].value()->sendMessage(temp);
        }
    }


    else if(CHANGE_PASSWORD == temp.requestKind)//
	{
		temp.tempStr = save.tempStr;
		temp.replyKind = data.changePasswordRequest(temp.tempStr);
		save.clientSocket->sendMessage(temp);
	}
	else if(GET_USER_INFORMATION == temp.requestKind)
	{
		temp.peerAccount = save.peerAccount;
		temp.replyKind = data.getUserInfRequest(temp.peerAccount, temp.userInf);
		save.clientSocket->sendMessage(temp);
	}
	else if(GET_FRIEND_INFORMATION == temp.requestKind)
	{
		temp.peerAccount = save.peerAccount;
		temp.replyKind = data.getFriendInfRequest(temp.peerAccount, temp.theFriend);
		save.clientSocket->sendMessage(temp);
	}
    else if(CHANGE_INFORMATION == temp.requestKind)//
	{
		temp.userInf = save.userInf;
		temp.replyKind = data.changeInformationRequest(temp.userInf);
		save.clientSocket->sendMessage(temp);
	}
	else if(DELETE_FRIEND == temp.requestKind)
	{
		temp.peerAccount = save.peerAccount;
		temp.myAccount = save.myAccount;
		temp.replyKind = data.deleteFriendRequest(
			temp.myAccount, temp.peerAccount);
		save.clientSocket->sendMessage(temp);

		QMap<QString, ClientSocket*>::iterator iter;
		iter = userMap.find(temp.peerAccount);
		if(userMap.end() == iter)
		{
			Message mes;
			mes.kind = BE_DELETED;
			mes.sender = temp.myAccount;
			mes.receiver = temp.peerAccount;
			data.messageRequest(mes);
		}
		else
		{
			temp.replyKind = BE_DELETED;
			temp.peerAccount = temp.myAccount;
			iter.value()->sendMessage(temp);
		}

	}
	else if(CHANGE_REMARK == temp.requestKind)
	{
		temp.message = save.message;
		temp.replyKind = data.changeRemarkRequset(temp.message);
		save.clientSocket->sendMessage(temp);
	}
	else if(ADD_FRIEND == temp.requestKind)
		friendRequest(save);


}

void Server::friendRequest(const saveStruct &save)
{
	temp.message = save.message;
	temp.replyKind = data.addFriendRequest(temp.message);

	if(REQUEST_FRIEND == temp.message.kind)
		save.clientSocket->sendMessage(temp);
	else if(AGREE_FRIEND == temp.message.kind)
	{
		data.getFriendInfRequest(temp.message.receiver, temp.theFriend);
		temp.replyKind = GET_FRIEND_SUCCESS;
		save.clientSocket->sendMessage(temp);
	}

	QMap<QString, ClientSocket*>::iterator iter;
	iter = userMap.find(temp.message.receiver);

	if(userMap.end() == iter)
		data.addMessageRequest(temp.message);
	else
	{
		temp.replyKind = ADD_FRIEND;
		iter.value()->sendMessage(temp);
	}
}

