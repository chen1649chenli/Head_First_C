#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*Define an error function*/
void error(char *msg)
{
 fprintf(stderr, "%s: %s\n", msg, strerror(errno));
 exit(1);
}


int main(int argc, char *argv[]){
  char *advice[] = {
    "Take smaller bites\r\n",
    "Go for the tight jeans. No they do NOT make you look fat.\r\n",
    "One word: inappropriate\r\n",
    "Just for today, be honest. Tell your boss what you *really* think\r\n",
    "You might want to rethink that haircut\r\n"
  };
  /*create a socket*/
  int listener_d = socket(PF_INET,SOCK_STREAM,0);
  if (listener_d == -1){
    error("Can't open a socket");
  }
  /*create a port*/
  struct sockaddr_in name;
  name.sin_family = PF_INET;
  name.sin_port = (in_port_t)htons(30001);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  /*reuse the port immediately after restart*/
  int reuse = 1;
  if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
    error("Can't set the reuse option on the socket");
  /*bind the socket to port 30000*/
  int c = bind (listener_d, (struct sockaddr *) &name, sizeof(name));

  /*listen*/
  listen(listener_d, 10);
  puts("Waiting for connection");

  while(1){
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);

    char *msg = advice[rand()%5];
    send(connect_d, msg, strlen(msg), 0);
    close(connect_d);
  }
  return 0;
}
