BEGIN;

-- ==========================================
-- CAMPUS CALENDAR
-- ==========================================

CREATE TABLE IF NOT EXISTS campus_events (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title VARCHAR(180) NOT NULL,
    description TEXT NOT NULL DEFAULT '',
    event_type VARCHAR(30) NOT NULL
        CHECK (
            event_type IN (
                'academic',
                'examination',
                'seminar',
                'workshop',
                'holiday',
                'university_closure',
                'other'
            )
        ),
    location VARCHAR(180) NOT NULL DEFAULT '',
    start_date DATE NOT NULL,
    end_date DATE NOT NULL,
    start_time TIME,
    end_time TIME,
    is_published BOOLEAN NOT NULL DEFAULT TRUE,
    created_by BIGINT REFERENCES users(id),
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CHECK (end_date >= start_date),
    CHECK (
        start_time IS NULL
        OR end_time IS NULL
        OR end_time > start_time
    )
);

CREATE INDEX IF NOT EXISTS idx_campus_events_dates
ON campus_events(start_date, end_date);

CREATE INDEX IF NOT EXISTS idx_campus_events_type
ON campus_events(event_type);


-- ==========================================
-- ANNOUNCEMENTS
-- ==========================================

CREATE TABLE IF NOT EXISTS announcements (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title VARCHAR(180) NOT NULL,
    content TEXT NOT NULL,
    priority VARCHAR(20) NOT NULL DEFAULT 'normal'
        CHECK (
            priority IN ('low', 'normal', 'high', 'urgent')
        ),
    audience VARCHAR(20) NOT NULL DEFAULT 'everyone'
        CHECK (
            audience IN (
                'everyone',
                'student',
                'faculty',
                'admin'
            )
        ),
    is_published BOOLEAN NOT NULL DEFAULT TRUE,
    publish_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    expires_at TIMESTAMPTZ,
    created_by BIGINT REFERENCES users(id),
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CHECK (
        expires_at IS NULL
        OR expires_at > publish_at
    )
);

CREATE INDEX IF NOT EXISTS idx_announcements_published
ON announcements(is_published, publish_at DESC);

CREATE INDEX IF NOT EXISTS idx_announcements_audience
ON announcements(audience);

COMMIT;