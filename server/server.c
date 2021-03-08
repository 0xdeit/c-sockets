/* from https://geeksforgeeks.org/tcp-server-client-implementation-in-c/ */
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// this headers were not in the src tutorial
#include <strings.h> // bzero()
#include <unistd.h>  // read()

#define MAX 80
#define PORT 8989

// chat interact
void chat(int sockfd) {
  char buff[MAX];
  int n;

  for (;;) {
    bzero(buff, MAX);

    read(sockfd, buff, sizeof(buff));
    printf("From client: %s\t To client: ", buff);
    bzero(buff, MAX);
    n = 0;

    while ((buff[n++] = getchar()) != '\n')
      ;

    write(sockfd, buff, sizeof(buff));

    if (strncmp("exit", buff, 4) == 0) {
      printf("Server exiting...\n");
      break;
    }
  }
}

int main() {
  int sockfd, connfd;
  socklen_t len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    printf("Socket could not be created...\n");
    exit(0);
  } else {
    printf("Socket was created successfully...\n");
  }

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
    printf("socket bind failed\n");
    exit(0);
  } else {
    printf("Socket was binded successfully...\n");
  }

  // TODO: try to get IP address or host name from server

  if ((listen(sockfd, 5)) != 0) {
    printf("Listen failed...\n");
    exit(0);
  } else {
    printf("Server listening ...\n");
  }

  len = sizeof(cli);

  connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
  if (connfd < 0) {
    printf("Server could not accept connection\n");
    exit(0);
  } else {
    printf("Server accepted connection with client\n");
  }

  chat(connfd);
  close(sockfd);

  return 0;
}
