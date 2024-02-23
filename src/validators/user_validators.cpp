#include "user_validators.hpp"


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

bool ValidateUserType(const std::string& user_type) {
  return (user_type == "regular" || user_type == "courier");
}

}  // namespace delivery_service::validator
