#ifndef SRC_COMPONENTS_HTTP_HANDLERS_HTTP_ENDPOINT
#define SRC_COMPONENTS_HTTP_HANDLERS_HTTP_ENDPOINT

#include <authbe/auth_backend.h>
#include "ihttphandler.h"
#include <memory>
#include <pistache/router.h>

using namespace Pistache;

namespace http_handlers {

class HttpEndpoint {
public:
  HttpEndpoint();
  void initHttp();
  void deInitHttp();
  void addHttpHandler(std::unique_ptr<IHttpHandler> &&);
  std::shared_ptr<Rest::Router>& getRouter() { return m_router; }

private:
  Http::Endpoint m_httpEndpointHandler;
  std::list<std::unique_ptr<IHttpHandler>> m_apiHandlers;
  std::shared_ptr<Rest::Router> m_router;
};

} // namespace http_handlers

#endif /* SRC_COMPONENTS_HTTP_HANDLERS_HTTP_ENDPOINT */
