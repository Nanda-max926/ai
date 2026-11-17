# UniVerse — University Management System

**A centralized digital platform for university administration, academic management and student services.**

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![React](https://img.shields.io/badge/React-Frontend-61DAFB?logo=react&logoColor=black)
![TypeScript](https://img.shields.io/badge/TypeScript-Frontend-3178C6?logo=typescript&logoColor=white)
![Drogon](https://img.shields.io/badge/Backend-Drogon-228B22)
![PostgreSQL](https://img.shields.io/badge/Database-PostgreSQL-336791?logo=postgresql&logoColor=white)
![CMake](https://img.shields.io/badge/Build-CMake-064F8C?logo=cmake&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green)

---

## Table of Contents

1. [About the Project](#1-about-the-project)
2. [Problem Statement](#2-problem-statement)
3. [Project Objectives](#3-project-objectives)
4. [Features and Modules](#4-features-and-modules)
5. [Technology Stack](#5-technology-stack)
6. [Object-Oriented Programming](#6-object-oriented-programming-concepts)
7. [System Architecture](#7-system-architecture)
8. [Project Structure](#8-project-structure)
9. [Database Design](#9-database-design)
10. [API Overview](#10-api-overview)
11. [Installation and Setup](#11-installation-and-setup)
12. [Running the Application](#12-running-the-application)
13. [Testing and Implementation Status](#13-testing-and-implementation-status)
14. [Team Members](#14-team-members-and-responsibilities)
15. [Future Enhancements](#15-future-enhancements)
16. [License](#16-license)

---

## 1. About the Project

**UniVerse** is a web-based University Management System developed to simplify and centralize university administration and academic activities.

Universities manage large amounts of information related to students, faculty, departments, academic programs, courses, attendance, examinations, classrooms and timetables. Managing these activities through disconnected systems or manual processes can result in duplicated work, inconsistent records and difficulties accessing information.

UniVerse brings these operations together in one centralized platform.

The application provides separate interfaces for administrators, students and faculty members. Its backend is primarily developed using **C++17**, with PostgreSQL for database management and React with TypeScript for the frontend.

The project demonstrates practical application of:

- Object-Oriented Programming using C++.
- REST API development.
- Relational database management.
- Client-server architecture.
- Authentication and role-based access.
- Frontend and backend integration.

**Repository:** [github.com/Nanda-max926/ai](https://github.com/Nanda-max926/ai)

## 2. Problem Statement

University management involves several interconnected activities, including student registration, faculty management, department administration, course allocation, timetable scheduling, attendance recording and examination management.

Traditional or disconnected management approaches can create:

- Difficulty maintaining student and faculty records.
- Repeated data entry and inconsistent information.
- Problems coordinating academic departments.
- Manual timetable preparation and classroom allocation.
- Delayed access to academic information.
- Difficulty tracking attendance and examination records.
- Inefficient communication between administrators, students and faculty.

UniVerse aims to address these challenges by providing a centralized digital system in which university information can be managed and accessed according to user roles.

## 3. Project Objectives

The main objectives are:

1. Develop a centralized university management application.
2. Implement role-based authentication.
3. Provide separate interfaces for administrators, students and faculty.
4. Manage student, faculty and department information.
5. Organize academic programs, courses and class sections.
6. Support classroom allocation and timetable management.
7. Provide students with access to academic information.
8. Store university records using PostgreSQL.
9. Implement backend business logic using C++.
10. Apply Object-Oriented Programming principles.
11. Integrate frontend, backend and database components.

## 4. Features and Modules

### 4.1 Authentication and User Management

- Role-based login interface.
- Administrator, student and faculty account types.
- C++ backend authentication.
- Server-side session management.
- Account status validation.
- Password hashing using libsodium.

### 4.2 Administrator Dashboard

The administrator dashboard provides a central workspace for university operations.

- Administration overview.
- Student account creation.
- Faculty account creation.
- Department management.
- Academic program management.
- Course and subject management.
- Section management.
- Timetable management.
- Attendance management.
- Examinations and results.
- Calendar and announcements.

### 4.3 Department Management

- Department codes and names.
- Department records and contact information.
- Department-wise academic organization.
- Association of students and faculty with departments.

### 4.4 Academic Management

- Degree program creation.
- Course and subject management.
- Course credits and course types.
- Semester organization.
- Academic section creation.
- Admission batch information.
- Student capacity.
- Program-course associations.

### 4.5 Timetable Management

- Classroom and laboratory records.
- Room capacity and room types.
- Faculty allocation.
- Course and section allocation.
- Day and time selection.
- Timetable scheduling.
- Database-level schedule conflict constraints.

### 4.6 Student Portal

The student portal provides a dedicated workspace for accessing academic information.

- Student dashboard.
- My Courses.
- My Timetable.
- Attendance.
- Examinations and Results.
- Announcements.
- My Profile.
- Student academic information.

### 4.7 Faculty Portal

The faculty-facing interface is designed to provide access to university-related information and academic activities.

- Faculty dashboard interface.
- Faculty profile.
- Assigned course information.
- Timetable information.
- Student-related academic activities.
- Attendance-related functionality.

### 4.8 Attendance and Examination Management

The application includes interfaces and database-related structures for attendance, examinations and academic results. These modules require further end-to-end verification before production use.

### 4.9 Calendar and Announcements

The system includes database support for university calendar events and announcements, including publication and audience-related information.

> **Implementation note:** UniVerse is an academic project prototype. Its interfaces, backend services, database structures and sample workflows have been developed to varying degrees. Some modules still need additional API/UI integration and end-to-end testing. A screen being present does not necessarily mean every operation is production-ready.

## 5. Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Primary backend language |
| Drogon | C++ web framework |
| PostgreSQL | Relational database |
| SQL | Database schema and queries |
| libsodium | Password hashing |
| React | Frontend interface |
| TypeScript | Frontend logic and type safety |
| HTML5 | Page structure |
| CSS3 | Styling and layouts |
| Vite | Frontend development and build tool |
| CMake | C++ build configuration |
| Git | Version control |
| GitHub | Repository hosting and collaboration |

### Why C++?

C++ is the primary backend programming language in UniVerse.

It is used for:

- Server-side application logic.
- REST API controllers.
- Authentication and session handling.
- Service-layer operations.
- Database repository operations.
- Input validation.
- Error handling.
- PostgreSQL integration.

React and TypeScript provide the user interface, while C++ handles backend processing and business logic.
# 6. Object-Oriented Programming Concepts Used in UniVerse

Object-Oriented Programming (OOP) is one of the fundamental programming approaches used in the development of UniVerse.

The C++ backend is organized into classes and objects that represent different responsibilities of the university management system, such as authentication, academic management, student management, timetable operations and database access.

Instead of placing the entire application logic in a single file or function, the system separates functionality into controllers, services, repositories and domain models.

This approach improves code organization, maintainability, reusability and separation of responsibilities.

The following sections explain the OOP concepts and their relationship with the UniVerse implementation.

## 6.1 Classes and Objects

### Definition

A **class** is a user-defined data type that acts as a blueprint for creating objects. It groups related data members and member functions into one logical unit.

An **object** is an instance of a class. It can access the operations and data exposed by that class according to its access permissions.

Classes and objects are the foundation of Object-Oriented Programming in C++.

### Application in UniVerse

UniVerse uses classes to represent the different components of its backend.

Examples include:

| Class | Responsibility |
|---|---|
| `AuthController` | Handles authentication-related HTTP requests |
| `AdminController` | Handles administrator operations |
| `AcademicController` | Handles academic management API requests |
| `TimetableController` | Handles timetable-related requests |
| `StudentController` | Handles student-related API operations |
| `AuthService` | Contains authentication business logic |
| `AcademicService` | Coordinates academic management operations |
| `UserRepository` | Handles user-related database operations |
| `AcademicRepository` | Handles academic database operations |
| `TimetableRepository` | Handles timetable database operations |

These classes group functionality according to the responsibility of each module.

### Example from UniVerse

The following is a simplified illustration of how an academic service is organized:

```cpp
class AcademicService
{
private:
    std::shared_ptr<AcademicRepository> repository_;

public:
    void createProgram(
        std::int64_t departmentId,
        const std::string& code,
        const std::string& name
    );
};
```

An `AcademicService` object can be created and used to perform academic operations.

The actual project implementation includes additional parameters, callbacks and validation.

### Benefits

- Separates application functionality into logical units.
- Makes the backend easier to understand.
- Supports modular development.
- Allows multiple objects to work together.
- Makes individual modules easier to test and maintain.

---

## 6.2 Encapsulation

### Definition

**Encapsulation** is the process of combining data and the functions that operate on that data into a single unit, generally a class.

It also involves restricting direct access to internal data and implementation details using access specifiers.

C++ provides three main access specifiers:

| Access Specifier | Meaning |
|---|---|
| `public` | Accessible from outside the class |
| `private` | Accessible only inside the class and its friends |
| `protected` | Accessible inside the class and its derived classes |

### Application in UniVerse

Encapsulation is reflected in the separation of responsibilities between the backend classes.

For example:

- Repository classes contain database query implementations.
- Services expose operations for performing business tasks.
- Controllers handle incoming HTTP requests.
- Internal SQL operations are separated from the HTTP request-handling code.

A controller does not need to know every detail of how an academic record is inserted into PostgreSQL.

It can use the service and repository operations provided by the backend architecture.

### Example

```cpp
class AcademicRepository
{
private:
    // Internal database handling and implementation

public:
    void createProgram();
    void createCourse();
    void createSection();
};
```

Here, the database implementation can remain internal while the public methods provide controlled access to repository functionality.

This is a simplified illustration of encapsulation rather than a copy of the complete project header.

### Where it is used

- `AcademicRepository`
- `TimetableRepository`
- `UserRepository`
- `AuthService`
- `AcademicService`
- `AccountProvisioningService`

### Benefits

- Protects internal implementation details.
- Reduces unwanted direct access to internal operations.
- Makes code easier to modify.
- Improves modularity.
- Helps prevent unrelated components from depending on database implementation details.

---

## 6.3 Abstraction

### Definition

**Abstraction** means displaying essential functionality while hiding unnecessary implementation details.

The purpose of abstraction is to allow one part of a program to use another part without needing to understand its internal working.

In C++, abstraction can be achieved using classes, public interfaces, abstract classes and well-defined methods.

### Application in UniVerse

The UniVerse backend follows a layered architecture that demonstrates abstraction.

A request for creating an academic program passes through different components:

1. The controller receives the HTTP request.
2. The service handles the application operation.
3. The repository performs the required database operation.
4. PostgreSQL stores the academic record.

Each component uses the functionality exposed by the next layer without needing to implement all its internal details.

### Example from UniVerse

The academic service invokes the repository operation:

```cpp
repository_->createProgram(
    departmentId,
    code,
    name,
    degreeLevel,
    durationYears,
    totalSemesters,
    description,
    std::move(success),
    std::move(error)
);
```

The service does not directly execute the SQL statement.

The repository handles the database operation.

### Where it is used

- Controller-to-service communication.
- Service-to-repository communication.
- API communication between React and the C++ backend.
- Database operations hidden behind repository methods.

### Benefits

- Hides unnecessary implementation complexity.
- Reduces dependency between application layers.
- Makes the backend easier to extend.
- Allows database implementation details to change without rewriting the frontend.
- Improves code readability.

---

## 6.4 Inheritance

### Definition

**Inheritance** is an OOP mechanism in which one class derives properties and behaviours from another class.

The existing class is called the base class, and the new class is called the derived class.

Inheritance helps reuse common functionality and establish relationships between related classes.

### Types of Inheritance in C++

1. **Single Inheritance:** One derived class inherits from one base class.
2. **Multiple Inheritance:** One derived class inherits from multiple base classes.
3. **Multilevel Inheritance:** A class inherits from another derived class.
4. **Hierarchical Inheritance:** Multiple derived classes inherit from one base class.
5. **Hybrid Inheritance:** A combination of multiple inheritance structures.

### Application in UniVerse

The UniVerse backend integrates with the Drogon C++ web framework, which provides framework-level abstractions and mechanisms for request handling.

However, the application's controllers are primarily organized through their own classes and route-registration methods.

A custom base-controller and derived-controller inheritance hierarchy has not been established as a central part of the current application architecture.

### Illustrative C++ Example

The following example demonstrates how inheritance could be used to share common functionality between university user types:

```cpp
class UniversityUser
{
protected:
    std::string name;
    std::string email;

public:
    void displayProfile()
    {
        std::cout << name << std::endl;
    }
};

class Student : public UniversityUser
{
public:
    void viewCourses()
    {
        std::cout << "Displaying student courses";
    }
};

class Faculty : public UniversityUser
{
public:
    void viewClasses()
    {
        std::cout << "Displaying faculty classes";
    }
};
```

In this example:

- `UniversityUser` is the base class.
- `Student` is a derived class.
- `Faculty` is another derived class.
- Both derived classes inherit common user information.

This illustrates a possible extension of UniVerse. It should not be described as an existing source-code implementation unless this inheritance hierarchy is added to the project.

### Benefits

- Enables code reuse.
- Reduces duplication of common functionality.
- Supports hierarchical class design.
- Makes future extensions easier.

---

## 6.5 Polymorphism

### Definition

**Polymorphism** means having multiple forms.

It allows the same interface or operation to behave differently depending on the object or implementation being used.

C++ supports two major forms of polymorphism.

### A. Compile-Time Polymorphism

Compile-time polymorphism is resolved by the compiler.

It can be implemented using:

- Function overloading.
- Operator overloading.
- Templates.

Example:

```cpp
class Calculator
{
public:
    int add(int a, int b)
    {
        return a + b;
    }

    double add(double a, double b)
    {
        return a + b;
    }
};
```

The same function name, `add`, supports different parameter types.

### B. Runtime Polymorphism

Runtime polymorphism is implemented using inheritance and virtual functions.

Example:

```cpp
class Notification
{
public:
    virtual void send()
    {
        std::cout << "Sending notification";
    }

    virtual ~Notification() = default;
};

class EmailNotification : public Notification
{
public:
    void send() override
    {
        std::cout << "Sending email notification";
    }
};
```

The overridden function is selected at runtime according to the actual object type.

### Application in UniVerse

UniVerse uses asynchronous callbacks in its repository layer.

For example, database operations provide separate success and error callbacks.

```cpp
database_->execSqlAsync(
    "INSERT INTO academic_programs ...",

    [success = std::move(success)](
        const drogon::orm::Result& result
    )
    {
        success(result[0]["id"].as<std::int64_t>());
    },

    [error = std::move(error)](
        const drogon::orm::DrogonDbException& exception
    )
    {
        error(exception);
    }
);
```

These callbacks allow different outcomes of a database operation to be handled separately.

**Technical distinction:** Asynchronous callbacks are a form of event-driven programming, but they do not, by themselves, demonstrate C++ runtime polymorphism. Runtime polymorphism specifically requires a suitable inheritance and virtual-function relationship.

### Benefits

- Supports flexible interfaces.
- Allows different implementations behind a common interface.
- Helps extend software functionality.
- Enables different operation outcomes to be handled appropriately.

---

## 6.6 Composition

### Definition

**Composition** is an OOP relationship in which a class contains or uses objects of other classes to perform its operations.

Instead of putting every operation into one large class, functionality is divided among smaller components that cooperate.

Composition is especially useful for developing large applications.

### Application in UniVerse

Composition is an important part of the UniVerse backend architecture.

Examples include:

- `AcademicService` uses an `AcademicRepository`.
- `TimetableService` uses a `TimetableRepository`.
- Controllers coordinate with application services.
- Application modules work together to manage university operations.

### Example

```cpp
class AcademicService
{
private:
    std::shared_ptr<AcademicRepository> repository_;

public:
    explicit AcademicService(
        std::shared_ptr<AcademicRepository> repository
    )
        : repository_(std::move(repository))
    {
    }
};
```

Here:

- `AcademicService` contains a repository object through a shared pointer.
- The repository provides database functionality.
- The service uses that functionality to perform academic operations.

This allows each class to focus on its own responsibility.

### Architecture Example

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

### Benefits

- Promotes reusable components.
- Reduces large, complicated classes.
- Improves separation of responsibilities.
- Makes testing easier.
- Encourages modular application architecture.

---

## 6.7 Constructors and Destructors

### Definition

A **constructor** is a special member function that is automatically called when an object is created.

A **destructor** is a special member function that is automatically called when an object is destroyed.

Constructors are commonly used to initialize data members and establish object dependencies.

Destructors are used for cleanup and resource management.

### Application in UniVerse

Constructors are useful for initializing backend service and repository objects, including their dependencies.

For example, a service may receive a repository dependency during construction.

```cpp
class AcademicService
{
private:
    std::shared_ptr<AcademicRepository> repository_;

public:
    AcademicService(
        std::shared_ptr<AcademicRepository> repository
    )
        : repository_(std::move(repository))
    {
    }
};
```

The constructor initializes the repository member when an `AcademicService` object is created.

The use of smart pointers helps manage object lifetime automatically.

### Benefits

- Ensures objects are initialized.
- Helps manage dependencies.
- Supports resource management.
- Reduces the likelihood of resource-management errors.

---

## 6.8 Access Specifiers

### Definition

Access specifiers define how class members can be accessed.

C++ provides:

- `public`
- `private`
- `protected`

### Application in UniVerse

Access specifiers are used in class definitions to control access to member functions and data.

For example, a repository may expose database operations through public methods while keeping internal implementation details private.

### Example

```cpp
class UserRepository
{
private:
    // Internal implementation details

public:
    void findUser();
    void createUser();
};
```

The public methods provide access to the repository's functionality without exposing every internal detail.

### Benefits

- Controls access to class members.
- Supports encapsulation.
- Protects internal implementation details.
- Improves maintainability.

---

## 6.9 Summary of OOP Concepts

| Concept | Meaning | Application in UniVerse |
|---|---|---|
| Classes and Objects | Blueprint and its instances | Controllers, services and repositories |
| Encapsulation | Combining data and related operations | Repository and service classes |
| Abstraction | Hiding implementation details | Controller-service-repository layers |
| Inheritance | Deriving one class from another | Framework-level concepts; custom hierarchy is a possible extension |
| Polymorphism | One interface, multiple behaviours | Framework interfaces; callbacks handle asynchronous outcomes |
| Composition | Combining smaller objects | Services use repository objects |
| Constructors | Initialize objects | Service and repository dependency initialization |
| Destructors | Object cleanup | C++ object and resource lifetime management |
| Access Specifiers | Control member visibility | Public/private class members |

### Overall Importance of OOP in UniVerse

The use of OOP principles contributes to:

1. **Modularity:** Each class handles a specific responsibility.
2. **Reusability:** Common functionality can be reused across modules.
3. **Maintainability:** Changes to one component can be made without redesigning the complete application.
4. **Encapsulation:** Internal implementation details remain separated.
5. **Abstraction:** Complex database and business operations are hidden behind clear interfaces.
6. **Extensibility:** New university management modules can be added more easily.
7. **Code Organization:** The backend remains structured as the application grows.

### OOP Explanation for Project Evaluation

The following is a concise way to explain the implementation during a viva:

"UniVerse is developed using C++ and follows an object-oriented, layered architecture. We have organized the backend into controller, service and repository classes. Encapsulation is reflected in how each class manages its own responsibilities, while abstraction is achieved by separating HTTP handling, business logic and database operations. Composition is used when services work with repository objects. We understand inheritance and polymorphism as core C++ concepts and have documented their application and possible extensions separately from the callback-based asynchronous processing used by our current backend."



## 7. System Architecture

UniVerse follows a layered client-server architecture.

```text
                   USERS
                     |
                     v
        React + TypeScript Frontend
                     |
                     v
               Vite Server
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
          University Management Data
```

### Architecture Layers

**Frontend:** Login forms, dashboards, management pages, tables and student-facing screens.

**Controller Layer:** Receives HTTP requests, validates inputs and access permissions, and prepares responses.

**Service Layer:** Contains business logic and coordinates operations between controllers and repositories.

**Repository Layer:** Handles PostgreSQL queries and database operations.

**Database Layer:** Stores university users, profiles, departments, academic records and timetable information.

## 8. Project Structure

### Backend

```text
backend/
├── config/
│   ├── config.example.json
│   └── config.json
├── include/
│   ├── controllers/
│   ├── domain/
│   ├── middleware/
│   ├── repositories/
│   ├── services/
│   └── utils/
├── src/
│   ├── controllers/
│   ├── repositories/
│   ├── services/
│   └── main.cc
├── tests/
├── tools/
├── uploads/
└── CMakeLists.txt
```

### Frontend

```text
frontend/
├── public/
├── src/
│   ├── assets/
│   ├── pages/
│   │   ├── admin/
│   │   ├── auth/
│   │   └── student/
│   ├── services/
│   ├── App.tsx
│   ├── App.css
│   ├── index.css
│   └── main.tsx
├── package.json
├── tsconfig.json
└── vite.config.ts
```

## 9. Database Design

PostgreSQL is the primary relational database.

### Main Database Entities

- Users.
- Student profiles.
- Faculty profiles.
- Departments.
- Academic programs.
- Academic courses.
- Program-course associations.
- Academic sections.
- Academic rooms.
- Timetable entries.
- Calendar events.
- Announcements.

### Database Migrations

Database migrations are stored in `database/migrations/`.

| Migration | Purpose |
|---|---|
| `001_users.sql` | User accounts and authentication fields |
| `002_departments_profiles.sql` | Departments and student/faculty profiles |
| `003_temporary_credentials.sql` | Temporary credential support |
| `004_department_management.sql` | Department management |
| `005_academic_management.sql` | Programs, courses and sections |
| `006_timetable_management.sql` | Classrooms and timetable structures |
| `007_calendar_announcements.sql` | Calendar events and announcements |

Migrations should be applied in numerical order.

### Database Relationships

- A department can contain multiple students and faculty members.
- A department can offer multiple academic programs.
- An academic program can contain multiple courses.
- An academic program can have multiple sections.
- A section can have multiple timetable entries.
- A timetable entry is associated with a course, faculty member and classroom.
- Student profiles can be associated with academic programs and sections.

The database uses primary keys, foreign keys, unique constraints, validation checks and timetable overlap constraints to maintain data integrity.

## 10. API Overview

The backend exposes versioned REST endpoints under `/api/v1`.

| Endpoint | Method | Purpose |
|---|---|---|
| `/api/v1/health` | GET | Backend health check |
| `/api/v1/health/database` | GET | Database connectivity |
| `/api/v1/auth/login` | POST | User authentication |
| `/api/v1/auth/me` | GET | Current authenticated user |
| `/api/v1/auth/logout` | POST | End current session |
| `/api/v1/admin/departments` | GET | Retrieve departments |
| `/api/v1/admin/students` | POST | Create student accounts |
| `/api/v1/admin/faculty` | POST | Create faculty accounts |
| `/api/v1/admin/academic/programs` | GET/POST | Retrieve or create programs |
| `/api/v1/admin/academic/courses` | GET/POST | Retrieve or create courses |
| `/api/v1/admin/academic/sections` | GET/POST | Retrieve or create sections |

Additional timetable, student, calendar and announcement routes are registered through the backend controllers.

## 11. Installation and Setup

The following instructions describe local development on Ubuntu/Linux.

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

Install Node.js and npm for the frontend.

Check installed versions:

```bash
g++ --version
cmake --version
psql --version
node -v
npm -v
```

### Step 1: Clone the Repository

```bash
git clone https://github.com/Nanda-max926/ai.git
cd ai
```

### Step 2: Create PostgreSQL User and Database

Open the PostgreSQL shell:

```bash
sudo -u postgres psql
```

Execute:

```sql
CREATE USER universe_user WITH PASSWORD 'CHANGE_THIS_PASSWORD';

CREATE DATABASE universe_db OWNER universe_user;

\q
```

Replace the example password with a secure local password.

### Step 3: Apply Database Migrations

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

### Step 4: Configure the Backend

Copy the example configuration:

```bash
cp backend/config/config.example.json backend/config/config.json
```

Edit the local configuration:

```bash
code backend/config/config.json
```

Update PostgreSQL connection details and other required settings.

**Never commit `backend/config/config.json` or local passwords.**

### Step 5: Build the C++ Backend

```bash
cd backend

cmake -S . -B build

cmake --build build -j2
```

## 12. Running the Application

### Start the Backend

From the `backend` directory:

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

### Start the Frontend

Open another terminal:

```bash
cd ~/projects/ai/frontend

npm install

npm run dev
```

Open:

`http://127.0.0.1:5173`

The Vite development proxy forwards `/api` requests to the C++ backend.

### Build the Frontend

```bash
cd ~/projects/ai/frontend

npm run build
```

Production frontend files are generated in `dist/`.

## 13. Testing and Implementation Status

The following backend checks have been performed during development:

- C++ backend builds successfully with CMake.
- Frontend production build succeeds with Vite and TypeScript.
- Backend health endpoint returns HTTP 200.
- PostgreSQL connectivity endpoint returns HTTP 200.
- Department records can be retrieved from PostgreSQL.
- Sample academic programs, courses, sections and timetable records have been inserted and verified directly in the database.

Some academic and timetable frontend operations still require API/UI integration fixes and further testing.

The project should therefore be considered an academic prototype rather than a production-ready university platform.

## 14. Team Members and Responsibilities

UniVerse is developed collaboratively by three team members.

| Team Member | Primary Responsibility |
|---|---|
| **Nanda** | C++ Backend Development |
| **Deevan** | Frontend Development |
| **Manoj** | Database Management, Testing and Documentation |

### Nanda — Backend Development

- C++17 backend implementation.
- Drogon framework integration.
- REST API development.
- Authentication and session management.
- Controller and service-layer implementation.
- Repository-layer integration.
- Backend validation and error handling.
- CMake build configuration.

### Deevan — Frontend Development

- React and TypeScript development.
- Login interface and role selection.
- Administrator dashboard.
- Student portal interface.
- Faculty-facing interface.
- Academic management screens.
- Timetable and attendance interfaces.
- CSS styling and responsive layouts.
- Frontend API integration.

### Manoj — Database, Testing and Documentation

- PostgreSQL database design.
- Relational table structures.
- SQL schema development.
- Database migrations.
- Foreign-key and uniqueness constraints.
- Sample academic data.
- Database verification and testing.
- Project documentation.

*The responsibilities listed above describe the team's planned division of work and should be adjusted to reflect actual individual contributions.*

## 15. Future Enhancements

- Complete faculty-specific dashboards and workflows.
- Full student and faculty profile management.
- Complete attendance recording and reporting.
- Examination scheduling and result publication.
- Full calendar and announcement management.
- Notifications for students and faculty.
- Timetable editing and cancellation.
- Search, filtering and reporting.
- Password reset and first-login password change.
- Automated unit, integration and API tests.
- Production deployment with HTTPS.
- Database backup and recovery.
- Audit logging and stronger access controls.

## 16. License

This project is distributed under the **MIT License**.

See the [LICENSE](LICENSE) file for details.

---

**UniVerse — Bringing University Operations Together.**

*Developed with C++, Drogon, React, TypeScript and PostgreSQL.*

**Team: Nanda | Deevan | Manoj**