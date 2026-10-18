
#include "controllers/StudentController.h"

#include <sstream>
#include <string>
#include <memory>
#include <utility>

using namespace drogon;

namespace universe::controllers
{
namespace
{
HttpResponsePtr makeResponse(
    HttpStatusCode status,
    const Json::Value &body
)
{
    auto response = HttpResponse::newHttpJsonResponse(body);
    response->setStatusCode(status);
    response->addHeader("Cache-Control", "no-store");
    return response;
}

HttpResponsePtr makeError(
    HttpStatusCode status,
    const std::string &message
)
{
    Json::Value body;
    body["status"] = "error";
    body["message"] = message;
    return makeResponse(status, body);
}

Json::Value parseJsonColumn(
    const drogon::orm::Row &row,
    const char *column
)
{
    const std::string raw = row[column].as<std::string>();

    Json::Value value;
    Json::CharReaderBuilder builder;
    std::istringstream input(raw);
    std::string errors;

    if (!Json::parseFromStream(builder, input, &value, &errors))
    {
        LOG_ERROR << "Could not parse student portal JSON: " << errors;
        return Json::Value();
    }

    return value;
}

const char *portalSql = R"SQL(
WITH student AS
(
    SELECT
        sp.user_id,
        sp.full_name,
        sp.department_id,
        sp.semester,
        sp.batch_year,
        sp.section,
        sp.program_id,
        sp.academic_section_id,
        u.login_id,
        u.email,
        COALESCE(d.name, '') AS department_name,
        COALESCE(d.code, '') AS department_code,
        COALESCE(ap.name, '') AS program_name,
        COALESCE(sec.section_code, sp.section, '') AS section_name
    FROM student_profiles sp
    JOIN users u ON u.id = sp.user_id
    LEFT JOIN departments d ON d.id = sp.department_id
    LEFT JOIN academic_programs ap ON ap.id = sp.program_id
    LEFT JOIN academic_sections sec ON sec.id = sp.academic_section_id
    WHERE sp.user_id = $1
),
student_courses AS
(
    SELECT DISTINCT
        c.id,
        c.code,
        c.title,
        c.credits,
        c.course_type,
        COALESCE(apc.semester_number, s.semester) AS semester_number
    FROM student s
    JOIN academic_courses c
      ON c.department_id = s.department_id
     AND c.is_active = TRUE
    LEFT JOIN academic_program_courses apc
      ON apc.program_id = s.program_id
     AND apc.course_id = c.id
     AND apc.semester_number = s.semester
    WHERE apc.id IS NOT NULL
       OR NOT EXISTS
       (
           SELECT 1
           FROM academic_program_courses x
           WHERE x.program_id = s.program_id
             AND x.semester_number = s.semester
       )
)
SELECT

    COALESCE(
        (
            SELECT json_build_object(
                'id', s.user_id,
                'loginId', s.login_id,
                'email', s.email,
                'fullName', s.full_name,
                'departmentId', s.department_id,
                'department', s.department_name,
                'departmentCode', s.department_code,
                'semester', s.semester,
                'batchYear', s.batch_year,
                'section', s.section_name,
                'programId', s.program_id,
                'program', s.program_name,
                'academicSectionId', s.academic_section_id
            )::text
            FROM student s
        ),
        '{}'::text
    ) AS profile,

    COALESCE(
        (
            SELECT json_agg(
                json_build_object(
                    'id', c.id,
                    'code', c.code,
                    'title', c.title,
                    'credits', c.credits,
                    'courseType', c.course_type,
                    'semesterNumber', c.semester_number
                )
                ORDER BY c.code
            )::text
            FROM student_courses c
        ),
        '[]'
    ) AS courses,

    COALESCE(
        (
            SELECT json_agg(
                json_build_object(
                    'id', t.id,
                    'dayOfWeek', t.day_of_week,
                    'startTime',
                        to_char(t.start_time, 'HH12:MI AM'),
                    'endTime',
                        to_char(t.end_time, 'HH12:MI AM'),
                    'courseCode', c.code,
                    'courseTitle', c.title,
                    'facultyName',
                        COALESCE(fp.full_name, fu.login_id, 'Not assigned'),
                    'room',
                        COALESCE(
                            concat_ws(' ', r.building, r.room_number),
                            'Not assigned'
                        )
                )
                ORDER BY t.day_of_week, t.start_time
            )::text
            FROM student s
            JOIN academic_timetable_entries t
              ON t.section_id = s.academic_section_id
             AND t.is_active = TRUE
            JOIN student_courses c ON c.id = t.course_id
            LEFT JOIN faculty_profiles fp
              ON fp.user_id = t.faculty_user_id
            LEFT JOIN users fu
              ON fu.id = t.faculty_user_id
            LEFT JOIN academic_rooms r
              ON r.id = t.room_id
        ),
        '[]'
    ) AS timetable,

    (
        SELECT json_build_object(
            'total', a.total,
            'present', a.present,
            'absent', a.absent,
            'percentage',
                CASE
                    WHEN a.total = 0 THEN 0
                    ELSE ROUND(
                        100.0 * a.present::numeric / a.total,
                        1
                    )
                END,
            'byCourse',
                COALESCE(
                    (
                        SELECT json_agg(
                            json_build_object(
                                'courseCode', x.course_code,
                                'courseTitle',
                                    COALESCE(c.title, x.course_code),
                                'total', x.total,
                                'present', x.present,
                                'absent', x.absent,
                                'percentage',
                                    CASE
                                        WHEN x.total = 0 THEN 0
                                        ELSE ROUND(
                                            100.0 * x.present::numeric
                                                / x.total,
                                            1
                                        )
                                    END
                            )
                            ORDER BY x.course_code
                        )
                        FROM
                        (
                            SELECT
                                course_code,
                                COUNT(*) AS total,
                                COUNT(*) FILTER (
                                    WHERE LOWER(status) = 'present'
                                ) AS present,
                                COUNT(*) FILTER (
                                    WHERE LOWER(status) = 'absent'
                                ) AS absent
                            FROM attendance_records
                            WHERE student_user_id = $1
                            GROUP BY course_code
                        ) x
                        LEFT JOIN student_courses c
                          ON UPPER(c.code) = UPPER(x.course_code)
                    ),
                    '[]'::json
                )
        )::text
        FROM
        (
            SELECT
                COUNT(*) AS total,
                COUNT(*) FILTER (
                    WHERE LOWER(status) = 'present'
                ) AS present,
                COUNT(*) FILTER (
                    WHERE LOWER(status) = 'absent'
                ) AS absent
            FROM attendance_records
            WHERE student_user_id = $1
        ) a
    ) AS attendance,

    COALESCE(
        (
            SELECT json_agg(
                json_build_object(
                    'id', e.id,
                    'courseCode', e.course_code,
                    'title', e.title,
                    'examType', e.exam_type,
                    'examDate', e.exam_date,
                    'totalMarks', e.total_marks,
                    'passingMarks', e.passing_marks,
                    'marksObtained', er.marks_obtained,
                    'grade', er.grade,
                    'remarks', COALESCE(er.remarks, '')
                )
                ORDER BY e.exam_date
            )::text
            FROM examinations e
            JOIN student_courses c
              ON UPPER(c.code) = UPPER(e.course_code)
            LEFT JOIN exam_results er
              ON er.exam_id = e.id
             AND er.student_user_id = $1
        ),
        '[]'
    ) AS examinations,

    COALESCE(
        (
            SELECT json_agg(
                json_build_object(
                    'id', a.id,
                    'title', a.title,
                    'content', a.content,
                    'priority', a.priority,
                    'publishAt', a.publish_at
                )
                ORDER BY a.publish_at DESC
            )::text
            FROM announcements a
            WHERE a.is_published = TRUE
              AND a.publish_at <= NOW()
              AND (
                  a.expires_at IS NULL
                  OR a.expires_at > NOW()
              )
              AND (
                  LOWER(a.audience) IN (
                      'all',
                      'students',
                      'student',
                      'everyone',
                      'university'
                  )
                  OR LOWER(a.audience) LIKE '%student%'
              )
        ),
        '[]'
    ) AS announcements,

    COALESCE(
        (
            SELECT json_agg(
                json_build_object(
                    'id', e.id,
                    'title', e.title,
                    'description', e.description,
                    'eventType', e.event_type,
                    'location', e.location,
                    'startDate', e.start_date,
                    'endDate', e.end_date,
                    'startTime', e.start_time,
                    'endTime', e.end_time
                )
                ORDER BY e.start_date
            )::text
            FROM campus_events e
            WHERE e.is_published = TRUE
              AND e.end_date >= CURRENT_DATE
        ),
        '[]'
    ) AS events
)SQL";

} // anonymous namespace

void StudentController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/student/portal",
        &StudentController::getPortal,
        {Get}
    );

    LOG_INFO << "Student portal routes registered.";
}

void StudentController::getPortal(
    const HttpRequestPtr &request,
    Callback &&callback
)
{
    auto cb = std::make_shared<Callback>(
        std::move(callback)
    );

    const auto session = request->session();

    if (!session ||
        session->get<std::string>("role") != "student")
    {
        (*cb)(makeError(
            k403Forbidden,
            "Student account access is required."
        ));
        return;
    }

    const long long studentId =
        session->get<long long>("user_id");

    if (studentId <= 0)
    {
        (*cb)(makeError(
            k401Unauthorized,
            "Student session is invalid. Please sign in again."
        ));
        return;
    }

    try
    {
        auto database = app().getDbClient("default");

        database->execSqlAsync(
            portalSql,

            [cb](const orm::Result &result)
            {
                if (result.empty())
                {
                    (*cb)(makeError(
                        k404NotFound,
                        "Student profile was not found."
                    ));
                    return;
                }

                const auto &row = result[0];

                Json::Value profile =
                    parseJsonColumn(row, "profile");

                if (!profile.isObject() || profile.empty())
                {
                    (*cb)(makeError(
                        k404NotFound,
                        "No student profile is linked to this account."
                    ));
                    return;
                }

                Json::Value body;
                body["status"] = "success";
                body["profile"] = profile;
                body["courses"] =
                    parseJsonColumn(row, "courses");
                body["timetable"] =
                    parseJsonColumn(row, "timetable");
                body["attendance"] =
                    parseJsonColumn(row, "attendance");
                body["examinations"] =
                    parseJsonColumn(row, "examinations");
                body["announcements"] =
                    parseJsonColumn(row, "announcements");
                body["events"] =
                    parseJsonColumn(row, "events");

                (*cb)(makeResponse(k200OK, body));
            },

            [cb](const orm::DrogonDbException &error)
            {
                LOG_ERROR
                    << "Student portal database error: "
                    << error.base().what();

                (*cb)(makeError(
                    k500InternalServerError,
                    "Unable to load student portal records."
                ));
            },

            studentId
        );
    }
    catch (const std::exception &error)
    {
        LOG_ERROR << "Student portal error: " << error.what();

        (*cb)(makeError(
            k500InternalServerError,
            "Unable to open the student portal."
        ));
    }
}

} // namespace universe::controllers
