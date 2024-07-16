#include <iostream>

#include "IPCServer.hpp"

IPCServer::IPCServer(Reactor& reactor, std::string localAddress)
: _reactor(reactor), _localAddress(localAddress){
  if ((_handle = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, _localAddress.c_str());

  if (bind(_handle, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(1);
  }

  if (listen(_handle, 1) == -1) {
    perror("listen error");
    exit(1);
  }
}

IPCServer::~IPCServer() {
  unlink(_localAddress.c_str());
}

void IPCServer::start() {
  _reactor.registerEventHandler(_handle, this, EventType::Read);

  // do something else
  // std::thread thread = std::thread([this](){
  //    while(!_ipcServerDone) {
  //       std::this_thread::sleep_for(std::chrono::miliseconds(1000))
  //    }
  // });
  //
  // thread.detach();
}

void IPCServer::stop() {
  _ipcServerDone = true;
}

void IPCServer::handleRead(int handle) {
  std::thread thread = std::thread([this](){
    int cl, rc;
    char buf[100];

    std::cout << "handleRead called" << std::endl;
    if ((cl = accept(_handle, NULL, NULL)) == -1) {
      std::cout << "Accept error" << std::endl;
    }

    std::cout << "New connection accepted" << std::endl;

    if ((rc = red(cl, buf, sizeof(buf))) > 0) {
      printf("Read %u bytes: %.*s\n\n", rc, rc, buf);
    }

    close(cl);
  });

  thread.join();
}
