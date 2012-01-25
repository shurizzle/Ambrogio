#include "Application.hpp"

namespace Ambrogio {
  void
  Application::route(Method meth, std::string reg, void (*fun)(Request&, Response&)) {
    _router.add(meth, reg, fun);
  }

  void
  Application::route(Method meth, CPP11LIB::regex reg, void (*fun)(Request&, Response&)) {
    _router.add(meth, reg, fun);
  }

#define _ROUTE(method, no) void \
  Application::method(std::string reg, void (*fun)(Request&, Response&)) {\
    route((Method) no, reg, fun);\
  } \
  \
  void \
  Application::method(CPP11LIB::regex reg, void (*fun)(Request&, Response&)) {\
    route((Method) no, reg, fun);\
  }
  _ROUTE(HEAD, 0)
  _ROUTE(GET, 1)
  _ROUTE(POST, 2)
  _ROUTE(PUT, 3)
  _ROUTE(DELETE, 4)
#undef _ROUTE

  void
  Application::run(void) {
    std::string port=":9000";
    int listenQueueBacklog = 400;
    FCGX_Init();
    int listen_socket = FCGX_OpenSocket(port.c_str(), listenQueueBacklog);
    FCGX_InitRequest(&_request, listen_socket, 0);

    while (FCGX_Accept_r(&_request) == 0) {
      {
        Request req(_request);
        Response res(_request.out, &_router);
        printParams(_request.envp);
        _router.dispatch(req, res);
      }
      FCGX_InitRequest(&_request, listen_socket, 0);
    }
  }

  void
  Application::onError(int error, void (*fun)(Request&, Response&)) {
    _router.onError(error, fun);
  }

  void
  Application::defaultHandler(void (*fun)(Request&, Response&)) {
    _router.defaultHandler(fun);
  }

  void
  Application::printParams(FCGX_ParamArray envp) {
    for (char **p = envp; *p; ++p) {
      std::cout << *p << std::endl;
    }
  }

  void
  run(Application& app) {
    app.run();
  }
}
