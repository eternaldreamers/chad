#ifndef SERVER_IPCSERVER_HPP
#define SERVER_IPCSERVER_HPP

#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "ractor.hpp"

class IPCServer : public EventHandler {
  private:
    int _handle;
    std::string _localAddress;
    std::atomic<bool> _ipcServerDone;
    Reactor& _reactor;

    void handleRead(int handle) override;

  public:
    IPCServer(Reactor& reactor, std::string localAddress);
    ~IPCServer();

    void start();
    void stop();
};

#endif
