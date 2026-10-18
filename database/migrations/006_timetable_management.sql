BEGIN;

CREATE EXTENSION IF NOT EXISTS btree_gist;

-- ==========================================
-- CLASSROOMS AND LABORATORIES
-- ==========================================

CREATE TABLE IF NOT EXISTS academic_rooms (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    building VARCHAR(80) NOT NULL DEFAULT '',
    room_number VARCHAR(30) NOT NULL,
    capacity SMALLINT NOT NULL DEFAULT 60
        CHECK (capacity BETWEEN 1 AND 500),
    room_type VARCHAR(20) NOT NULL DEFAULT 'classroom'
        CHECK (room_type IN ('classroom', 'laboratory', 'auditorium', 'other')),
    is_active BOOLEAN NOT NULL DEFAULT TRUE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE (building, room_number)
);

-- ==========================================
-- WEEKLY TIMETABLE
-- day_of_week: 1 = Monday, 7 = Sunday
-- ==========================================

CREATE TABLE IF NOT EXISTS academic_timetable_entries (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,

    section_id BIGINT NOT NULL
        REFERENCES academic_sections(id),

    course_id BIGINT NOT NULL
        REFERENCES academic_courses(id),

    faculty_user_id BIGINT NOT NULL
        REFERENCES users(id),

    room_id BIGINT NOT NULL
        REFERENCES academic_rooms(id),

    day_of_week SMALLINT NOT NULL
        CHECK (day_of_week BETWEEN 1 AND 7),

    start_time TIME NOT NULL,
    end_time TIME NOT NULL,

    is_active BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CHECK (start_time < end_time),

    -- Convert time into a comparable range for a recurring weekday.
    slot_range TSRANGE GENERATED ALWAYS AS (
        tsrange(
            TIMESTAMP '2000-01-03 00:00:00' + start_time,
            TIMESTAMP '2000-01-03 00:00:00' + end_time,
            '[)'
        )
    ) STORED,

    -- A section cannot have two classes at the same time.
    CONSTRAINT timetable_section_no_overlap
    EXCLUDE USING gist (
        section_id WITH =,
        day_of_week WITH =,
        slot_range WITH &&
    ) WHERE (is_active),

    -- A faculty member cannot teach two classes at the same time.
    CONSTRAINT timetable_faculty_no_overlap
    EXCLUDE USING gist (
        faculty_user_id WITH =,
        day_of_week WITH =,
        slot_range WITH &&
    ) WHERE (is_active),

    -- A room cannot host two classes at the same time.
    CONSTRAINT timetable_room_no_overlap
    EXCLUDE USING gist (
        room_id WITH =,
        day_of_week WITH =,
        slot_range WITH &&
    ) WHERE (is_active)
);

CREATE INDEX IF NOT EXISTS idx_timetable_section
ON academic_timetable_entries(section_id);

CREATE INDEX IF NOT EXISTS idx_timetable_faculty
ON academic_timetable_entries(faculty_user_id);

CREATE INDEX IF NOT EXISTS idx_timetable_room
ON academic_timetable_entries(room_id);

CREATE INDEX IF NOT EXISTS idx_timetable_day
ON academic_timetable_entries(day_of_week, start_time);

COMMIT;