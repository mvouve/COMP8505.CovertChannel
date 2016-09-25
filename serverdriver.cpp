#include "serverdriver.h"

ServerDriver::ServerDriver(int port) : _listenPort(port)
{

}

ServerDriver::~ServerDriver() {
    close(_recv_socket);
}

void ServerDriver::process() {
    struct recv_tcp tcpHead;

    int i = 0;

    int recv_socket = 0;
    recv_socket = socket(AF_INET, SOCK_RAW, 6);
    for(;;) {
        if(recv_socket < 0) {
            emit error("receive socket cannot be open. Are you root?");
            emit finished();
        }
        qDebug("%d", i++);
        read(recv_socket, &tcpHead, 9999);
        if(tcpHead.tcp.dest == htons(_listenPort)) {
            emit readByte(htons(tcpHead.tcp.source) % OFFSET_INCREMENT, tcpHead.tcp.source / OFFSET_INCREMENT);
        }
    }



}
