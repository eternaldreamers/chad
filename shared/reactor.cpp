#include "reactor.hpp"

#include <unistd.h>
#include <iostream>
#include <stdexcept>

Reactor::Reactor() : _dispaching(false) {
  int ret = pipe(_wakeUpPipe.data());
  if (ret != 0) {
    std::runtime_error("pipe");
  }
}

Reactor::~Reactor() {
  unblock();

  // TODO: fix me
  close(_wakeUpPipe[0]);
  close(_wakeUpPipe[1]);
}

void Reactor::registerEventHandler(
  int handle,
  EventHandler* eventHandler,
  unsigned int eventTypeMask,
  bool oneShot
) {
  if (eventTypeMask & EventType::Read) {
    _readEventHandlers.insert(std::pair<int, EventHandlerMapEntry>(
      handle,
      { eventHandler, oneShot }
    ));
  }

  if (eventTypeMask & EventType::Write) {
    _writeEventHandlers.insert(std::pair<int, EventHandlerMapEntry>(
      handle,
      { eventHandler, oneShot }
    ));
  }

  if (eventTypeMask & EventType::Exception) {
    _exceptionEventHandlers.insert(std::pair<int, EventHandlerMapEntry>(
      handle,
      { eventHandler, oneShot }
    ));
  }

  if (!_dispaching) {
    sendWakeUp();
  }
}

void Reactor::unregisterEventHandler(
  int handle,
  unsigned int eventTypeMask
) {
  size_t count = 0;
  
  if (eventTypeMask & EventType::Read) {
    count += _readEventHandlers.erase(handle);
  }

  if (eventTypeMask & EventType::Write) {
     count += _writeEventHandlers.erase(handle);
  }

  if (eventTypeMask & EventType::Exception) {
    count += _exceptionEventHandlers.erase(handle);
  }

  if (count > 0 && !_dispaching) {
    sendWakeUp();
  }
}

void Reactor::handleEvents(const struct timeval* timeout) {
  struct timeval tv;
  struct timeval* tvPtr = nullptr;

  if (timeout != nullptr) {
    tv = *timeout;
    tvPtr = &tv;
  }

  int maxFd = setupFdSets();

  int count = select(maxfd + 1, &_rfds, &_wfds, &_efds, tvPtr);
  if (count > 0) {
    dispatchEventHandlers();
  }
}

void Reactor::unblock() {
  sendWakeUp();
};

int Reactor::setupFdSets() {
  FD_ZERO(&_rfds);
  FD_ZERO(&_wfds);
  FD_ZERO(&_efds);

  FD_SET(_wakeUpPipe[0], &_rfds);
  int maxFd = _wakeUpPipe[0];

  for (const auto& p : _readEventHandlers) {
    FD_SET(p.first, &_rfds);
    maxFd = std::max(maxFd, p.first);
  }
  
   for (const auto& p : _writeEventHandlers) {
    FD_SET(p.first, &_wfds);
    maxFd = std::max(maxFd, p.first);
  }

  for (const auto& p : _exceptionEventHandlers) {
    FD_SET(p.first, &_efds);
    maxFd = std::max(maxFd, p.first);
  }

  return maxFd;
}

void Reactor::dispatchEventHandlers() {
  _dispaching = true;

  if (FD_ISSET(_wakeUpPipe[0], &_rfds)) {
    handleWakeUp();
  }
  
  for (auto it = _readEventHandlers.begin(); it != _readEventHandlers.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &_rfds)) {
      std::cout << "Dispaching read event..." << std::endl;
      if(entry.oneShot) {
        it = _readEventHandlers.erase(it);
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }

  for (auto it = _writeEventHandlers.begin(); it != _writeEventHandlers.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &_wfds)) {
      std::cout << "Dispaching write event..." << std::endl;
      if(entry.oneShot) {
        it = _writeEventHandlers.erase(it);
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }

  for (auto it = _exceptionEventHandlers.begin(); it != _exceptionEventHandlers.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &_efds)) {
      std::cout << "Dispaching exception event..." << std::endl;
      if(entry.oneShot) {
        it = _exceptionEventHandlers.erase(it);
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }

  _dispaching = false;
}

void Reactor::sendWakeUp() {
  int ret;
  char dummy;

  ret = write(_wakeUpPipe[1], &dummy, sizeof(dummy));
  if (ret != sizeof(dummy)) {
    std::runtime_error("write");
  }
}

void Reactor::HandleWakeUp() {
  int ret;
  char dummy;

  ret = read(_wakeUpPipe[0], &dummy, sizeof(dummy));
  if (ret != sizeof(dummy)) {
    std::runtime_error("read");
  }
}
