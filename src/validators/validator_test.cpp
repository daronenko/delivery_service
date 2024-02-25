#include "user_validators.hpp"
#include "utils/errors.hpp"
#include "validators.hpp"
#include "models/user.hpp"

#include <userver/utest/utest.hpp>


using namespace delivery_service::validator;
using namespace delivery_service::utils::error;
using namespace delivery_service::models;


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

  UEXPECT_NO_THROW(Validate(UserLoginDTO{"example@mail.com", "password"}));
  UEXPECT_THROW(Validate(UserLoginDTO{std::nullopt, "password"}),
                ValidationException);
  UEXPECT_THROW(Validate(UserLoginDTO{"example@mail.com", std::nullopt}),
                ValidationException);
}

UTEST(UserValidation, RegisterValidation) {
  using delivery_service::dto::UserRegistrationDTO;

  UEXPECT_NO_THROW(Validate(UserRegistrationDTO{
      "username", "example@mail.com", "password", UserType::kRegular}));
  UEXPECT_NO_THROW(Validate(UserRegistrationDTO{
      "username", "example@mail.com", "password", UserType::kCourier}));
  UEXPECT_THROW(Validate(UserRegistrationDTO{std::nullopt, "example@mail.com",
                                             "password", UserType::kRegular}),
                ValidationException);
  UEXPECT_THROW(Validate(UserRegistrationDTO{"username", std::nullopt,
                                             "password", UserType::kRegular}),
                ValidationException);
  UEXPECT_THROW(Validate(UserRegistrationDTO{"username", "example@mail.com",
                                             std::nullopt, UserType::kRegular}),
                ValidationException);
  UEXPECT_THROW(Validate(UserRegistrationDTO{"username", "example@mail.com",
                                             "password", std::nullopt}),
                ValidationException);
}
