#include <Ambrogio/Response.hpp>
#include <Ambrogio/Router.hpp>

namespace Ambrogio {
  void
  Response::writeHeaders(void) {
    for (iterator it = begin(); it != end(); it++) {
      FCGX_FPrintF(out, "%s: %s\r\n", it->first.c_str(), it->second.c_str());
    }
    FCGX_FPrintF(out, "\r\n");
    _head_sent = true;
  }

  void
  Response::writeSegments(void) {
    for (std::vector<std::string>::iterator it = segments.begin(); it < segments.end(); it++) {
      FCGX_FPrintF(out, it->c_str());
    }
    segments.clear();
    _body_sent = true;
  }

  void
  Response::write(void) {
    if (!_head_sent)
      writeHeaders();
    writeSegments();
  }

  void
  Response::write(std::string segment) {
    append(segment);
    write();
  }

  void
  Response::append(std::string segment) {
    segments.push_back(segment);
  }

  Response&
  Response::operator<<(std::string segment) {
    append(segment);
    _body_sent = false;
    return *this;
  }

  void
  Response::runError(int error, Request& req) {
    _router->runError(error, req, *this);
  }

  Response::~Response(void) {
    if (!_head_sent)
      writeHeaders();
    if (!_body_sent)
      writeSegments();
  }
}
