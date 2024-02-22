CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

DROP SCHEMA IF EXISTS delivery_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS delivery_schema;

CREATE TYPE delivery_schema.user_type AS ENUM ('regular', 'courier');

CREATE TABLE IF NOT EXISTS delivery_schema.users(
    user_id text PRIMARY KEY DEFAULT uuid_generate_v4(),
    username varchar(255) NOT NULL,
    email varchar(255) NOT NULL,
    password_hash varchar(255) NOT NULL,
    user_type delivery_schema.user_type NOT NULL DEFAULT 'regular',
    CONSTRAINT uniq_username UNIQUE (username),
    CONSTRAINT uniq_email UNIQUE (email)
);
