#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <QtCore>
#include <QtGui>
#include "QtNetwork/QTcpSocket"
#include "QtNetwork/QtNetwork"
#include <QtSql/QSql>

class ClientSocket;
class Server;

//用户详细信息
struct UserInformation
{
	QString account;			//帐号
	QString password;			//密码
	QString nickName;			//呢称
	int avatarNumber;			//头像编号
	QString birthday;			//生日
	QString city;				//所在城市

	UserInformation()
    {	avatarNumber = 0;	}

	//重载输入输出操作符
	friend QDataStream & operator<< (QDataStream &qos, const UserInformation &pi)
	{	
		qos << pi.account << pi.password << pi.nickName << pi.avatarNumber
            << pi.birthday<< pi.city ;
		return qos;
	}
	friend QDataStream & operator>> (QDataStream &qis, UserInformation &pi)
	{	
		qis >> pi.account >> pi.password >> pi.nickName >> pi.avatarNumber
            >> pi.birthday >> pi.city;
		return qis;
	}
};

struct TempStrings
{
	QString one;
	QString two;
	QString three;
	friend QDataStream & operator<< (QDataStream &qos, const TempStrings &ti)
	{
		qos << ti.one << ti.two << ti.three;
		return qos;
	}
	friend QDataStream & operator>> (QDataStream &qis, TempStrings &ti)
	{
		qis >> ti.one >> ti.two >> ti.three;
		return qis;
	}
};

//用户登录信息
struct LoginInformation
{
	QString account;
	QString password;

	LoginInformation()
    {	}
	friend QDataStream & operator<< (QDataStream &qos, const LoginInformation &li)
	{	
        qos << li.account << li.password;
		return qos;
	}
	friend QDataStream & operator>> (QDataStream &qis, LoginInformation &li)
	{	
        qis >> li.account >> li.password;
		return qis;
	}
};

//好友信息
struct FriendInformation
{
	QString account;
	QString name;
	int avatarNumber;
	int friendKind;			//好友类型
	QString remark;			//好友备注

	FriendInformation()
    {	avatarNumber = friendKind = 0;	}

	friend QDataStream & operator<< (QDataStream &qos, const FriendInformation &fi)
	{	
        qos << fi.account << fi.name << fi.avatarNumber << fi.friendKind << fi.remark;
		return qos;
	}
	friend QDataStream & operator>> (QDataStream &qis, FriendInformation &fi)
	{	
        qis >> fi.account >> fi.name >> fi.avatarNumber >> fi.friendKind >> fi.remark;
		return qis;
	}
};

struct Message
{
	int kind;				//消息类型
	QString sender;			//发送者
	QString receiver;		//接收者
	QString text;			//消息内容

	Message()
	{	kind = 0;	}

	friend QDataStream & operator<< (QDataStream &qos, const Message &mes)
	{
		qos << mes.kind << mes.sender << mes.receiver << mes.text;
		return qos;
	}
	friend QDataStream & operator>> (QDataStream &qis, Message &mes)
	{
		qis >> mes.kind >> mes.sender >> mes.receiver >> mes.text;
		return qis;
	}
};

struct GroupMessage
{
    int kind;				//消息类型
    QString sender;			//发送者
    QVector<QString> receiver;		//接收者
    QString text;			//消息内容

    GroupMessage()
    {	kind = 10;}
    friend QDataStream & operator<< (QDataStream &qos, const GroupMessage &mes)
    {
        qos << mes.kind << mes.sender << mes.receiver << mes.text;
        return qos;
    }
    friend QDataStream & operator>> (QDataStream &qis, GroupMessage &mes)
    {
        qis >> mes.kind >> mes.sender >> mes.receiver >> mes.text;
        return qis;
    }
};

//存储临时信息的结构体，用于信号/糟
struct saveStruct
{
	qint32 requestKind;
	qint32 replyKind;
	ClientSocket *clientSocket;

	QString myAccount;
	QString peerAccount;
	UserInformation userInf;
	LoginInformation logInf;
	FriendInformation theFriend;
	Message message;
    GroupMessage groupmessage;
//	qint32 status;
	TempStrings tempStr;
	QVector<Message> messageVec;
	QVector<FriendInformation> friendsVec;
	QVector<QString> accountVec;
};

enum
{
	LOGIN = 1,						//登录
	REGISTER = 2,					//注册
	TALK = 3,						//保存信息
	CHECK_MESSAGE = 4,				//检查有无给自己的信息（登录时检查）
	ADD_FRIEND = 5,					//添加好友
	GET_FRIEND_INFORMATION = 6,		//获取好友信息
	DELETE_FRIEND = 7,				//删除好友
	GET_USER_INFORMATION = 8,		//获取用户（好友）信息
	CHANGE_INFORMATION = 9,			//改变自己的信息
	CHANGE_REMARK = 10,				//改变（好友）备注
	CHANGE_PASSWORD = 11,			//修改密码
//	CHANGE_STATUE = 12,				//改变自身状态
	CHECK_CONNECTION = 19,			//检查是否能与服务器连接
    QUIT = 20,						//退出
    GROUPTALK = 21

};

enum
{
	LOGIN_SUCCESS = 11,				//登录成功
	LOGIN_NO_ACCOUNT = 12,			//没有这个用户名
	LOGIN_WRONG_PWD = 13,			//密码错误
	HAVE_LOGINED = 14,				//该用户已经登录了
	GET_ALL_SUCCESS = 16,

	REGISTER_EXIST = 21,			//该帐号已被注册了
	REGISTER_SUCCESS = 22,			//注册成功

	NO_MESSAGE = 41,				//没有信息
	HAVE_MESSAGE = 42,				//有信息
	HAVE_TALK_MESSAGE = 43,

	FRIEND_REQUESTED = 51,			//好友请求
	FRIEDN_NO_ACCOUNT = 52,			//没有这个用户名
	ALREAD_FRIENDS = 53,			//回应好友请求

	GET_FRIEND_SUCCESS = 61,			//获得好友资料
	DELETE_FRIEND_SUCCESS = 62,				//删除好友成功
	GET_USER_INF_SUCCESS = 63,				//获得（好友的）详细资料
	CHANGE_INFORMATION_SUCCESS = 64,	//改变自身资料成功
	CHANGE_REMARK_SUCCESS = 65,			//修改备注成功

	OLD_PWD_IS_WRONG = 70,				//旧密码是错的
	CHANGE_PWD_SUCCESS = 71,			//修改密码成功
};


enum
{
	OFFLINE = 0,
	INLINE = 1,
};

//（数据库）用户表中的字段
enum
{
	ACCOUNT = 0,
	PASSWORD = 1,
	NAME = 2,
	AVATAR_NUMBER = 3,
//	STATUS = 4,
	MOBILE_NUMBER = 5,
	PHONE_NUMBER = 6,
	BIRTHDAY = 7,
	CITY = 8,
	ABOUT = 9
};

//（数据库）临时信息表中的字段
enum
{
	KIND = 0,
	SENDER = 1,
	RECEIVER = 2,
	TEXT = 3
};

//（数据库）好友表中的字段
enum
{
	VERIFYING = 0,
	FRIEND = 1,
	GOOD_FRIEND = 2,
	MYSELF = 5
};

enum
{
	MESSAGE = 1,				//普通信息
	REQUEST_FRIEND = 2,			//请求添加好友
	AGREE_FRIEND = 3,			//同意好友请求
	DISAGREE_FRIEND = 4,		//不同意好友请求
	END_FRIEND = 5,				//好友添加结束
	BE_DELETED = 8,				//自己被好友删除了
    TALK_MESSAGE = 9,			//聊天信息
//    GROUPTALK_MESSAGE = 10      //群聊信息
};


#endif
