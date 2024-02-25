#pragma once

#include <string>


namespace delivery_service::validator {

bool ValidateEmail(const std::string& email);

bool ValidatePassword(const std::string& password);

bool ValidateUsername(const std::string& username);

}  // namespace delivery_service::validator
