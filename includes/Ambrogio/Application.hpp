#ifndef __AMBROGIO_APPLICATION_HPP
#define __AMBROGIO_APPLICATION_HPP

#include <Ambrogio/Router.hpp>
#include <fcgiapp.h>

namespace Ambrogio {
#define FUNC void (*)(Request&, Response&)
  class Application {
    Router _router;
    FCGX_Request _request;

    static void printParams(FCGX_ParamArray);

  public:
    Application() : debug(false) {}
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
    void onError(int, FUNC);
    void defaultHandler(FUNC);
    void run(std::string);
    void run(int);
    static void __run_session(FCGX_Request*, Router*, bool);

    bool debug;
  };
#undef FUNC

  void run(Application&, std::string);
  void run(Application&, int);
}

#endif // __AMBROGIO_APPLICA>ION_HPP
