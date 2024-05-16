/*
 * @file main.cpp
 * @brief entry point for the program in jetson nano
 */
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 1024

void *receive(void *arg);
void *send_command(void *arg);

void print_usage(const char *prog_name) {
  std::cerr << "Usage: " << prog_name << " <IP> <port>" << std::endl;
}

// ESP32와 연결할 TCP socket 생성 (err: -1)
int create_socket() {
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "socket() error" << std::endl;
  }
  return sockfd;
}

/*
 * @brief ESP32의 server에 연결 (err: -1)
 * @detail 매개변수로 받은 IP와 PORT로socket에 연결
 */
bool connect_socket(int sockfd, const char *ip, int port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    std::cerr << "connect() error" << std::endl;
    return false;
  }
  return true;
}

// ESP32에 message 전송 (err: -1)
bool send_message(int sockfd, const char *message) {
  if (send(sockfd, message, strlen(message), 0) < 0) {
    std::cerr << "send() error" << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char **argv) {
  // 프로그램 실행시 매개변수 개수 확인
  if (argc != 3)
    print_usage(argv[0]);

  int sockfd = create_socket();
  if (sockfd < 0)
    return 1;

  if (!connect_socket(sockfd, argv[1], atoi(argv[2]))) {
    close(sockfd);
    return 1;
  }

  // test용 메시지 - esp에서 받아, echo 테스트 필요
  const char *message = "Hello, server!";
  // test 메시지 보내기 (err: -1)
  if (!send_message(sockfd, message)) {
    close(sockfd);
    return 1;
  }

  // read message from ESP32
  pthread_t receive_thread, send_thread;
  // receive()를 THREAD로 실행 (success: 0)
  if (pthread_create(&receive_thread, nullptr, receive, &sockfd) != 0) {
    std::cerr << "receive pthread_create() error\n" << std::endl;
    close(sockfd);
    return 1;
  }

  // send command to ESP32 until it exits
  if (pthread_create(&send_thread, nullptr, send_command, &sockfd) != 0) {
    std::cerr << "send_command pthread_create() error\n" << std::endl;
    close(sockfd);
    return 1;
  }

  // wait for threads to finish
  pthread_join(receive_thread, nullptr);
  pthread_join(send_thread, nullptr);

  close(sockfd);
}

void *receive(void *arg) {
  int sockfd = *(int *)arg;
  char buffer[BUF_SIZE] = {0};

  int bytesRead;
  while (true) {
    std::cout << "Waiting for message..." << std::endl;
    bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
      if (bytesRead < 0)
        std::cerr << "recv() error\n" << std::endl;
      close(sockfd);
      return nullptr;
    }

    buffer[bytesRead] = '\0'; // ensure string termination (null-terminated)
    std::cout << "Message from server: " << buffer << std::endl;
  }
}

void *send_command(void *arg) {
  int sockfd = *(int *)arg;
  std::string command;

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, command);

    if (command == "exit") {
      close(sockfd);
      return nullptr;
    }

    if (!send_message(sockfd, command.c_str())) {
      close(sockfd);
      return nullptr;
    }
  }
}
