# 🦥 Slots for Sloths – Facility Booking System

A C++ Object-Oriented Programming (OOP) project that simulates a **campus facility booking system**.
This console-based system allows users to **view, book, unbook, edit, and manage time slots** for various facilities like **libraries, cafes, sports courts, and hybrid venues**.

---

## 📌 Features

* 🔍 **View all facilities** with capacity and operational details
* 🗓️ **Book a time slot** for a facility on a given day
* ❌ **Unbook** previously booked slots
* ✏️ **Edit bookings** to change time or day
* 📄 **View all bookings** for any facility
* 📀 **Automatic file saving** after every change
* ⛔ **Cancel anytime** by entering `0` during input

---

## 🧠 OOP Concepts Demonstrated

* ✅ **Inheritance** (`Library`, `Cafe`, `SportsFacility` inherit from `Location`)
* ✅ **Polymorphism** (Overridden methods for facility-specific behavior)
* ✅ **Encapsulation** (Protected data accessed via public methods)
* ✅ **Operator Overloading** (`<<` used for displaying facilities)
* ✅ **Static Members** (Track number of facilities)
* ✅ **Friend Functions** (Secure access for overloaded operators)
* ✅ **Templates** (Generic facility search function)
* ✅ **File Handling** (Persistent booking data via `bookings.txt`)
* ✅ **Multiple Inheritance** (`SportsCafe` using virtual inheritance)

---

## 🏠 System Structure

```
📁 Slots-for-Sloths/
🔽 main.cpp            # Main application source code
🔽 bookings.txt        # File used for saving/loading bookings
🔽 README.md           # Project overview (this file)
🔽 report.pdf          # Final project report (optional)
```

---

## ⚙️ How to Run

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/slots-for-sloths.git
   cd slots-for-sloths
   ```

2. Compile the program:

   ```bash
   g++ -o sloths main.cpp
   ```

3. Run the program:

   ```bash
   ./sloths
   ```

---

## 💻 Requirements

* C++11 or above
* g++ compiler (or any standard C++ compiler)
* Terminal / Command line environment

---

## 👥 Team Members

* Abeeha Binte Aamer [24K-0940]
* Laiba Khan [24K-0644]
* Zunairah Aziz Shaikh [24K-0986]
* Aamna Rizwan [24K-0695]
