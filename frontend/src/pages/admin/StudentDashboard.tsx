
import { useState } from "react";
import type { AuthUser } from "../../services/api";
import "./StudentDashboard.css";

type Props = {
  user: AuthUser;
  onLogout: () => void;
};

type StudentPage =
  | "overview"
  | "courses"
  | "timetable"
  | "attendance"
  | "examinations"
  | "announcements"
  | "profile";

const pageTitles: Record<StudentPage, string> = {
  overview: "Student Dashboard",
  courses: "My Courses",
  timetable: "My Timetable",
  attendance: "My Attendance",
  examinations: "Examinations & Results",
  announcements: "Announcements",
  profile: "My Profile",
};

const modules: {
  id: StudentPage;
  icon: string;
  title: string;
  description: string;
}[] = [
  {
    id: "courses",
    icon: "📚",
    title: "My Courses",
    description: "View enrolled subjects and course information.",
  },
  {
    id: "timetable",
    icon: "📅",
    title: "My Timetable",
    description: "View your weekly class schedule.",
  },
  {
    id: "attendance",
    icon: "✅",
    title: "Attendance",
    description: "Track your subject-wise attendance.",
  },
  {
    id: "examinations",
    icon: "📝",
    title: "Examinations & Results",
    description: "View examination schedules and published results.",
  },
  {
    id: "announcements",
    icon: "📢",
    title: "Announcements",
    description: "Read university notices and updates.",
  },
  {
    id: "profile",
    icon: "👤",
    title: "My Profile",
    description: "View your student account information.",
  },
];

function StudentDashboard({ user, onLogout }: Props) {
  const [page, setPage] = useState<StudentPage>("overview");

  const menu: {
    id: StudentPage;
    icon: string;
    label: string;
  }[] = [
    { id: "overview", icon: "⌂", label: "Dashboard" },
    { id: "courses", icon: "📚", label: "My Courses" },
    { id: "timetable", icon: "📅", label: "My Timetable" },
    { id: "attendance", icon: "✅", label: "Attendance" },
    { id: "examinations", icon: "📝", label: "Examinations & Results" },
    { id: "announcements", icon: "📢", label: "Announcements" },
    { id: "profile", icon: "👤", label: "My Profile" },
  ];

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
          <div className="student-sidebar-user">
            <strong>{user.loginId}</strong>
            <small>Student</small>
          </div>
        </div>
      </aside>

      <main className="student-main">
        <header className="student-topbar">
          <div>
            <span>UNIVERSE / STUDENT</span>
            <h1>{pageTitles[page]}</h1>
          </div>

          <button
            type="button"
            className="student-logout"
            onClick={onLogout}
          >
            Sign out →
          </button>
        </header>

        <div className="student-content">
          {page === "overview" && (
            <>
              <section className="student-welcome">
                <span>STUDENT WORKSPACE</span>
                <h2>Welcome back, {user.loginId}!</h2>
                <p>
                  Your university information, academic activities and
                  updates will be available from this portal.
                </p>
              </section>

              <section className="student-section">
                <div className="student-section-heading">
                  <div>
                    <h2>Your Academic Workspace</h2>
                    <p>Quick access to your university services.</p>
                  </div>
                </div>

                <div className="student-module-grid">
                  {modules.map((module) => (
                    <button
                      type="button"
                      className="student-module-card"
                      key={module.id}
                      onClick={() => setPage(module.id)}
                    >
                      <span className="student-module-icon">
                        {module.icon}
                      </span>
                      <strong>{module.title}</strong>
                      <small>{module.description}</small>
                      <span className="student-module-link">
                        Open module →
                      </span>
                    </button>
                  ))}
                </div>
              </section>
            </>
          )}

          {page !== "overview" && page !== "profile" && (
            <section className="student-data-panel">
              <div className="student-panel-icon">
                {modules.find((module) => module.id === page)?.icon}
              </div>
              <h2>{pageTitles[page]}</h2>
              <p>
                This section is ready for your student records. It needs
                to be connected to the corresponding academic records
                service before it can display live data.
              </p>
              <div className="student-data-notice">
                No live records have been loaded for this section yet.
                This does not mean that you have no academic records.
              </div>
              <button
                type="button"
                className="student-back-button"
                onClick={() => setPage("overview")}
              >
                ← Back to dashboard
              </button>
            </section>
          )}

          {page === "profile" && (
            <section className="student-data-panel student-profile-panel">
              <div className="student-panel-icon">👤</div>
              <h2>My Profile</h2>
              <p>Your authenticated UniVerse account information.</p>

              <div className="student-profile-grid">
                <div>
                  <span>Student Login ID</span>
                  <strong>{user.loginId}</strong>
                </div>
                <div>
                  <span>Email Address</span>
                  <strong>{user.email}</strong>
                </div>
                <div>
                  <span>Account ID</span>
                  <strong>{user.id}</strong>
                </div>
                <div>
                  <span>Account Role</span>
                  <strong>Student</strong>
                </div>
              </div>
            </section>
          )}

          <footer className="student-footer">
            UniVerse University Management System · Student Portal
          </footer>
        </div>
      </main>
    </div>
  );
}

export default StudentDashboard;
