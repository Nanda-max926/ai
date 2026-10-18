import { useEffect, useState } from "react";
import "./AttendanceManagement.css";

type Status = "present" | "absent" | "late" | "excused" | "";

interface Student {
  id: number;
  loginId: string;
  email: string;
}

interface RecordItem {
  studentUserId: number;
  status: Status;
  remarks: string;
}

async function request<T>(url: string, options: RequestInit = {}): Promise<T> {
  const headers = new Headers(options.headers);
  if (options.body && !headers.has("Content-Type")) {
    headers.set("Content-Type", "application/json");
  }

  const response = await fetch(url, {
    ...options,
    credentials: "include",
    headers,
  });

  const body = await response.json();

  if (!response.ok) {
    throw new Error(body.message || `Request failed: ${response.status}`);
  }

  return body as T;
}

function localDate() {
  const date = new Date();
  const month = String(date.getMonth() + 1).padStart(2, "0");
  const day = String(date.getDate()).padStart(2, "0");
  return `${date.getFullYear()}-${month}-${day}`;
}

export default function AttendanceManagement() {
  const [students, setStudents] = useState<Student[]>([]);
  const [statuses, setStatuses] = useState<Record<number, Status>>({});
  const [remarks, setRemarks] = useState<Record<number, string>>({});
  const [date, setDate] = useState(localDate());
  const [courseCode, setCourseCode] = useState("");
  const [loading, setLoading] = useState(true);
  const [savingId, setSavingId] = useState<number | null>(null);
  const [error, setError] = useState("");
  const [success, setSuccess] = useState("");

  async function loadStudents() {
    const result = await request<{ students: Student[] }>(
      "/api/v1/attendance/students"
    );
    setStudents(result.students);
  }

  async function loadRecords() {
    const params = new URLSearchParams({
      date,
      courseCode: courseCode.trim(),
    });

    const result = await request<{ records: RecordItem[] }>(
      `/api/v1/attendance/records?${params.toString()}`
    );

    const nextStatuses: Record<number, Status> = {};
    const nextRemarks: Record<number, string> = {};

    result.records.forEach((record) => {
      nextStatuses[record.studentUserId] = record.status;
      nextRemarks[record.studentUserId] = record.remarks;
    });

    setStatuses(nextStatuses);
    setRemarks(nextRemarks);
  }

  async function refresh() {
    setError("");
    await Promise.all([loadStudents(), loadRecords()]);
  }

  useEffect(() => {
    refresh()
      .catch((err: unknown) => {
        setError(err instanceof Error ? err.message : "Unable to load attendance.");
      })
      .finally(() => setLoading(false));
  }, []);

  async function saveAttendance(student: Student) {
    const status = statuses[student.id];

    if (!status) {
      setError(`Select an attendance status for ${student.loginId}.`);
      return;
    }

    setSavingId(student.id);
    setError("");
    setSuccess("");

    try {
      const result = await request<{ message: string }>(
        "/api/v1/attendance/records",
        {
          method: "POST",
          body: JSON.stringify({
            studentUserId: student.id,
            courseCode: courseCode.trim(),
            attendanceDate: date,
            status,
            remarks: remarks[student.id] || "",
          }),
        }
      );

      setSuccess(`${student.loginId}: ${result.message}`);
    } catch (err) {
      setError(err instanceof Error ? err.message : "Unable to save attendance.");
    } finally {
      setSavingId(null);
    }
  }

  const marked = students.filter((student) => statuses[student.id]).length;
  const present = students.filter(
    (student) => statuses[student.id] === "present"
  ).length;
  const absent = students.filter(
    (student) => statuses[student.id] === "absent"
  ).length;

  if (loading) {
    return <div className="attendance-panel">Loading attendance...</div>;
  }

  return (
    <div className="attendance-page">
      <header className="attendance-heading">
        <div>
          <span>ACADEMIC OPERATIONS</span>
          <h2>Attendance Management</h2>
          <p>Record and review student attendance by course and date.</p>
        </div>

        <button type="button" onClick={() => {
          setLoading(true);
          refresh()
            .catch((err: unknown) =>
              setError(err instanceof Error ? err.message : "Refresh failed.")
            )
            .finally(() => setLoading(false));
        }}>
          ↻ Refresh
        </button>
      </header>

      <section className="attendance-stats">
        <div><span>Total Students</span><strong>{students.length}</strong></div>
        <div><span>Marked</span><strong>{marked}</strong></div>
        <div><span>Present</span><strong>{present}</strong></div>
        <div><span>Absent</span><strong>{absent}</strong></div>
      </section>

      {error && <div className="attendance-error">{error}</div>}
      {success && <div className="attendance-success">{success}</div>}

      <section className="attendance-panel">
        <div className="attendance-filters">
          <label>
            Attendance Date
            <input
              type="date"
              value={date}
              onChange={(event) => setDate(event.target.value)}
            />
          </label>

          <label>
            Course Code
            <input
              required
              maxLength={30}
              placeholder="e.g. CS101"
              value={courseCode}
              onChange={(event) => setCourseCode(event.target.value)}
            />
          </label>

          <button
            type="button"
            disabled={!courseCode.trim()}
            onClick={() => {
              setLoading(true);
              loadRecords()
                .catch((err: unknown) =>
                  setError(err instanceof Error ? err.message : "Unable to load records.")
                )
                .finally(() => setLoading(false));
            }}
          >
            Load Records
          </button>
        </div>
      </section>

      <section className="attendance-panel">
        <h3>Student Attendance Register</h3>

        {!students.length ? (
          <p>No active student accounts found.</p>
        ) : (
          <div className="attendance-table-wrap">
            <table className="attendance-table">
              <thead>
                <tr>
                  <th>Login ID</th>
                  <th>Email</th>
                  <th>Status</th>
                  <th>Remarks</th>
                  <th>Action</th>
                </tr>
              </thead>

              <tbody>
                {students.map((student) => (
                  <tr key={student.id}>
                    <td>{student.loginId}</td>
                    <td>{student.email}</td>
                    <td>
                      <select
                        value={statuses[student.id] || ""}
                        onChange={(event) =>
                          setStatuses({
                            ...statuses,
                            [student.id]: event.target.value as Status,
                          })
                        }
                      >
                        <option value="">Not marked</option>
                        <option value="present">Present</option>
                        <option value="absent">Absent</option>
                        <option value="late">Late</option>
                        <option value="excused">Excused</option>
                      </select>
                    </td>
                    <td>
                      <input
                        value={remarks[student.id] || ""}
                        placeholder="Optional"
                        onChange={(event) =>
                          setRemarks({
                            ...remarks,
                            [student.id]: event.target.value,
                          })
                        }
                      />
                    </td>
                    <td>
                      <button
                        type="button"
                        disabled={savingId === student.id || !courseCode.trim()}
                        onClick={() => saveAttendance(student)}
                      >
                        {savingId === student.id ? "Saving..." : "Save"}
                      </button>
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        )}
      </section>
    </div>
  );
}