BEGIN;

ALTER TABLE users
ADD COLUMN IF NOT EXISTS temporary_password_expires_at
TIMESTAMPTZ;

CREATE INDEX IF NOT EXISTS idx_users_temporary_password_expiry
ON users (temporary_password_expires_at)
WHERE must_change_password = TRUE;

COMMIT;