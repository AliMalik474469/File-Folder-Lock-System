/*
 * ╔══════════════════════════════════════════════════════════╗
 * ║       FILE & FOLDER LOCK / HIDE SYSTEM (OOP C++)        ║
 * ║              Works on Windows (Dev-C++)                  ║
 * ╚══════════════════════════════════════════════════════════╝
 *
 * Features:
 *  - Lock (hide) files and folders using attrib command
 *  - Unlock (unhide) files and folders
 *  - Set a password to protect the system
 *  - View all currently locked items
 *  - OOP design: Base class + derived classes
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <windows.h>
using namespace std;

// ─────────────────────────────────────────────
// Utility: Clear screen & colored text (Windows)
// ─────────────────────────────────────────────
void clearScreen() { system("cls"); }

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetColor() { setColor(7); }

void printBanner() {
    setColor(11); // Cyan
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════╗\n";
    cout << "  ║      FILE & FOLDER LOCK SYSTEM v1.0      ║\n";
    cout << "  ║         Developed in OOP C++             ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n";
    resetColor();
}

// ─────────────────────────────────────────────
// Base Class: LockableItem
// ─────────────────────────────────────────────
class LockableItem {
protected:
    string path;
    bool isLocked;

public:
    LockableItem(string p) : path(p), isLocked(false) {}

    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void displayInfo() = 0;

    string getPath() { return path; }
    bool getStatus() { return isLocked; }

    virtual ~LockableItem() {}
};

// ─────────────────────────────────────────────
// Derived Class: FileLocker
// ─────────────────────────────────────────────
class FileLocker : public LockableItem {
public:
    FileLocker(string p) : LockableItem(p) {}

    void lock() override {
        // +H = Hidden, +S = System (double protection)
        string cmd = "attrib +H +S \"" + path + "\"";
        int result = system(cmd.c_str());
        if (result == 0) {
            isLocked = true;
            setColor(10); // Green
            cout << "  [✔] File locked/hidden: " << path << "\n";
            resetColor();
        } else {
            setColor(12); // Red
            cout << "  [✘] Failed to lock file. Check path!\n";
            resetColor();
        }
    }

    void unlock() override {
        string cmd = "attrib -H -S \"" + path + "\"";
        int result = system(cmd.c_str());
        if (result == 0) {
            isLocked = false;
            setColor(10);
            cout << "  [✔] File unlocked/visible: " << path << "\n";
            resetColor();
        } else {
            setColor(12);
            cout << "  [✘] Failed to unlock file. Check path!\n";
            resetColor();
        }
    }

    void displayInfo() override {
        setColor(14); // Yellow
        cout << "  [FILE]   " << path << "  -->  ";
        isLocked ? (setColor(12), cout << "LOCKED\n") : (setColor(10), cout << "UNLOCKED\n");
        resetColor();
    }
};

// ─────────────────────────────────────────────
// Derived Class: FolderLocker
// ─────────────────────────────────────────────
class FolderLocker : public LockableItem {
public:
    FolderLocker(string p) : LockableItem(p) {}

    void lock() override {
        // Hide folder and all contents recursively
        string cmd = "attrib +H +S \"" + path + "\" /S /D";
        int result = system(cmd.c_str());
        if (result == 0) {
            isLocked = true;
            setColor(10);
            cout << "  [✔] Folder locked/hidden: " << path << "\n";
            resetColor();
        } else {
            setColor(12);
            cout << "  [✘] Failed to lock folder. Check path!\n";
            resetColor();
        }
    }

    void unlock() override {
        string cmd = "attrib -H -S \"" + path + "\" /S /D";
        int result = system(cmd.c_str());
        if (result == 0) {
            isLocked = false;
            setColor(10);
            cout << "  [✔] Folder unlocked/visible: " << path << "\n";
            resetColor();
        } else {
            setColor(12);
            cout << "  [✘] Failed to unlock folder. Check path!\n";
            resetColor();
        }
    }

    void displayInfo() override {
        setColor(14);
        cout << "  [FOLDER] " << path << "  -->  ";
        isLocked ? (setColor(12), cout << "LOCKED\n") : (setColor(10), cout << "UNLOCKED\n");
        resetColor();
    }
};

// ─────────────────────────────────────────────
// Class: PasswordManager
// ─────────────────────────────────────────────
class PasswordManager {
private:
    string passwordFile;
    string currentPassword;

    string hashSimple(string pass) {
        // Simple XOR-based obfuscation (lightweight for local use)
        string hashed = pass;
        for (int i = 0; i < hashed.size(); i++)
            hashed[i] ^= (42 + i);
        return hashed;
    }

public:
    PasswordManager(string file = "lock_pass.dat") : passwordFile(file) {
        loadPassword();
    }

    void loadPassword() {
        ifstream f(passwordFile.c_str());
        if (f.is_open()) {
            getline(f, currentPassword);
            f.close();
        } else {
            currentPassword = "";
        }
    }

    void savePassword(string pass) {
        ofstream f(passwordFile.c_str());
        string hashed = hashSimple(pass);
        f << hashed;
        f.close();
        currentPassword = hashed;
    }

    bool isPasswordSet() {
        return !currentPassword.empty();
    }

    bool verify(string pass) {
        return hashSimple(pass) == currentPassword;
    }

    void setNewPassword() {
        string p1, p2;
        setColor(11);
        cout << "\n  Enter new password : ";
        resetColor();
        cin >> p1;
        setColor(11);
        cout << "  Confirm password   : ";
        resetColor();
        cin >> p2;
        if (p1 == p2) {
            savePassword(p1);
            setColor(10);
            cout << "  [✔] Password set successfully!\n";
            resetColor();
        } else {
            setColor(12);
            cout << "  [✘] Passwords do not match!\n";
            resetColor();
        }
    }

    bool promptLogin() {
        if (!isPasswordSet()) {
            setColor(13);
            cout << "\n  No password set. Please create one first.\n";
            resetColor();
            setNewPassword();
            return true;
        }
        string pass;
        setColor(11);
        cout << "\n  Enter password to continue: ";
        resetColor();
        cin >> pass;
        if (verify(pass)) {
            setColor(10);
            cout << "  [✔] Access granted!\n";
            resetColor();
            return true;
        } else {
            setColor(12);
            cout << "  [✘] Wrong password! Access denied.\n";
            resetColor();
            return false;
        }
    }
};

// ─────────────────────────────────────────────
// Class: LockManager (Main Controller)
// ─────────────────────────────────────────────
class LockManager {
private:
    vector<LockableItem*> items;
    PasswordManager pwdMgr;

    void saveSession() {
        ofstream f("lock_session.dat");
        for (int i = 0; i < items.size(); i++) {
            f << (dynamic_cast<FolderLocker*>(items[i]) ? "F" : "f")
              << "|" << items[i]->getPath()
              << "|" << items[i]->getStatus() << "\n";
        }
        f.close();
    }

public:
    LockManager() {}

    ~LockManager() {
        for (int i = 0; i < items.size(); i++)
            delete items[i];
    }

    void run() {
        clearScreen();
        printBanner();

        if (!pwdMgr.promptLogin()) {
            cout << "\n  Exiting...\n";
            return;
        }

        int choice;
        do {
            showMenu();
            setColor(11);
            cout << "\n  Enter choice: ";
            resetColor();
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: lockItem(false); break;   // Lock File
                case 2: lockItem(true);  break;   // Lock Folder
                case 3: unlockItem();    break;
                case 4: viewAll();       break;
                case 5: pwdMgr.setNewPassword(); break;
                case 0:
                    saveSession();
                    setColor(13);
                    cout << "\n  Session saved. Goodbye!\n\n";
                    resetColor();
                    break;
                default:
                    setColor(12);
                    cout << "  [!] Invalid choice!\n";
                    resetColor();
            }
        } while (choice != 0);
    }

    void showMenu() {
        setColor(11);
        cout << "\n  ┌─────────────────────────────┐\n";
        cout << "  │         MAIN MENU            │\n";
        cout << "  ├─────────────────────────────┤\n";
        setColor(14);
        cout << "  │  1. Lock a FILE              │\n";
        cout << "  │  2. Lock a FOLDER            │\n";
        cout << "  │  3. Unlock File/Folder       │\n";
        cout << "  │  4. View All Items           │\n";
        cout << "  │  5. Change Password          │\n";
        cout << "  │  0. Exit                     │\n";
        setColor(11);
        cout << "  └─────────────────────────────┘\n";
        resetColor();
    }

    void lockItem(bool isFolder) {
        string path;
        setColor(11);
        if (isFolder)
            cout << "\n  Enter full folder path (e.g. C:\\Users\\YourName\\MyFolder): ";
        else
            cout << "\n  Enter full file path (e.g. C:\\Users\\YourName\\secret.txt): ";
        resetColor();
        getline(cin, path);

        LockableItem* item;
        if (isFolder)
            item = new FolderLocker(path);
        else
            item = new FileLocker(path);

        item->lock();
        items.push_back(item);
    }

    void unlockItem() {
        if (items.empty()) {
            setColor(12);
            cout << "  [!] No items in the list!\n";
            resetColor();
            return;
        }

        viewAll();
        setColor(11);
        cout << "\n  Enter item number to unlock (1-" << items.size() << "): ";
        resetColor();
        int idx;
        cin >> idx;
        cin.ignore();

        if (idx >= 1 && idx <= (int)items.size()) {
            items[idx - 1]->unlock();
        } else {
            setColor(12);
            cout << "  [!] Invalid number!\n";
            resetColor();
        }
    }

    void viewAll() {
        setColor(11);
        cout << "\n  ── Locked/Tracked Items ──────────────────\n";
        resetColor();
        if (items.empty()) {
            setColor(7);
            cout << "  (No items added yet)\n";
            return;
        }
        for (int i = 0; i < items.size(); i++) {
            cout << "  " << (i + 1) << ". ";
            items[i]->displayInfo();
        }
        setColor(11);
        cout << "  ───────────────────────────────────────────\n";
        resetColor();
    }
};

// ─────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────
int main() {
    LockManager manager;
    manager.run();
    return 0;
}
