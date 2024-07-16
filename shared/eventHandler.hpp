#ifndef SHARED_EVENT_HANDLER_HPP
#define SHARED_EVENT_HANDLER_HPP

class EventHander {
  public:
    virtual ~EventHandler() = default;
    virtual void handleRead(int handler);
    virtual void handleWrite(int handler);
    virtual void handleException(int handler);
};

#endif
