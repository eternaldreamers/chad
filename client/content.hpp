#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include "../shared/contentType.hpp"

class Content {
public:
  virtual ~Content() = default;
  virtual void handleContent(const std::string &content) = 0;
};

class TextPlain : public Content {
public:
  void handleContent(const std::string &content) override;
};

class TextCode : public Content {
public:
  void handleContent(const std::string &content) override;
};

class File : public Content {
public:
  void handleContent(const std::string &content) override;
};

class ContentHandler {
public:
  ContentHandler();
  void handleContent(ContentType type, const std::string &content);

private:
  std::unordered_map<ContentType, std::function<void(const std::string&)>> strategies_;
};

#endif
