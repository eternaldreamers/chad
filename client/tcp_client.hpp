#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "content.hpp"

class TCPClient {
public:
  TCPClient(const std::string &server_ip, int server_port);
  ~TCPClient();
  
  bool connect();
  void disconnect();
  bool send(const std::string &message);
  std::string receive();

private:
  std::string server_ip_;
  int server_port_;
  int sock_;
  struct sockaddr_in server_address_;
  ContentHandler content_handler_;
};

#endif
