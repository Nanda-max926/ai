import { useEffect, useState, type FormEvent } from "react";
import "./ExaminationsManagement.css";

interface Exam {
  id: number;
  courseCode: string;
  title: string;
  examType: string;
  examDate: string;
  totalMarks: number;
  passingMarks: number;
}

interface Student {
  id: number;
  loginId: string;
  email: string;
}

interface ExamResult {
  id: number;
  studentUserId: number;
  loginId: string;
  marksObtained: number;
  totalMarks: number;
  grade: string;
  remarks: string;
}

async function api<T>(
  url: string,
  options: RequestInit = {}
): Promise<T> {
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
    throw new Error(body.message || `Request failed (${response.status})`);
  }

  return body as T;
}

export default function ExaminationsManagement() {
  const [exams, setExams] = useState<Exam[]>([]);
  const [students, setStudents] = useState<Student[]>([]);
  const [results, setResults] = useState<ExamResult[]>([]);
  const [selectedExamId, setSelectedExamId] = useState("");
  const [loading, setLoading] = useState(true);
  const [saving, setSaving] = useState(false);
  const [error, setError] = useState("");
  const [success, setSuccess] = useState("");

  const [examForm, setExamForm] = useState({
    courseCode: "",
    title: "",
    examType: "internal",
    examDate: new Date().toLocaleDateString("en-CA"),
    totalMarks: "100",
    passingMarks: "40",
  });

  const [resultForm, setResultForm] = useState({
    studentUserId: "",
    marksObtained: "",
    grade: "",
    remarks: "",
  });

  async function refreshData() {
    const [examData, studentData] = await Promise.all([
      api<{ exams: Exam[] }>("/api/v1/admin/exams"),
      api<{ students: Student[] }>("/api/v1/attendance/students"),
    ]);

    setExams(examData.exams);
    setStudents(studentData.students);

    if (!selectedExamId && examData.exams.length > 0) {
      setSelectedExamId(String(examData.exams[0].id));
    }
  }

  async function loadResults(examId: string) {
    if (!examId) {
      setResults([]);
      return;
    }

    const data = await api<{ results: ExamResult[] }>(
      `/api/v1/admin/exam-results?examId=${encodeURIComponent(examId)}`
    );

    setResults(data.results);
  }

  useEffect(() => {
    refreshData()
      .catch((err: unknown) => {
        setError(err instanceof Error ? err.message : "Unable to load records.");
      })
      .finally(() => setLoading(false));
  }, []);

  useEffect(() => {
    if (!selectedExamId) {
      setResults([]);
      return;
    }

    loadResults(selectedExamId).catch((err: unknown) => {
      setError(err instanceof Error ? err.message : "Unable to load results.");
    });
  }, [selectedExamId]);

  async function createExam(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setSuccess("");

    try {
      const result = await api<{ id: number; message: string }>(
        "/api/v1/admin/exams",
        {
          method: "POST",
          body: JSON.stringify({
            ...examForm,
            totalMarks: Number(examForm.totalMarks),
            passingMarks: Number(examForm.passingMarks),
          }),
        }
      );

      await refreshData();
      setSelectedExamId(String(result.id));
      setSuccess(result.message);

      setExamForm({
        courseCode: "",
        title: "",
        examType: "internal",
        examDate: new Date().toLocaleDateString("en-CA"),
        totalMarks: "100",
        passingMarks: "40",
      });
    } catch (err) {
      setError(err instanceof Error ? err.message : "Exam creation failed.");
    } finally {
      setSaving(false);
    }
  }

  async function saveResult(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setSuccess("");

    try {
      const result = await api<{ message: string }>(
        "/api/v1/admin/exam-results",
        {
          method: "POST",
          body: JSON.stringify({
            examId: Number(selectedExamId),
            studentUserId: Number(resultForm.studentUserId),
            marksObtained: Number(resultForm.marksObtained),
            grade: resultForm.grade,
            remarks: resultForm.remarks,
          }),
        }
      );

      await loadResults(selectedExamId);
      setSuccess(result.message);

      setResultForm({
        studentUserId: "",
        marksObtained: "",
        grade: "",
        remarks: "",
      });
    } catch (err) {
      setError(err instanceof Error ? err.message : "Saving result failed.");
    } finally {
      setSaving(false);
    }
  }

  if (loading) {
    return <div className="exam-panel">Loading examinations...</div>;
  }

  const selectedExam = exams.find(
    (exam) => String(exam.id) === selectedExamId
  );

  return (
    <div className="exam-page">
      <header className="exam-heading">
        <div>
          <span>ACADEMIC ASSESSMENT</span>
          <h2>Examinations & Results</h2>
          <p>Schedule examinations, record student marks and review results.</p>
        </div>

        <button
          type="button"
          onClick={() => {
            setLoading(true);
            refreshData()
              .catch((err: unknown) =>
                setError(err instanceof Error ? err.message : "Refresh failed.")
              )
              .finally(() => setLoading(false));
          }}
        >
          ↻ Refresh
        </button>
      </header>

      <div className="exam-stats">
        <div><span>Total Examinations</span><strong>{exams.length}</strong></div>
        <div><span>Active Students</span><strong>{students.length}</strong></div>
        <div><span>Results Entered</span><strong>{results.length}</strong></div>
      </div>

      {error && <div className="exam-error">{error}</div>}
      {success && <div className="exam-success">{success}</div>}

      <section className="exam-panel">
        <h3>Create Examination</h3>

        <form className="exam-form" onSubmit={createExam}>
          <label>
            Course Code
            <input
              required
              maxLength={30}
              placeholder="e.g. CS101"
              value={examForm.courseCode}
              onChange={(e) =>
                setExamForm({ ...examForm, courseCode: e.target.value })
              }
            />
          </label>

          <label>
            Examination Title
            <input
              required
              maxLength={180}
              placeholder="e.g. Internal Assessment 1"
              value={examForm.title}
              onChange={(e) =>
                setExamForm({ ...examForm, title: e.target.value })
              }
            />
          </label>

          <label>
            Examination Type
            <select
              value={examForm.examType}
              onChange={(e) =>
                setExamForm({ ...examForm, examType: e.target.value })
              }
            >
              <option value="quiz">Quiz</option>
              <option value="internal">Internal</option>
              <option value="midterm">Midterm</option>
              <option value="practical">Practical</option>
              <option value="assignment">Assignment</option>
              <option value="final">Final Examination</option>
            </select>
          </label>

          <label>
            Examination Date
            <input
              required
              type="date"
              value={examForm.examDate}
              onChange={(e) =>
                setExamForm({ ...examForm, examDate: e.target.value })
              }
            />
          </label>

          <label>
            Maximum Marks
            <input
              required
              type="number"
              min="1"
              step="0.5"
              value={examForm.totalMarks}
              onChange={(e) =>
                setExamForm({ ...examForm, totalMarks: e.target.value })
              }
            />
          </label>

          <label>
            Passing Marks
            <input
              required
              type="number"
              min="0"
              step="0.5"
              value={examForm.passingMarks}
              onChange={(e) =>
                setExamForm({ ...examForm, passingMarks: e.target.value })
              }
            />
          </label>

          <button type="submit" disabled={saving}>
            {saving ? "Saving..." : "Create Examination"}
          </button>
        </form>
      </section>

      <section className="exam-panel">
        <h3>Enter Student Result</h3>

        {!exams.length ? (
          <p>Create an examination before entering results.</p>
        ) : (
          <form className="exam-form" onSubmit={saveResult}>
            <label className="exam-wide">
              Select Examination
              <select
                required
                value={selectedExamId}
                onChange={(e) => setSelectedExamId(e.target.value)}
              >
                <option value="">Select examination</option>
                {exams.map((exam) => (
                  <option key={exam.id} value={exam.id}>
                    {exam.courseCode} — {exam.title} ({exam.examDate})
                  </option>
                ))}
              </select>
            </label>

            {selectedExam && (
              <p className="exam-hint exam-wide">
                Maximum marks: {selectedExam.totalMarks} · Passing marks:{" "}
                {selectedExam.passingMarks}
              </p>
            )}

            <label>
              Student
              <select
                required
                value={resultForm.studentUserId}
                onChange={(e) =>
                  setResultForm({
                    ...resultForm,
                    studentUserId: e.target.value,
                  })
                }
              >
                <option value="">Select student</option>
                {students.map((student) => (
                  <option key={student.id} value={student.id}>
                    {student.loginId} — {student.email}
                  </option>
                ))}
              </select>
            </label>

            <label>
              Marks Obtained
              <input
                required
                type="number"
                min="0"
                step="0.5"
                max={selectedExam?.totalMarks}
                value={resultForm.marksObtained}
                onChange={(e) =>
                  setResultForm({
                    ...resultForm,
                    marksObtained: e.target.value,
                  })
                }
              />
            </label>

            <label>
              Grade
              <input
                maxLength={10}
                placeholder="e.g. A+"
                value={resultForm.grade}
                onChange={(e) =>
                  setResultForm({ ...resultForm, grade: e.target.value })
                }
              />
            </label>

            <label>
              Remarks
              <input
                maxLength={2000}
                placeholder="Optional remarks"
                value={resultForm.remarks}
                onChange={(e) =>
                  setResultForm({ ...resultForm, remarks: e.target.value })
                }
              />
            </label>

            <button
              type="submit"
              disabled={saving || !selectedExamId || !students.length}
            >
              {saving ? "Saving..." : "Save Student Result"}
            </button>
          </form>
        )}
      </section>

      <section className="exam-panel">
        <h3>Examination Results</h3>

        {!selectedExamId ? (
          <p>Select an examination to view results.</p>
        ) : !results.length ? (
          <p>No results have been entered for this examination yet.</p>
        ) : (
          <div className="exam-table-wrap">
            <table className="exam-table">
              <thead>
                <tr>
                  <th>Student Login ID</th>
                  <th>Marks</th>
                  <th>Grade</th>
                  <th>Remarks</th>
                </tr>
              </thead>

              <tbody>
                {results.map((result) => (
                  <tr key={result.id}>
                    <td>{result.loginId}</td>
                    <td>
                      {result.marksObtained} / {result.totalMarks}
                    </td>
                    <td>{result.grade || "—"}</td>
                    <td>{result.remarks || "—"}</td>
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