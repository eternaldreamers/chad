#include "gui.hpp"

GUI::GUI() {}

void GUI::printMessage(const std::string &message) {
  setColor(Color::GREEN);
  std::cout << message << std::endl;
  resetColor();
}

void GUI::printError(const std::string &error) {
  setColor(Color::RED);
  std::cerr << error << std::endl;
  resetColor();
}

void GUI::printReceivedMessage(const std::string &message) {
  setColor(Color::BLUE);
  std::cout << "Server: " << message << std::endl;
  resetColor();
}

std::string GUI::getInput() {
  setColor(Color::YELLOW);
  std::string input;
  std::cout << "You: ";
  std::getline(std::cin, input);
  resetColor();
  return input;
}

void GUI::setColor(Color color) {
  std::cout << "\033[" << static_cast<int>(color) << "m";
}

void GUI::resetColor() {
  std::cout << "\033[0m";
}
