#ifndef SRC_CLOUDCONTROLLER_CLOUDCONTROLLER
#define SRC_CLOUDCONTROLLER_CLOUDCONTROLLER

#include "../components/http_handlers/file_upload_handler.h"
#include "../components/http_handlers/ihttphandler.h"
#include "../components/http_handlers/http_endpoint.h"
#include "../components/authbe/auth_backend.h"
#include <memory>
#include <pistache/router.h>
#include <set>

namespace cloudcontroller {
class CloudController {
public:
  CloudController();
private:
    http_handlers::HttpEndpoint m_httpEndpointHandler;
    authbe::AuthBackend m_authBE;
};
} // namespace cloudcontroller

#endif /* SRC_CLOUDCONTROLLER_CLOUDCONTROLLER */
