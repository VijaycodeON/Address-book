# Address Book Management System – Project Description

## 1. Overview
This project is a console-based Address Book application implemented
in the C programming language. The application allows a user to
manage contact information using a menu-driven interface and
file-based persistent storage.

The program is designed around a central AddressBook structure
that stores contacts in memory and synchronizes them with a CSV
file for persistence.

## 2. Program Flow
- The program starts execution from main()
- AddressBook structure is initialized
- Existing contacts are loaded from a CSV file (if available)
- A menu-driven loop allows the user to perform operations
- Changes are written back to the file before program termination

## 3. Data Model
Each contact is represented using a structure containing:
- Name
- Phone number
- Email address

All contacts are stored in an array inside the AddressBook structure.
The application maintains a contact count to track valid entries.

A fixed maximum limit (MAX_CONTACTS) is enforced.

## 4. Menu Operations
The application presents a numeric menu with options similar to:

1. Add Contact  
2. Search Contact  
3. Edit Contact  
4. Delete Contact  
5. List Contacts  
6. Save Contacts  
7. Exit  

User input is validated to ensure the selected option is within
the valid range before execution.

## 5. Add Contact
- Prompts the user to enter name, phone number, and email
- Input validation is applied:
  - Name length is restricted
  - Phone number format is checked
  - Email format is validated
- Duplicate entries are checked using validation functions
- If validation fails, the user is re-prompted
- On success, the contact is added to memory

## 6. Search Contact
- Allows searching based on user-provided input
- Compares search key against stored contacts
- Displays matching contact details if found
- Displays an informative message if no match exists

## 7. Edit Contact
- Prompts the user to select an existing contact
- Displays current contact details
- Allows modification of name, phone, and email
- Updated values are validated before saving
- Changes are applied directly to the in-memory structure

## 8. Delete Contact
- Allows the user to remove a contact from the address book
- Contacts are shifted in memory after deletion
- Contact count is updated accordingly
- Provides confirmation-style feedback to the user

## 9. List Contacts
- Displays all stored contacts in a formatted output
- Uses spacing and alignment functions for readability
- Ensures consistent column display regardless of name length

## 10. File Handling
- Contacts are stored in a CSV file (contact.csv)
- File format: name,phone,email
- On program start:
- File is opened in read mode
- Contacts are loaded into memory
- On save:
- Entire contact list is rewritten to the file
- If file is missing:
- Program continues with an empty address book

## 11. Populate Function
- A dedicated populate module exists
- Used to insert predefined dummy contacts
- Helps in testing and demonstration
- Keeps test data separate from core logic

## 12. Error Handling and Limitations
- Basic input validation is implemented
- File open failures are checked
- Buffer clearing is handled using standard techniques
- Program assumes valid numeric menu input
- No dynamic memory allocation is used
- No contact grouping or import/export features are implemented

## 13. Design Characteristics
- Modular source files
- Clear separation of concerns:
- main.c → control flow and menu
- contact.c → contact operations
- file.c → file I/O
- populate.c → test data
- Uses static arrays instead of dynamic memory
- Focuses on clarity and correctness over complexity

## 14. Project Intent
This project is intended as a learning implementation to:
- Understand structured programming in C
- Practice file handling with real data
- Design a menu-driven application
- Apply validation and modular design principles

The implementation reflects practical constraints and
realistic behavior rather than theoretical completeness.

