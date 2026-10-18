
import { useEffect, useState } from "react";
import type { AuthUser, StudentPortalData } from "../../services/api";
import { getStudentPortal } from "../../services/api";
import "./StudentDashboard.css";

interface Props {
  user: AuthUser;
  onLogout: () => void;
}

type Page =
  | "overview"
  | "courses"
  | "timetable"
  | "attendance"
  | "examinations"
  | "announcements"
  | "profile";

const menu: { id: Page; icon: string; label: string }[] = [
  { id: "overview", icon: "⌂", label: "Dashboard" },
  { id: "courses", icon: "📚", label: "My Courses" },
  { id: "timetable", icon: "📅", label: "My Timetable" },
  { id: "attendance", icon: "✅", label: "Attendance" },
  { id: "examinations", icon: "📝", label: "Examinations & Results" },
  { id: "announcements", icon: "📢", label: "Announcements" },
  { id: "profile", icon: "👤", label: "My Profile" },
];

const titles: Record<Page, string> = {
  overview: "Student Dashboard",
  courses: "My Courses",
  timetable: "My Timetable",
  attendance: "My Attendance",
  examinations: "Examinations & Results",
  announcements: "Announcements & Events",
  profile: "My Profile",
};

function dateLabel(value: string | null | undefined) {
  if (!value) return "—";

  const date = new Date(value);
  return Number.isNaN(date.getTime())
    ? value
    : date.toLocaleDateString();
}

function EmptyState({ message }: { message: string }) {
  return <div className="student-data-notice">{message}</div>;
}

export default function StudentDashboard({ user, onLogout }: Props) {
  const [page, setPage] = useState<Page>("overview");
  const [portal, setPortal] = useState<StudentPortalData | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState("");

  async function refreshPortal() {
    setError("");

    try {
      const result = await getStudentPortal();
      setPortal(result);
    } catch (err) {
      setError(
        err instanceof Error
          ? err.message
          : "Unable to load student records."
      );
    } finally {
      setLoading(false);
    }
  }

  useEffect(() => {
    let active = true;

    getStudentPortal()
      .then((result) => {
        if (active) setPortal(result);
      })
      .catch((err: unknown) => {
        if (active) {
          setError(
            err instanceof Error
              ? err.message
              : "Unable to load student records."
          );
        }
      })
      .finally(() => {
        if (active) setLoading(false);
      });

    return () => {
      active = false;
    };
  }, []);

  const profile = portal?.profile;
  const attendance = portal?.attendance;

  return (
    <div className="student-shell">
      <aside className="student-sidebar">
        <div className="student-brand">
          <div className="student-brand-mark">U</div>
          <div>
            <strong>UniVerse</strong>
            <small>STUDENT PORTAL</small>
          </div>
        </div>

        <p className="student-nav-label">MY UNIVERSITY</p>

        <nav className="student-nav">
          {menu.map((item) => (
            <button
              type="button"
              key={item.id}
              className={page === item.id ? "active" : ""}
              onClick={() => setPage(item.id)}
            >
              <span>{item.icon}</span>
              {item.label}
            </button>
          ))}
        </nav>

        <div className="student-sidebar-bottom">
          <div className="student-avatar">ST</div>
          <div>
            <strong>{user.loginId}</strong>
            <small>Student</small>
          </div>
        </div>
      </aside>

      <main className="student-main">
        <header className="student-topbar">
          <div>
            <span>UNIVERSE / STUDENT</span>
            <h1>{titles[page]}</h1>
          </div>

          <div className="student-topbar-actions">
            <button
              type="button"
              className="student-refresh"
              onClick={refreshPortal}
              disabled={loading}
            >
              ↻ Refresh
            </button>
            <button
              type="button"
              className="student-logout"
              onClick={onLogout}
            >
              Sign out →
            </button>
          </div>
        </header>

        <div className="student-content">
          {loading && (
            <section className="student-data-panel">
              Loading your university records...
            </section>
          )}

          {!loading && error && (
            <section className="student-data-panel">
              <h2>Unable to load your records</h2>
              <div className="student-data-notice">{error}</div>
              <button
                type="button"
                className="student-back-button"
                onClick={refreshPortal}
              >
                Try again
              </button>
            </section>
          )}

          {!loading && !error && portal && (
            <>
              {page === "overview" && (
                <>
                  <section className="student-welcome">
                    <span>STUDENT WORKSPACE</span>
                    <h2>
                      Welcome back, {profile?.fullName || user.loginId}!
                    </h2>
                    <p>
                      Your academic information, attendance, schedule and
                      university updates in one place.
                    </p>
                  </section>

                  <section className="student-stats">
                    <div className="student-stat">
                      <span>My Courses</span>
                      <strong>{portal.courses.length}</strong>
                    </div>
                    <div className="student-stat">
                      <span>Attendance</span>
                      <strong>{attendance?.percentage ?? 0}%</strong>
                    </div>
                    <div className="student-stat">
                      <span>Timetable Entries</span>
                      <strong>{portal.timetable.length}</strong>
                    </div>
                    <div className="student-stat">
                      <span>Examinations</span>
                      <strong>{portal.examinations.length}</strong>
                    </div>
                  </section>

                  <h2 className="student-section-title">
                    Your Academic Workspace
                  </h2>

                  <div className="student-module-grid">
                    {menu.filter((item) => item.id !== "overview").map(
                      (item) => (
                        <button
                          type="button"
                          className="student-module-card"
                          key={item.id}
                          onClick={() => setPage(item.id)}
                        >
                          <span className="student-module-icon">
                            {item.icon}
                          </span>
                          <strong>{item.label}</strong>
                          <small>View your {item.label.toLowerCase()}.</small>
                          <span className="student-module-link">
                            Open module →
                          </span>
                        </button>
                      )
                    )}
                  </div>

                  <section className="student-data-panel student-summary">
                    <h2>Student Information</h2>
                    <div className="student-profile-grid">
                      <div>
                        <span>Registration / Login ID</span>
                        <strong>{profile?.loginId || user.loginId}</strong>
                      </div>
                      <div>
                        <span>Department</span>
                        <strong>{profile?.department || "Not assigned"}</strong>
                      </div>
                      <div>
                        <span>Program</span>
                        <strong>{profile?.program || "Not assigned"}</strong>
                      </div>
                      <div>
                        <span>Semester</span>
                        <strong>{profile?.semester ?? "Not assigned"}</strong>
                      </div>
                    </div>
                  </section>
                </>
              )}

              {page === "courses" && (
                <section className="student-data-panel">
                  <h2>My Courses</h2>
                  <p>Courses associated with your department and curriculum.</p>

                  {portal.courses.length === 0 ? (
                    <EmptyState message="No active courses are currently assigned to your department or program." />
                  ) : (
                    <div className="student-table-wrap">
                      <table className="student-record-table">
                        <thead>
                          <tr>
                            <th>Course Code</th>
                            <th>Course Name</th>
                            <th>Credits</th>
                            <th>Type</th>
                            <th>Semester</th>
                          </tr>
                        </thead>
                        <tbody>
                          {portal.courses.map((course) => (
                            <tr key={course.id}>
                              <td>{course.code}</td>
                              <td>{course.title}</td>
                              <td>{course.credits}</td>
                              <td>{course.courseType}</td>
                              <td>{course.semesterNumber}</td>
                            </tr>
                          ))}
                        </tbody>
                      </table>
                    </div>
                  )}
                </section>
              )}

              {page === "timetable" && (
                <section className="student-data-panel">
                  <h2>My Timetable</h2>
                  <p>Your scheduled classes for your assigned section.</p>

                  {portal.timetable.length === 0 ? (
                    <EmptyState message="No timetable entries have been created for your academic section yet." />
                  ) : (
                    <div className="student-table-wrap">
                      <table className="student-record-table">
                        <thead>
                          <tr>
                            <th>Day</th>
                            <th>Time</th>
                            <th>Course</th>
                            <th>Faculty</th>
                            <th>Room</th>
                          </tr>
                        </thead>
                        <tbody>
                          {portal.timetable.map((entry) => (
                            <tr key={entry.id}>
                              <td>Day {entry.dayOfWeek}</td>
                              <td>
                                {entry.startTime} – {entry.endTime}
                              </td>
                              <td>
                                <strong>{entry.courseCode}</strong>
                                <br />
                                {entry.courseTitle}
                              </td>
                              <td>{entry.facultyName}</td>
                              <td>{entry.room}</td>
                            </tr>
                          ))}
                        </tbody>
                      </table>
                    </div>
                  )}
                </section>
              )}

              {page === "attendance" && (
                <section className="student-data-panel">
                  <h2>My Attendance</h2>

                  <div className="student-attendance-summary">
                    <div>
                      <span>Overall attendance</span>
                      <strong>{attendance?.percentage ?? 0}%</strong>
                    </div>
                    <div>
                      <span>Present</span>
                      <strong>{attendance?.present ?? 0}</strong>
                    </div>
                    <div>
                      <span>Absent</span>
                      <strong>{attendance?.absent ?? 0}</strong>
                    </div>
                    <div>
                      <span>Total classes recorded</span>
                      <strong>{attendance?.total ?? 0}</strong>
                    </div>
                  </div>

                  <h3>Subject-wise Attendance</h3>

                  {!attendance?.byCourse?.length ? (
                    <EmptyState message="No attendance records have been entered for your account yet." />
                  ) : (
                    <div className="student-table-wrap">
                      <table className="student-record-table">
                        <thead>
                          <tr>
                            <th>Course</th>
                            <th>Total</th>
                            <th>Present</th>
                            <th>Absent</th>
                            <th>Percentage</th>
                          </tr>
                        </thead>
                        <tbody>
                          {attendance.byCourse.map((item) => (
                            <tr key={item.courseCode}>
                              <td>
                                {item.courseCode} — {item.courseTitle}
                              </td>
                              <td>{item.total}</td>
                              <td>{item.present}</td>
                              <td>{item.absent}</td>
                              <td>
                                <strong>{item.percentage}%</strong>
                              </td>
                            </tr>
                          ))}
                        </tbody>
                      </table>
                    </div>
                  )}
                </section>
              )}

              {page === "examinations" && (
                <section className="student-data-panel">
                  <h2>Examinations & Results</h2>
                  <p>Examinations and results for your courses.</p>

                  {portal.examinations.length === 0 ? (
                    <EmptyState message="No examinations have been published for your courses yet." />
                  ) : (
                    <div className="student-table-wrap">
                      <table className="student-record-table">
                        <thead>
                          <tr>
                            <th>Exam</th>
                            <th>Course</th>
                            <th>Type</th>
                            <th>Date</th>
                            <th>Maximum Marks</th>
                            <th>Your Result</th>
                          </tr>
                        </thead>
                        <tbody>
                          {portal.examinations.map((exam) => (
                            <tr key={exam.id}>
                              <td>{exam.title}</td>
                              <td>{exam.courseCode}</td>
                              <td>{exam.examType}</td>
                              <td>{dateLabel(exam.examDate)}</td>
                              <td>{exam.totalMarks}</td>
                              <td>
                                {exam.marksObtained == null
                                  ? "Not published"
                                  : `${exam.marksObtained} / ${exam.totalMarks} (${exam.grade || "No grade"})`}
                              </td>
                            </tr>
                          ))}
                        </tbody>
                      </table>
                    </div>
                  )}
                </section>
              )}

              {page === "announcements" && (
                <>
                  <section className="student-data-panel">
                    <h2>University Announcements</h2>

                    {!portal.announcements.length ? (
                      <EmptyState message="There are no published student announcements right now." />
                    ) : (
                      <div className="student-notice-list">
                        {portal.announcements.map((notice) => (
                          <article key={notice.id}>
                            <span>{notice.priority || "NOTICE"}</span>
                            <h3>{notice.title}</h3>
                            <p>{notice.content}</p>
                            <small>{dateLabel(notice.publishAt)}</small>
                          </article>
                        ))}
                      </div>
                    )}
                  </section>

                  <section className="student-data-panel student-summary">
                    <h2>Campus Events</h2>

                    {!portal.events.length ? (
                      <EmptyState message="No upcoming published campus events are available." />
                    ) : (
                      <div className="student-notice-list">
                        {portal.events.map((event) => (
                          <article key={event.id}>
                            <span>{event.eventType}</span>
                            <h3>{event.title}</h3>
                            <p>{event.description}</p>
                            <small>
                              {dateLabel(event.startDate)} · {event.location}
                            </small>
                          </article>
                        ))}
                      </div>
                    )}
                  </section>
                </>
              )}

              {page === "profile" && (
                <section className="student-data-panel">
                  <h2>My Profile</h2>
                  <div className="student-profile-grid">
                    <div>
                      <span>Full Name</span>
                      <strong>{profile?.fullName || "Not available"}</strong>
                    </div>
                    <div>
                      <span>Login / Registration ID</span>
                      <strong>{profile?.loginId || user.loginId}</strong>
                    </div>
                    <div>
                      <span>Email</span>
                      <strong>{profile?.email || user.email}</strong>
                    </div>
                    <div>
                      <span>Department</span>
                      <strong>{profile?.department || "Not assigned"}</strong>
                    </div>
                    <div>
                      <span>Department Code</span>
                      <strong>{profile?.departmentCode || "—"}</strong>
                    </div>
                    <div>
                      <span>Program</span>
                      <strong>{profile?.program || "Not assigned"}</strong>
                    </div>
                    <div>
                      <span>Semester</span>
                      <strong>{profile?.semester ?? "Not assigned"}</strong>
                    </div>
                    <div>
                      <span>Admission Batch</span>
                      <strong>{profile?.batchYear ?? "Not assigned"}</strong>
                    </div>
                    <div>
                      <span>Section</span>
                      <strong>{profile?.section || "Not assigned"}</strong>
                    </div>
                    <div>
                      <span>Account ID</span>
                      <strong>{user.id}</strong>
                    </div>
                  </div>
                </section>
              )}
            </>
          )}

          <footer className="student-footer">
            UniVerse University Management System · Student Portal
          </footer>
        </div>
      </main>
    </div>
  );
}
