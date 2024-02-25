#include "user_get.hpp"
#include "db/sql.hpp"
#include "dto/user.hpp"
#include "models/user.hpp"
#include "utils/make_error.hpp"


namespace delivery_service::handlers::users::get {

GetUser::GetUser(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      pg_cluster_(component_context
                      .FindComponent<userver::components::Postgres>(
                          "postgres-db-1")
                      .GetCluster()) {}

userver::formats::json::Value GetUser::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context) const {
  auto user_id = context.GetData<std::optional<std::string>>("id");

  const auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      db::sql::kFindUserById.data(), user_id);

  if (result.IsEmpty()) {
    auto& response = request.GetHttpResponse();
    response.SetStatus(userver::server::http::HttpStatus::kNotFound);
    return utils::error::MakeError("user_id", "Ivanlid user_id. Not found.");
  }

  auto user = result.AsSingleRow<models::User>(
      userver::storages::postgres::kRowTag);

  userver::formats::json::ValueBuilder builder;
  builder["user"] = user;

  return builder.ExtractValue();
}

}  // namespace delivery_service::handlers::users::get
