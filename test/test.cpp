#include <Ambrogio/dsl.hpp>

INIT()

DEFAULT(STATUS(500); response << "<html><body><h1>Mi spiace ma " << request.uri() << " &egrave; impotente.</h1></body></html>")
ON_ERROR(404, STATUS(404); response << "<html><body><h1>" << request.uri() << " non sburra :(</h1></body></html>")

GET("/merda/", {
      response["Sburro"] = "Pene";
      response << "LA BORRA";
    })

GET("/merda/sburro", ERROR(500))

GET("/merda/:ciccio", response << "<html><body><h1>" << request["ciccio"] << "</h1></body></html>")
PUT("/merda/:ciccio", response << "PUT " << request["ciccio"] << " in your ass.")

//GET(RE("^/+merda(.+?)$"), response << request[0])

RUN(":9000")
