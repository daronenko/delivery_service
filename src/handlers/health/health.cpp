#include "health.hpp"

#include <userver/server/handlers/http_handler_base.hpp>


namespace delivery_service::handlers::health {

namespace {

class Health final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-health";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest&,
      userver::server::request::RequestContext&) const override {
    return "OK\n";
  }
};

}  // namespace

void AppendHealth(userver::components::ComponentList& component_list) {
  component_list.Append<Health>();
}

}  // namespace delivery_service::handlers::health
