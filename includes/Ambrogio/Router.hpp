#ifndef __AMBROGIO_ROUTER_HPP
#define __AMBROGIO_ROUTER_HPP

#include <vector>
#include <map>

#define CPP11LIB boost
#include <boost/regex.hpp>
#include <boost/assign.hpp>

#include "Request.hpp"
#include "Response.hpp"

#define STATUS(res,stat)    res["Status"] = ::Ambrogio::Status[stat]
#define ERROR(res,stat,req) {\
  res["Status"] = ::Ambrogio::Status[stat];\
  res.runError(stat, req);\
}

namespace Ambrogio {
  typedef CPP11LIB::match_results<std::string::const_iterator> regex_match;

  typedef enum {
    HEAD,
    GET,
    POST,
    PUT,
    DELETE
  } Method;

  extern std::map<int, std::string> Status;

  class route {
    bool _named;
    std::vector<std::string> _names;

  public:
    Method method;
    CPP11LIB::regex match;
    void (*func)(Request&, Response&);

    route(Method m, CPP11LIB::regex r, void (*f)(Request&, Response&)) :
      _named(false), method(m), match(r), func(f) {}
    route(Method m, std::string r, void(*f)(Request&, Response&));
    bool isNamed(void);
    std::vector<std::string> getNames(void);
    std::vector<std::string> getMatches(std::string);
  };

  class Router {
    public:
    Router(void);
    void add(Method,
             CPP11LIB::regex,
             void (*func)(Request&,
                          Response&));
    void add(Method,
             std::string,
             void (*func)(Request&,
                          Response&));
    void dispatch(Request&, Response&);
    static void default_404_error(Request&, Response&);
    static void default_500_error(Request&, Response&);
    static void default_error(Request&, Response&);
    void runError(int, Request&, Response&);
    static Method string2method(std::string);
    void onError(int, void (*)(Request&, Response&));
    void defaultHandler(void (*)(Request&, Response&));

    private:
    std::map<int, void (*)(Request&, Response&)> _errors;
    std::vector<route> routes;
    void (*_default_error)(Request&, Response&);
  };
}

#endif // __AMBROGIO_ROUTER_HPP
