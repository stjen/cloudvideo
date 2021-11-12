#include "cloudcontroller.h"
#include <http_handlers/auth_handler.h>
#include <memory>

using namespace cloudcontroller;

CloudController::CloudController() {

  auto router = m_httpEndpointHandler.getRouter();

  auto authHandler = std::make_unique<http_handlers::AuthHTTPHandler>(
      router,
      [this](const std::string &key) -> bool {
        return m_authBE.verifyAPIKey(key);
      },
      "/auth", "db");

  m_httpEndpointHandler.addHttpHandler(std::move(authHandler));

  m_httpEndpointHandler.initHttp();
}
