#include "validators.hpp"
#include "user_validators.hpp"
#include "utils/errors.hpp"


namespace delivery_service::validator {

void Validate(const dto::UserLoginDTO& dto) {
  if (!dto.email) {
    throw utils::error::ValidationException("email", "Field is missing");
  } else if (!ValidateEmail(dto.email.value())) {
    throw utils::error::ValidationException("email", "Invalid field");
  }

  if (!dto.password) {
    throw utils::error::ValidationException("password", "Field is missing");
  } else if (!ValidatePassword(dto.password.value())) {
    throw utils::error::ValidationException("password", "Invalid field");
  }
}

void Validate(const dto::UserRegistrationDTO& dto) {
  if (!dto.username) {
    throw utils::error::ValidationException("username", "Field is missing");
  } else if (!ValidateUsername(dto.username.value())) {
    throw utils::error::ValidationException("username", "Invalid field");
  }

  if (!dto.email) {
    throw utils::error::ValidationException("email", "Field is missing");
  } else if (!ValidateEmail(dto.email.value())) {
    throw utils::error::ValidationException("email", "Invalid field");
  }

  if (!dto.password) {
    throw utils::error::ValidationException("password", "Field is missing");
  } else if (!ValidatePassword(dto.password.value())) {
    throw utils::error::ValidationException("password", "Invalid value");
  }

  if (!dto.user_type) {
    throw utils::error::ValidationException("user_type", "Field is missing");
  }
}

void Validate(const dto::UserUpdateDTO& dto) {
  if (dto.username && !ValidateUsername(dto.username.value())) {
    throw utils::error::ValidationException("username", "Invalid field");
  }

  if (dto.email && !ValidateEmail(dto.email.value())) {
    throw utils::error::ValidationException("email", "Invalid field");
  }

  if (dto.password && !ValidatePassword(dto.password.value())) {
    throw utils::error::ValidationException("password", "Invalid field");
  }
}

}  // namespace delivery_service::validator
