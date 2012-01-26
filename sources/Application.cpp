#include <Ambrogio/Application.hpp>
#include <boost/thread.hpp>

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
  Application::run(std::string path) {
    FCGX_Init();
    run(FCGX_OpenSocket(path.c_str(), 400));
  }

  void
  Application::__run_session(FCGX_Request *request, Router *router) {
    Request req(request);
    Response res(request->out, router);
    printParams(request->envp);
    router->dispatch(req, res);
  }

  void
  Application::run(int listen_socket) {
    FCGX_Request *request = new FCGX_Request();
    FCGX_InitRequest(request, listen_socket, 0);

    while (FCGX_Accept_r(request) == 0) {
      boost::thread(&::Ambrogio::Application::__run_session, request, &_router);
      request = new FCGX_Request();
      FCGX_InitRequest(request, listen_socket, 0);
    }

    { Request req(request); }
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
  run(Application& app, int on) {
    app.run(on);
  }

  void
  run(Application& app, std::string on) {
    app.run(on);
  }
}
