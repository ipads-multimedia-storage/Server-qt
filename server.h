#ifndef SERVER_H
#define SERVER_H

#include <QUdpSocket>
#include <QVector2D>

class Client{
public:
    QHostAddress addr;
    quint16 port;
    int id;

    bool operator==(const Client& u) const{
        return u.addr == addr && u.port == port;
    }
    bool operator<(const Client& u) const{
        return u.port > port;
    }

    Client(QHostAddress a, quint16 p, int id):addr(a),port(p), id(id){}
    ~Client(){}
};

class Request{
private:

public:
    QHostAddress addr;
    quint16 port;
    int id;
    long long event_time;

    QString type;
    QByteArray content;
    Request(int i, QByteArray b);
    ~Request();
};

class Server : public QObject
{
    Q_OBJECT
private:
    int mNextId = 0;

    QVector<Request*> mRequestPool;
    QVector<Client> mClientPool;
    QUdpSocket* mSocket;
protected:

public:
    void initSocket();
    void readPendingDatagrams();
    void broadcast(QString msg);
    void sendTo(QString msg, int id);

    Request* getRequest();

    Server();
    ~Server();
};

#endif // SERVER_H
