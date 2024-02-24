#pragma once

#include <string_view>


namespace delivery_service::db::sql {

inline constexpr std::string_view kInsertUser = R"~(
INSERT INTO delivery_schema.users(username, email, user_type, password_hash)
VALUES($1, $2, $3, $4)
RETURNING *
)~";

inline constexpr std::string_view kSelectUserByEmailAndPassword = R"~(
SELECT * FROM delivery_schema.users
WHERE email = $1 AND password_hash = $2
)~";

inline constexpr std::string_view kUpdateUser = R"~(
UPDATE delivery_schema.users SET
  username = COALESCE($2, username),
  email = COALESCE($3, email),
  user_type = COALESCE($4, user_type),
  password_hash = COALESCE($5, password_hash)
WHERE user_id = $1
RETURNING *
)~";

inline constexpr std::string_view kFindUserById = R"~(
SELECT * FROM delivery_schema.users WHERE user_id = $1
)~";

inline constexpr std::string_view kFindUserIDByUsername = R"~(
SELECT user_id FROM delivery_schema.users WHERE username = $1
)~";

}  // namespace delivery_service::db::sql
