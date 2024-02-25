#include "user_validators.hpp"

#include <regex>
#include <string>


namespace delivery_service::validator {

bool ValidateEmail(const std::string& email) {
  const std::regex pattern{R"(^[\w\-\.]+@([\w-]+\.)+[\w-]{2,4}$)"};
  return regex_match(email.begin(), email.end(), pattern);
}

bool ValidatePassword(const std::string& password) {
  return !password.empty();
}

bool ValidateUsername(const std::string& username) {
  return !username.empty();
}

}  // namespace delivery_service::validator
