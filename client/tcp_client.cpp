#include "tcp_client.hpp"
#include <iostream>

TCPClient::TCPClient(const std::string &server_ip, int server_port)
  : server_ip_(server_ip), server_port_(server_port), sock_(-1) {
  server_address_.sin_family = AF_INET;
  server_address_.sin_port = htons(server_port_);
  inet_pton(AF_INET, server_ip_.c_str(), &server_address_.sin_addr);
}

TCPClient::~TCPClient() {
  if (sock_ != -1) {
    close(sock_);
  }
}

bool TCPClient::connect() {
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ == -1) {
    return false;
  }
  if (::connect(sock_, (struct sockaddr *)&server_address_, sizeof(server_address_)) == -1) {
    return false;
  }
  return true;
}

void TCPClient::disconnect() {
  if (sock_ != -1) {
    close(sock_);
    sock_ = -1;
  }
}

bool TCPClient::send(const std::string &message) {
  if (sock_ == -1) {
    return false;
  }
  if (::send(sock_, message.c_str(), message.length(), 0) == -1) {
    return false;
  }
  return true;
}

std::string TCPClient::receive() {
  if (sock_ == -1) {
    return "";
  }
  char buffer[1024];
  int received = recv(sock_, buffer, sizeof(buffer) - 1, 0);
  if (received > 0) {
    buffer[received] = '\0';
    std::string content(buffer + 5, received - 5);
    ContentType type = static_cast<ContentType>(buffer[4]);
    content_handler_.handleContent(type, content);
    return std::string(buffer);
  }
  return "";
}
