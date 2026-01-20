
# 🏨 Epstein Hotel Management System

[![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)](https://isocpp.org/) [![License](https://img.shields.io/badge/License-MIT-green)](https://opensource.org/licenses/MIT)

A **C++ console-based hotel management system** that allows **admins, employees, and customers** to manage hotel bookings. The system uses **dynamic memory allocation** and **linked lists** for floors, rooms, and booking history, with **stack-based undo functionality**.

---

## 🌟 Features

### Customer:
- Book **VIP or Normal rooms**
- Choose **room preferences** (view type, smoking/non-smoking) for VIP rooms
- Automatic room assignment based on priority

### Employee:
- View **number of free rooms per floor**
- **Undo the last booking**

### Admin:
- View **free and booked rooms per floor**
- View **booking history**
- Undo the **last booking**

---

## 🏗️ System Structure

**Main Structures:**
1. **Room**
   - Room number, booking status, VIP status
   - View type (City/Garden)
   - Smoking/Non-smoking

2. **Floor**
   - Floor number
   - Linked list of rooms
   - VIP status

3. **History**
   - Customer ID
   - Floor & Room number
   - VIP status
   - Room preferences

All floors and rooms are dynamically allocated. Booking history is implemented as a **stack** to support undo operations.

---

## 💻 How to Run

1. Clone the repository:

```bash
git clone <repository-url>
cd <repository-folder>
```

2. Compile the program:

```bash
g++ hotel.cpp -o hotel
```

3. Run the program:

```bash
./hotel
```

4. Console Menu Options:
   - **Login as Admin**: username `admin`, password `123`
   - **Login as Employee**: username `emp`, password `123`
   - **Customer Booking**: enter your ID and priority to book a room

---

## 📖 Usage Examples

### Admin Menu:
- View free rooms per floor
- View all rooms (free/booked)
- View booking history
- Undo last booking

### Employee Menu:
- View free rooms per floor
- Undo last booking

### Customer Menu:
- Book a VIP room (choose view and smoking preference)
- Book a Normal room automatically assigned

---

## 🛠️ Key Concepts Implemented

- **Dynamic Memory Allocation** (`new` / `delete`)
- **Doubly Linked Lists** for floors
- **Singly Linked Lists** for rooms & booking history
- **Stack-based Undo Functionality**
- **Menu-driven Programming**

---

## 🚀 Future Enhancements

- Save booking data to **file or database** for persistence
- Add **GUI interface**
- Improve **room search & filtering**
- Multi-user login and authentication system

---

## 📝 Author

**Muhammad Ahmed Yousaf**

# Hotel-Management-System
