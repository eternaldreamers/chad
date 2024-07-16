#ifndef GUI_HPP
#define GUI_HPP

#include <string>
#include <iostream>
#include "color.hpp"

class GUI {
public:
  GUI();
  void printMessage(const std::string &message);
  void printError(const std::string &error);
  void printReceivedMessage(const std::string &message);
  std::string getInput();

private:
  void setColor(Color color);
  void resetColor();
};

#endif
