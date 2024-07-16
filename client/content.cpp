#include "content.hpp"

void TextPlain::handleContent(const std::string &content) {
  std::cout << "Handling text/plain content: " << content << std::endl;
}

void TextCode::handleContent(const std::string &content) {
  std::cout << "Handling text/code content: " << content << std::endl;
}

void File::handleContent(const std::string &content) {
  std::cout << "Handling file content: " << content << std::endl;
}

ContentHandler::ContentHandler() {
  strategies_[ContentType::TextPlain] = [](const std::string &content) {
    TextPlain().handleContent(content);
  };
  strategies_[ContentType::TextCode] = [](const std::string &content) {
    TextCode().handleContent(content);
  };
  strategies_[ContentType::File] = [](const std::string &content) {
    File().handleContent(content);
  };
}

void ContentHandler::handleContent(ContentType type, const std::string &content) {
  auto it = strategies_.find(type);
  if (it != strategies_.end()) {
    it->second(content);
  } else {
    std::cerr << "No strategy found for content type." << std::endl;
  }
}
