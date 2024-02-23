#include "user.hpp"


namespace delivery_service::dto {

UserRegistrationDTO Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UserRegistrationDTO>) {
  return UserRegistrationDTO{
      json["username"].As<std::optional<std::string>>(),
      json["email"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
      json["user_type"].As<std::optional<std::string>>(),
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
  return UserUpdateDTO{
      json["email"].As<std::optional<std::string>>(),
      json["username"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
      json["user_type"].As<std::optional<std::string>>(),
  };
}

}  // namespace delivery_service::dto
