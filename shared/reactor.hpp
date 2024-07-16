#ifndef SHARED_REACTOR_HPP
#define SHARED_REACTOR_HPP

#include <sys/select.h>
#include <atomic>
#include <array>
#include <map>

#include "eventHandler.hpp"

enum EventType {
  Read,
  Write,
  Exception
};

struct EventHandlerMapEntry {
  Eventhandler* eventHandler;
  bool oneShot;
};

class Reactor {
  private:
    std::array<int, 2> _wakeUpPipe;
    std::atomic_bool _dispaching;

    fd_set _rfds, _wfds, _efds;
    std::map<int, EventHandlerMapEntry> _readEventHandlers;
    std::map<int, EventHandlerMapEntry> _writeEventHandlers;
    std::map<int, EventHandlerMapEntry> _exceptionEventHandlers;

    void dispatchEventHandlers();
    int setupFdSets();
    void sendWakeUp();

  public:
    Reactor();
    ~Reactor();

    void registerEventHandler(
      int handler,
      EventHandler* eventHandler,
      unsigned int eventTypeMask,
      bool oneShot = false
    );
    void unregisterEventHandler(
      int handler,
      unsigned int eventTypeMask
    );
    void handleEvents(const struct timeval* timeout = nullptr);
    void unblock();
};

#endif
