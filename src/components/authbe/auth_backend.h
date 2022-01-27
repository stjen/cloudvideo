#ifndef SRC_AUTH_AUTH_HANDLER
#define SRC_AUTH_AUTH_HANDLER

#include <string>

namespace authbe {

class AuthBackend {
public:
  AuthBackend() = default;
  ~AuthBackend() = default;
  bool verifyAPIKey(const std::string key);
  std::string generateAPIToken(const std::string key);

private:
};
} // namespace auth_backend

#endif /* SRC_AUTH_AUTH_HANDLER */
