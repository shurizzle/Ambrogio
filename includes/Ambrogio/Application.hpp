#ifndef __AMBROGIO_APPLICATION_HPP
#define __AMBROGIO_APPLICATION_HPP

#include "Router.hpp"
#include <fcgiapp.h>

namespace Ambrogio {
#define FUNC void (*)(Request&, Response&)
  class Application {
    Router _router;
    FCGX_Request _request;

    static void printParams(FCGX_ParamArray);

  public:
    void route(Method, std::string, FUNC);
    void route(Method, CPP11LIB::regex, FUNC);
    void HEAD(std::string, FUNC);
    void HEAD(CPP11LIB::regex, FUNC);
    void GET(std::string, FUNC);
    void GET(CPP11LIB::regex, FUNC);
    void POST(std::string, FUNC);
    void POST(CPP11LIB::regex, FUNC);
    void PUT(std::string, FUNC);
    void PUT(CPP11LIB::regex, FUNC);
    void DELETE(std::string, FUNC);
    void DELETE(CPP11LIB::regex, FUNC);
    void onError(int, void (*)(Request&, Response&));
    void defaultHandler(void (*)(Request&, Response&));
    void run(std::string);
    void run(int);
    static void __run_session(FCGX_Request*, Router*);
  };
#undef FUNC

  void run(Application&, std::string);
  void run(Application&, int);
}

#endif // __AMBROGIO_APPLICATION_HPP
