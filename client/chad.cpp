#include "tcp_client.hpp"
#include "gui.hpp"
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

bool starts_with(const std::string &str, const std::string &prefix) {
  return str.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool ends_with(const std::string &str, const std::string &suffix) {
  return str.size() >= suffix.size() && std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::string createMessage(ContentType type, const std::string& data) {
  std::string message;
  uint32_t size = data.size() + 1;
  message.push_back((size >> 24) & 0xFF);
  message.push_back((size >> 16) & 0xFF);
  message.push_back((size >> 8) & 0xFF);
  message.push_back(size & 0xFF);
  message.push_back(static_cast<uint8_t>(type));
  message += data;
  return message;
}

std::string prepareMessage(const std::string& input) {
  if (starts_with(input, "```") && ends_with(input, "```")) {
    std::string code = input.substr(3, input.size() - 6);
    return createMessage(ContentType::TextCode, code);
  } else if (starts_with(input, "file ")) {
    std::string filePath = input.substr(5);
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();
    return createMessage(ContentType::File, fileContent);
  } else {
    return createMessage(ContentType::TextPlain, input);
  }
}

int main() {
  TCPClient client("127.0.0.1", 8080);
  GUI gui;

  if (!client.connect()) {
    gui.printError("Failed to connect to server.");
    return -1;
  }

  gui.printMessage("Connected to server!");

  std::thread receiveThread([&client, &gui]() {
    while (true) {
      std::string message = client.receive();
      if (!message.empty()) {
        gui.printReceivedMessage(message);
      }
    }
  });

  while (true) {
    std::string input = gui.getInput();
    if (input == "/quit") break;
    std::string message = prepareMessage(input);
    if (!message.empty()) {
      client.send(message);
    }
  }

  client.disconnect();
  receiveThread.join();

  return 0;
}
