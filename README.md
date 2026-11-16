# universe-university-portal


# UniVerse — University Management System

<p align="center">
  <strong>A Centralized Digital Platform for University Administration, Academics and Student Services</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue" alt="C++17">
  <img src="https://img.shields.io/badge/Frontend-React%20%2B%20TypeScript-3178C6" alt="React and TypeScript">
  <img src="https://img.shields.io/badge/Backend-Drogon-2E8B57" alt="Drogon">
  <img src="https://img.shields.io/badge/Database-PostgreSQL-336791" alt="PostgreSQL">
  <img src="https://img.shields.io/badge/Build-CMake-064F8C" alt="CMake">
  <img src="https://img.shields.io/badge/License-MIT-green" alt="MIT License">
</p>

---

## 1. Introduction

**UniVerse** is a web-based University Management System designed to bring major university operations together in one centralized digital platform.

Universities manage large amounts of academic and administrative information, including student records, faculty details, departments, courses, class sections, timetables, attendance, examinations and announcements. When these activities are handled through disconnected systems or manual processes, information becomes difficult to maintain, retrieve and coordinate.

UniVerse aims to simplify these activities by providing a centralized system with role-based access for administrators, students and faculty.

The project uses a **C++ backend** for server-side business logic and database operations, together with a React and TypeScript frontend for the user interface.

**Repository:** https://github.com/Nanda-max926/ai

## 2. Project Objectives

The main objectives of UniVerse are:

- Develop a centralized university information management platform.
- Reduce dependency on disconnected or manual academic records.
- Provide role-based authentication and access control.
- Allow administrators to manage departments, users and academic structures.
- Organize academic programs, courses and class sections.
- Support classroom allocation and timetable management.
- Provide students with a dedicated portal for academic information.
- Establish a structured PostgreSQL database for university records.
- Demonstrate the integration of C++, database programming, REST APIs and modern frontend development.

## 3. Problem Statement

University administration involves coordination between multiple departments, students, faculty members and academic activities.

Traditional or disconnected management approaches can lead to:

- Repeated data entry and inconsistent information.
- Difficulty maintaining student and faculty records.
- Manual coordination of courses, classrooms and timetables.
- Delayed access to academic information.
- Limited visibility into attendance and examination information.
- Difficulty distributing announcements and university updates.

UniVerse addresses these challenges by providing a common application through which university information can be managed and accessed according to the user's role.

## 4. Main Modules

### 4.1 Authentication and User Management

- Role-based login interface.
- Student, faculty and administrator account types.
- Backend authentication using C++.
- Server-side session management.
- Account status checks.
- Password hashing using libsodium's password-hashing functionality.

### 4.2 Administrator Dashboard

The administrator interface acts as the central management workspace.

Its modules include:

- Administration overview.
- Student account creation.
- Faculty account creation.
- Department management.
- Academic management.
- Timetable management.
- Attendance management.
- Examinations and results.
- Calendar and announcements.

### 4.3 Department Management

- Maintain university departments.
- Store department codes and names.
- Manage department contact information.
- Associate users and academic information with departments.

### 4.4 Academic Management

Academic management organizes the university's academic structure.

It includes:

- Degree program management.
- Course and subject management.
- Course credits and course types.
- Semester organization.
- Academic section creation.
- Admission batch information.
- Student capacity information.
- Program-to-course associations.

### 4.5 Timetable Management

The timetable module is designed to organize class schedules and university resources.

Its functionality includes:

- Classroom and laboratory records.
- Room capacity and room types.
- Faculty allocation.
- Section allocation.
- Course scheduling.
- Day and time selection.
- Schedule conflict prevention through database constraints.

### 4.6 Student Portal

The student portal provides a dedicated student-facing workspace.

The interface includes:

- Student dashboard.
- My Courses.
- My Timetable.
- Attendance.
- Examinations and Results.
- Announcements.
- My Profile.
- Student academic information.

### 4.7 Attendance Management

The attendance module provides an interface for managing student attendance records and viewing attendance information.

### 4.8 Examinations and Results

The examination module provides an interface for organizing examination-related information and academic results.

### 4.9 Calendar and Announcements

The system includes database support for university calendar events and announcements, including publication and audience-related information.

> **Implementation status:** UniVerse is a developing academic software project. The major interfaces, backend services, database structures and sample academic workflows have been implemented to varying degrees. Some academic and timetable operations still require end-to-end API and UI verification. A module's presence in the interface does not necessarily mean every operation is fully complete.

## 5. Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Main backend programming language |
| Drogon | C++ web framework and REST API server |
| PostgreSQL | Relational database |
| SQL | Database schema, migrations and queries |
| libsodium | Secure password hashing |
| React | Frontend user interface |
| TypeScript | Frontend application logic and type safety |
| HTML5 | Application structure |
| CSS3 | Layout, styling and responsive design |
| Vite | Frontend development server and build tool |
| CMake | C++ build configuration |
| Git and GitHub | Version control and collaboration |

### Why C++?

C++ was selected as the primary backend language to demonstrate strong programming fundamentals and practical systems development.

The backend uses C++ for:

- Server-side application logic.
- REST API controllers.
- Business and service-layer operations.
- Database repository operations.
- Authentication and session handling.
- Validation and error handling.
- Integration with PostgreSQL.

React and TypeScript are used for the frontend so that users can interact with the C++ backend through a modern web interface.

## 6. System Architecture

UniVerse follows a layered client-server architecture.

```text
                  USER
                   |
                   v
         React + TypeScript Frontend
                   |
                   v
             Vite Dev Server
                   |
                   v
             REST API Requests
                   |
                   v
          C++ / Drogon Backend
                   |
        +----------+----------+
        |          |          |
        v          v          v
   Controllers  Services  Authentication
        |          |          |
        +----------+----------+
                   |
                   v
           Repository Layer
                   |
                   v
             PostgreSQL
                   |
                   v
          University Records
```

### Architecture Layers

**Frontend Layer**

Responsible for presenting login forms, dashboards, data tables, management screens and student-facing pages.

**Controller Layer**

Receives HTTP requests, validates request data, checks access permissions and returns API responses.

**Service Layer**

Contains application logic and coordinates operations between controllers and repositories.

**Repository Layer**

Handles database communication using parameterized PostgreSQL queries.

**Database Layer**

Stores users, profiles, departments, academic structures, classroom information, timetable entries and university updates.

## 7. Backend Structure

The backend is organized into controllers, services, repositories, domain models and utilities.

```text
backend/
│
├── config/
│   ├── config.example.json
│   └── config.json
│
├── include/
│   ├── controllers/
│   ├── domain/
│   ├── middleware/
│   ├── repositories/
│   ├── services/
│   └── utils/
│
├── src/
│   ├── controllers/
│   ├── repositories/
│   ├── services/
│   └── main.cc
│
├── database/
├── tests/
├── tools/
└── CMakeLists.txt
```

### Backend Responsibilities

- `main.cc`: Application entry point and route registration.
- `controllers/`: HTTP endpoints and request/response handling.
- `services/`: Business logic and application workflows.
- `repositories/`: PostgreSQL query execution.
- `domain/`: Core application models.
- `middleware/`: Request-level processing and access-related components.
- `tools/`: Supporting utilities such as administrator bootstrap tools.
- `CMakeLists.txt`: Backend build configuration.

## 8. Frontend Structure

```text
frontend/
│
├── public/
├── src/
│   ├── pages/
│   │   ├── admin/
│   │   ├── auth/
│   │   └── student/
│   │
│   ├── services/
│   ├── assets/
│   ├── App.tsx
│   ├── App.css
│   ├── index.css
│   └── main.tsx
│
├── package.json
├── tsconfig.json
└── vite.config.ts
```

The frontend communicates with the backend through HTTP API requests. During local development, the Vite development server proxies `/api` requests to the C++ backend.

## 9. Database Design

PostgreSQL is used as the primary relational database.

The database is designed to maintain relationships between university users, departments, academic programs, courses, sections, rooms and timetable records.

### Main Database Entities

- Users
- Student profiles
- Faculty profiles
- Departments
- Academic programs
- Academic courses
- Program-course associations
- Academic sections
- Academic rooms
- Timetable entries
- Calendar events
- Announcements

### Database Migrations

Database migrations are maintained in:

`database/migrations/`

| Migration | Purpose |
|---|---|
| `001_users.sql` | User accounts and authentication-related fields |
| `002_departments_profiles.sql` | Departments and student/faculty profiles |
| `003_temporary_credentials.sql` | Temporary credential support |
| `004_department_management.sql` | Department management structures |
| `005_academic_management.sql` | Academic programs, courses and sections |
| `006_timetable_management.sql` | Rooms and timetable structures |
| `007_calendar_announcements.sql` | Calendar events and announcements |

Migrations should be applied in numerical order.

### Database Relationships

Examples of important relationships include:

- A department can have multiple students and faculty members.
- A department can offer multiple academic programs.
- An academic program can contain multiple courses.
- An academic program can have multiple sections.
- A section can have multiple timetable entries.
- A timetable entry is associated with a course, faculty member and classroom.

The database also uses foreign keys, uniqueness constraints, validation checks and timetable overlap constraints to help maintain data integrity.

## 10. Authentication and Security

The application includes several security-related mechanisms:

- Password hashing with libsodium.
- Server-side session-based authentication.
- Role checks for administrative API operations.
- Parameterized database queries.
- Account status validation.
- Input validation in backend request handling.
- HTTP-only session cookies.
- Separation of local configuration from committed source files.

Passwords and database credentials should never be committed to GitHub.

**Security note:** This project is an academic prototype and should not be deployed with real university or student data without further security review, HTTPS deployment, CSRF protection, request rate limiting, credential-change enforcement and production configuration hardening.

## 11. API Overview

The backend exposes versioned REST API endpoints under `/api/v1`.

Examples of implemented endpoint groups include:

| Endpoint | Method | Purpose |
|---|---|---|
| `/api/v1/health` | GET | Backend health check |
| `/api/v1/health/database` | GET | Database connectivity check |
| `/api/v1/auth/login` | POST | User authentication |
| `/api/v1/auth/me` | GET | Current authenticated user |
| `/api/v1/auth/logout` | POST | End the current session |
| `/api/v1/admin/departments` | GET | Retrieve departments |
| `/api/v1/admin/students` | POST | Create a student account |
| `/api/v1/admin/faculty` | POST | Create a faculty account |
| `/api/v1/admin/academic/programs` | GET/POST | Retrieve or create academic programs |
| `/api/v1/admin/academic/courses` | GET/POST | Retrieve or create academic courses |
| `/api/v1/admin/academic/sections` | GET/POST | Retrieve or create academic sections |

Additional timetable, student, calendar and announcement endpoints are registered in the backend controllers.

## 12. Installation and Setup

The following instructions describe a local development setup on Ubuntu/Linux.

### Prerequisites

Install the required development tools and libraries:

```bash
sudo apt update

sudo apt install -y \
  build-essential \
  cmake \
  git \
  pkg-config \
  postgresql \
  postgresql-contrib \
  libpq-dev \
  libsodium-dev \
  libjsoncpp-dev \
  libuuid-dev \
  libdrogon-dev
```

The frontend uses Node.js. Node.js 24 was used during development.

Check installed versions:

```bash
g++ --version
cmake --version
psql --version
node -v
npm -v
```

### Step 1: Clone the repository

```bash
git clone https://github.com/Nanda-max926/ai.git
cd ai
```

### Step 2: Create the PostgreSQL database

Create a PostgreSQL database and application user.

Open the PostgreSQL shell:

```bash
sudo -u postgres psql
```

Run the following SQL, replacing the placeholder password with a strong local password:

```sql
CREATE USER universe_user WITH PASSWORD 'CHANGE_THIS_LOCAL_PASSWORD';
CREATE DATABASE universe_db OWNER universe_user;
\q
```

Do not put the real password in this README or commit it to GitHub.

### Step 3: Apply database migrations

From the repository root:

```bash
psql -h 127.0.0.1 -U universe_user -d universe_db \
  -v ON_ERROR_STOP=1 \
  -f database/migrations/001_users.sql

psql -h 127.0.0.1 -U universe_user -d universe_db \
  -v ON_ERROR_STOP=1 \
  -f database/migrations/002_departments_profiles.sql

psql -h 127.0.0.1 -U universe_user -d universe_db \
  -v ON_ERROR_STOP=1 \
  -f database/migrations/003_temporary_credentials.sql

psql -h 127.0.0.1 -U universe_user -d universe_db \
  -v ON_ERROR_STOP=1 \
  -f database/migrations/004_department_management.sql

psql -h 127.0.0.1 -U universe_user -d universe_db \
  -v ON_ERROR_STOP=1 \
  -f database/migrations/005_academic_management.sql

psql -h 127.0.0.1 -U universe_user -d universe_db \
  -v ON_ERROR_STOP=1 \
  -f database/migrations/006_timetable_management.sql

psql -h 127.0.0.1 -U universe_user -d universe_db \
  -v ON_ERROR_STOP=1 \
  -f database/migrations/007_calendar_announcements.sql
```

Enter the PostgreSQL application user's password when prompted.

### Step 4: Configure the backend

Copy the example configuration:

```bash
cp backend/config/config.example.json backend/config/config.json
```

Edit the local configuration:

```bash
code backend/config/config.json
```

Update the PostgreSQL connection settings and other local configuration values according to the example file and your environment.

**Never commit `backend/config/config.json`.**

### Step 5: Build the C++ backend

```bash
cd backend

cmake -S . -B build
cmake --build build -j2
```

### Step 6: Run the backend

```bash
./build/universe_api
```

The backend is configured for local development on:

`http://127.0.0.1:8080`

Health checks:

```bash
curl http://127.0.0.1:8080/api/v1/health

curl http://127.0.0.1:8080/api/v1/health/database
```

### Step 7: Install and run the frontend

Open another terminal:

```bash
cd frontend

npm install
npm run dev
```

Open the local application:

`http://127.0.0.1:5173`

The Vite proxy forwards `/api` requests to the local Drogon backend.

### Step 8: Build the frontend for production

```bash
cd frontend
npm run build
```

The generated frontend build is placed in the `dist/` directory.

## 13. Local Development Notes

- Run PostgreSQL before starting the backend.
- Keep the backend running while using the frontend.
- Start the frontend in a separate terminal.
- Apply migrations before using database-backed modules.
- Create or bootstrap an administrator account using the provided backend bootstrap utility where applicable.
- Create departments, accounts and academic records before testing dependent workflows.
- Timetable entries require valid sections, courses, faculty and rooms.
- A student's courses and timetable depend on the student's academic program and section assignment.
- Local development records are not automatically included in a fresh database.

## 14. Project Demonstration Workflow

A useful demonstration of UniVerse follows this sequence:

1. Start PostgreSQL.
2. Start the C++ backend.
3. Start the React frontend.
4. Sign in using the administrator account.
5. Open the administrator dashboard.
6. View and manage university departments.
7. Create or review academic programs and courses.
8. Create student and faculty accounts.
9. Create academic sections and classroom records.
10. Configure a timetable.
11. Sign in through the student login option.
12. Review the student dashboard and available academic information.

Some workflows may require additional configuration or API fixes depending on the database state.

## 15. Future Enhancements

Potential future improvements include:

- Complete faculty-specific dashboards and workflows.
- Full student and faculty profile management.
- End-to-end attendance recording and reporting.
- Examination scheduling and result publication.
- Complete calendar and announcement management.
- Notifications for students and faculty.
- Timetable editing and cancellation.
- Search, filtering and advanced reporting.
- Password reset and first-login password change workflows.
- Automated unit, integration and API tests.
- Production deployment with HTTPS and hardened security.
- Database backup and recovery procedures.

## 16. Contributors and Participants

The project is maintained in the GitHub repository:

**Repository owner:** Nanda-max926

**Contributors shown on the GitHub repository:**

- **Nanda-max926** — Project development and repository maintenance.
- **Second contributor:** Add the collaborator's full name and GitHub username here.

Contributor responsibilities should be updated to reflect the actual work performed by each participant.

## 17. Academic Project Information

**Project Name:** UniVerse — University Management System

**Project Type:** Web-Based Application / University Management System

**Primary Backend Language:** C++

**Frontend:** React and TypeScript

**Database:** PostgreSQL

**Backend Framework:** Drogon

**Build System:** CMake

**Version Control:** Git and GitHub

**Repository:** https://github.com/Nanda-max926/ai

## 18. License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

<p align="center">
  <strong>UniVerse — Bringing University Operations Together.</strong>
</p>