#include <Ambrogio/Router.hpp>

#include <sstream>

namespace Ambrogio {
  std::map<int, std::string> Status = boost::assign::map_list_of
    (100, "100 Continue")
    (101, "101 Switching Protocols")
    (200, "200 OK")
    (201, "201 Created")
    (202, "202 Accepted")
    (203, "203 Non-Authoritative Information")
    (204, "204 No Content")
    (205, "205 Reset Content")
    (206, "206 Partial Content")
    (300, "300 Multiple Choices")
    (301, "301 Moved Permanently")
    (302, "302 Found")
    (303, "303 See Other")
    (304, "304 Not Modified")
    (305, "305 Use Proxy")
    (307, "307 Temporary Redirect")
    (400, "400 Bad Request")
    (401, "401 Unauthorized")
    (402, "402 Payment Required")
    (403, "403 Forbidden")
    (404, "404 Not Found")
    (405, "405 Method Not Allowed")
    (406, "406 Not Acceptable")
    (407, "407 Proxy Authentication Required")
    (408, "408 Request Timeout")
    (409, "409 Conflict")
    (410, "410 Gone")
    (411, "411 Length Required")
    (412, "412 Precondition Failed")
    (413, "413 Request Entity Too Large")
    (414, "414 Request-URI Too Long")
    (415, "415 Unsupported Media Type")
    (416, "416 Requested Range Not Satisfiable")
    (417, "417 Expectation Failed")
    (500, "500 Internal Server Error")
    (501, "501 Not Implemented")
    (502, "502 Bad Gateway")
    (503, "503 Service Unavailable")
    (504, "504 Gateway Timeout")
    (505, "505 HTTP Version Not Supported");

  route::route(Method m, std::string r, void (*f)(Request&, Response&)) {
    method = m;
    _named = true;
    func = f;
    std::stringstream uri (r.c_str());
    std::string re ("^/+");
    char ch;

    while (uri.peek() == '/')
      uri.get();

    while (uri.peek() > 0) {
      switch (ch = uri.get()) {
        case '/':
          while (uri.peek() == '/')
            uri.get();
          re.push_back('/');
          re.push_back('+');

          if (uri.peek() == ':') {
            uri.get();
            re.append("([^/]+)/+");

            {
              std::string name;
              while (uri.peek() > 0 && (ch = uri.get()) != '/')
                name.push_back(ch);
              _names.push_back(name);
            }
          }
          break;
        case '\\':
        case '^':
        case '$':
        case '[':
        case ']':
        case '(':
        case ')':
        case '+':
        case '*':
        case '?':
        case '|':
          re.push_back('\\');
          re.push_back(ch);
          break;
        default:
          re.push_back(ch);
          break;
      }
    }

    if (*(re.end() - 1) == '+')
      re.erase(re.size() - 1, 1);
    if (*(re.end() - 1) != '/')
      re.push_back('/');
    re.push_back('*');
    re.push_back('$');

    match = re;
  }

  bool
  route::isNamed() {
    return _named;
  }

  std::vector<std::string>
  route::getNames(void) {
    return _names;
  }

  std::vector<std::string>
  route::getMatches(std::string uri) {
    regex_match res;
    std::vector<std::string> matches;
    regex_search((std::string::const_iterator) uri.begin(),
                 (std::string::const_iterator) uri.end(),
                 res,
                 match,
                 CPP11LIB::match_default);
    for (regex_match::iterator it = res.begin() + 1; it < res.end(); it++) {
      matches.push_back(std::string(it->first, it->second));
    }

    return matches;
  }

  void
  Router::default_404_error(Request& req, Response& res) {
    STATUS(res, 404);
    res << "<html><body><h1>Page " << req.uri() << " not found</h1></body></html>";
  }

  void
  Router::default_500_error(Request& req, Response& res) {
    STATUS(res, 500);
    res << "<html><body><h1>Error in page " << req.uri() << "</h1></body></html>";
  }

  void
  Router::default_error(Request& req, Response& res) {
    STATUS(res, 500);
    res << "<html><body><h1>Unknown error in page " << req.uri() << "</h1></body></html>";
  }

  Router::Router(void) {
    _errors[404] = Router::default_404_error;
    _errors[500] = Router::default_500_error;
    _default_error = Router::default_error;
  }

  void
  Router::runError(int error, Request& req, Response& res) {
    if (_errors.find(error) != _errors.end())
      (*_errors[error])(req, res);
    else
      (*_default_error)(req, res);
  }

  Method
  Router::string2method(std::string method) {
    if (!method.compare("HEAD")) return HEAD;
    else if (!method.compare("GET")) return GET;
    else if (!method.compare("POST")) return POST;
    else if (!method.compare("PUT")) return PUT;
    else if (!method.compare("DELETE")) return DELETE;
    else throw "Method unrecognized";
  }

  void
  Router::add(Method method,
              std::string match,
              void (*func)(Request&,
                           Response&)) {
    route r (method, match, func);
    routes.push_back(r);
  }

  void
  Router::add(Method method,
              CPP11LIB::regex match,
              void (*func)(Request&,
                           Response&)) {
    route r (method, match, func);
    routes.push_back(r);
  }

  void
  Router::onError(int error, void (*fun)(Request&, Response&)) {
    _errors[error] = fun;
  }

  void
  Router::defaultHandler(void (*fun)(Request&, Response&)) {
    _default_error = fun;
  }

  void
  Router::dispatch(Request& req, Response& r) {

    try {
      Method meth = string2method(req.method());
      std::string uri = req.uri();

      for (std::vector<route>::iterator it = routes.begin(); it < routes.end(); it++) {
        if (it->method == meth &&
            CPP11LIB::regex_match(uri, it->match)) {
          if (it->isNamed())
            req.setMatches(it->getNames(), it->getMatches(uri));
          else
            req.setMatches(it->getMatches(uri));

          it->func(req, r);
          return;
        }
      }

      runError(404, req, r);
    } catch (...) {
      runError(500, req, r);
    }
  }
}
