#pragma once

#include "db/types.hpp"

#include <optional>
#include <string>
#include <tuple>

#include <userver/formats/json/value_builder.hpp>
#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>


namespace delivery_service::models {

using UserId = std::string;

struct User final {
  UserId id;
  std::string username;
  std::string email;
  std::string user_type;
  std::string password_hash;

  auto Introspect() {
    return std::tie(id, username, email, user_type, password_hash);
  }
};

userver::formats::json::Value Serialize(
    const User& user,
    userver::formats::serialize::To<userver::formats::json::Value>);

}  // namespace delivery_service::models

namespace userver::storages::postgres::io {

template <>
struct CppToUserPg<delivery_service::models::User> {
  static constexpr DBTypeName postgres_name{
      delivery_service::db::types::kUser.data()};
};

}  // namespace userver::storages::postgres::io
