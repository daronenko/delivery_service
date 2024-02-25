#include "user_register.hpp"
#include "db/sql.hpp"
#include "dto/user.hpp"
#include "models/user.hpp"
#include "utils/errors.hpp"
#include "utils/make_error.hpp"
#include "validators/validators.hpp"

#include <userver/crypto/hash.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>


namespace delivery_service::handlers::users::post {

RegisterUser::RegisterUser(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      pg_cluster_(component_context
                      .FindComponent<userver::components::Postgres>(
                          "postgres-db-1")
                      .GetCluster()) {}

userver::formats::json::Value RegisterUser::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
  dto::UserRegistrationDTO user_register =
      request_json["user"].As<dto::UserRegistrationDTO>();

  try {
    validator::Validate(user_register);
  } catch (const utils::error::ValidationException& err) {
    request.SetResponseStatus(
        userver::server::http::HttpStatus::kUnprocessableEntity);
    return err.GetDetails();
  }

  auto hash_password =
      userver::crypto::hash::Sha256(user_register.password.value());
  models::User result_user;
  try {
    auto query_result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        db::sql::kInsertUser.data(), user_register.username, user_register.email,
        user_register.user_type, hash_password);
    result_user = query_result.AsSingleRow<models::User>(
        userver::storages::postgres::kRowTag);
  } catch (const userver::storages::postgres::UniqueViolation& ex) {
    auto& response = request.GetHttpResponse();
    response.SetStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
    return utils::error::MakeError(ex.GetServerMessage().GetConstraint(),
                                   ex.GetServerMessage().GetDetail());
  }

  userver::formats::json::ValueBuilder builder;
  builder["user"] = result_user;
  return builder.ExtractValue();
}

}  // namespace delivery_service::handlers::users::post
