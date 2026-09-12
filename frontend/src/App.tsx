
import { useMemo, useState } from "react";
import "./App.css";

type Role = "Student" | "Faculty" | "Admin";

type Stat = {
  label: string;
  value: string;
  note: string;
  icon: string;
};

type AttendanceItem = {
  subject: string;
  percentage: number;
};

const MENU_ITEMS: Record<Role, string[]> = {
  Student: [
    "Dashboard",
    "My Profile",
    "My Courses",
    "Attendance",
    "Assignments",
    "Examinations",
    "Results",
    "Timetable",
    "Library",
    "Events",
    "Notifications",
  ],
  Faculty: [
    "Dashboard",
    "My Profile",
    "My Classes",
    "Attendance",
    "Assignments",
    "Gradebook",
    "Study Materials",
    "Announcements",
    "Notifications",
  ],
  Admin: [
    "Dashboard",
    "Students",
    "Faculty",
    "Departments",
    "Courses",
    "Timetable",
    "Reports",
    "Announcements",
    "System Settings",
  ],
};

const STATS: Record<Role, Stat[]> = {
  Student: [
    { label: "Attendance", value: "91.6%", note: "Above the 75% target", icon: "◷" },
    { label: "Current CGPA", value: "8.72", note: "Sample academic record", icon: "▥" },
    { label: "Assignments", value: "04", note: "2 awaiting submission", icon: "✎" },
    { label: "Enrolled courses", value: "06", note: "Current semester", icon: "▤" },
  ],
  Faculty: [
    { label: "My classes", value: "08", note: "Assigned courses", icon: "▤" },
    { label: "Students", value: "240", note: "Across your classes", icon: "♙" },
    { label: "Assignments", value: "12", note: "3 awaiting review", icon: "✎" },
    { label: "Attendance", value: "94%", note: "Average class attendance", icon: "◷" },
  ],
  Admin: [
    { label: "Students", value: "4,820", note: "Registered students", icon: "♙" },
    { label: "Faculty", value: "286", note: "Active faculty", icon: "♟" },
    { label: "Departments", value: "12", note: "Academic departments", icon: "▦" },
    { label: "Courses", value: "164", note: "Available courses", icon: "▤" },
  ],
};

const SUBTITLES: Record<Role, string> = {
  Student: "Your academic activities, courses, and progress in one place.",
  Faculty: "Manage your classes, assignments, and student progress.",
  Admin: "Monitor university operations and manage academic resources.",
};

const MENU_ICONS: Record<string, string> = {
  Dashboard: "⌂",
  "My Profile": "♙",
  "My Courses": "▤",
  "My Classes": "▤",
  Attendance: "◷",
  Assignments: "✎",
  Examinations: "▣",
  Results: "▥",
  Timetable: "▦",
  Library: "▧",
  Events: "◇",
  Notifications: "♢",
  Gradebook: "▥",
  "Study Materials": "▤",
  Announcements: "◉",
  Students: "♙",
  Faculty: "♟",
  Departments: "▦",
  Courses: "▤",
  Reports: "▥",
  "System Settings": "⚙",
};

const ATTENDANCE: AttendanceItem[] = [
  { subject: "Programming", percentage: 92 },
  { subject: "Data Structures", percentage: 84 },
  { subject: "Mathematics", percentage: 76 },
  { subject: "Data Science", percentage: 96 },
];

const QUICK_ACTIONS = [
  "Assignments",
  "Timetable",
  "Results",
  "Notifications",
];

function App() {
  const [role, setRole] = useState<Role>("Student");
  const [activePage, setActivePage] = useState("Dashboard");
  const [searchTerm, setSearchTerm] = useState("");

  const visibleMenu = useMemo(() => {
    const query = searchTerm.trim().toLowerCase();

    if (!query) {
      return MENU_ITEMS[role];
    }

    return MENU_ITEMS[role].filter((item) =>
      item.toLowerCase().includes(query)
    );
  }, [role, searchTerm]);

  function handleRoleChange(nextRole: Role) {
    setRole(nextRole);
    setActivePage("Dashboard");
    setSearchTerm("");
  }

  const avatarText =
    role === "Student" ? "ST" :
    role === "Faculty" ? "FC" : "AD";

  return (
    <div className="portal">

      {/* SIDEBAR */}
      <aside className="sidebar">

        <div className="brand">
          <div className="brand-mark">U</div>

          <div className="brand-copy">
            <h1>UniVerse</h1>
            <span>UNIVERSITY PORTAL</span>
          </div>
        </div>

        <p className="sidebar-label">MAIN MENU</p>

        <nav className="sidebar-nav" aria-label="Main navigation">

          {visibleMenu.length > 0 ? (
            visibleMenu.map((item) => (
              <button
                key={item}
                type="button"
                className={`nav-item ${
                  activePage === item ? "active" : ""
                }`}
                onClick={() => setActivePage(item)}
              >
                <span className="nav-icon" aria-hidden="true">
                  {MENU_ICONS[item] ?? "•"}
                </span>

                <span>{item}</span>
              </button>
            ))
          ) : (
            <p className="empty-menu">
              No matching pages.
            </p>
          )}

        </nav>

        <div className="sidebar-footer">

          <div className="help-card">
            <span className="help-icon">?</span>

            <strong>Need help?</strong>

            <p>
              Contact the university support team
              for assistance.
            </p>

            <button
              type="button"
              onClick={() =>
                window.alert("Support module coming soon.")
              }
            >
              Get support
            </button>
          </div>

          <p className="version-text">
            UniVerse Portal · Version 1.0
          </p>

        </div>
      </aside>

      {/* MAIN CONTENT */}
      <main className="main-content">

        {/* TOP NAVIGATION */}
        <header className="topbar">

          <div className="breadcrumb">
            <strong>UniVerse</strong>
            <span>/</span>
            <span>{activePage}</span>
          </div>

          <div className="topbar-actions">

            <label className="search-box">
              <span aria-hidden="true">⌕</span>

              <input
                type="search"
                value={searchTerm}
                onChange={(event) =>
                  setSearchTerm(event.currentTarget.value)
                }
                placeholder="Search menu..."
                aria-label="Search menu"
              />
            </label>

            <button
              className="notification-button"
              type="button"
              aria-label="Open notifications"
              onClick={() => setActivePage("Notifications")}
            >
              ♢
              <span className="notification-dot" />
            </button>

            <div className="avatar" title={`${role} preview`}>
              {avatarText}
            </div>

          </div>
        </header>

        {/* PAGE CONTENT */}
        <div className="page-content">

          {/* WELCOME BANNER */}
          <section className="welcome-banner">

            <div>
              <span className="eyebrow">
                SMART UNIVERSITY MANAGEMENT
              </span>

              <h2>Welcome to UniVerse</h2>

              <p>{SUBTITLES[role]}</p>

              <button
                className="light-button"
                type="button"
                onClick={() =>
                  setActivePage(
                    role === "Student" ? "My Profile" : "Dashboard"
                  )
                }
              >
                Explore portal <span aria-hidden="true">→</span>
              </button>
            </div>

            <div className="welcome-art" aria-hidden="true">
              🎓
            </div>

          </section>

          {/* OVERVIEW */}
          <section className="section-heading">

            <div>
              <h2>Overview</h2>
              <p>Your portal at a glance</p>
            </div>

            <label className="role-picker">
              <span>Preview role</span>

              <select
                value={role}
                onChange={(event) =>
                  handleRoleChange(event.currentTarget.value as Role)
                }
                aria-label="Select preview role"
              >
                <option value="Student">Student</option>
                <option value="Faculty">Faculty</option>
                <option value="Admin">Admin</option>
              </select>
            </label>

          </section>

          {activePage === "Dashboard" ? (

            <>

              {/* STATISTICS */}
              <section
                className="stats-grid"
                aria-label="Summary statistics"
              >

                {STATS[role].map((stat, index) => (

                  <article className="stat-card" key={stat.label}>

                    <div className="stat-top">
                      <span>{stat.label}</span>

                      <span
                        className={`stat-icon stat-icon-${index}`}
                        aria-hidden="true"
                      >
                        {stat.icon}
                      </span>
                    </div>

                    <strong className="stat-value">
                      {stat.value}
                    </strong>

                    <span className="stat-note">
                      {stat.note}
                    </span>

                  </article>

                ))}

              </section>

              {/* DASHBOARD PANELS */}
              <section className="dashboard-grid">

                {/* ATTENDANCE */}
                <article className="content-card">

                  <div className="card-heading">

                    <div>
                      <h3>
                        {role === "Student"
                          ? "Attendance overview"
                          : role === "Faculty"
                          ? "Class overview"
                          : "University overview"}
                      </h3>

                      <p>Illustrative sample data</p>
                    </div>

                    <button
                      className="text-button"
                      type="button"
                      onClick={() => setActivePage("Attendance")}
                    >
                      View details →
                    </button>

                  </div>

                  {ATTENDANCE.map((item) => (

                    <div className="attendance-row" key={item.subject}>

                      <div className="attendance-label">
                        <span>{item.subject}</span>
                        <strong>{item.percentage}%</strong>
                      </div>

                      <div className="progress-track">
                        <div
                          className="progress-fill"
                          style={{ width: `${item.percentage}%` }}
                        />
                      </div>

                    </div>

                  ))}

                </article>

                {/* QUICK ACTIONS */}
                <article className="content-card">

                  <div className="card-heading">
                    <div>
                      <h3>Quick actions</h3>
                      <p>Frequently used sections</p>
                    </div>
                  </div>

                  <div className="quick-action-list">

                    {QUICK_ACTIONS.map((action) => (

                      <button
                        className="quick-action"
                        type="button"
                        key={action}
                        onClick={() => setActivePage(action)}
                      >

                        <span
                          className="quick-action-icon"
                          aria-hidden="true"
                        >
                          {MENU_ICONS[action] ?? "•"}
                        </span>

                        <span>{action}</span>

                        <span
                          className="quick-action-arrow"
                          aria-hidden="true"
                        >
                          →
                        </span>

                      </button>

                    ))}

                  </div>

                </article>

                {/* ANNOUNCEMENTS */}
                <article className="content-card">

                  <div className="card-heading">

                    <div>
                      <h3>Latest announcements</h3>
                      <p>Important university updates</p>
                    </div>

                    <button
                      className="text-button"
                      type="button"
                      onClick={() => setActivePage("Announcements")}
                    >
                      View all →
                    </button>

                  </div>

                  <div className="announcement-row">

                    <span className="announcement-icon" aria-hidden="true">
                      📢
                    </span>

                    <div>
                      <strong>Data Science Club activity</strong>
                      <p>
                        Submit your one-page write-up through
                        Google Classroom.
                      </p>
                      <small>Club announcement</small>
                    </div>

                  </div>

                  <div className="announcement-row">

                    <span className="announcement-icon" aria-hidden="true">
                      📅
                    </span>

                    <div>
                      <strong>Examination updates</strong>
                      <p>
                        Check the latest examination schedule
                        from your department.
                      </p>
                      <small>Academic announcement</small>
                    </div>

                  </div>

                </article>

                {/* UPCOMING ACTIVITIES */}
                <article className="content-card">

                  <div className="card-heading">
                    <div>
                      <h3>Upcoming activities</h3>
                      <p>Keep track of important tasks</p>
                    </div>
                  </div>

                  <div className="activity-row">

                    <div className="date-tile">
                      <strong>12</strong>
                      <span>NOV</span>
                    </div>

                    <div>
                      <strong>DSC club activity</strong>
                      <p>One-page write-up</p>
                    </div>

                  </div>

                  <div className="activity-row">

                    <div className="date-tile">
                      <strong>15</strong>
                      <span>NOV</span>
                    </div>

                    <div>
                      <strong>Project review</strong>
                      <p>Academic project milestone</p>
                    </div>

                  </div>

                </article>

              </section>

            </>

          ) : (

            /* OTHER MODULES */
            <section className="content-card module-placeholder">

              <div className="placeholder-symbol">
                ✦
              </div>

              <h2>{activePage}</h2>

              <p>
                The {activePage.toLowerCase()} module
                will be developed in a later phase.
              </p>

              <p className="placeholder-role">
                Current preview role: {role}
              </p>

              <button
                className="primary-button"
                type="button"
                onClick={() => setActivePage("Dashboard")}
              >
                Back to dashboard
              </button>

            </section>

          )}

          <footer className="page-footer">
            © 2026 UniVerse University Portal.
            Student project demonstration.
          </footer>

        </div>
      </main>
    </div>
  );
}

export default App;
