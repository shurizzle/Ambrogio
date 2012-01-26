#ifndef __AMBROGIO_REQUEST_HPP
#define __AMBROGIO_REQUEST_HPP

#include <string>
#include <vector>
#include <map>
#include <fcgi_stdio.h>

namespace Ambrogio {
  class Request {
    FCGX_Request *_request;
    std::vector<std::string> _names;
    std::vector<std::string> _matches;
    bool _named;

  public:
    Request(FCGX_Request*, std::vector<std::string>, std::vector<std::string>);
    Request(FCGX_Request*, std::vector<std::string>);
    Request(FCGX_Request*);
    void setMatches(std::vector<std::string>, std::vector<std::string>);
    void setMatches(std::vector<std::string>);
    ~Request(void);
    std::string getParam(std::string&);
    std::string getParam(const char *);
    std::string referer(void);
    std::string user_agent(void);
    std::string path_info(void);
    std::string uri(void);
    std::string query_string(void);
    std::string remote_addr(void);
    std::string method(void);
    std::string server_name(void);
    std::string server_port(void);
    std::string operator[](std::string);
    std::string operator[](int);
  };
}

#endif // __AMBROGIO_REQUEST_HPP
