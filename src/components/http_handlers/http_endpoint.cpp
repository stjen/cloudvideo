#include "file_upload_handler.h"
#include "http_endpoint.h"
#include <memory>
#include <pistache/endpoint.h>
#include <pistache/net.h>
#include <pistache/router.h>

using namespace http_handlers;

HttpEndpoint::HttpEndpoint()
    : m_httpEndpointHandler(
          Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(9090))),
      m_router(std::make_unique<Rest::Router>()) {}

void HttpEndpoint::addHttpHandler(std::unique_ptr<IHttpHandler> &&handler) {
  m_apiHandlers.push_back(std::move(handler));
}

void HttpEndpoint::deInitHttp() { m_httpEndpointHandler.shutdown(); }

void HttpEndpoint::initHttp() {
  {

    auto authBE = authbe::AuthBackend();


    std::cout << "Cores = " << Pistache::hardware_concurrency() << std::endl;
    m_httpEndpointHandler.init();

    // Setup routes for all registered API handlers
    for (auto &handler : m_apiHandlers) {
      handler->setupRoutes();
    }
    std::cout << m_router->hasNotFoundHandler() << std::endl;
    m_httpEndpointHandler.setHandler(m_router->handler());
    m_httpEndpointHandler.serveThreaded();

    // stats.init(thr);
    // stats.start();
    std::cout << "hest" << std::endl;
  }
}