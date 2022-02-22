#include <Control.hpp>

Server::Server()
{
  connected = 0;
  if((s_sock = socket(AF_INET,SOCK_STREAM,0))==0)
  {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, (void*)1, sizeof(int))<0)
  {
    perror("Failed to set socket options");
    exit(EXIT_FAILURE);
  }
  addr.sin_addr.s_addr = INADDR_ANY; //inet_addr("10.9.42.235");
  addr.sin_port = htons(PORT);
  addr.sin_family = AF_INET;
  if (bind(s_sock, (struct sockaddr*)&addr, sizeof(addr))<0)
  {
    perror("Failed to bind socket");
    exit(EXIT_FAILURE);
  }
  sockfds[connected].fd = s_sock;
  sockfds[connected].events = POLLIN | POLLOUT;
  getsockname(s_sock, (struct sockaddr*)&self, (socklen_t*)sizeof(self));
  

  listen(s_sock, 3);

}

void Server::remove_connection(int fd)
{
  for (int i=0; i<=connected; i++)
  {
    if (sockfds[i].fd==fd) { 
      close(fd);
      for (int j=i+1; j<=connected ;j++) { sockfds[j-1]=sockfds[j]; }
      connected--;
      break;
    }
  }
}

void Server::manage_connections()
{
  poll(sockfds, connected, 10);
  for (int i=0; i<=connected; i++)
  {
    if (sockfds[i].revents & ( POLLERR | POLLHUP | POLLNVAL ) )
    {
      remove_connection(sockfds[i].fd);
    }
  }
}
