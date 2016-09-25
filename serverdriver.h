#ifndef SERVERDRIVER_H
#define SERVERDRIVER_H

#include <QObject>
#include "netutils.h"

class ServerDriver : public QObject
{
    Q_OBJECT
public:
    ServerDriver(int);
    ~ServerDriver();
    void listen();
public slots:
    void process();
signals:
    void finished();
    void error(QString);
    void readByte(char, int);
private:
    short unsigned int _listenPort;
    int _recv_socket;

};

#endif // SERVERDRIVER_H
