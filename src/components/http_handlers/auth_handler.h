#ifndef SRC_COMPONENTS_HTTP_HANDLERS_AUTH_HANDLER
#define SRC_COMPONENTS_HTTP_HANDLERS_AUTH_HANDLER

#include "authbe/auth_backend.h"
#include "ihttphandler.h"

using namespace Pistache;

namespace http_handlers {

class AuthHTTPHandler : public IHttpHandler {
public:
  AuthHTTPHandler(std::shared_ptr<Pistache::Rest::Router> &router,
                  IHttpHandler::AuthVerifierCb cb,
                  const IHttpHandler::RoutePrefix &routePrefix,
                  const std::string &mockDb);
  ~AuthHTTPHandler() = default;
  virtual void setupRoutes() override;

private:
  const std::string m_mockDb;
  authbe::AuthBackend m_authBe;
  void handleAuths(const Rest::Request &request, Http::ResponseWriter response);
};

} // namespace http_handlers

#endif /* SRC_COMPONENTS_HTTP_HANDLERS_AUTH_HANDLER */
