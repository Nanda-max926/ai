
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
}

interface LoginResponse {
  status: string;
  message: string;
  mustChangePassword: boolean;
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
