#ifndef SRC_COMPONENTS_HTTP_HANDLERS_FILE_UPLOAD_HANDLER
#define SRC_COMPONENTS_HTTP_HANDLERS_FILE_UPLOAD_HANDLER

 #include <functional>
#include <string>

#include "ihttphandler.h"
#include <pistache/endpoint.h>
#include <pistache/router.h>

namespace http_handlers {

class FileUploadHTTPHandler : public IHttpHandler {
public:
  FileUploadHTTPHandler(std::shared_ptr<Pistache::Rest::Router> &,
                IHttpHandler::AuthVerifierCb,
                const IHttpHandler::RoutePrefix &routePrefix);
  ~FileUploadHTTPHandler() = default;

  void handleUploadFile(const Pistache::Rest::Request &request,
                        Pistache::Http::ResponseWriter response);

  virtual void setupRoutes() override;

private:
};

} // namespace api::file_upload

#endif /* SRC_COMPONENTS_HTTP_HANDLERS_FILE_UPLOAD_HANDLER */
