# File & Folder Lock System (C++)

A Windows console application built with **C++ and Object-Oriented Programming (OOP)**. It lets users hide and unhide files and folders using Windows file attributes and provides a simple password prompt.

## Features

- Hide and unhide individual files.
- Hide and unhide folders and their contents.
- Set a password and prompt for it when the program starts.
- Change the password from the menu.
- Display tracked files and folders with their current in-memory status.
- Console menu with colored text.
- OOP concepts: abstraction, inheritance, polymorphism, and encapsulation.

## Technologies

- C++
- Windows API (`windows.h`)
- Windows `attrib` command
- Standard C++ library

## Requirements

- Windows
- A C++ compiler such as MinGW / Dev-C++
- A terminal that supports the Windows Console API

> This project uses Windows-specific functionality and will not compile unchanged on Linux or macOS.

## Project structure

```text
File-Folder-Lock-System/
├── LockSystem.cpp
├── README.md
└── .gitignore
```

## How to compile and run

### Option 1: Dev-C++

1. Open `LockSystem.cpp` in Dev-C++.
2. Compile and run the program.
3. On the first launch, enter a password and confirm it.
4. Use the menu to lock or unlock a file or folder.

### Option 2: MinGW g++

Open Command Prompt or a terminal configured with MinGW and run:

```bash
g++ LockSystem.cpp -o LockSystem.exe
LockSystem.exe
```

## Menu options

| Option | Action |
|---|---|
| `1` | Hide a file |
| `2` | Hide a folder |
| `3` | Unhide a tracked file or folder |
| `4` | View tracked items |
| `5` | Change password |
| `0` | Save the current session list and exit |

When asked for a path, enter the full Windows path, for example:

```text
C:\Users\YourName\Documents\secret.txt
C:\Users\YourName\Documents\PrivateFolder
```

Use a test file or folder first. Confirm that you can unhide it before using the program with important data.

## Important limitations and security notes

- **Hiding is not encryption.** Files are not securely protected; users can reveal hidden items through Windows settings or command-line tools.
- The password is transformed using a simple XOR routine, which is **not secure password hashing**. Do not use a real or reused password.
- The session list is written to `lock_session.dat`, but this version does not reload that list when the program starts. The displayed item list is therefore for the current run only.
- The password file `lock_pass.dat` is created at runtime and should not be committed to GitHub.
- Use only on files and folders you own or have permission to manage.

## Files created at runtime

- `lock_pass.dat` — stores the transformed password.
- `lock_session.dat` — stores the tracked-item list when exiting.

These runtime files are ignored by Git.

## Future improvements

- Use a standard password-hashing method with a salt.
- Add input validation and clearer error handling.
- Load the saved session list when the application starts.
- Verify file and folder paths before attempting to hide them.
- Add automated tests for non-Windows logic.

## License

No license has been specified yet. Add a license file if you want to define how others may use, modify, and distribute this project.
