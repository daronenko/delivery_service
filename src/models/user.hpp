#pragma once

#include "db/types.hpp"

#include <string>
#include <tuple>

#include <userver/formats/json.hpp>
#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include <userver/utils/trivial_map.hpp>


namespace delivery_service::models {

using UserId = std::string;

enum class UserType { kRegular, kCourier };

UserType StringToUserType(const std::string& value);

std::string UserTypeToString(UserType value);

struct User final {
  UserId id;
  std::string username;
  std::string email;
  UserType user_type;
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

template <>
struct CppToUserPg<delivery_service::models::UserType> {
  static constexpr DBTypeName postgres_name = "delivery_schema.user_type";
  static constexpr USERVER_NAMESPACE::utils::TrivialBiMap enumerators =
      [](auto selector) {
        return selector()
            .Case("regular", delivery_service::models::UserType::kRegular)
            .Case("courier", delivery_service::models::UserType::kCourier);
      };
};

}  // namespace userver::storages::postgres::io
