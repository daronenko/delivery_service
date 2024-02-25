#include "user.hpp"
#include "utils/jwt.hpp"

#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include <string>


namespace delivery_service::models {

UserType StringToUserType(const std::string& value) {
  if (value == "courier") {
    return UserType::kCourier;
  } else {
    return UserType::kRegular;
  }
}

std::string UserTypeToString(UserType value) {
  if (value == UserType::kCourier) {
    return "courier";
  } else {
    return "regular";
  }
}

userver::formats::json::Value Serialize(
    const User& user,
    userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder builder;

  builder["email"] = user.email;
  builder["username"] = user.username;
  builder["token"] = utils::jwt::GenerateJWT(user.id);
  switch (user.user_type) {
    case UserType::kRegular:
      builder["user_type"] = "regular";
      break;
    case UserType::kCourier:
      builder["user_type"] = "courier";
      break;
  }
  return builder.ExtractValue();
}

}  // namespace delivery_service::models
