#ifndef NETUTILS_H
#define NETUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/ip.h>

#define OFFSET_INCREMENT 200
#define START_OFFSET 2000

struct tcp_head
{
   struct iphdr ip;
   struct tcphdr tcp;
};

struct recv_tcp
{
  struct iphdr ip;
  struct tcphdr tcp;
  char buffer[10000];
};

/* From synhose.c by knight */
struct pseudo_header
{
   unsigned int source_address;
   unsigned int dest_address;
   unsigned char placeholder;
   unsigned char protocol;
   unsigned short tcp_length;
   struct tcphdr tcp;
};

class NetUtils
{
public:
    NetUtils();
    static unsigned int host_convert(char *hostname);
    static unsigned short in_cksum(unsigned short *ptr, int nbytes);
};

#endif // NETUTILS_H
