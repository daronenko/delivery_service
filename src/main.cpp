#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/utils/daemon_run.hpp>

#include "handlers/health/health.hpp"
#include "handlers/auth/auth_bearer.hpp"
#include "handlers/users/user_register.hpp"
#include "handlers/users/user_login.hpp"
#include "handlers/users/user_get.hpp"


int main(int argc, char* argv[]) {
  userver::server::handlers::auth::RegisterAuthCheckerFactory(
      "bearer", std::make_unique<delivery_service::auth::CheckerFactory>());

  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<userver::components::Postgres>("postgres-db-1")
          .Append<userver::clients::dns::Component>()
          .Append<delivery_service::handlers::users::post::RegisterUser>()
          .Append<delivery_service::handlers::users::post::LoginUser>()
          .Append<delivery_service::handlers::users::get::GetUser>();

  delivery_service::handlers::health::AppendHealth(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
