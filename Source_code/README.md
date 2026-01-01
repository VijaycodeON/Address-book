# Source Code Overview

This directory contains the complete source code for the
Address Book Management System.

Each file has a clearly defined responsibility to keep
the application modular and maintainable.

---

## File Descriptions

### main.c
- Entry point of the application
- Displays the menu interface
- Handles user input
- Controls program flow

---

### contact.c / contact.h
- Defines the contact data structure
- Implements core contact operations:
  - Add
  - Search
  - Edit
  - Delete
  - Display

---

### file.c / file.h
- Handles all file input/output operations
- Reads contact data from storage
- Writes updated data back to file
- Ensures persistence across executions

---

### populate.c / populate.h
- Used for initializing or populating sample data
- Helps in testing and validation
- Kept separate from core logic

---

## Compilation Notes
- All source files must be compiled together
- Missing any file will result in linker errors

Example:
```bash
gcc *.c -o address_book
