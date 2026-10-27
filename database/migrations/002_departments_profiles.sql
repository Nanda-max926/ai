
BEGIN;

CREATE TABLE IF NOT EXISTS departments (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    code VARCHAR(20) NOT NULL UNIQUE,
    name VARCHAR(150) NOT NULL,
    is_active BOOLEAN NOT NULL DEFAULT TRUE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO departments (code, name)
VALUES
    ('CSE', 'Computer Science and Engineering'),
    ('CSE-DS', 'Computer Science and Data Science'),
    ('AIML', 'Artificial Intelligence and Machine Learning'),
    ('ECE', 'Electronics and Communication Engineering'),
    ('EEE', 'Electrical and Electronics Engineering'),
    ('IT', 'Information Technology'),
    ('MECH', 'Mechanical Engineering'),
    ('CIVIL', 'Civil Engineering')
ON CONFLICT (code) DO NOTHING;

CREATE SEQUENCE IF NOT EXISTS student_registration_seq START 1;
CREATE SEQUENCE IF NOT EXISTS faculty_employee_seq START 1;

CREATE TABLE IF NOT EXISTS student_profiles (
    user_id BIGINT PRIMARY KEY
        REFERENCES users(id) ON DELETE CASCADE,

    full_name VARCHAR(150) NOT NULL,

    department_id BIGINT NOT NULL
        REFERENCES departments(id),

    semester SMALLINT NOT NULL
        CHECK (semester BETWEEN 1 AND 8),

    batch_year INTEGER NOT NULL
        CHECK (batch_year BETWEEN 2000 AND 2100),

    section VARCHAR(20) NOT NULL DEFAULT 'A',

    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS faculty_profiles (
    user_id BIGINT PRIMARY KEY
        REFERENCES users(id) ON DELETE CASCADE,

    full_name VARCHAR(150) NOT NULL,

    department_id BIGINT NOT NULL
        REFERENCES departments(id),

    designation VARCHAR(100) NOT NULL,

    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE UNIQUE INDEX IF NOT EXISTS users_login_id_lower_unique
ON users (LOWER(login_id));

CREATE UNIQUE INDEX IF NOT EXISTS users_email_lower_unique
ON users (LOWER(email));

CREATE INDEX IF NOT EXISTS idx_students_department
ON student_profiles(department_id);

CREATE INDEX IF NOT EXISTS idx_faculty_department
ON faculty_profiles(department_id);

COMMIT;
