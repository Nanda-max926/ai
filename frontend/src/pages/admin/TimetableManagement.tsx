import { useEffect, useState, type FormEvent } from "react";
import {
  getTimetableRooms,
  getTimetableFaculty,
  getTimetableEntries,
  createTimetableRoom,
  createTimetableEntry,
  getAcademicCourses,
  getAcademicSections,
  type TimetableRoom,
  type TimetableFaculty,
  type TimetableEntry,
  type AcademicCourse,
  type AcademicSection,
} from "../../services/api";
import "./TimetableManagement.css";

const DAYS = [
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday",
  "Sunday",
];

export default function TimetableManagement() {
  const [rooms, setRooms] = useState<TimetableRoom[]>([]);
  const [faculty, setFaculty] = useState<TimetableFaculty[]>([]);
  const [entries, setEntries] = useState<TimetableEntry[]>([]);
  const [courses, setCourses] = useState<AcademicCourse[]>([]);
  const [sections, setSections] = useState<AcademicSection[]>([]);

  const [loading, setLoading] = useState(true);
  const [saving, setSaving] = useState(false);
  const [error, setError] = useState("");
  const [success, setSuccess] = useState("");

  const [roomForm, setRoomForm] = useState({
    building: "",
    roomNumber: "",
    capacity: 60,
    roomType: "classroom",
  });

  const [slotForm, setSlotForm] = useState({
    sectionId: "",
    courseId: "",
    facultyUserId: "",
    roomId: "",
    dayOfWeek: 1,
    startTime: "09:00",
    endTime: "10:00",
  });

  async function refreshData() {
    const [r, f, e, c, s] = await Promise.all([
      getTimetableRooms(),
      getTimetableFaculty(),
      getTimetableEntries(),
      getAcademicCourses(),
      getAcademicSections(),
    ]);

    setRooms(r.rooms);
    setFaculty(f.faculty);
    setEntries(e.entries);
    setCourses(c.courses);
    setSections(s.sections);
  }

  useEffect(() => {
    refreshData()
      .catch((err: unknown) => {
        setError(
          err instanceof Error ? err.message : "Failed to load timetable data."
        );
      })
      .finally(() => setLoading(false));
  }, []);

  async function handleRoomSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setSuccess("");

    try {
      const result = await createTimetableRoom(roomForm);
      await refreshData();

      setSuccess(`${result.message} Room ID: ${result.id}`);
      setRoomForm({
        building: "",
        roomNumber: "",
        capacity: 60,
        roomType: "classroom",
      });
    } catch (err) {
      setError(err instanceof Error ? err.message : "Room creation failed.");
    } finally {
      setSaving(false);
    }
  }

  async function handleSlotSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setSuccess("");

    try {
      const result = await createTimetableEntry({
        sectionId: Number(slotForm.sectionId),
        courseId: Number(slotForm.courseId),
        facultyUserId: Number(slotForm.facultyUserId),
        roomId: Number(slotForm.roomId),
        dayOfWeek: Number(slotForm.dayOfWeek),
        startTime: slotForm.startTime,
        endTime: slotForm.endTime,
      });

      await refreshData();

      setSuccess(`${result.message} Timetable entry ID: ${result.id}`);
    } catch (err) {
      setError(err instanceof Error ? err.message : "Schedule creation failed.");
    } finally {
      setSaving(false);
    }
  }

  if (loading) {
    return <div className="tt-panel">Loading timetable...</div>;
  }

  return (
    <div className="tt-page">
      <header className="tt-heading">
        <div>
          <span>UNIVERSITY OPERATIONS</span>
          <h2>Timetable Management</h2>
          <p>Manage classrooms, faculty allocation and weekly schedules.</p>
        </div>
        <button type="button" onClick={() => {
          setLoading(true);
          refreshData()
            .catch((err: unknown) =>
              setError(err instanceof Error ? err.message : "Refresh failed.")
            )
            .finally(() => setLoading(false));
        }}>
          ↻ Refresh
        </button>
      </header>

      <div className="tt-stats">
        <div><span>Rooms</span><strong>{rooms.length}</strong></div>
        <div><span>Faculty</span><strong>{faculty.length}</strong></div>
        <div><span>Scheduled Classes</span><strong>{entries.length}</strong></div>
      </div>

      {error && <div className="tt-error">{error}</div>}
      {success && <div className="tt-success">{success}</div>}

      <section className="tt-panel">
        <h3>Add Classroom / Laboratory</h3>

        <form className="tt-form" onSubmit={handleRoomSubmit}>
          <label>
            Building
            <input
              required
              maxLength={80}
              value={roomForm.building}
              placeholder="e.g. Block A"
              onChange={(e) =>
                setRoomForm({ ...roomForm, building: e.target.value })
              }
            />
          </label>

          <label>
            Room Number
            <input
              required
              maxLength={30}
              value={roomForm.roomNumber}
              placeholder="e.g. 101"
              onChange={(e) =>
                setRoomForm({ ...roomForm, roomNumber: e.target.value })
              }
            />
          </label>

          <label>
            Capacity
            <input
              type="number"
              min="1"
              max="500"
              value={roomForm.capacity}
              onChange={(e) =>
                setRoomForm({ ...roomForm, capacity: Number(e.target.value) })
              }
            />
          </label>

          <label>
            Room Type
            <select
              value={roomForm.roomType}
              onChange={(e) =>
                setRoomForm({ ...roomForm, roomType: e.target.value })
              }
            >
              <option value="classroom">Classroom</option>
              <option value="laboratory">Laboratory</option>
              <option value="auditorium">Auditorium</option>
              <option value="other">Other</option>
            </select>
          </label>

          <button disabled={saving} type="submit">
            {saving ? "Saving..." : "Add Room"}
          </button>
        </form>
      </section>

      <section className="tt-panel">
        <h3>Schedule a Class</h3>

        <form className="tt-form" onSubmit={handleSlotSubmit}>
          <label>
            Section
            <select
              required
              value={slotForm.sectionId}
              onChange={(e) =>
                setSlotForm({ ...slotForm, sectionId: e.target.value })
              }
            >
              <option value="">Select section</option>
              {sections.map((item) => (
                <option key={item.id} value={item.id}>
                  {item.sectionCode} — Batch {item.batchYear} (ID {item.id})
                </option>
              ))}
            </select>
          </label>

          <label>
            Course
            <select
              required
              value={slotForm.courseId}
              onChange={(e) =>
                setSlotForm({ ...slotForm, courseId: e.target.value })
              }
            >
              <option value="">Select course</option>
              {courses.map((item) => (
                <option key={item.id} value={item.id}>
                  {item.code} — {item.title}
                </option>
              ))}
            </select>
          </label>

          <label>
            Faculty
            <select
              required
              value={slotForm.facultyUserId}
              onChange={(e) =>
                setSlotForm({ ...slotForm, facultyUserId: e.target.value })
              }
            >
              <option value="">Select faculty</option>
              {faculty.map((item) => (
                <option key={item.id} value={item.id}>
                  {item.loginId} — {item.email}
                </option>
              ))}
            </select>
          </label>

          <label>
            Classroom
            <select
              required
              value={slotForm.roomId}
              onChange={(e) =>
                setSlotForm({ ...slotForm, roomId: e.target.value })
              }
            >
              <option value="">Select room</option>
              {rooms.map((item) => (
                <option key={item.id} value={item.id}>
                  {item.building} / {item.roomNumber} ({item.roomType})
                </option>
              ))}
            </select>
          </label>

          <label>
            Day
            <select
              value={slotForm.dayOfWeek}
              onChange={(e) =>
                setSlotForm({ ...slotForm, dayOfWeek: Number(e.target.value) })
              }
            >
              {DAYS.map((day, index) => (
                <option key={day} value={index + 1}>{day}</option>
              ))}
            </select>
          </label>

          <label>
            Start Time
            <input
              type="time"
              required
              value={slotForm.startTime}
              onChange={(e) =>
                setSlotForm({ ...slotForm, startTime: e.target.value })
              }
            />
          </label>

          <label>
            End Time
            <input
              type="time"
              required
              value={slotForm.endTime}
              onChange={(e) =>
                setSlotForm({ ...slotForm, endTime: e.target.value })
              }
            />
          </label>

          <button
            disabled={
              saving ||
              !rooms.length ||
              !faculty.length ||
              !courses.length ||
              !sections.length
            }
            type="submit"
          >
            {saving ? "Scheduling..." : "Schedule Class"}
          </button>

          {(!rooms.length || !faculty.length || !courses.length || !sections.length) && (
            <p className="tt-hint">
              Add a room, faculty account, academic course and section before scheduling.
            </p>
          )}
        </form>
      </section>

      <section className="tt-panel">
        <h3>Weekly Timetable</h3>

        <div className="tt-week">
          {DAYS.slice(0, 6).map((day, index) => {
            const dayEntries = entries
              .filter((entry) => entry.dayOfWeek === index + 1)
              .sort((a, b) => a.startTime.localeCompare(b.startTime));

            return (
              <div className="tt-day" key={day}>
                <h4>{day}</h4>

                {dayEntries.length === 0 && (
                  <p className="tt-empty">No classes</p>
                )}

                {dayEntries.map((entry) => (
                  <article className="tt-slot" key={entry.id}>
                    <strong>{entry.courseCode}</strong>
                    <span>{entry.courseTitle}</span>
                    <small>
                      {entry.startTime} – {entry.endTime}
                    </small>
                    <small>
                      Section {entry.sectionCode} · {entry.batchYear}
                    </small>
                    <small>{entry.facultyLoginId}</small>
                    <small>{entry.roomLabel}</small>
                  </article>
                ))}
              </div>
            );
          })}
        </div>
      </section>
    </div>
  );
}