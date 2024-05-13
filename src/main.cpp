/*
 * @file main.cpp
 * @brief entry point for the program in jetson nano
 */
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <IP> <port>\n" << std::endl;
  }

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "socket() error\n" << std::endl;
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    std::cerr << "connect() error\n" << std::endl;
    close(sockfd);
    return 1;
  }

  const char *message = "Hello, server!";
  if (send(sockfd, message, strlen(message), 0) < 0) {
    std::cerr << "send() error\n" << std::endl;
    close(sockfd);
    return 1;
  }

  char buffer[BUF_SIZE] = {0};

  int bytesRead;
  while (true) {
    bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytesRead < 0) {
      std::cerr << "recv() error\n" << std::endl;
      close(sockfd);
      return 1;
    }

    if (bytesRead < sizeof(buffer)) {
    }

    std::cout << "Message from Arduino: " << buffer << std::endl;
  }

  close(sockfd);
}
