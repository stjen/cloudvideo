
#include "auth_handler.h"
#include "ihttphandler.h"
#include <pistache/http_defs.h>

using namespace http_handlers;

AuthHTTPHandler::AuthHTTPHandler(
    std::shared_ptr<Pistache::Rest::Router> &router,
    IHttpHandler::AuthVerifierCb cb,
    const IHttpHandler::RoutePrefix &routePrefix, const std::string &mockDb)
    : IHttpHandler(router, cb, routePrefix) {}

void AuthHTTPHandler::handleAuths(const Rest::Request &request,
                                  Http::ResponseWriter response) {
  response.send(Http::Code::Ok, "oy m8");
}

void AuthHTTPHandler::setupRoutes() {
    m_router->addRoute(Http::Method::Post, m_routePrefix,Rest::Routes::bind(&AuthHTTPHandler::handleAuths, this));
}