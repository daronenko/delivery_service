#include "user.hpp"
#include "models/user.hpp"

#include <optional>
#include <string>

#include <userver/formats/json.hpp>
#include <userver/formats/parse/common_containers.hpp>


namespace delivery_service::dto {

UserRegistrationDTO Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UserRegistrationDTO>) {
  auto user_type_str = json["user_type"].As<std::optional<std::string>>();
  std::optional<models::UserType> user_type;
  if (user_type_str.has_value()) {
    user_type = models::StringToUserType(user_type_str.value());
  }
  
  return UserRegistrationDTO{
      json["username"].As<std::optional<std::string>>(),
      json["email"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
      user_type,
  };
}

UserLoginDTO Parse(const userver::formats::json::Value& json,
                   userver::formats::parse::To<UserLoginDTO>) {
  return UserLoginDTO{
      json["email"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
  };
}

UserUpdateDTO Parse(const userver::formats::json::Value& json,
                    userver::formats::parse::To<UserUpdateDTO>) {
  auto user_type_str = json["user_type"].As<std::optional<std::string>>();
  std::optional<models::UserType> user_type;
  if (user_type_str.has_value()) {
    user_type = models::StringToUserType(user_type_str.value());
  }

  return UserUpdateDTO{
      json["email"].As<std::optional<std::string>>(),
      json["username"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
      user_type,
  };
}

}  // namespace delivery_service::dto
