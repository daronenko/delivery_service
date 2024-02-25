#include "user_put.hpp"
#include "db/sql.hpp"
#include "dto/user.hpp"
#include "models/user.hpp"
#include "utils/errors.hpp"
#include "validators/validators.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/crypto/hash.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include <string>
#include <string_view>
#include <fmt/format.h>


namespace delivery_service::handlers::users::put {

PutUser::PutUser(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      pg_cluster_(component_context
                      .FindComponent<userver::components::Postgres>(
                          "postgres-db-1")
                      .GetCluster()) {
}

userver::formats::json::Value PutUser::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context) const {
  auto user_id = context.GetData<std::optional<std::string>>("id");

  dto::UserUpdateDTO user_change_data =
      request_json["user"].As<dto::UserUpdateDTO>();

  try {
    validator::Validate(user_change_data);
  } catch (const utils::error::ValidationException& err) {
    request.SetResponseStatus(
        userver::server::http::HttpStatus::kUnprocessableEntity);
    return err.GetDetails();
  }

  std::optional<std::string> password_hash = std::nullopt;
  if (user_change_data.password) {
    password_hash =
        userver::crypto::hash::Sha256(user_change_data.password.value());
  }

  const auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      db::sql::kUpdateUser.data(), user_id, user_change_data.username,
      user_change_data.email, user_change_data.user_type,
      password_hash);

  auto user_result_data = result.AsSingleRow<models::User>(
      userver::storages::postgres::kRowTag);

  userver::formats::json::ValueBuilder builder;
  builder["user"] = user_result_data;

  return builder.ExtractValue();
}

}  // namespace delivery_service::handlers::users::put
