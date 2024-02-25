#include "user_login.hpp"
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

#include <string_view>


namespace delivery_service::handlers::users::post {

LoginUser::LoginUser(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      pg_cluster_(
          component_context
              .FindComponent<userver::components::Postgres>("postgres-db-1")
              .GetCluster()) {
}

userver::formats::json::Value LoginUser::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
  dto::UserLoginDTO user_login = request_json["user"].As<dto::UserLoginDTO>();

  try {
    validator::validate(user_login);
  } catch (const utils::error::ValidationException& err) {
    request.SetResponseStatus(
        userver::server::http::HttpStatus::kUnprocessableEntity);
    return err.GetDetails();
  }

  auto password_hash =
      userver::crypto::hash::Sha256(user_login.password.value());

  auto userResult = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      db::sql::kSelectUserByEmailAndPassword.data(), user_login.email,
      password_hash);

  if (userResult.IsEmpty()) {
    auto& response = request.GetHttpResponse();
    response.SetStatus(userver::server::http::HttpStatus::kNotFound);
    return {};
  }

  auto user = userResult.AsSingleRow<models::User>(
      userver::storages::postgres::kRowTag);

  userver::formats::json::ValueBuilder response;
  response["user"] = user;

  return response.ExtractValue();
}

}  // namespace delivery_service::handlers::users::post
