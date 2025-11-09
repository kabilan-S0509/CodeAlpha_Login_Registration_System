#include <iostream>
#include <fstream>
#include <string>
#include <functional>   // std::hash
#include <cctype>

using namespace std;

// --------- Utilities ---------
string sanitizeUsername(const string& u) {
    // allow letters, digits, underscore, hyphen
    string s;
    for (char c : u) {
        if (isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-') s += c;
    }
    return s;
}

string userFile(const string& username) {
    return sanitizeUsername(username) + ".usr";
}

string hashPassword(const string& password, const string& salt) {
    // For assignment/demo: std::hash (NOT crypto-secure; fine for this project).
    // In real apps, use Argon2/bcrypt/PBKDF2.
    return to_string(hash<string>{}(salt + password));
}

bool readUserRecord(const string& username, string& storedUser, string& storedSalt, string& storedHash) {
    ifstream in(userFile(username));
    if (!in) return false;
    getline(in, storedUser);
    getline(in, storedSalt);
    getline(in, storedHash);
    return true;
}

bool writeUserRecord(const string& username, const string& salt, const string& passHash) {
    ofstream out(userFile(username), ios::trunc);
    if (!out) return false;
    out << username << '\n' << salt << '\n' << passHash;
    return true;
}

bool isStrongPassword(const string& p) {
    // Minimal demo policy: length >= 6, has letter and digit
    if (p.size() < 6) return false;
    bool hasAlpha = false, hasDigit = false;
    for (char c : p) {
        if (isalpha(static_cast<unsigned char>(c))) hasAlpha = true;
        if (isdigit(static_cast<unsigned char>(c))) hasDigit = true;
    }
    return hasAlpha && hasDigit;
}

// --------- Features ---------
void registerUser() {
    cout << "\n========== REGISTER ==========\n";
    cout << "Enter username (letters/digits/_/-): ";
    string username; cin >> username;
    username = sanitizeUsername(username);

    if (username.empty()) {
        cout << "❌ Invalid username.\n";
        return;
    }

    ifstream dupe(userFile(username));
    if (dupe.good()) {
        cout << "⚠️  Username already exists. Choose another.\n";
        return;
    }

    cout << "Enter password (min 6 chars, letters+digits): ";
    string password; cin >> password;
    if (!isStrongPassword(password)) {
        cout << "❌ Weak password. Use at least 6 chars with letters and digits.\n";
        return;
    }

    // Per-user salt (simple, deterministic for demo)
    string salt = "CA_SALT_" + username + "_v1";
    string passHash = hashPassword(password, salt);

    if (!writeUserRecord(username, salt, passHash)) {
        cout << "❌ Could not save user file.\n";
        return;
    }

    cout << "✅ Registration successful for: " << username << "\n";
}

void loginUser() {
    cout << "\n=========== LOGIN ===========\n";
    cout << "Username: ";
    string username; cin >> username;

    string storedUser, storedSalt, storedHash;
    if (!readUserRecord(username, storedUser, storedSalt, storedHash)) {
        cout << "❌ No such user. Please register first.\n";
        return;
    }

    cout << "Password: ";
    string password; cin >> password;

    string attemptHash = hashPassword(password, storedSalt);
    if (storedUser == username && attemptHash == storedHash) {
        cout << "✅ Login successful. Welcome, " << username << "!\n";
    } else {
        cout << "❌ Incorrect username or password.\n";
    }
}

void resetPassword() {
    cout << "\n======= RESET PASSWORD =======\n";
    cout << "Username: ";
    string username; cin >> username;

    string storedUser, storedSalt, storedHash;
    if (!readUserRecord(username, storedUser, storedSalt, storedHash)) {
        cout << "❌ No such user.\n";
        return;
    }

    // Simple reset flow (for assignment). In real apps, verify old password or email/OTP.
    cout << "Enter NEW password (min 6 chars, letters+digits): ";
    string newPass; cin >> newPass;
    if (!isStrongPassword(newPass)) {
        cout << "❌ Weak password. Use at least 6 chars with letters and digits.\n";
        return;
    }

    string newHash = hashPassword(newPass, storedSalt);
    if (!writeUserRecord(username, storedSalt, newHash)) {
        cout << "❌ Failed to update password.\n";
        return;
    }
    cout << "✅ Password updated for " << username << ".\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "======================================\n";
    cout << "     LOGIN & REGISTRATION SYSTEM      \n";
    cout << "======================================\n";

    while (true) {
        cout << "\n1) Register\n2) Login\n3) Reset Password\n4) Exit\n";
        cout << "Enter choice: ";
        int ch; 
        if (!(cin >> ch)) { 
            cout << "❌ Invalid input. Exiting.\n"; 
            return 0; 
        }

        switch (ch) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: resetPassword(); break;
            case 4: cout << "👋 Bye!\n"; return 0;
            default: cout << "⚠️  Invalid choice. Try again.\n";
        }
    }
}
