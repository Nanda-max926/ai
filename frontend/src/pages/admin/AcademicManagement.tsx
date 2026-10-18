import { useEffect, useState, type FormEvent } from "react";
import "./AcademicManagement.css";

import {
  getAcademicPrograms,
  getAcademicCourses,
  getAcademicSections,
  createAcademicProgram,
  createAcademicCourse,
  createAcademicSection,
  type AcademicProgram,
  type AcademicCourse,
  type AcademicSection,
} from "../../services/api";

type Tab = "programs" | "courses" | "sections";

async function loadAcademicData() {
  return Promise.all([
    getAcademicPrograms(),
    getAcademicCourses(),
    getAcademicSections(),
  ]);
}

export default function AcademicManagement() {
  const [tab, setTab] = useState<Tab>("programs");

  const [programs, setPrograms] = useState<AcademicProgram[]>([]);
  const [courses, setCourses] = useState<AcademicCourse[]>([]);
  const [sections, setSections] = useState<AcademicSection[]>([]);

  const [loading, setLoading] = useState(true);
  const [saving, setSaving] = useState(false);
  const [error, setError] = useState("");
  const [success, setSuccess] = useState("");

  const [programForm, setProgramForm] = useState({
    departmentId: "",
    code: "",
    name: "",
    degreeLevel: "undergraduate",
    durationYears: 4,
    totalSemesters: 8,
    description: "",
  });

  const [courseForm, setCourseForm] = useState({
    departmentId: "",
    code: "",
    title: "",
    description: "",
    credits: 3,
    courseType: "core",
  });

  const [sectionForm, setSectionForm] = useState({
    programId: "",
    batchYear: new Date().getFullYear(),
    academicYearStart: new Date().getFullYear(),
    semesterNumber: 1,
    sectionCode: "A",
    capacity: 60,
  });

  useEffect(() => {
    let active = true;

    loadAcademicData()
      .then(([programResult, courseResult, sectionResult]) => {
        if (!active) return;

        setPrograms(programResult.programs);
        setCourses(courseResult.courses);
        setSections(sectionResult.sections);
      })
      .catch((err: unknown) => {
        if (!active) return;
        setError(
          err instanceof Error
            ? err.message
            : "Unable to load academic records."
        );
      })
      .finally(() => {
        if (active) setLoading(false);
      });

    return () => {
      active = false;
    };
  }, []);

  async function refreshLists() {
    const [programResult, courseResult, sectionResult] =
      await loadAcademicData();

    setPrograms(programResult.programs);
    setCourses(courseResult.courses);
    setSections(sectionResult.sections);
  }

  async function handleProgramSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setSuccess("");

    try {
      const result = await createAcademicProgram({
        departmentId: Number(programForm.departmentId),
        code: programForm.code.trim(),
        name: programForm.name.trim(),
        degreeLevel: programForm.degreeLevel,
        durationYears: Number(programForm.durationYears),
        totalSemesters: Number(programForm.totalSemesters),
        description: programForm.description.trim(),
      });

      await refreshLists();

      setSuccess(`${result.message} Program ID: ${result.id}`);
      setProgramForm({
        departmentId: "",
        code: "",
        name: "",
        degreeLevel: "undergraduate",
        durationYears: 4,
        totalSemesters: 8,
        description: "",
      });
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Program creation failed."
      );
    } finally {
      setSaving(false);
    }
  }

  async function handleCourseSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setSuccess("");

    try {
      const result = await createAcademicCourse({
        departmentId: Number(courseForm.departmentId),
        code: courseForm.code.trim(),
        title: courseForm.title.trim(),
        description: courseForm.description.trim(),
        credits: Number(courseForm.credits),
        courseType: courseForm.courseType,
      });

      await refreshLists();

      setSuccess(`${result.message} Course ID: ${result.id}`);
      setCourseForm({
        departmentId: "",
        code: "",
        title: "",
        description: "",
        credits: 3,
        courseType: "core",
      });
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Course creation failed."
      );
    } finally {
      setSaving(false);
    }
  }

  async function handleSectionSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setSuccess("");

    try {
      const result = await createAcademicSection({
        programId: Number(sectionForm.programId),
        batchYear: Number(sectionForm.batchYear),
        academicYearStart: Number(sectionForm.academicYearStart),
        semesterNumber: Number(sectionForm.semesterNumber),
        sectionCode: sectionForm.sectionCode.trim(),
        capacity: Number(sectionForm.capacity),
      });

      await refreshLists();

      setSuccess(`${result.message} Section ID: ${result.id}`);
      setSectionForm((previous) => ({
        ...previous,
        sectionCode: "A",
      }));
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Section creation failed."
      );
    } finally {
      setSaving(false);
    }
  }

  return (
    <div className="academic-page">
      <div className="academic-heading">
        <div>
          <span className="academic-eyebrow">ACADEMIC ADMINISTRATION</span>
          <h2>Academic Management</h2>
          <p>Manage degree programs, subjects and class sections.</p>
        </div>

        <button
          type="button"
          className="academic-refresh"
          disabled={loading || saving}
          onClick={() => {
            setLoading(true);
            setError("");
            loadAcademicData()
              .then(([p, c, s]) => {
                setPrograms(p.programs);
                setCourses(c.courses);
                setSections(s.sections);
              })
              .catch((err: unknown) =>
                setError(
                  err instanceof Error
                    ? err.message
                    : "Unable to refresh records."
                )
              )
              .finally(() => setLoading(false));
          }}
        >
          ↻ Refresh
        </button>
      </div>

      <div className="academic-stats">
        <div>
          <span>Programs</span>
          <strong>{programs.length}</strong>
        </div>
        <div>
          <span>Courses</span>
          <strong>{courses.length}</strong>
        </div>
        <div>
          <span>Sections</span>
          <strong>{sections.length}</strong>
        </div>
      </div>

      <div className="academic-tabs">
        <button
          type="button"
          className={tab === "programs" ? "selected" : ""}
          onClick={() => setTab("programs")}
        >
          Degree Programs
        </button>

        <button
          type="button"
          className={tab === "courses" ? "selected" : ""}
          onClick={() => setTab("courses")}
        >
          Courses / Subjects
        </button>

        <button
          type="button"
          className={tab === "sections" ? "selected" : ""}
          onClick={() => setTab("sections")}
        >
          Sections
        </button>
      </div>

      {error && <div className="academic-error">{error}</div>}
      {success && <div className="academic-success">{success}</div>}

      {loading ? (
        <div className="academic-panel">Loading academic records...</div>
      ) : (
        <>
          {tab === "programs" && (
            <section className="academic-panel">
              <h3>Create Degree Program</h3>

              <form
                className="academic-form"
                onSubmit={handleProgramSubmit}
              >
                <label>
                  Department ID
                  <input
                    type="number"
                    min="1"
                    required
                    value={programForm.departmentId}
                    onChange={(e) =>
                      setProgramForm({
                        ...programForm,
                        departmentId: e.target.value,
                      })
                    }
                    placeholder="Enter department ID"
                  />
                </label>

                <label>
                  Program Code
                  <input
                    required
                    maxLength={20}
                    value={programForm.code}
                    onChange={(e) =>
                      setProgramForm({
                        ...programForm,
                        code: e.target.value.toUpperCase(),
                      })
                    }
                    placeholder="e.g. BTECH-CSE"
                  />
                </label>

                <label>
                  Program Name
                  <input
                    required
                    maxLength={150}
                    value={programForm.name}
                    onChange={(e) =>
                      setProgramForm({
                        ...programForm,
                        name: e.target.value,
                      })
                    }
                    placeholder="e.g. B.Tech Computer Science"
                  />
                </label>

                <label>
                  Degree Level
                  <select
                    value={programForm.degreeLevel}
                    onChange={(e) =>
                      setProgramForm({
                        ...programForm,
                        degreeLevel: e.target.value,
                      })
                    }
                  >
                    <option value="diploma">Diploma</option>
                    <option value="undergraduate">Undergraduate</option>
                    <option value="postgraduate">Postgraduate</option>
                    <option value="doctoral">Doctoral</option>
                  </select>
                </label>

                <label>
                  Duration (years)
                  <input
                    type="number"
                    min="1"
                    max="8"
                    required
                    value={programForm.durationYears}
                    onChange={(e) =>
                      setProgramForm({
                        ...programForm,
                        durationYears: Number(e.target.value),
                      })
                    }
                  />
                </label>

                <label>
                  Total Semesters
                  <input
                    type="number"
                    min="1"
                    max="16"
                    required
                    value={programForm.totalSemesters}
                    onChange={(e) =>
                      setProgramForm({
                        ...programForm,
                        totalSemesters: Number(e.target.value),
                      })
                    }
                  />
                </label>

                <label className="academic-wide">
                  Description
                  <textarea
                    value={programForm.description}
                    onChange={(e) =>
                      setProgramForm({
                        ...programForm,
                        description: e.target.value,
                      })
                    }
                    placeholder="Program description"
                  />
                </label>

                <button disabled={saving} type="submit">
                  {saving ? "Creating..." : "Create Program"}
                </button>
              </form>

              <h3>Existing Programs</h3>

              <div className="academic-table-wrap">
                <table>
                  <thead>
                    <tr>
                      <th>Code</th>
                      <th>Name</th>
                      <th>Degree</th>
                      <th>Duration</th>
                      <th>Semesters</th>
                      <th>Status</th>
                    </tr>
                  </thead>
                  <tbody>
                    {programs.map((program) => (
                      <tr key={program.id}>
                        <td>{program.code}</td>
                        <td>{program.name}</td>
                        <td>{program.degreeLevel}</td>
                        <td>{program.durationYears} years</td>
                        <td>{program.totalSemesters}</td>
                        <td>{program.isActive ? "Active" : "Inactive"}</td>
                      </tr>
                    ))}
                    {programs.length === 0 && (
                      <tr>
                        <td colSpan={6}>No programs created yet.</td>
                      </tr>
                    )}
                  </tbody>
                </table>
              </div>
            </section>
          )}

          {tab === "courses" && (
            <section className="academic-panel">
              <h3>Create Course / Subject</h3>

              <form
                className="academic-form"
                onSubmit={handleCourseSubmit}
              >
                <label>
                  Department ID
                  <input
                    type="number"
                    min="1"
                    required
                    value={courseForm.departmentId}
                    onChange={(e) =>
                      setCourseForm({
                        ...courseForm,
                        departmentId: e.target.value,
                      })
                    }
                    placeholder="Enter department ID"
                  />
                </label>

                <label>
                  Course Code
                  <input
                    required
                    maxLength={30}
                    value={courseForm.code}
                    onChange={(e) =>
                      setCourseForm({
                        ...courseForm,
                        code: e.target.value.toUpperCase(),
                      })
                    }
                    placeholder="e.g. CS301"
                  />
                </label>

                <label>
                  Course Title
                  <input
                    required
                    maxLength={150}
                    value={courseForm.title}
                    onChange={(e) =>
                      setCourseForm({
                        ...courseForm,
                        title: e.target.value,
                      })
                    }
                    placeholder="e.g. Data Structures"
                  />
                </label>

                <label>
                  Credits
                  <input
                    type="number"
                    min="0.5"
                    max="20"
                    step="0.5"
                    required
                    value={courseForm.credits}
                    onChange={(e) =>
                      setCourseForm({
                        ...courseForm,
                        credits: Number(e.target.value),
                      })
                    }
                  />
                </label>

                <label>
                  Course Type
                  <select
                    value={courseForm.courseType}
                    onChange={(e) =>
                      setCourseForm({
                        ...courseForm,
                        courseType: e.target.value,
                      })
                    }
                  >
                    <option value="core">Core</option>
                    <option value="elective">Elective</option>
                    <option value="laboratory">Laboratory</option>
                    <option value="project">Project</option>
                    <option value="other">Other</option>
                  </select>
                </label>

                <label className="academic-wide">
                  Description
                  <textarea
                    value={courseForm.description}
                    onChange={(e) =>
                      setCourseForm({
                        ...courseForm,
                        description: e.target.value,
                      })
                    }
                    placeholder="Course description"
                  />
                </label>

                <button disabled={saving} type="submit">
                  {saving ? "Creating..." : "Create Course"}
                </button>
              </form>

              <h3>Existing Courses</h3>

              <div className="academic-table-wrap">
                <table>
                  <thead>
                    <tr>
                      <th>Code</th>
                      <th>Course</th>
                      <th>Credits</th>
                      <th>Type</th>
                      <th>Status</th>
                    </tr>
                  </thead>
                  <tbody>
                    {courses.map((course) => (
                      <tr key={course.id}>
                        <td>{course.code}</td>
                        <td>{course.title}</td>
                        <td>{course.credits}</td>
                        <td>{course.courseType}</td>
                        <td>{course.isActive ? "Active" : "Inactive"}</td>
                      </tr>
                    ))}
                    {courses.length === 0 && (
                      <tr>
                        <td colSpan={5}>No courses created yet.</td>
                      </tr>
                    )}
                  </tbody>
                </table>
              </div>
            </section>
          )}

          {tab === "sections" && (
            <section className="academic-panel">
              <h3>Create Class Section</h3>

              <form
                className="academic-form"
                onSubmit={handleSectionSubmit}
              >
                <label>
                  Academic Program
                  <select
                    required
                    value={sectionForm.programId}
                    onChange={(e) =>
                      setSectionForm({
                        ...sectionForm,
                        programId: e.target.value,
                      })
                    }
                  >
                    <option value="">Select program</option>
                    {programs.map((program) => (
                      <option key={program.id} value={program.id}>
                        {program.code} — {program.name}
                      </option>
                    ))}
                  </select>
                </label>

                <label>
                  Admission Batch
                  <input
                    type="number"
                    min="2000"
                    max="2100"
                    required
                    value={sectionForm.batchYear}
                    onChange={(e) =>
                      setSectionForm({
                        ...sectionForm,
                        batchYear: Number(e.target.value),
                      })
                    }
                  />
                </label>

                <label>
                  Academic Year Start
                  <input
                    type="number"
                    min="2000"
                    max="2100"
                    required
                    value={sectionForm.academicYearStart}
                    onChange={(e) =>
                      setSectionForm({
                        ...sectionForm,
                        academicYearStart: Number(e.target.value),
                      })
                    }
                  />
                </label>

                <label>
                  Semester
                  <input
                    type="number"
                    min="1"
                    max="16"
                    required
                    value={sectionForm.semesterNumber}
                    onChange={(e) =>
                      setSectionForm({
                        ...sectionForm,
                        semesterNumber: Number(e.target.value),
                      })
                    }
                  />
                </label>

                <label>
                  Section
                  <input
                    required
                    maxLength={10}
                    value={sectionForm.sectionCode}
                    onChange={(e) =>
                      setSectionForm({
                        ...sectionForm,
                        sectionCode: e.target.value.toUpperCase(),
                      })
                    }
                  />
                </label>

                <label>
                  Student Capacity
                  <input
                    type="number"
                    min="1"
                    max="500"
                    required
                    value={sectionForm.capacity}
                    onChange={(e) =>
                      setSectionForm({
                        ...sectionForm,
                        capacity: Number(e.target.value),
                      })
                    }
                  />
                </label>

                <button
                  disabled={saving || programs.length === 0}
                  type="submit"
                >
                  {saving ? "Creating..." : "Create Section"}
                </button>

                {programs.length === 0 && (
                  <p className="academic-hint">
                    Create a degree program before creating a section.
                  </p>
                )}
              </form>

              <h3>Existing Sections</h3>

              <div className="academic-table-wrap">
                <table>
                  <thead>
                    <tr>
                      <th>Program</th>
                      <th>Batch</th>
                      <th>Academic Year</th>
                      <th>Semester</th>
                      <th>Section</th>
                      <th>Capacity</th>
                    </tr>
                  </thead>
                  <tbody>
                    {sections.map((section) => {
                      const program = programs.find(
                        (item) => item.id === section.programId
                      );

                      return (
                        <tr key={section.id}>
                          <td>{program?.code || section.programId}</td>
                          <td>{section.batchYear}</td>
                          <td>{section.academicYearStart}</td>
                          <td>{section.semesterNumber}</td>
                          <td>{section.sectionCode}</td>
                          <td>{section.capacity}</td>
                        </tr>
                      );
                    })}
                    {sections.length === 0 && (
                      <tr>
                        <td colSpan={6}>No sections created yet.</td>
                      </tr>
                    )}
                  </tbody>
                </table>
              </div>
            </section>
          )}
        </>
      )}
    </div>
  );
}