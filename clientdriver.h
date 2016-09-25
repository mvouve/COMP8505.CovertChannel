#ifndef CLIENTDRIVER_H
#define CLIENTDRIVER_H

#include <string>
#include <QObject>
#include <climits>
#include "netutils.h"




// Client Driver definition
class ClientDriver : public QObject
{
    Q_OBJECT
public:
    ClientDriver(std::string, std::string, short);
    ~ClientDriver();
    void writeCharacter(char);
    void finalizePacket();
public slots:
    void process();
signals:
    void finished();
    void error(QString err);
private:
    struct sockaddr_in makeSockAddrIn(char);
    struct tcp_head _tcpHeader;
    short _dstPort;
    std::string _host;
    std::string _msg;
    int _offset;


};

#endif // CLIENTDRIVER_H
