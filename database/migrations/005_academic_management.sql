BEGIN;

-- ==========================================
-- 1. ACADEMIC PROGRAMS
-- ==========================================

CREATE TABLE IF NOT EXISTS academic_programs (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    department_id BIGINT NOT NULL
        REFERENCES departments(id),
    code VARCHAR(20) NOT NULL,
    name VARCHAR(150) NOT NULL,
    degree_level VARCHAR(30) NOT NULL
        CHECK (degree_level IN (
            'diploma',
            'undergraduate',
            'postgraduate',
            'doctoral'
        )),
    duration_years SMALLINT NOT NULL
        CHECK (duration_years BETWEEN 1 AND 8),
    total_semesters SMALLINT NOT NULL
        CHECK (total_semesters BETWEEN 1 AND 16),
    description TEXT NOT NULL DEFAULT '',
    is_active BOOLEAN NOT NULL DEFAULT TRUE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE (department_id, code)
);

-- ==========================================
-- 2. COURSES / SUBJECTS
-- ==========================================

CREATE TABLE IF NOT EXISTS academic_courses (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    department_id BIGINT NOT NULL
        REFERENCES departments(id),
    code VARCHAR(30) NOT NULL,
    title VARCHAR(150) NOT NULL,
    description TEXT NOT NULL DEFAULT '',
    credits NUMERIC(3,1) NOT NULL
        CHECK (credits BETWEEN 0.5 AND 20),
    course_type VARCHAR(20) NOT NULL
        CHECK (course_type IN (
            'core',
            'elective',
            'laboratory',
            'project',
            'other'
        )),
    is_active BOOLEAN NOT NULL DEFAULT TRUE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE (department_id, code)
);

-- ==========================================
-- 3. PROGRAM-COURSE MAPPING
-- ==========================================

CREATE TABLE IF NOT EXISTS academic_program_courses (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    program_id BIGINT NOT NULL
        REFERENCES academic_programs(id) ON DELETE CASCADE,
    course_id BIGINT NOT NULL
        REFERENCES academic_courses(id),
    semester_number SMALLINT NOT NULL
        CHECK (semester_number BETWEEN 1 AND 16),
    is_mandatory BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE (program_id, course_id),
    UNIQUE (program_id, semester_number, course_id)
);

-- ==========================================
-- 4. ACADEMIC SECTIONS
-- ==========================================

CREATE TABLE IF NOT EXISTS academic_sections (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    program_id BIGINT NOT NULL
        REFERENCES academic_programs(id),
    batch_year SMALLINT NOT NULL
        CHECK (batch_year BETWEEN 2000 AND 2100),
    academic_year_start SMALLINT NOT NULL
        CHECK (academic_year_start BETWEEN 2000 AND 2100),
    semester_number SMALLINT NOT NULL
        CHECK (semester_number BETWEEN 1 AND 16),
    section_code VARCHAR(10) NOT NULL,
    capacity SMALLINT NOT NULL DEFAULT 60
        CHECK (capacity BETWEEN 1 AND 500),
    is_active BOOLEAN NOT NULL DEFAULT TRUE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE (
        program_id,
        batch_year,
        semester_number,
        section_code
    )
);

-- ==========================================
-- 5. STUDENT ACADEMIC LINK
-- ==========================================

ALTER TABLE student_profiles
ADD COLUMN IF NOT EXISTS program_id BIGINT
REFERENCES academic_programs(id);

ALTER TABLE student_profiles
ADD COLUMN IF NOT EXISTS academic_section_id BIGINT
REFERENCES academic_sections(id);

-- ==========================================
-- 6. PERFORMANCE INDEXES
-- ==========================================

CREATE INDEX IF NOT EXISTS idx_programs_department
ON academic_programs(department_id);

CREATE INDEX IF NOT EXISTS idx_courses_department
ON academic_courses(department_id);

CREATE INDEX IF NOT EXISTS idx_program_courses_program
ON academic_program_courses(program_id, semester_number);

CREATE INDEX IF NOT EXISTS idx_sections_program
ON academic_sections(program_id, batch_year, semester_number);

CREATE INDEX IF NOT EXISTS idx_student_profiles_program
ON student_profiles(program_id);

CREATE INDEX IF NOT EXISTS idx_student_profiles_section
ON student_profiles(academic_section_id);

COMMIT;