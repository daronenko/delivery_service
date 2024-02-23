#include <userver/utest/utest.hpp>

#include "user_validators.hpp"
#include "utils/errors.hpp"
#include "validators.hpp"


using namespace delivery_service::validator;
using namespace delivery_service::utils::error;


UTEST(UserValidation, EmailValidation) {
  EXPECT_TRUE(ValidateEmail("example@mail.com"));
  EXPECT_FALSE(ValidateEmail(""));
  EXPECT_FALSE(ValidateEmail("example@mail.com@foo.com"));
}

UTEST(UserValidation, PasswordValidation) {
  EXPECT_TRUE(ValidatePassword("password"));
  EXPECT_FALSE(ValidatePassword(""));
}

UTEST(UserValidation, UsernameValidation) {
  EXPECT_TRUE(ValidateUsername("username"));
  EXPECT_FALSE(ValidateUsername(""));
}

UTEST(UserValidation, LoginValidation) {
  using delivery_service::dto::UserLoginDTO;

  UEXPECT_NO_THROW(validate(UserLoginDTO{"example@mail.com", "password"}));
  UEXPECT_THROW(validate(UserLoginDTO{std::nullopt, "password"}),
                ValidationException);
  UEXPECT_THROW(validate(UserLoginDTO{"example@mail.com", std::nullopt}),
                ValidationException);
}

UTEST(UserValidation, RegisterValidation) {
  using delivery_service::dto::UserRegistrationDTO;

  UEXPECT_NO_THROW(
      validate(UserRegistrationDTO{"username", "example@mail.com", "password", "regular"}));
  UEXPECT_NO_THROW(
      validate(UserRegistrationDTO{"username", "example@mail.com", "password", "courier"}));
  UEXPECT_THROW(
      validate(UserRegistrationDTO{"username", "example@mail.com", "password", "another_type"}),
      ValidationException);
  UEXPECT_THROW(
      validate(UserRegistrationDTO{std::nullopt, "example@mail.com", "password", "regular"}),
      ValidationException);
  UEXPECT_THROW(validate(UserRegistrationDTO{"username", std::nullopt, "password", "regular"}),
                ValidationException);
  UEXPECT_THROW(
      validate(UserRegistrationDTO{"username", "example@mail.com", std::nullopt, "regular"}),
      ValidationException);
  UEXPECT_THROW(
      validate(UserRegistrationDTO{"username", "example@mail.com", "password", std::nullopt}),
      ValidationException);
}
