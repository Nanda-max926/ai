# UniVerse — University Management System

\<p align="center">   \<strong>A Centralized Digital Platform for University Administration, Academic Management and Student Services\</strong> \</p>

\<p align="center">   ![C++]\([https://img.shields.io/badge/C%2B%2B-17-blue](https://img.shields.io/badge/C%2B%2B-17-blue))   ![React]\([https://img.shields.io/badge/Frontend-React-61DAFB](https://img.shields.io/badge/Frontend-React-61DAFB))   ![TypeScript]\([https://img.shields.io/badge/TypeScript-3178C6](https://img.shields.io/badge/TypeScript-3178C6))   ![Drogon]\([https://img.shields.io/badge/Backend-Drogon-green](https://img.shields.io/badge/Backend-Drogon-green))   ![PostgreSQL]\([https://img.shields.io/badge/Database-PostgreSQL-336791](https://img.shields.io/badge/Database-PostgreSQL-336791))   ![CMake]\([https://img.shields.io/badge/Build-CMake-064F8C](https://img.shields.io/badge/Build-CMake-064F8C))   ![License]\([https://img.shields.io/badge/License-MIT-green](https://img.shields.io/badge/License-MIT-green)) \</p>

---

## 1. Introduction

**UniVerse** is a web-based University Management System developed to simplify and centralize university administration and academic activities.

Universities handle large amounts of information related to students, faculty, departments, courses, academic programs, attendance, examinations, classrooms and timetables. Managing these activities through separate systems or manual processes can result in duplicated work, inconsistent records and difficulties accessing information.

UniVerse aims to bring these operations together in one centralized platform.

The application provides separate interfaces for administrators, students and faculty members. It uses **C++ as the primary backend programming language**, PostgreSQL for data storage, and React with TypeScript for the frontend.

The project demonstrates the practical application of Object-Oriented Programming, database management, REST API development, client-server architecture and modern web application development.

**GitHub Repository:**
[https://github.com/Nanda-max926/ai](https://github.com/Nanda-max926/ai)

---

## 2. Problem Statement

University management involves several interconnected activities, including student registration, faculty management, department administration, course allocation, timetable scheduling, attendance recording and examination management.

When these activities are handled manually or through disconnected systems, universities may experience:

- Difficulty maintaining student and faculty records.
- Repeated data entry and inconsistent information.
- Problems coordinating academic departments.
- Manual timetable preparation and classroom allocation.
- Delayed access to academic information.
- Difficulty tracking student attendance and examination records.
- Inefficient communication between administrators, students and faculty.

UniVerse addresses these challenges by providing a centralized digital platform where information can be managed, stored and accessed according to user roles.

---

## 3. Project Objectives

The main objectives of UniVerse are:

1. Develop a centralized university management application.
2. Implement secure role-based authentication.
3. Provide separate portals for administrators, students and faculty.
4. Manage student, faculty and department information.
5. Organize academic programs, courses and class sections.
6. Support classroom allocation and timetable management.
7. Provide students with access to their academic information.
8. Establish a structured relational database using PostgreSQL.
9. Implement backend business logic using C++.
10. Demonstrate practical Object-Oriented Programming concepts.
11. Integrate frontend, backend and database components into one application.

---

## 4. Main Features and Modules

### 4.1 Authentication and User Management

- Role-based login interface.
- Administrator, student and faculty account types.
- Backend authentication using C++.
- Server-side session management.
- Account status validation.
- Password hashing using libsodium.
- Separate dashboard access based on user roles.

### 4.2 Administrator Dashboard

The administrator dashboard provides a centralized workspace for managing university operations.

Features include:

- Administration overview.
- Student account creation.
- Faculty account creation.
- Department management.
- Academic program management.
- Course and subject management.
- Section management.
- Timetable management.
- Attendance management.
- Examination and result management.
- Calendar and announcements.

### 4.3 Department Management

- Maintain university departments.
- Store department codes and names.
- Manage department contact information.
- Associate students and faculty with departments.
- Organize academic records department-wise.

### 4.4 Academic Management

The academic module organizes the university's academic structure.

It includes:

- Degree program creation.
- Course and subject management.
- Course credits and course types.
- Semester organization.
- Academic section creation.
- Admission batch information.
- Student capacity management.
- Program-to-course associations.

### 4.5 Timetable Management

The timetable module is designed to coordinate academic schedules and university resources.

Features include:

- Classroom and laboratory management.
- Room capacity and room types.
- Faculty allocation.
- Course allocation.
- Section allocation.
- Day and time selection.
- Timetable scheduling.
- Database-level schedule conflict prevention.

### 4.6 Student Portal

The student portal provides students with a dedicated interface to access academic information.

Features include:

- Student dashboard.
- My Courses.
- My Timetable.
- Attendance.
- Examinations and Results.
- Announcements.
- My Profile.
- Student academic information.

### 4.7 Faculty Portal

The faculty interface is designed to provide faculty members with access to their university-related information and academic activities.

Planned and developed interface areas include:

- Faculty dashboard.
- Faculty profile.
- Assigned courses.
- Timetable information.
- Student-related academic activities.
- Attendance-related activities.

### 4.8 Attendance Management

The attendance module provides an interface for recording and managing student attendance information.

### 4.9 Examinations and Results

The examination module provides an interface for examination-related information and academic results.

### 4.10 Calendar and Announcements

The system includes database support for university calendar events and announcements, including publication dates and audience-related information.

> **Implementation status:** UniVerse is an academic project prototype. Its major interfaces, database structures, backend services and sample workflows have been developed to varying degrees. Some modules still require additional end-to-end testing and API/UI integration. The presence of a screen does not mean every operation is production-ready.

---

## 5. Technology Stack

| Technology | Purpose                               |
| ---------- | ------------------------------------- |
| C++17      | Primary backend programming language  |
| Drogon     | C++ web framework                     |
| PostgreSQL | Relational database                   |
| SQL        | Database schema and queries           |
| libsodium  | Password hashing                      |
| React      | Frontend user interface               |
| TypeScript | Frontend logic and type safety        |
| HTML5      | Web page structure                    |
| CSS3       | Styling and responsive layouts        |
| Vite       | Frontend development and build tool   |
| CMake      | C++ build configuration               |
| Git        | Version control                       |
| GitHub     | Source code hosting and collaboration |

### Why C++?

C++ was selected as the primary backend language to demonstrate strong programming fundamentals and practical systems development.

It is used for:

- Backend application logic.
- REST API controllers.
- Authentication and session handling.
- Service-layer operations.
- Database repository operations.
- Input validation.
- Error handling.
- Integration with PostgreSQL.

React and TypeScript are used for the frontend, while C++ handles the core backend processing and business logic.

---

# 6. Object-Oriented Programming Concepts Used in UniVerse

Object-Oriented Programming (OOP) is an important part of the UniVerse backend architecture.

The backend is organized using classes and objects that represent system components such as controllers, services, repositories and domain models.

The following concepts explain how OOP is applied in the project.

## 6.1 Classes and Objects

A **class** is a blueprint that defines data members and functions. An **object** is an instance of a class.

In UniVerse, classes are used to represent backend components and organize related functionality.

**Where used:**

- `AuthController`
- `AdminController`
- `AcademicController`
- `TimetableController`
- `StudentController`
- `AuthService`
- `AcademicService`
- `UserRepository`
- `AcademicRepository`
- `TimetableRepository`

For example, the `AcademicService` class groups operations related to academic programs, courses and sections.

This makes the code easier to organize, maintain and extend.

## 6.2 Encapsulation

**Encapsulation** means combining data and the functions that operate on that data into a class while controlling access to its internal implementation.

In UniVerse, encapsulation is used in the service and repository layers.

**Where used:**

- Database connection and query handling are kept inside repository classes.
- Internal database operations are accessed through repository methods.
- Controllers do not need to contain the SQL implementation.
- Services expose methods for performing application operations.

For example, the repository layer handles SQL queries internally and exposes operations such as retrieving or creating academic records.

This reduces direct access to internal database logic and makes changes easier to manage.

## 6.3 Abstraction

**Abstraction** means hiding complex implementation details and exposing only the necessary operations.

UniVerse uses abstraction through its layered backend architecture.

**Where used:**

- Controllers communicate with service classes.
- Service classes coordinate business operations.
- Repository classes handle database queries.
- The frontend communicates with backend functionality through REST API endpoints.

For example, the frontend can request academic programs through an API without knowing how the C++ repository executes its PostgreSQL query.

The repository implementation is hidden behind a simpler interface.

## 6.4 Inheritance

**Inheritance** allows one class to acquire or extend the properties and functionality of another class.

In UniVerse, the Drogon framework provides framework-level class and handler abstractions that the backend integrates with.

The application primarily organizes its own functionality using separate controller, service and repository classes rather than relying on a large custom inheritance hierarchy.

This keeps the application structure modular and focused on individual responsibilities.

## 6.5 Polymorphism

**Polymorphism** allows a common interface or operation to support different implementations or behaviours.

UniVerse uses callback-driven asynchronous database operations through the Drogon ORM framework.

**Where used:**

- Database success callbacks.
- Database error callbacks.
- Asynchronous query result handling.
- Request and response handler integration.

For example, a repository operation can trigger a success callback when a database query completes or an error callback when the query fails.

This allows the calling service or controller to handle different outcomes without directly managing the database execution process.

## 6.6 Composition

**Composition** means building larger components by combining smaller objects or classes.

This is an important part of UniVerse's architecture.

**Where used:**

- Services use repository objects to access database operations.
- Controllers coordinate with service components.
- The application combines authentication, academic, department and timetable functionality.

For example:

```text
AcademicController
        |
        v
AcademicService
        |
        v
AcademicRepository
        |
        v
PostgreSQL Database
```

Each component has its own responsibility, and together they perform a complete operation.

## 6.7 Summary of OOP Concepts

| OOP Concept         | How it is applied in UniVerse                                 |
| ------------------- | ------------------------------------------------------------- |
| Classes and Objects | Controllers, services, repositories and domain models         |
| Encapsulation       | Internal database and business logic grouped inside classes   |
| Abstraction         | Service and repository interfaces hide implementation details |
| Inheritance         | Framework-level class and handler abstractions                |
| Polymorphism        | Asynchronous callbacks and handler interfaces                 |
| Composition         | Controllers, services and repositories work together          |

**Why OOP is useful in UniVerse:**

- Improves code organization.
- Separates different responsibilities.
- Reduces unnecessary code duplication.
- Makes the backend easier to maintain.
- Helps extend the system with additional modules.
- Supports structured development and debugging.

---

# 7. System Architecture

UniVerse follows a layered client-server architecture.

```text
                 USERS
                   |
                   v
       React + TypeScript Frontend
                   |
                   v
             Vite Development
                   |
                   v
             REST API Requests
                   |
                   v
           C++ / Drogon Backend
                   |
        +----------+-----------+
        |          |           |
        v          v           v
   Controllers  Services  Authentication
        |          |           |
        +----------+-----------+
                   |
                   v
            Repository Layer
                   |
                   v
             PostgreSQL
                   |
                   v
       University Management Records
```

### Architecture Layers

**Frontend Layer**

Responsible for login forms, dashboards, management pages, forms, tables and student-facing screens.

**Controller Layer**

Receives HTTP requests, validates input and access permissions, and prepares API responses.

**Service Layer**

Contains business logic and coordinates operations between controllers and repositories.

**Repository Layer**

Handles database queries and communication with PostgreSQL.

**Database Layer**

Stores users, profiles, departments, academic programs, courses, sections, rooms, timetables and university updates.

---

# 8. Backend Project Structure

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
├── tests/
├── tools/
├── uploads/
└── CMakeLists.txt
```

### Backend Responsibilities

- `main.cc`: Application entry point and route registration.
- `controllers/`: HTTP endpoints and request/response processing.
- `services/`: Application logic and workflows.
- `repositories/`: PostgreSQL queries.
- `domain/`: Data models and domain structures.
- `middleware/`: Request and access-related components.
- `tools/`: Supporting utilities, including administrator bootstrap functionality.
- `CMakeLists.txt`: C++ build configuration.

---

# 9. Frontend Project Structure

```text
frontend/
│
├── public/
├── src/
│   ├── assets/
│   ├── pages/
│   │   ├── admin/
│   │   ├── auth/
│   │   └── student/
│   │
│   ├── services/
│   ├── App.tsx
│   ├── App.css
│   ├── index.css
│   └── main.tsx
│
├── package.json
├── tsconfig.json
└── vite.config.ts
```

The frontend communicates with the C++ backend through HTTP API requests. During local development, Vite forwards `/api` requests to the backend.

---

# 10. Database Design

PostgreSQL is used as the primary relational database.

The database maintains relationships between users, departments, academic programs, courses, sections, classrooms and timetable entries.

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

Database migrations are stored in:

`database/migrations/`

| Migration                        | Purpose                                  |
| -------------------------------- | ---------------------------------------- |
| `001_users.sql`                  | User accounts and authentication fields  |
| `002_departments_profiles.sql`   | Departments and student/faculty profiles |
| `003_temporary_credentials.sql`  | Temporary credential support             |
| `004_department_management.sql`  | Department management                    |
| `005_academic_management.sql`    | Academic programs, courses and sections  |
| `006_timetable_management.sql`   | Classroom and timetable structures       |
| `007_calendar_announcements.sql` | Calendar events and announcements        |

Migrations should be applied in numerical order when setting up a fresh database.

### Important Relationships

- A department can contain multiple students and faculty members.
- A department can offer multiple academic programs.
- An academic program can contain multiple courses.
- An academic program can have multiple sections.
- A section can have multiple timetable entries.
- A timetable entry is associated with a course, faculty member and classroom.
- Student profiles can be associated with academic programs and sections.

The database uses foreign keys, unique constraints, validation checks and timetable overlap constraints to help maintain data integrity.

---

# 11. Authentication and Security

UniVerse includes the following security-related mechanisms:

- Password hashing using libsodium.
- Server-side session-based authentication.
- Role checks for administrative API operations.
- Parameterized database queries.
- Account status validation.
- Backend input validation.
- HTTP-only session cookies.
- Separation of local configuration from committed source files.

Passwords and database credentials must never be committed to GitHub.

**Security note:** UniVerse is an academic prototype. Before production deployment with real student information, additional hardening is required, including HTTPS, CSRF protection, request rate limiting, password reset and first-login password change enforcement, audit logging and production configuration management.

---

# 12. API Overview

The C++ backend exposes REST API endpoints under `/api/v1`.

Examples of implemented endpoint groups include:

| Endpoint                          | Method   | Purpose                     |
| --------------------------------- | -------- | --------------------------- |
| `/api/v1/health`                  | GET      | Backend health check        |
| `/api/v1/health/database`         | GET      | Database connectivity check |
| `/api/v1/auth/login`              | POST     | User authentication         |
| `/api/v1/auth/me`                 | GET      | Current authenticated user  |
| `/api/v1/auth/logout`             | POST     | End the current session     |
| `/api/v1/admin/departments`       | GET      | Retrieve departments        |
| `/api/v1/admin/students`          | POST     | Create student accounts     |
| `/api/v1/admin/faculty`           | POST     | Create faculty accounts     |
| `/api/v1/admin/academic/programs` | GET/POST | Retrieve or create programs |
| `/api/v1/admin/academic/courses`  | GET/POST | Retrieve or create courses  |
| `/api/v1/admin/academic/sections` | GET/POST | Retrieve or create sections |

Additional timetable, student, calendar and announcement endpoints are registered through the backend controllers.

---

# 13. Installation and Setup

The following instructions describe local development on Ubuntu/Linux.

## Prerequisites

Install the required tools and libraries:

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

Install Node.js 24 and npm for the frontend.

Check versions:

```bash
g++ --version
cmake --version
psql --version
node -v
npm -v
```

## Step 1: Clone the Repository

```bash
git clone https://github.com/Nanda-max926/ai.git

cd ai
```

## Step 2: Create the PostgreSQL User and Database

Open the PostgreSQL shell:

```bash
sudo -u postgres psql
```

Create the application user and database:

```sql
CREATE USER universe_user WITH PASSWORD 'CHANGE_THIS_PASSWORD';

CREATE DATABASE universe_db OWNER universe_user;

\q
```

Replace the example password with your own secure local password.

## Step 3: Apply Database Migrations

From the repository root:

```bash
for file in database/migrations/00*.sql; do
    echo "Applying $file"
    psql -X -v ON_ERROR_STOP=1 \
      -h 127.0.0.1 \
      -U universe_user \
      -d universe_db \
      -f "$file" || exit 1
done
```

Enter the PostgreSQL password when prompted.

## Step 4: Configure the Backend

Copy the example configuration:

```bash
cp backend/config/config.example.json backend/config/config.json
```

Edit the local configuration:

```bash
code backend/config/config.json
```

Update the PostgreSQL connection details and other required settings according to the example configuration.

**Do not commit `backend/config/config.json` or any local password.**

## Step 5: Build the C++ Backend

```bash
cd backend

cmake -S . -B build

cmake --build build -j2
```

## Step 6: Run the Backend

```bash
./build/universe_api
```

The backend runs locally on:

`http://127.0.0.1:8080`

Check backend health:

```bash
curl http://127.0.0.1:8080/api/v1/health
```

Check database connectivity:

```bash
curl http://127.0.0.1:8080/api/v1/health/database
```

## Step 7: Install and Run the Frontend

Open another terminal:

```bash
cd frontend

npm install

npm run dev
```

Open the application:

`http://127.0.0.1:5173`

The Vite proxy forwards `/api` requests to the C++ backend.

## Step 8: Build the Frontend

```bash
cd frontend

npm run build
```

The production frontend files are generated in the `dist/` directory.

---

# 14. Local Development Notes

- Start PostgreSQL before starting the backend.
- Keep the backend running while using the frontend.
- Run the frontend in a separate terminal.
- Apply all required migrations before using database-backed modules.
- Configure the local database connection before running the backend.
- Create or bootstrap an administrator account before testing administrative operations.
- Create departments, accounts and academic records before testing dependent workflows.
- Timetable entries require valid sections, courses, faculty members and classrooms.
- Student academic information depends on the student's program and section assignment.
- Sample records created on one local machine are not automatically included in a fresh database.

---

# 15. Project Demonstration Workflow

A recommended demonstration sequence is:

1. Start PostgreSQL.
2. Start the C++ Drogon backend.
3. Start the React frontend.
4. Sign in using the administrator account.
5. Open the administrator dashboard.
6. View university departments.
7. Review academic programs and courses.
8. Create or review student and faculty accounts.
9. Configure academic sections.
10. Review classroom and timetable records.
11. Sign in through the student login option.
12. Review the student portal and available academic information.

Some workflows may require additional database configuration or API/UI integration fixes depending on the local database state.

---

# 16. Team Members and Responsibilities

UniVerse is developed as a collaborative team project by three members.

| Team Member | Primary Responsibility                         |
| ----------- | ---------------------------------------------- |
| **Nanda**   | C++ Backend Development                        |
| **Deevan**  | Frontend Development                           |
| **Manoj**   | Database Management, Testing and Documentation |

### Nanda — Backend Development

Responsibilities:

- C++17 backend implementation.
- Drogon framework integration.
- REST API development.
- Authentication and session management.
- Controller and service-layer implementation.
- Repository-layer database integration.
- Backend validation and error handling.
- Backend build configuration using CMake.

### Deevan — Frontend Development

Responsibilities:

- React and TypeScript application development.
- Login interface and role selection.
- Administrator dashboard.
- Student portal interface.
- Faculty-facing interface.
- Academic management screens.
- Timetable and attendance interfaces.
- CSS styling and responsive layouts.
- Frontend-to-backend API integration.

### Manoj — Database, Testing and Documentation

Responsibilities:

- PostgreSQL database design.
- Relational table structures.
- SQL schema development.
- Database migrations.
- Foreign-key and uniqueness constraints.
- Sample academic data and database verification.
- Testing database operations and application workflows.
- Project documentation and README maintenance.

**Team Note:** This is the project's responsibility allocation. Individual contribution descriptions should reflect the actual work completed by each member.

---

# 17. Future Enhancements

Possible future improvements include:

- Complete faculty-specific dashboards and workflows.
- Full student and faculty profile management.
- Complete attendance recording and reporting.
- Examination scheduling and result publication.
- Full calendar and announcement management.
- Notifications for students and faculty.
- Timetable editing and cancellation.
- Search, filtering and advanced reporting.
- Password reset and first-login password change.
- Automated unit and integration tests.
- Production deployment with HTTPS.
- Database backup and recovery mechanisms.
- Audit logging and stronger access controls.

---

# 18. Academic Project Information

| Category          | Details                                                                  |
| ----------------- | ------------------------------------------------------------------------ |
| Project Name      | UniVerse — University Management System                                  |
| Project Type      | Web-Based Application                                                    |
| Primary Language  | C++                                                                      |
| Backend Framework | Drogon                                                                   |
| Frontend          | React and TypeScript                                                     |
| Database          | PostgreSQL                                                               |
| Build System      | CMake                                                                    |
| Version Control   | Git and GitHub                                                           |
| Team Members      | Nanda, Deevan and Manoj                                                  |
| Repository        | [https://github.com/Nanda-max926/ai](https://github.com/Nanda-max926/ai) |

---

# 19. License

This project is distributed under the MIT License.

See the [LICENSE](LICENSE) file for details.

---

\<p align="center">
&#x20; \<strong>UniVerse — Bringing University Operations Together.\</strong>
\</p>

\<p align="center">
&#x20; Developed with C++, React, TypeScript and PostgreSQL.
\</p>
