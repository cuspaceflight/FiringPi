#include <stdio.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>

#define PORT 6969
#define MAX_CONNECTIONS 10


class Server
{
public:
  struct sockaddr_in addr;
  struct sockaddr_in self;
  int s_sock;
  int connected;
  struct pollfd sockfds[MAX_CONNECTIONS];

  Server();
  void remove_connection(int fd);
  void manage_connections();

};
