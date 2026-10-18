BEGIN;

ALTER TABLE departments
ADD COLUMN IF NOT EXISTS description TEXT;

ALTER TABLE departments
ADD COLUMN IF NOT EXISTS office_location VARCHAR(160);

ALTER TABLE departments
ADD COLUMN IF NOT EXISTS contact_email VARCHAR(254);

ALTER TABLE departments
ADD COLUMN IF NOT EXISTS contact_phone VARCHAR(30);

ALTER TABLE departments
ADD COLUMN IF NOT EXISTS hod_user_id BIGINT;

ALTER TABLE departments
ADD COLUMN IF NOT EXISTS updated_at TIMESTAMPTZ
NOT NULL DEFAULT CURRENT_TIMESTAMP;

DO $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM pg_constraint
        WHERE conname = 'departments_hod_user_id_fkey'
          AND conrelid = 'departments'::regclass
    ) THEN
        ALTER TABLE departments
        ADD CONSTRAINT departments_hod_user_id_fkey
        FOREIGN KEY (hod_user_id)
        REFERENCES faculty_profiles(user_id)
        ON DELETE SET NULL;
    END IF;
END $$;

CREATE INDEX IF NOT EXISTS idx_departments_hod
ON departments(hod_user_id);

CREATE UNIQUE INDEX IF NOT EXISTS idx_departments_code_lower
ON departments(LOWER(code));

COMMIT;