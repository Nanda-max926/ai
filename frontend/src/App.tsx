
import { useEffect, useState } from "react";
import "./App.css";

import Login from "./pages/auth/Login";
import AdminDashboard from "./pages/admin/AdminDashboard";
import StudentDashboard from "./pages/student/StudentDashboard";

import {
  getCurrentUser,
  logoutUser,
  type AuthUser,
} from "./services/api";

function BasicRoleDashboard({
  user,
  onLogout,
}: {
  user: AuthUser;
  onLogout: () => void;
}) {
  const title =
    user.role === "faculty" ? "Faculty Portal" : "Department Portal";

  return (
    <main className="basic-dashboard">
      <header className="basic-header">
        <div>
          <div className="brand-mark">U</div>
          <strong>UniVerse</strong>
        </div>
        <button className="logout-button" onClick={onLogout}>
          Sign out
        </button>
      </header>

      <section className="basic-welcome">
        <span>AUTHENTICATED ACCOUNT</span>
        <h1>Welcome, {user.loginId}</h1>
        <p>Your {title} account is authenticated.</p>
        <div className="basic-user-card">
          <p><strong>Account ID:</strong> {user.id}</p>
          <p><strong>Email:</strong> {user.email}</p>
          <p><strong>Role:</strong> {user.role}</p>
        </div>
        <p className="development-note">
          Faculty workspace is the next portal to be connected.
        </p>
      </section>
    </main>
  );
}

function App() {
  const [user, setUser] = useState<AuthUser | null>(null);
  const [checkingSession, setCheckingSession] = useState(true);

  useEffect(() => {
    let mounted = true;

    getCurrentUser()
      .then((currentUser) => {
        if (mounted) setUser(currentUser);
      })
      .catch(() => {
        if (mounted) setUser(null);
      })
      .finally(() => {
        if (mounted) setCheckingSession(false);
      });

    return () => {
      mounted = false;
    };
  }, []);

  async function handleLogout() {
    try {
      await logoutUser();
    } catch (error) {
      console.error("Logout request failed:", error);
    } finally {
      setUser(null);
    }
  }

  if (checkingSession) {
    return (
      <div className="app-loading">
        <div className="loading-mark">U</div>
        <p>Loading UniVerse...</p>
      </div>
    );
  }

  if (!user) {
    return (
      <Login onLogin={(authenticatedUser) => setUser(authenticatedUser)} />
    );
  }

  if (user.role === "admin") {
    return <AdminDashboard user={user} onLogout={handleLogout} />;
  }

  if (user.role === "student") {
    return <StudentDashboard user={user} onLogout={handleLogout} />;
  }

  return <BasicRoleDashboard user={user} onLogout={handleLogout} />;
}

export default App;