
#if __has_include(<libpq-fe.h>)
#include <libpq-fe.h>
#elif __has_include(<postgresql/libpq-fe.h>)
#include <postgresql/libpq-fe.h>
#else
#error "libpq development headers not found"
#endif

#if __has_include(<sodium.h>)
#include <sodium.h>
#else
#error "libsodium development headers not found"
#endif

#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <memory>
#include <cctype>

namespace
{

struct ResultDeleter
{
    void operator()(PGresult *result) const
    {
        if (result)
            PQclear(result);
    }
};

using QueryResult =
    std::unique_ptr<PGresult, ResultDeleter>;

void clearSecret(std::string &value)
{
    if (!value.empty())
    {
        sodium_memzero(value.data(), value.size());
        value.clear();
    }
}

std::string readHidden(const std::string &prompt)
{
    std::cout << prompt << std::flush;

    termios oldSettings{};

    if (tcgetattr(STDIN_FILENO, &oldSettings) != 0)
    {
        std::cerr << "\nCannot access terminal settings.\n";
        return {};
    }

    termios newSettings = oldSettings;
    newSettings.c_lflag &= static_cast<tcflag_t>(~ECHO);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newSettings) != 0)
    {
        std::cerr << "\nCannot disable password echo.\n";
        return {};
    }

    std::string value;
    std::getline(std::cin, value);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldSettings);

    std::cout << '\n';

    return value;
}

bool validLoginId(const std::string &value)
{
    if (value.size() < 3 || value.size() > 80)
        return false;

    for (unsigned char ch : value)
    {
        if (!std::isalnum(ch) &&
            ch != '_' && ch != '-' && ch != '.')
        {
            return false;
        }
    }

    return true;
}

bool validEmail(const std::string &value)
{
    if (value.empty() || value.size() > 254)
        return false;

    const auto at = value.find('@');
    const auto dot = value.find('.', at);

    return at != std::string::npos &&
           at > 0 &&
           dot != std::string::npos &&
           dot > at + 1 &&
           dot + 1 < value.size();
}

bool hashPassword(
    const std::string &password,
    std::string &outputHash
)
{
    if (password.size() < 12 || password.size() > 1024)
        return false;

    char hash[crypto_pwhash_STRBYTES] = {};

    const int result = crypto_pwhash_str(
        hash,
        password.data(),
        static_cast<unsigned long long>(password.size()),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    );

    if (result != 0)
    {
        sodium_memzero(hash, sizeof(hash));
        return false;
    }

    outputHash = hash;

    sodium_memzero(hash, sizeof(hash));

    return true;
}

} // namespace

int main()
{
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "     UNIVERSE ADMIN BOOTSTRAP\n";
    std::cout << "========================================\n\n";

    // Initialize libsodium.
    if (sodium_init() < 0)
    {
        std::cerr << "Password security initialization failed.\n";
        return 1;
    }

    // Collect account information.
    std::string loginId;
    std::string email;
    std::string password;
    std::string confirmation;

    std::cout << "Admin Login ID: ";
    std::getline(std::cin, loginId);

    std::cout << "Admin Email: ";
    std::getline(std::cin, email);

    if (!validLoginId(loginId))
    {
        std::cerr << "Invalid login ID.\n";
        return 1;
    }

    if (!validEmail(email))
    {
        std::cerr << "Invalid email format.\n";
        return 1;
    }

    password = readHidden("Create Admin Password: ");

    if (!std::cin.good() || password.size() < 12 ||
        password.size() > 1024)
    {
        clearSecret(password);
        std::cerr << "Password must contain at least 12 characters.\n";
        return 1;
    }

    confirmation = readHidden("Confirm Admin Password: ");

    if (!std::cin.good() || password != confirmation)
    {
        clearSecret(password);
        clearSecret(confirmation);

        std::cerr << "Passwords do not match.\n";
        return 1;
    }

    clearSecret(confirmation);

    // Hash password before database insertion.
    std::string passwordHash;

    if (!hashPassword(password, passwordHash))
    {
        clearSecret(password);
        std::cerr << "Password hashing failed.\n";
        return 1;
    }

    clearSecret(password);

    // Connect using PostgreSQL environment variables.
    PGconn *connection = PQconnectdb("");

    if (!connection ||
        PQstatus(connection) != CONNECTION_OK)
    {
        std::cerr << "PostgreSQL connection failed: "
                  << (connection
                      ? PQerrorMessage(connection)
                      : "Unable to create connection")
                  << '\n';

        if (connection)
            PQfinish(connection);

        return 1;
    }

    // Start transaction.
    QueryResult transaction(
        PQexec(connection, "BEGIN")
    );

    if (!transaction ||
        PQresultStatus(transaction.get()) != PGRES_COMMAND_OK)
    {
        std::cerr << "Unable to start database transaction.\n";
        PQfinish(connection);
        return 1;
    }

    // Prevent simultaneous first-admin bootstrap attempts.
    QueryResult lockResult(
        PQexec(
            connection,
            "LOCK TABLE users IN EXCLUSIVE MODE"
        )
    );

    if (!lockResult ||
        PQresultStatus(lockResult.get()) != PGRES_COMMAND_OK)
    {
        PQexec(connection, "ROLLBACK");

        std::cerr << "Unable to lock the users table: "
                  << PQerrorMessage(connection) << '\n';

        sodium_memzero(passwordHash.data(), passwordHash.size());
        PQfinish(connection);
        return 1;
    }

    // Allow bootstrap only if no admin account exists.
    QueryResult check(
        PQexec(
            connection,
            "SELECT 1 FROM users "
            "WHERE role = 'admin'::app_role "
            "LIMIT 1"
        )
    );

    if (!check ||
        PQresultStatus(check.get()) != PGRES_TUPLES_OK)
    {
        PQexec(connection, "ROLLBACK");

        std::cerr << "Unable to check existing administrators: "
                  << PQerrorMessage(connection) << '\n';

        sodium_memzero(passwordHash.data(), passwordHash.size());
        PQfinish(connection);
        return 1;
    }

    if (PQntuples(check.get()) > 0)
    {
        PQexec(connection, "ROLLBACK");

        std::cout << "An administrator already exists.\n";
        std::cout << "Use the authorized admin management process.\n";

        sodium_memzero(passwordHash.data(), passwordHash.size());
        PQfinish(connection);
        return 0;
    }

    // Insert the first administrator with parameterized SQL.
    const char *parameters[] = {
        loginId.c_str(),
        email.c_str(),
        passwordHash.c_str()
    };

    QueryResult inserted(
        PQexecParams(
            connection,

            "INSERT INTO users "
            "(login_id, email, password_hash, role, status, "
            "must_change_password) "
            "VALUES ($1, $2, $3, "
            "'admin'::app_role, "
            "'active'::account_status, FALSE) "
            "RETURNING id",

            3,
            nullptr,
            parameters,
            nullptr,
            nullptr,
            0
        )
    );

    sodium_memzero(passwordHash.data(), passwordHash.size());

    if (!inserted ||
        PQresultStatus(inserted.get()) != PGRES_TUPLES_OK)
    {
        PQexec(connection, "ROLLBACK");

        std::cerr << "Admin creation failed: "
                  << PQerrorMessage(connection) << '\n';

        PQfinish(connection);
        return 1;
    }

    const std::string adminId =
        PQgetvalue(inserted.get(), 0, 0);

    QueryResult committed(
        PQexec(connection, "COMMIT")
    );

    if (!committed ||
        PQresultStatus(committed.get()) != PGRES_COMMAND_OK)
    {
        PQexec(connection, "ROLLBACK");

        std::cerr << "Transaction commit failed: "
                  << PQerrorMessage(connection) << '\n';

        PQfinish(connection);
        return 1;
    }

    std::cout << "\n========================================\n";
    std::cout << " SUPER ADMIN CREATED SUCCESSFULLY\n";
    std::cout << "========================================\n";
    std::cout << "Admin ID: " << adminId << '\n';
    std::cout << "Login ID: " << loginId << '\n';
    std::cout << "Email: " << email << '\n';
    std::cout << "Role: admin\n";
    std::cout << "Status: active\n";
    std::cout << "========================================\n";

    PQfinish(connection);

    return 0;
}
