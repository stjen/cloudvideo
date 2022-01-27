
#include "auth_handler.h"
#include "ihttphandler.h"
#include <pistache/http_defs.h>
#include <authbe/auth_backend.h>
#include <pistache/router.h>
#include <string>

using namespace http_handlers;

AuthHTTPHandler::AuthHTTPHandler(
    std::shared_ptr<Pistache::Rest::Router> &router,
    IHttpHandler::AuthVerifierCb cb,
    const IHttpHandler::RoutePrefix &routePrefix, const std::string &mockDb)
    : IHttpHandler(router, cb, routePrefix) {}

void AuthHTTPHandler::handleAuths(const Rest::Request &request,
                                  Http::ResponseWriter response) {
                                    //   if (request.hasParam("key")) {
                                    //     if (m_authBe.verifyAPIKey(request.param("key").as<std::string>()) {
                                    //         response.send(Http::Code::Ok, "oy m8");
                                    //     }
                                        
                                    //   }
}

void AuthHTTPHandler::setupRoutes() {
    m_router->addRoute(Http::Method::Post, m_routePrefix,Rest::Routes::bind(&AuthHTTPHandler::handleAuths, this));
}