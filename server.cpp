#include "server.h"

Request::Request(int i, QByteArray b){
    id = i;
    type = QString(b).split("#")[0];
    event_time = QString(b).split("#")[1].toLongLong();
    content = b.right(b.length()-type.length()-15);
}

Server::Server()
{
    mSocket = new QUdpSocket();
    initSocket();
}

Server::~Server(){

}

void Server::initSocket()
{
    mSocket = new QUdpSocket(this);
    mSocket->bind(QHostAddress::AnyIPv4, 50090);
    connect(mSocket, &QUdpSocket::readyRead, this, &Server::readPendingDatagrams);

    qDebug() << "[Server] Bind!";
}

void Server::readPendingDatagrams()
{
    //读取来自客户端（相机、机械臂）的信息
    //
    QByteArray array;
    QHostAddress address;
    quint16 port;
    array.resize(mSocket->bytesAvailable());
    mSocket->readDatagram(array.data(), array.size(), &address, &port);

    if (QString(array).length() <= 3) return;
    Client tempClient(address, port, mNextId);

    if (!mClientPool.contains(tempClient)){
        mClientPool.append(tempClient);
        mNextId++;
        qDebug() << "New Client: " << array << address << port << tempClient.id << "\nTotal:" << mClientPool.length();
    }

    mRequestPool.append(new Request(mClientPool[mClientPool.indexOf(tempClient)].id, array));
}

Request* Server::getRequest(){
    if (mRequestPool.length()){
        Request* r = mRequestPool.first();
        mRequestPool.pop_front();
        return r;
    }else return nullptr;
}

void Server::broadcast(QString msg){
    for (auto i:mClientPool){
        //qDebug()<<"Sendto:"<<i.addr<<i.port;
        mSocket->writeDatagram(msg.toLocal8Bit(), i.addr, i.port);
    }
}

void Server::sendTo(QString msg, int id){
    for (auto i:mClientPool){
        if (i.id == id){
            qDebug()<<"Sendto:"<<i.addr<<i.port;
            mSocket->writeDatagram(msg.toLocal8Bit(), i.addr, i.port);
            break;
        }
    }
}
