#include "server.h"
//#include <QtWidgets/QApplication>
#include <QtCore/QTextCodec>
#include <qdebug.h>
int main(int argc, char *argv[])
{
//	QApplication a(argc, argv);

	//确保只运行一次
	QSystemSemaphore sema("ServerKey",1,QSystemSemaphore::Open);
	//在临界区操作共享内存
	sema.acquire();
	QSharedMemory mem("ServerObject");

	// 如果全局对象以存在则退出
	if (!mem.create(1))
	{
        qDebug("errorA server has already been running.");
		sema.release();
		return 0;
	}
	sema.release();

    Server *server = new Server();
    QString ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug()<<"The server is running on\nIP:"<<ipAddress<<"  port: "<<server->serverPort();
    return 0;
//	return a.exec();
}
