BEGIN;

CREATE TABLE IF NOT EXISTS attendance_records (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,

    student_user_id BIGINT NOT NULL
        REFERENCES users(id) ON DELETE CASCADE,

    marked_by BIGINT NOT NULL
        REFERENCES users(id),

    course_code VARCHAR(30) NOT NULL,

    attendance_date DATE NOT NULL DEFAULT CURRENT_DATE,

    status VARCHAR(12) NOT NULL
        CHECK (status IN ('present', 'absent', 'late', 'excused')),

    remarks TEXT NOT NULL DEFAULT '',

    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT attendance_one_record_per_course_day
        UNIQUE (student_user_id, course_code, attendance_date)
);

CREATE INDEX IF NOT EXISTS idx_attendance_student_date
    ON attendance_records(student_user_id, attendance_date DESC);

CREATE INDEX IF NOT EXISTS idx_attendance_date_course
    ON attendance_records(attendance_date, course_code);

COMMIT;