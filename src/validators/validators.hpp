#pragma once

#include "dto/user.hpp"


namespace delivery_service::validator {

void Validate(const dto::UserLoginDTO& dto);

void Validate(const dto::UserRegistrationDTO& dto);

void Validate(const dto::UserUpdateDTO& dto);

}  // namespace delivery_service::validator
