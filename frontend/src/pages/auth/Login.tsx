
import { useState, type FormEvent } from "react";

import {
  loginUser,
  type AuthUser,
  type UserRole,
} from "../../services/api";

interface LoginProps {
  onLogin: (user: AuthUser) => void;
}

const roles: {
  value: UserRole;
  label: string;
  description: string;
  icon: string;
}[] = [
  {
    value: "student",
    label: "Student",
    description: "Access your academic account",
    icon: "🎓",
  },
  {
    value: "faculty",
    label: "Faculty",
    description: "Manage classes and students",
    icon: "👨‍🏫",
  },
  {
    value: "admin",
    label: "Administrator",
    description: "Manage university operations",
    icon: "🛡️",
  },
];

export default function Login({ onLogin }: LoginProps) {
  const [role, setRole] = useState<UserRole>("student");
  const [loginId, setLoginId] = useState("");
  const [password, setPassword] = useState("");

  const [error, setError] = useState("");
  const [loading, setLoading] = useState(false);

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    setError("");
    setLoading(true);

    try {
      const result = await loginUser(
        loginId.trim(),
        password,
        role
      );

      setPassword("");

      onLogin(result.user);
    } catch (err) {
      setError(
        err instanceof Error
          ? err.message
          : "Unable to log in. Please try again."
      );
    } finally {
      setLoading(false);
    }
  }

  return (
    <main className="login-screen">

      <section className="login-card">

        <div className="login-brand">
          <div className="login-logo">U</div>

          <div>
            <h1>UniVerse</h1>
            <span>UNIVERSITY MANAGEMENT SYSTEM</span>
          </div>
        </div>

        <div className="login-heading">
          <p className="login-eyebrow">WELCOME BACK</p>
          <h2>Sign in to your portal</h2>
          <p>Choose your account type and enter your credentials.</p>
        </div>

        <div className="login-role-grid">

          {roles.map((item) => (
            <button
              type="button"
              key={item.value}
              onClick={() => {
                setRole(item.value);
                setError("");
              }}
              className={
                role === item.value
                  ? "role-card role-card-active"
                  : "role-card"
              }
            >
              <span className="role-card-icon">{item.icon}</span>
              <strong>{item.label}</strong>
              <small>{item.description}</small>
            </button>
          ))}

        </div>

        <form onSubmit={handleSubmit} className="login-form">

          <label>
            {role === "student"
              ? "Registration Number"
              : role === "faculty"
              ? "Faculty ID"
              : "Admin Login ID"}

            <input
              type="text"
              value={loginId}
              onChange={(event) =>
                setLoginId(event.currentTarget.value)
              }
              placeholder={
                role === "student"
                  ? "Enter registration number"
                  : role === "faculty"
                  ? "Enter faculty ID"
                  : "Enter administrator ID"
              }
              autoComplete="username"
              required
            />
          </label>

          <label>
            Password

            <input
              type="password"
              value={password}
              onChange={(event) =>
                setPassword(event.currentTarget.value)
              }
              placeholder="Enter your password"
              autoComplete="current-password"
              required
            />
          </label>

          {error && (
            <div className="login-error" role="alert">
              {error}
            </div>
          )}

          <button
            type="submit"
            className="login-submit"
            disabled={loading}
          >
            {loading ? "Signing in..." : "Sign In →"}
          </button>

        </form>

        <div className="login-footer">
          <p>Secure University Access</p>
          <span>UniVerse · Student Project</span>
        </div>

      </section>
    </main>
  );
}
