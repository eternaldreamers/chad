#ifndef SHARED_EVENT_HANDLER_HPP
#define SHARED_EVENT_HANDLER_HPP

class EventHander {
  public:
    virtual ~EventHandler() = default;
    virtual void handleRead(int handle);
    virtual void handleWrite(int handle);
    virtual void handleException(int handle);
};

#endif
