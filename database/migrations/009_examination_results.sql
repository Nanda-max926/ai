BEGIN;

CREATE TABLE IF NOT EXISTS examinations (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    course_code VARCHAR(30) NOT NULL,
    title VARCHAR(180) NOT NULL,
    exam_type VARCHAR(20) NOT NULL
        CHECK (exam_type IN (
            'quiz', 'internal', 'midterm', 'practical',
            'assignment', 'final'
        )),
    exam_date DATE NOT NULL,
    total_marks NUMERIC(7,2) NOT NULL CHECK (total_marks > 0),
    passing_marks NUMERIC(7,2) NOT NULL
        CHECK (passing_marks >= 0 AND passing_marks <= total_marks),
    created_by BIGINT REFERENCES users(id),
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS exam_results (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    exam_id BIGINT NOT NULL
        REFERENCES examinations(id) ON DELETE CASCADE,
    student_user_id BIGINT NOT NULL
        REFERENCES users(id) ON DELETE CASCADE,
    marks_obtained NUMERIC(7,2) NOT NULL CHECK (marks_obtained >= 0),
    grade VARCHAR(10) NOT NULL DEFAULT '',
    remarks TEXT NOT NULL DEFAULT '',
    entered_by BIGINT NOT NULL REFERENCES users(id),
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT exam_result_student_unique
        UNIQUE (exam_id, student_user_id)
);

CREATE INDEX IF NOT EXISTS idx_examinations_date
    ON examinations(exam_date DESC);

CREATE INDEX IF NOT EXISTS idx_exam_results_student
    ON exam_results(student_user_id);

COMMIT;