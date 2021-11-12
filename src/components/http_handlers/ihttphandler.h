#pragma once

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <string>
#include <vector>

class IHttpHandler {

public:
  typedef const std::function<bool(const std::string &)> AuthVerifierCb;
  typedef std::string RoutePrefix;

  IHttpHandler(std::shared_ptr<Pistache::Rest::Router> &router,
              AuthVerifierCb cb, const RoutePrefix& routePrefix)
      : m_router(router.get()), m_authVerifier(cb),
        m_routePrefix(routePrefix){};
  virtual ~IHttpHandler() {}
  virtual void setupRoutes() = 0;

protected:
  std::unique_ptr<Pistache::Rest::Router> m_router;
  AuthVerifierCb m_authVerifier;
  RoutePrefix m_routePrefix;
};
