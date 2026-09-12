
-- ==========================================
-- UNIVERSE UNIVERSITY PORTAL
-- USER AUTHENTICATION DATABASE
-- ==========================================

CREATE TYPE app_role AS ENUM (
    'student',
    'faculty',
    'admin',
    'coordinator'
);

CREATE TYPE account_status AS ENUM (
    'pending',
    'active',
    'suspended'
);

CREATE TABLE users (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,

    login_id VARCHAR(80) NOT NULL UNIQUE,

    email VARCHAR(254) NOT NULL UNIQUE,

    password_hash TEXT NOT NULL,

    role app_role NOT NULL,

    status account_status NOT NULL DEFAULT 'pending',

    must_change_password BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    last_login_at TIMESTAMPTZ
);

CREATE INDEX idx_users_role
ON users(role);

CREATE INDEX idx_users_status
ON users(status);
