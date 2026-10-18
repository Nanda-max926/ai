export type UserRole =
  | "student"
  | "faculty"
  | "admin"
  | "coordinator";

export interface AuthUser {
  id: number;
  loginId: string;
  email: string;
  role: UserRole;
  mustChangePassword?: boolean;
}

export interface Department {
  id: number;
  code: string;
  name: string;
}

export interface CreatedAccount {
  id: number;
  loginId: string;
  role: UserRole;
  status: string;
  mustChangePassword: boolean;
  temporaryPassword: string;
  temporaryPasswordExpiresInHours: number;
}

interface LoginResponse {
  status: string;
  message: string;
  mustChangePassword: boolean
  user: AuthUser;
}

interface CurrentUserResponse {
  status: string;
  user: AuthUser;
}

interface DepartmentsResponse {
  status: string;
  departments: Department[];
}

interface CreatedAccountResponse {
  status: string;
  message: string;
  account: CreatedAccount;
}

interface ErrorResponse {
  message?: string;
}

async function apiRequest<T>(
  path: string,
  options: RequestInit = {}
): Promise<T> {
  const response = await fetch(path, {
    ...options,
    credentials: "include",
    headers: {
      ...(options.body
        ? { "Content-Type": "application/json" }
        : {}),
      ...options.headers,
    },
  });

  const result = await response.json().catch(() => null);

  if (!response.ok) {
    const error = result as ErrorResponse | null;

    throw new Error(
      error?.message || `Request failed: HTTP ${response.status}`
    );
  }

  return result as T;
}

// ==========================================
// LOGIN
// ==========================================

export async function loginUser(
  loginId: string,
  password: string,
  role: UserRole
): Promise<LoginResponse> {
  return apiRequest<LoginResponse>("/api/v1/auth/login", {
    method: "POST",
    body: JSON.stringify({
      loginId,
      password,
      role,
    }),
  });
}

// ==========================================
// CURRENT USER
// ==========================================

export async function getCurrentUser(): Promise<AuthUser> {
  const result =
    await apiRequest<CurrentUserResponse>("/api/v1/auth/me");

  return result.user;
}

// ==========================================
// LOGOUT
// ==========================================

export async function logoutUser(): Promise<void> {
  await apiRequest("/api/v1/auth/logout", {
    method: "POST",
  });
}

// ==========================================
// DEPARTMENTS
// ==========================================

export async function getDepartments(): Promise<Department[]> {
  const result =
    await apiRequest<DepartmentsResponse>(
      "/api/v1/admin/departments"
    );

  return result.departments;
}

// ==========================================
// CREATE STUDENT
// ==========================================

export async function createStudent(data: {
  fullName: string;
  email: string;
  temporaryPassword: string;
  departmentId: number;
  semester: number;
  batchYear: number;
  section: string;
}): Promise<CreatedAccount> {
  const result =
    await apiRequest<CreatedAccountResponse>(
      "/api/v1/admin/students",
      {
        method: "POST",
        body: JSON.stringify(data),
      }
    );

  return result.account;
}

// ==========================================
// CREATE FACULTY
// ==========================================

export async function createFaculty(data: {
  fullName: string;
  email: string;
  temporaryPassword: string;
  departmentId: number;
  designation: string;
}): Promise<CreatedAccount> {
  const result =
    await apiRequest<CreatedAccountResponse>(
      "/api/v1/admin/faculty",
      {
        method: "POST",
        body: JSON.stringify(data),
      }
    );

  return result.account;
}

export interface AcademicProgram {
  id: number;
  departmentId: number;
  code: string;
  name: string;
  degreeLevel: string;
  durationYears: number;
  totalSemesters: number;
  description: string;
  isActive: boolean;
}

export interface AcademicCourse {
  id: number;
  departmentId: number;
  code: string;
  title: string;
  description: string;
  credits: number;
  courseType: string;
  isActive: boolean;
}

export interface AcademicSection {
  id: number;
  programId: number;
  batchYear: number;
  academicYearStart: number;
  semesterNumber: number;
  sectionCode: string;
  capacity: number;
  isActive: boolean;
}

export async function getAcademicPrograms() {
  return apiRequest<{ status: string; programs: AcademicProgram[] }>(
    "/api/v1/admin/academic/programs"
  );
}

export async function getAcademicCourses() {
  return apiRequest<{ status: string; courses: AcademicCourse[] }>(
    "/api/v1/admin/academic/courses"
  );
}

export async function getAcademicSections() {
  return apiRequest<{ status: string; sections: AcademicSection[] }>(
    "/api/v1/admin/academic/sections"
  );
}

export async function createAcademicProgram(
  data: Omit<AcademicProgram, "id" | "isActive">
) {
  return apiRequest<{ status: string; id: number; message: string }>(
    "/api/v1/admin/academic/programs",
    { method: "POST", body: JSON.stringify(data) }
  );
}

export async function createAcademicCourse(
  data: Omit<AcademicCourse, "id" | "isActive">
) {
  return apiRequest<{ status: string; id: number; message: string }>(
    "/api/v1/admin/academic/courses",
    { method: "POST", body: JSON.stringify(data) }
  );
}

export async function createAcademicSection(data: Omit<AcademicSection, "id" | "isActive">) {
  return apiRequest<{ status: string; id: number; message: string }>(
    "/api/v1/admin/academic/sections",
    { method: "POST", body: JSON.stringify(data) }
  );
}

export interface TimetableRoom {
  id: number;
  building: string;
  roomNumber: string;
  capacity: number;
  roomType: string;
  isActive: boolean;
}

export interface TimetableFaculty {
  id: number;
  loginId: string;
  email: string;
}

export interface TimetableEntry {
  id: number;
  sectionId: number;
  courseId: number;
  facultyUserId: number;
  roomId: number;
  dayOfWeek: number;
  startTime: string;
  endTime: string;
  sectionCode: string;
  batchYear: number;
  courseCode: string;
  courseTitle: string;
  facultyLoginId: string;
  roomLabel: string;
}

export async function getTimetableRooms() {
  return apiRequest<{ status: string; rooms: TimetableRoom[] }>(
    "/api/v1/admin/timetable/rooms"
  );
}

export async function getTimetableFaculty() {
  return apiRequest<{ status: string; faculty: TimetableFaculty[] }>(
    "/api/v1/admin/timetable/faculty"
  );
}

export async function getTimetableEntries() {
  return apiRequest<{ status: string; entries: TimetableEntry[] }>(
    "/api/v1/admin/timetable/entries"
  );
}

export async function createTimetableRoom(data: {
  building: string;
  roomNumber: string;
  capacity: number;
  roomType: string;
}) {
  return apiRequest<{ status: string; id: number; message: string }>(
    "/api/v1/admin/timetable/rooms",
    { method: "POST", body: JSON.stringify(data) }
  );
}

export async function createTimetableEntry(data: {
  sectionId: number;
  courseId: number;
  facultyUserId: number;
  roomId: number;
  dayOfWeek: number;
  startTime: string;
  endTime: string;
}) {
  return apiRequest<{ status: string; id: number; message: string }>(
    "/api/v1/admin/timetable/entries",
    { method: "POST", body: JSON.stringify(data) }
  );
}
export interface StudentPortalProfile {
  id: number;
  loginId: string;
  email: string;
  fullName: string;
  departmentId: number;
  department: string;
  departmentCode: string;
  semester: number;
  batchYear: number;
  section: string;
  programId: number | null;
  program: string;
  academicSectionId: number | null;
}

export interface StudentPortalCourse {
  id: number;
  code: string;
  title: string;
  credits: number;
  courseType: string;
  semesterNumber: number;
}

export interface StudentPortalTimetableEntry {
  id: number;
  dayOfWeek: number;
  startTime: string;
  endTime: string;
  courseCode: string;
  courseTitle: string;
  facultyName: string;
  room: string;
}

export interface StudentAttendanceCourse {
  courseCode: string;
  courseTitle: string;
  total: number;
  present: number;
  absent: number;
  percentage: number;
}

export interface StudentPortalAttendance {
  total: number;
  present: number;
  absent: number;
  percentage: number;
  byCourse: StudentAttendanceCourse[];
}

export interface StudentPortalExam {
  id: number;
  courseCode: string;
  title: string;
  examType: string;
  examDate: string;
  totalMarks: number;
  passingMarks: number;
  marksObtained: number | null;
  grade: string | null;
  remarks: string;
}

export interface StudentPortalAnnouncement {
  id: number;
  title: string;
  content: string;
  priority: string;
  publishAt: string;
}

export interface StudentPortalEvent {
  id: number;
  title: string;
  description: string;
  eventType: string;
  location: string;
  startDate: string;
  endDate: string;
  startTime: string | null;
  endTime: string | null;
}

export interface StudentPortalData {
  status: string;
  profile: StudentPortalProfile;
  courses: StudentPortalCourse[];
  timetable: StudentPortalTimetableEntry[];
  attendance: StudentPortalAttendance;
  examinations: StudentPortalExam[];
  announcements: StudentPortalAnnouncement[];
  events: StudentPortalEvent[];
}

export async function getStudentPortal(): Promise<StudentPortalData> {
  return apiRequest<StudentPortalData>("/api/v1/student/portal");
}