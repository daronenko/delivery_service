#pragma once

#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>

#include <string_view>


namespace delivery_service::handlers::users::post {

class RegisterUser final
    : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "handler-register-user";

  RegisterUser(const userver::components::ComponentConfig&,
               const userver::components::ComponentContext&);

  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest&,
      const userver::formats::json::Value&,
      userver::server::request::RequestContext&) const override final;

  using HttpHandlerJsonBase::HttpHandlerJsonBase;

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace delivery_service::handlers::users::post
