#include <Ambrogio/Request.hpp>

#include <cstdlib>

namespace Ambrogio {
  Request::Request(FCGX_Request *req) {
    _request = req;
    _named = false;
  }

  Request::Request(FCGX_Request *req, std::vector<std::string> matches) {
    _request = req;
    setMatches(matches);
  }

  Request::Request(FCGX_Request *req, std::vector<std::string> names, std::vector<std::string> matches) {
    _request = req;
    setMatches(names, matches);
  }

  void
  Request::setMatches(std::vector<std::string> names, std::vector<std::string> matches) {
    _named = true;
    _names = names;
    _matches = matches;
  }

  void
  Request::setMatches(std::vector<std::string> matches) {
    _named = false;
    _matches = matches;
  }

  std::string
  Request::operator[](std::string name) {
    if (!_named) return "";

    for (int i = 0; i < (int) _names.size(); i++) {
      if (name.compare(_names[i]) == 0)
        return _matches[i];
    }

    return "";
  }

  std::string
  Request::operator[](int pos) {
    return _matches[pos];
  }

  Request::~Request(void) {
    FCGX_Finish_r(_request);
    FCGX_Free(_request, 1);
    delete _request;
  }

  std::string
  Request::getParam(std::string& env) {
    return getParam(env.c_str());
  }

  std::string
  Request::getParam(const char *env) {
    char *res = FCGX_GetParam(env, _request->envp);
    if (res == NULL)
      return "";
    return std::string(res);
  }

  #define GETENV(name, env) std::string\
  Request::name(void) {\
    return getParam(env);\
  }
  GETENV(referer, "HTTP_REFERER")
  GETENV(user_agent, "HTTP_USER_AGENT")
  GETENV(path_info, "PATH_INFO")
  GETENV(uri, "DOCUMENT_URI")
  GETENV(query_string, "QUERY_STRING")
  GETENV(remote_addr, "REMOTE_ADDR")
  GETENV(method, "REQUEST_METHOD")
  GETENV(server_name, "SERVER_NAME")
  GETENV(server_port, "SERVER_PORT")
  #undef GETENV
}
