#include "file_upload_handler.h"
#include "ihttphandler.h"
#include <fmt/format.h>
#include <pistache/router.h>

using namespace Pistache;
using namespace http_handlers;

FileUploadHTTPHandler::FileUploadHTTPHandler(
    std::shared_ptr<Pistache::Rest::Router> &router,
    IHttpHandler::AuthVerifierCb cb,
    const IHttpHandler::RoutePrefix &routePrefix)
    : IHttpHandler(router, cb, routePrefix) {}

void FileUploadHTTPHandler::handleUploadFile(const Rest::Request &request,
                                             Http::ResponseWriter response) {
  response.send(Http::Code::Ok, "hello world");
}

void FileUploadHTTPHandler::setupRoutes() {

  std::cout << fmt::format("{}{}", m_routePrefix, "/hello") << std::endl;
  Rest::Routes::Get(
      *m_router, m_routePrefix + "/hello",
      Rest::Routes::bind(&FileUploadHTTPHandler::handleUploadFile, this));
  std::cout << "Setting up route" << std::endl;
}