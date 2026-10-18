import { useEffect, useState, type FormEvent, type ChangeEvent, } from "react";
import "./DepartmentManagement.css";

interface Department {
  id: number;
  code: string;
  name: string;
  isActive: boolean;
  description: string;
  officeLocation: string;
  contactEmail: string;
  contactPhone: string;
  hodUserId: number;
  updatedAt: string;
}

interface DepartmentForm {
  code: string;
  name: string;
  description: string;
  officeLocation: string;
  contactEmail: string;
  contactPhone: string;
}

const emptyForm: DepartmentForm = {
  code: "",
  name: "",
  description: "",
  officeLocation: "",
  contactEmail: "",
  contactPhone: "",
};

async function apiRequest<T>(
  path: string,
  method = "GET",
  body?: unknown
): Promise<T> {
  const response = await fetch(path, {
    method,
    credentials: "include",
    headers: { "Content-Type": "application/json" },
    ...(body === undefined ? {} : { body: JSON.stringify(body) }),
  });

  const data = await response.json().catch(() => ({}));

  if (!response.ok) {
    throw new Error(data.message || `Request failed (${response.status})`);
  }

  return data as T;
}

export default function DepartmentManagement() {
  const [departments, setDepartments] = useState<Department[]>([]);
  const [form, setForm] = useState<DepartmentForm>(emptyForm);
  const [editingId, setEditingId] = useState<number | null>(null);
  const [loading, setLoading] = useState(true);
  const [saving, setSaving] = useState(false);
  const [error, setError] = useState("");
  const [notice, setNotice] = useState("");
  const [search, setSearch] = useState("");
  const [showInactive, setShowInactive] = useState(true);

       async function loadDepartments() {
     try {
      const result = await apiRequest<{
        status: string;
        departments: Department[];
      }>("/api/v1/admin/departments/full");

      setDepartments(result.departments || []);
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Could not load departments."
      );
    } finally {
      setLoading(false);
    }
  }

  useEffect(() => {
    void loadDepartments();
  }, []);

  function updateField(
    event: ChangeEvent<HTMLInputElement | HTMLTextAreaElement>
  ) {
    const { name, value } = event.target;
    setForm((current) => ({ ...current, [name]: value }));
  }

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setSaving(true);
    setError("");
    setNotice("");

    try {
      const body = editingId === null
        ? form
        : { id: editingId, ...form };

      const result = await apiRequest<{ message: string }>(
        "/api/v1/admin/departments",
        editingId === null ? "POST" : "PUT",
        body
      );

      setNotice(result.message);
      setForm(emptyForm);
      setEditingId(null);
      await loadDepartments();
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Department operation failed."
      );
    } finally {
      setSaving(false);
    }
  }

  function startEdit(department: Department) {
    setEditingId(department.id);
    setForm({
      code: department.code,
      name: department.name,
      description: department.description,
      officeLocation: department.officeLocation,
      contactEmail: department.contactEmail,
      contactPhone: department.contactPhone,
    });
    setNotice("");
    setError("");
    window.scrollTo({ top: 0, behavior: "smooth" });
  }

  async function toggleStatus(department: Department) {
    const action = department.isActive ? "deactivate" : "activate";

    if (!window.confirm(`Are you sure you want to ${action} ${department.name}?`)) {
      return;
    }

    setError("");
    setNotice("");

    try {
      const result = await apiRequest<{ message: string }>(
        "/api/v1/admin/departments/status",
        "PATCH",
        {
          id: department.id,
          isActive: !department.isActive,
        }
      );

      setNotice(result.message);
      await loadDepartments();
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Status update failed."
      );
    }
  }

  const visibleDepartments = departments.filter((department) => {
    const matchesSearch =
      department.name.toLowerCase().includes(search.toLowerCase()) ||
      department.code.toLowerCase().includes(search.toLowerCase());

    return (
      matchesSearch &&
      (showInactive || department.isActive)
    );
  });

  const activeCount = departments.filter((d) => d.isActive).length;

  return (
    <div className="department-page">
      <div className="department-heading">
        <div>
          <span className="department-eyebrow">UNIVERSE / ADMINISTRATION</span>
          <h2>Department Management</h2>
          <p>Manage university departments and their contact information.</p>
        </div>

       <button
               className="department-refresh"
                       onClick={() => {
                                setLoading(true);
                                 setError("");
                          void loadDepartments();
  } }
>
  ↻ Refresh
</button>
      </div>

      <div className="department-stats">
        <div className="department-stat">
          <span>Total departments</span>
          <strong>{departments.length}</strong>
        </div>

        <div className="department-stat">
          <span>Active departments</span>
          <strong>{activeCount}</strong>
        </div>

        <div className="department-stat">
          <span>Inactive departments</span>
          <strong>{departments.length - activeCount}</strong>
        </div>
      </div>

      <section className="department-panel">
        <h3>{editingId === null ? "Create Department" : "Edit Department"}</h3>

        {error && <div className="department-error">{error}</div>}
        {notice && <div className="department-success">{notice}</div>}

        <form onSubmit={handleSubmit}>
          <div className="department-form-grid">
            <label>
              Department Code *
              <input
                name="code"
                value={form.code}
                onChange={updateField}
                maxLength={20}
                placeholder="CSE"
                required
              />
            </label>

            <label>
              Department Name *
              <input
                name="name"
                value={form.name}
                onChange={updateField}
                maxLength={150}
                placeholder="Computer Science and Engineering"
                required
              />
            </label>

            <label>
              Office Location
              <input
                name="officeLocation"
                value={form.officeLocation}
                onChange={updateField}
                maxLength={160}
                placeholder="Block A, Room 101"
              />
            </label>

            <label>
              Contact Email
              <input
                name="contactEmail"
                type="email"
                value={form.contactEmail}
                onChange={updateField}
                maxLength={254}
                placeholder="cse@universe.edu"
              />
            </label>

            <label>
              Contact Phone
              <input
                name="contactPhone"
                value={form.contactPhone}
                onChange={updateField}
                maxLength={30}
                placeholder="Department contact number"
              />
            </label>
          </div>

          <label className="department-description">
            Description
            <textarea
              name="description"
              value={form.description}
              onChange={updateField}
              maxLength={2000}
              rows={3}
              placeholder="Describe the department..."
            />
          </label>

          <div className="department-form-actions">
            <button type="submit" disabled={saving}>
              {saving
                ? "Saving..."
                : editingId === null
                  ? "Create Department"
                  : "Save Changes"}
            </button>

            {editingId !== null && (
              <button
                type="button"
                className="department-cancel"
                onClick={() => {
                  setEditingId(null);
                  setForm(emptyForm);
                }}
              >
                Cancel Edit
              </button>
            )}
          </div>
        </form>
      </section>

      <section className="department-panel">
        <div className="department-list-heading">
          <h3>All Departments</h3>

          <input
            className="department-search"
            value={search}
            onChange={(event) => setSearch(event.target.value)}
            placeholder="Search by name or code..."
          />
        </div>

        <label className="department-filter">
          <input
            type="checkbox"
            checked={showInactive}
            onChange={(event) => setShowInactive(event.target.checked)}
          />
          Show inactive departments
        </label>

        {loading ? (
          <p>Loading departments...</p>
        ) : visibleDepartments.length === 0 ? (
          <p>No matching departments found.</p>
        ) : (
          <div className="department-table-wrap">
            <table className="department-table">
              <thead>
                <tr>
                  <th>Code</th>
                  <th>Department</th>
                  <th>Contact</th>
                  <th>Status</th>
                  <th>Actions</th>
                </tr>
              </thead>

              <tbody>
                {visibleDepartments.map((department) => (
                  <tr key={department.id}>
                    <td><strong>{department.code}</strong></td>
                    <td>
                      <strong>{department.name}</strong>
                      <small>{department.officeLocation || "Location not set"}</small>
                    </td>
                    <td>
                      {department.contactEmail || "No email"}
                      <small>{department.contactPhone}</small>
                    </td>
                    <td>
                      <span className={
                        department.isActive
                          ? "department-badge-active"
                          : "department-badge-inactive"
                      }>
                        {department.isActive ? "Active" : "Inactive"}
                      </span>
                    </td>
                    <td>
                      <div className="department-row-actions">
                        <button onClick={() => startEdit(department)}>
                          Edit
                        </button>
                        <button
                          className="department-status-button"
                          onClick={() => void toggleStatus(department)}
                        >
                          {department.isActive ? "Deactivate" : "Activate"}
                        </button>
                      </div>
                    </td>
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