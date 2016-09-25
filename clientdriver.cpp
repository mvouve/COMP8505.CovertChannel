#include "clientdriver.h"

ClientDriver::ClientDriver(std::string msg, std::string ip, short dstPort) : QObject(), _dstPort(dstPort),
    _host(ip), _msg(msg), _offset(START_OFFSET)
{
    // Initialise structure to 0.
    memset(&_tcpHeader, 0, sizeof(_tcpHeader));

    // IP Header
    _tcpHeader.ip.ihl = 5;
    _tcpHeader.ip.version = 4;
    _tcpHeader.ip.tot_len = htons(40);
    _tcpHeader.ip.ttl = 64;
    _tcpHeader.ip.protocol = IPPROTO_TCP;
    _tcpHeader.ip.saddr = 0; //NetUtils::host_convert("google.ca");
    _tcpHeader.ip.daddr = NetUtils::host_convert((char *) _host.c_str());

    // TCP Header
    _tcpHeader.tcp.dest = htons(_dstPort);
    _tcpHeader.tcp.doff = 5;
    _tcpHeader.tcp.syn = 1;
    _tcpHeader.tcp.window = htons(512);
}

ClientDriver::~ClientDriver() {

}

/**
 * @brief ClientDriver::process Triggered by thread.
 */
void ClientDriver::process() {
    for(unsigned int i = 0; i < _msg.length(); i++) {
        writeCharacter(_msg.c_str()[i]);
        usleep(1);
    }
    emit finished();
}

void ClientDriver::writeCharacter(char c) {
    struct sockaddr_in sin = makeSockAddrIn(c);
    _tcpHeader.tcp.source = htons(c + _offset);
    finalizePacket();

    int send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(send_socket < 0) {
        emit error(QString("Send socket could not be opened."));
        exit(1);
    }
    sendto(send_socket, &(_tcpHeader), 40, 0, (struct sockaddr *)&sin, sizeof(sin));
    close(send_socket);
    _offset += OFFSET_INCREMENT;
    if(_offset > 60000) {
        _offset = START_OFFSET;
    }
}

/**
 * @brief ClientDriver::finalizePacket Fill in fields with random information before sending.
 */
void ClientDriver::finalizePacket() {
    _tcpHeader.ip.id = (int) (255.0*rand()/(RAND_MAX + 1.0));
    _tcpHeader.tcp.seq = 1+(int)(10000.0*rand()/(RAND_MAX+1.0));
    _tcpHeader.ip.check = NetUtils::in_cksum((unsigned short *)&_tcpHeader.ip, 20);
}

struct sockaddr_in ClientDriver::makeSockAddrIn(char c) {
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = _tcpHeader.ip.daddr;
    sin.sin_port = htons(c + _offset);
    qDebug("hey: %u", (unsigned short) (htons(sin.sin_port) - 1024)); //;

    return sin;
}
