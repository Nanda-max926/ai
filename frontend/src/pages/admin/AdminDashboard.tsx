
import { useEffect, useState, type FormEvent } from "react";
import DepartmentManagement from "./DepartmentManagement";
import AcademicManagement from "./AcademicManagement";
import TimetableManagement from "./TimetableManagement";
import AttendanceManagement from "./AttendanceManagement";
import ExaminationsManagement from "./ExaminationsManagement";
import {
  createStudent,
  createFaculty,
  getDepartments,
  type AuthUser,
  type Department,
  type CreatedAccount,
} from "../../services/api";

interface Props {
  user: AuthUser;
  onLogout: () => void;
}

type Page =
  | "overview"
  | "students"
  | "faculty"
  | "departments"
  | "academic"
  | "timetable"
  | "attendance"
  | "examinations";


function AccountResult({
  createdAccount,
}: {
     createdAccount: CreatedAccount | null;
}) {
  if (!createdAccount) return null;

  return (
    <div className="account-result">
      <strong>Account created successfully!</strong>

      <p>Generated Login ID</p>
      <h2>{createdAccount.loginId}</h2>

      <p>Account type: {createdAccount.role}</p>
      <p>Status: {createdAccount.status}</p>

      <div className="result-warning">
        <strong>Temporary Password</strong>
        <h2>{createdAccount.temporaryPassword}</h2>

        <p>
          Expires in {createdAccount.temporaryPasswordExpiresInHours} hours.
        </p>

        <p>Copy this password now. It will not be displayed again after leaving this result.</p>

        <button
          type="button"
          onClick={() =>
            navigator.clipboard.writeText(
              createdAccount.temporaryPassword
            )
          }
        >
          Copy Temporary Password
        </button>
      </div>
    </div>
  );
}

export default function AdminDashboard({ user, onLogout }: Props) {

  const [page, setPage] = useState<Page>("overview");

  const [departments, setDepartments] = useState<Department[]>([]);
  const [departmentError, setDepartmentError] = useState("");
  const [departmentLoading, setDepartmentLoading] = useState(true);

  const [message, setMessage] = useState("");
  const [createdAccount, setCreatedAccount] =
    useState<CreatedAccount | null>(null);

  const [loading, setLoading] = useState(false);

  useEffect(() => {
    let active = true;

    getDepartments()
      .then((data) => {
        if (active) {
          setDepartments(data);
          setDepartmentError("");
        }
      })
      .catch((error: unknown) => {
        if (active) {
          setDepartmentError(
            error instanceof Error
              ? error.message
              : "Unable to load departments."
          );
        }
      })
      .finally(() => {
        if (active) {
          setDepartmentLoading(false);
        }
      });

    return () => {
      active = false;
    };
  }, []);

  function changePage(nextPage: Page) {
    setPage(nextPage);
    setMessage("");
    setCreatedAccount(null);
  }

  async function handleStudentSubmit(
    event: FormEvent<HTMLFormElement>
  ) {
    event.preventDefault();

    const form = event.currentTarget;
    const data = new FormData(form);

    setLoading(true);
    setMessage("");
    setCreatedAccount(null);

    try {
      const account = await createStudent({
        fullName: String(data.get("fullName") ?? ""),
        email: String(data.get("email") ?? ""),
        temporaryPassword: String(
          data.get("temporaryPassword") ?? ""
        ),
        departmentId: Number(data.get("departmentId")),
        semester: Number(data.get("semester")),
        batchYear: Number(data.get("batchYear")),
        section: String(data.get("section") ?? "A"),
      });

      setCreatedAccount(account);
      setMessage("Student account created successfully.");

      form.reset();

    } catch (error) {
      setMessage(
        error instanceof Error
          ? error.message
          : "Student creation failed."
      );
    } finally {
      setLoading(false);
    }
  }

  async function handleFacultySubmit(
    event: FormEvent<HTMLFormElement>
  ) {
    event.preventDefault();

    const form = event.currentTarget;
    const data = new FormData(form);

    setLoading(true);
    setMessage("");
    setCreatedAccount(null);

    try {
      const account = await createFaculty({
        fullName: String(data.get("fullName") ?? ""),
        email: String(data.get("email") ?? ""),
        temporaryPassword: String(
          data.get("temporaryPassword") ?? ""
        ),
        departmentId: Number(data.get("departmentId")),
        designation: String(data.get("designation") ?? ""),
      });

      setCreatedAccount(account);
      setMessage("Faculty account created successfully.");

      form.reset();

    } catch (error) {
      setMessage(
        error instanceof Error
          ? error.message
          : "Faculty creation failed."
      );
    } finally {
      setLoading(false);
    }
  }

 

  const pageTitles: Record<Page, string> = {
    overview: "Administration Overview",
    students: "Create Student Account",
    faculty: "Create Faculty Account",
    departments: "Department Management",
    academic: "Academic Management",
    timetable: "Timetable Management",
    attendance: "Attendance Management",
    examinations: "Examinations & Results",
  };

  return (
                                          
<div className="admin-shell">
  <aside className="admin-sidebar">
    <div className="admin-brand">
      <div className="brand-mark">U</div>
      <div>
        <strong>UniVerse</strong>
        <small>ADMIN CONSOLE</small>
      </div>
    </div>

    <p className="admin-nav-label">ADMINISTRATION</p>

    <button
      type="button"
      className={`admin-nav ${page === "overview" ? "active" : ""}`}
      onClick={() => setPage("overview")}
    >
      <span>⌂</span>
      <span>Overview</span>
    </button>

    <button
      type="button"
      className={`admin-nav ${page === "departments" ? "active" : ""}`}
      onClick={() => setPage("departments")}
    >
      <span>🏢</span>
      <span>Manage Departments</span>
    </button>

    <button
      type="button"
      className={`admin-nav ${page === "academic" ? "active" : ""}`}
      onClick={() => setPage("academic")}
    >
      <span>🎓</span>
      <span>Academic Management</span>
    </button>

    <button
      type="button"
      className={`admin-nav ${page === "timetable" ? "active" : ""}`}
      onClick={() => setPage("timetable")}
    >
      <span>📅</span>
      <span>Timetable Management</span>
    </button>

    <button
      type="button"
      className={`admin-nav ${page === "attendance" ? "active" : ""}`}
      onClick={() => setPage("attendance")}
    >
      <span>📋</span>
      <span>Attendance Management</span>
    </button>

    <button
      type="button"
      className={`admin-nav ${page === "examinations" ? "active" : ""}`}
      onClick={() => setPage("examinations")}
    >
      <span>📝</span>
      <span>Examinations & Results</span>
    </button>

    <button
      type="button"
      className={`admin-nav ${page === "students" ? "active" : ""}`}
      onClick={() => setPage("students")}
    >
      <span>👨‍🎓</span>
      <span>Create Student</span>
    </button>

    <button
      type="button"
      className={`admin-nav ${page === "faculty" ? "active" : ""}`}
      onClick={() => setPage("faculty")}
    >
      <span>👨‍🏫</span>
      <span>Create Faculty</span>
    </button>

    <div className="admin-sidebar-bottom">
      <div className="admin-profile">
        <div className="avatar">AD</div>
        <div>
          <strong>{user.loginId}</strong>
          <small>Administrator</small>
        </div>
      </div>
    </div>   
</aside>

<main className="admin-main">

        <header className="admin-topbar">
          <div>
            <span>UNIVERSE / ADMIN</span>
            <h1>{pageTitles[page]}</h1>
          </div>

          <button
            className="logout-button"
            onClick={onLogout}
          >
            Sign out ↗
          </button>
        </header>

        <div className="admin-content">

          {page === "overview" && (

            <>
              <section className="admin-welcome">
                <div>
                  <span>SUPER ADMINISTRATION</span>

                  <h2>Welcome, {user.loginId}!</h2>

                  <p>
                    Manage students, faculty and university departments
                    from one central workspace.
                  </p>
                </div>

                <div className="welcome-icon">🏛️</div>
              </section>

              <section className="admin-cards">

                <button
                  className="admin-feature-card"
                  onClick={() => changePage("students")}
                >
                  <span className="feature-icon blue">♙</span>
                  <h3>Student Management</h3>
                  <p>Create student accounts and generate registration IDs.</p>
                  <strong>Open module →</strong>
                </button>

                <button
                  className="admin-feature-card"
                  onClick={() => changePage("faculty")}
                >
                  <span className="feature-icon green">♟</span>
                  <h3>Faculty Management</h3>
                  <p>Create faculty accounts and assign departments.</p>
                  <strong>Open module →</strong>
                </button>

                <div className="admin-feature-card">
                  <span className="feature-icon purple">▦</span>
                  <h3>University Departments</h3>

                  <p>
                    {departmentLoading
                      ? "Loading departments..."
                      : `${departments.length} departments loaded from PostgreSQL.`}
                  </p>
                </div>

              </section>

              {departmentError && (
                <div className="form-error">
                  Department loading error: {departmentError}
                </div>
              )}
            </>

          )}

          {page === "students" && (

            <section className="admin-panel">

              <div className="panel-title">
                <h2>Create Student Account</h2>
                <p>
                  Registration ID will be generated automatically.
                </p>
              </div>

              {message && (
                <div className={
                  createdAccount ? "form-success" : "form-error"
                }>
                  {message}
                </div>
              )}

              <form
                className="account-form"
                onSubmit={handleStudentSubmit}
              >

                <label>
                  Full Name
                  <input name="fullName" required maxLength={150} />
                </label>

                <label>
                  Student Email
                  <input
                    name="email"
                    type="email"
                    required
                    maxLength={254}
                  />
                </label>

                <label>
                  Department
                  <select
                    name="departmentId"
                    required
                    defaultValue=""
                    disabled={departmentLoading || departments.length === 0}
                  >
                    <option value="" disabled>
                      Select department
                    </option>

                    {departments.map((department) => (
                      <option
                        key={department.id}
                        value={department.id}
                      >
                        {department.code} - {department.name}
                      </option>
                    ))}
                  </select>
                </label>

                <label>
                  Semester
                  <select name="semester" defaultValue="1">
                    {[1, 2, 3, 4, 5, 6, 7, 8].map((number) => (
                      <option key={number} value={number}>
                        Semester {number}
                      </option>
                    ))}
                  </select>
                </label>

                <label>
                  Batch Year
                  <input
                    name="batchYear"
                    type="number"
                    min="2000"
                    max="2100"
                    defaultValue={2026}
                    required
                  />
                </label>

                <label>
                  Section
                  <input
                    name="section"
                    maxLength={20}
                    defaultValue="A"
                    required
                  />
                </label>

                <label className="full-field">
                  Temporary Password
                  <input
                    name="temporaryPassword"
                    type="password"
                    minLength={12}
                    maxLength={1024}
                    autoComplete="new-password"
                    required
                  />
                  <small>
                    Minimum 12 characters. Give it to the student securely.
                  </small>
                </label>

                <button
                  className="primary-button full-field"
                  type="submit"
                  disabled={
                    loading ||
                    departmentLoading ||
                    departments.length === 0
                  }
                >
                  {loading
                    ? "Creating account..."
                    : "Create Student Account"}
                </button>

              </form>

              <AccountResult createdAccount={createdAccount} />

            </section>

          )}

          {page === "faculty" && (

            <section className="admin-panel">

              <div className="panel-title">
                <h2>Create Faculty Account</h2>
                <p>
                  Faculty employee ID will be generated automatically.
                </p>
              </div>

              {message && (
                <div className={
                  createdAccount ? "form-success" : "form-error"
                }>
                  {message}
                </div>
              )}

              <form
                className="account-form"
                onSubmit={handleFacultySubmit}
              >

                <label>
                  Faculty Full Name
                  <input name="fullName" required maxLength={150} />
                </label>

                <label>
                  Official Email
                  <input
                    name="email"
                    type="email"
                    required
                    maxLength={254}
                  />
                </label>

                <label>
                  Department
                  <select
                    name="departmentId"
                    required
                    defaultValue=""
                    disabled={departmentLoading || departments.length === 0}
                  >
                    <option value="" disabled>
                      Select department
                    </option>

                    {departments.map((department) => (
                      <option
                        key={department.id}
                        value={department.id}
                      >
                        {department.code} - {department.name}
                      </option>
                    ))}
                  </select>
                </label>

                <label>
                  Designation
                  <select
                    name="designation"
                    required
                    defaultValue=""
                  >
                    <option value="" disabled>
                      Select designation
                    </option>

                    <option>Assistant Professor</option>
                    <option>Associate Professor</option>
                    <option>Professor</option>
                    <option>Lecturer</option>
                    <option>Department Coordinator</option>
                  </select>
                </label>

                <label className="full-field">
                  Temporary Password
                  <input
                    name="temporaryPassword"
                    type="password"
                    minLength={12}
                    maxLength={1024}
                    autoComplete="new-password"
                    required
                  />

                  <small>
                    Minimum 12 characters.
                  </small>
                </label>

                <button
                  className="primary-button full-field"
                  type="submit"
                  disabled={
                    loading ||
                    departmentLoading ||
                    departments.length === 0
                  }
                >
                  {loading
                    ? "Creating account..."
                    : "Create Faculty Account"}
                </button>

              </form>

             <AccountResult createdAccount={createdAccount} />

            </section>

          )}

          {page === "departments" && <DepartmentManagement />}
          {page === "academic" && <AcademicManagement />}
          {page === "timetable" && <TimetableManagement />}
          {page === "attendance" && <AttendanceManagement />}
          {page === "examinations" && <ExaminationsManagement />}

              <footer className="admin-footer">
                      UniVerse University Management System · Admin Console
                       </footer>

        </div>
      </main>
    </div>
  );
}
