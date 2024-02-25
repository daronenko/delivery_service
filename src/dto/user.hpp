#pragma once

#include "models/user.hpp"

#include <string>

#include <userver/formats/json.hpp>
#include <userver/formats/parse/common_containers.hpp>


namespace delivery_service::dto {

struct UserRegistrationDTO {
  std::optional<std::string> username;
  std::optional<std::string> email;
  std::optional<std::string> password;
  std::optional<models::UserType> user_type;
};

struct UserLoginDTO {
  std::optional<std::string> email;
  std::optional<std::string> password;
};

struct UserUpdateDTO {
  std::optional<std::string> email;
  std::optional<std::string> username;
  std::optional<std::string> password;
  std::optional<models::UserType> user_type;
};

UserRegistrationDTO Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UserRegistrationDTO>);

UserLoginDTO Parse(const userver::formats::json::Value& json,
                   userver::formats::parse::To<UserLoginDTO>);

UserUpdateDTO Parse(const userver::formats::json::Value& json,
                    userver::formats::parse::To<UserUpdateDTO>);

}  // namespace delivery_service::dto
