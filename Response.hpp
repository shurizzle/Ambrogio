#ifndef __AMBROGIO_RESPONSE_HPP
#define __AMBROGIO_RESPONSE_HPP

#include <string>
#include <vector>
#include <map>
#include <fcgi_stdio.h>

#include "Request.hpp"

namespace Ambrogio {
  class Router;

  class Response : public std::map<std::string, std::string> {
    FCGX_Stream *out;
    bool _head_sent, _body_sent;
    Router *_router;
    std::vector<std::string> segments;

  public:
    Response(FCGX_Stream *_out, Router *r) : out(_out), _head_sent(false), _body_sent(false), _router(r) {}
    ~Response(void);
    void writeHeaders(void);
    void writeSegments(void);
    void write(void);
    void write(std::string);
    void append(std::string);
    void runError(int, Request&);
    Response& operator<<(std::string);
  };
}

#endif // __AMBROGIO_RESPONSE_HPP
