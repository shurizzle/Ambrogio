#ifndef __AMBROGIO_DSL_HPP
#define __AMBROGIO_DSL_HPP

# include "Application.hpp"

# ifdef STATUS
#   undef STATUS
# endif

# ifdef ERROR
#   undef ERROR
# endif

# define STATUS(stat)    response["Status"] = ::Ambrogio::Status[stat]
# define ERROR(stat) {\
  response["Status"] = ::Ambrogio::Status[stat];\
  response.runError(stat, request);\
}

# define INIT() using namespace Ambrogio; int main(void) { Application application;

# define RE(str) CPP11LIB::regex(str)

# define RUN(on) run(application, on); return 0; }

# ifdef USE_CPP11
#   define DEFAULT(body) application.defaultHandler([](Ambrogio::Request& request, Ambrogio::Response& response) { body; });
#   define ON_ERROR(no,body) application.onError(no, [](Ambrogio::Request& request, Ambrogio::Response& response) { body; });
#   define HEAD(path,body) application.HEAD(path, [](Ambrogio::Request& request, Ambrogio::Response& response) { body; });
#   define GET(path,body) application.GET(path, [](Ambrogio::Request& request, Ambrogio::Response& response) { body; });
#   define POST(path,body) application.POST(path, [](Ambrogio::Request& request, Ambrogio::Response& response) { body; });
#   define PUT(path,body) application.PUT(path, [](Ambrogio::Request& request, Ambrogio::Response& response) { body; });
#   define DELETE(path,body) application.DELETE(path, [](Ambrogio::Request& request, Ambrogio::Response& response) { body; });
# else
template<class T>
void
__dsl_get_function(Ambrogio::Request& request, Ambrogio::Response& response) {
  T c;
  c.run(request, response, matches);
}

#   define __XY(X,Y) X##Y
#   define __MakeNameXY(FX,LINE) __XY(FX,LINE)
#   define __MakeName(FX) __MakeNameXY(FX,__LINE__)

#   define LAMBDA(body) class __MakeName(__LAMBDA___) { public: void run(Ambrogio::Request& request, Ambrogio::Response& response) { body; } }

#   define DEFAULT(body) LAMBDA(body);application.defaultHandler(&__dsl_get_function<__MakeName(__LAMBDA___)>);
#   define ON_ERROR(no,body) LAMBDA(body);application.onError(no, &__dsl_get_function<__MakeName(__LAMBDA___)>);
#   define HEAD(path,body) LAMBDA(body);application.HEAD(path, &__dsl_get_function<__MakeName(__LAMBDA___)>);
#   define GET(path,body) LAMBDA(body);application.GET(path, &__dsl_get_function<__MakeName(__LAMBDA___)>);
#   define POST(path,body) LAMBDA(body);application.POST(path, &__dsl_get_function<__MakeName(__LAMBDA___)>);
#   define PUT(path,body) LAMBDA(body);application.PUT(path, &__dsl_get_function<__MakeName(__LAMBDA___)>);
#   define DELETE(path,body) LAMBDA(body);application.DELETE(path, &__dsl_get_function<__MakeName(__LAMBDA___)>);
# endif // USE_CPP11

#endif // __AMBROGIO_DSL_HPP
