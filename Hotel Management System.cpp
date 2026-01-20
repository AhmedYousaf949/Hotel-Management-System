#include <iostream>
using namespace std;

struct Room {
    int roomNo;
    bool booked;
    Room* next;
};

struct Floor {
    int floorNo;
    Room* rooms;
    Floor* next;
    Floor* prev;
};

struct Booking {
    int customerId;
    int priority;
    Booking* next;
};

struct History {
    int customerId;
    int roomNo;
    History* next;
};

Floor* floorHead = NULL;
Booking* bookingFront = NULL;
History* historyTop = NULL;

bool match(char a[], char b[]) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

void addFloor(int fno) {
    Floor* f = new Floor{fno, NULL, NULL, NULL};
    if (!floorHead) floorHead = f;
    else {
        Floor* t = floorHead;
        while (t->next) t = t->next;
        t->next = f;
        f->prev = t;
    }
}

void addRoom(int fno, int rno) {
    Floor* f = floorHead;
    while (f && f->floorNo != fno) f = f->next;
    if (!f) return;
    Room* r = new Room{rno, false, f->rooms};
    f->rooms = r;
}

void addBooking(int id, int p) {
    Booking* b = new Booking{id, p, NULL};
    if (!bookingFront || p < bookingFront->priority) {
        b->next = bookingFront;
        bookingFront = b;
    } else {
        Booking* t = bookingFront;
        while (t->next && t->next->priority <= p)
            t = t->next;
        b->next = t->next;
        t->next = b;
    }
    cout << "Booking request added\n";
}

void assignRoom() {
    if (!bookingFront) {
        cout << "No booking requests\n";
        return;
    }
    Floor* f = floorHead;
    while (f) {
        Room* r = f->rooms;
        while (r) {
            if (!r->booked) {
                r->booked = true;
                History* h = new History{bookingFront->customerId, r->roomNo, historyTop};
                historyTop = h;
                cout << "Room assigned: " << r->roomNo << endl;
                Booking* temp = bookingFront;
                bookingFront = bookingFront->next;
                delete temp;
                return;
            }
            r = r->next;
        }
        f = f->next;
    }
    cout << "No available rooms\n";
}

void undoBooking() {
    if (!historyTop) {
        cout << "No booking to undo\n";
        return;
    }
    int roomNo = historyTop->roomNo;
    Floor* f = floorHead;
    while (f) {
        Room* r = f->rooms;
        while (r) {
            if (r->roomNo == roomNo) {
                r->booked = false;
                break;
            }
            r = r->next;
        }
        f = f->next;
    }
    History* temp = historyTop;
    historyTop = historyTop->next;
    delete temp;
    cout << "Last booking undone\n";
}

void viewHistory() {
    History* h = historyTop;
    while (h) {
        cout << "Customer " << h->customerId << " Room " << h->roomNo << endl;
        h = h->next;
    }
}

void showAvailability() {
    Floor* f = floorHead;
    if (!f) {
        cout << "No floors available\n";
        return;
    }
    while (f) {
        cout << "Floor " << f->floorNo << ": ";
        Room* r = f->rooms;
        while (r) {
            if (!r->booked)
                cout << r->roomNo << "(Free) ";
            r = r->next;
        }
        cout << endl;
        f = f->next;
    }
}

void listAllRooms() {
    Floor* f = floorHead;
    if (!f) {
        cout << "No floors available\n";
        return;
    }
    while (f) {
        cout << "Floor " << f->floorNo << ": ";
        Room* r = f->rooms;
        while (r) {
            if (r->booked)
                cout << r->roomNo << "(Booked) ";
            else
                cout << r->roomNo << "(Free) ";
            r = r->next;
        }
        cout << endl;
        f = f->next;
    }
}

void adminMenu() {
    int c;
    do {
        cout << "\n1 AddFloor 2 AddRoom 3 Availability 4 AllRooms 5 History 6 Undo 0 Logout: ";
        cin >> c;
        if (c == 1) { int f; cin >> f; addFloor(f); }
        else if (c == 2) { int f, r; cin >> f >> r; addRoom(f, r); }
        else if (c == 3) showAvailability();
        else if (c == 4) listAllRooms();
        else if (c == 5) viewHistory();
        else if (c == 6) undoBooking();
    } while (c);
}

void employeeMenu() {
    int c;
    do {
        cout << "\n1 AssignRoom 2 Availability 3 AllRooms 4 Undo 0 Logout: ";
        cin >> c;
        if (c == 1) assignRoom();
        else if (c == 2) showAvailability();
        else if (c == 3) listAllRooms();
        else if (c == 4) undoBooking();
    } while (c);
}

void adminEmpLogin() {
    char user[20], pass[20];
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    if (match(user, (char*)"admin") && match(pass, (char*)"123"))
        adminMenu();
    else if (match(user, (char*)"emp") && match(pass, (char*)"123"))
        employeeMenu();
    else
        cout << "Invalid login\n";
}

void customerMenu() {
    int id, p;
    cout << "Customer ID: ";
    cin >> id;
    cout << "Priority (1 VIP 2 Normal): ";
    cin >> p;
    addBooking(id, p);
}

int main() {
    int choice;
    do {
        cout << "\n--- HOTEL MANAGEMENT SYSTEM ---\n";
        cout << "1 Login (Admin/Employee)\n";
        cout << "2 Book Room (Customer)\n";
        cout << "0 Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) adminEmpLogin();
        else if (choice == 2) customerMenu();

    } while (choice != 0);

    return 0;
}