# 📚 LibraryPlusPlus  
*A Qt-based Library Management System written in C++*

## 📖 Overview  
**LibraryPlusPlus** is a desktop application designed to help small libraries manage books, users, and circulation with an easy-to-use graphical interface. Built using **Qt Widgets** and **C++**, the system provides a complete workflow for logging in, adding books, viewing the collection, checking books out, returning them, and maintaining persistent data storage.

This project was developed for CPSC 362 as a team assignment following software engineering principles.

---

## 🚀 Features  
- **User Authentication**
  - Login and signup windows  
- **Book Management**
  - Add new books  
  - View list of all books  
  - Remove books  
  - Edit book details (if implemented)  
- **Book Circulation**
  - Checkout and return books  
  - View available vs. unavailable books  
- **Search / Filter**
  - Find books by keywords or properties  
- **Data Persistence**
  - Local database or file-based storage located in `/databases/`  
- **Qt GUI**
  - Multiple windows (Login, Signup, Dashboard, Book List, Checkout, etc.)  
  - Built using `.ui` files for cleaner layout management  

---

## 🛠️ Tech Stack  
- **Language:** C++  
- **Framework:** Qt (Qt Widgets, UI Designer)  
- **Build System:** qmake (`.pro` file)  
- **Storage:** SQLite or file-based storage  
- **Platform:** Cross-platform (Windows, macOS, Linux)

