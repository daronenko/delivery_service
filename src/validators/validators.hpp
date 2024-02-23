#pragma once

#include "dto/user.hpp"


namespace delivery_service::validator {

void validate(const dto::UserLoginDTO& dto);

void validate(const dto::UserRegistrationDTO& dto);

void validate(const dto::UserUpdateDTO& dto);

}  // namespace delivery_service::validator
