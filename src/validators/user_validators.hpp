#pragma once

#include <regex>
#include <string_view>


namespace delivery_service::validator {

bool ValidateEmail(const std::string& email);

bool ValidatePassword(const std::string& password);

bool ValidateUsername(const std::string& username);

bool ValidateUserType(const std::string& user_type);

}  // namespace delivery_service::validator
