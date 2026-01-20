#include <iostream>
using namespace std;
struct Room {
    int roomNo;
    bool isBooked;
    Room* next;
};
struct Floor {
    int floorNo;
    Room* roomHead;
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
void addFloor(int floorNo) {
    Floor* newFloor = new Floor;
    newFloor->floorNo = floorNo;
    newFloor->roomHead = NULL;
    newFloor->next = NULL;
    newFloor->prev = NULL;
    if (floorHead == NULL) {
        floorHead = newFloor;
    } else {
        Floor* temp = floorHead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newFloor;
        newFloor->prev = temp;
    }
    cout << "Floor added successfully\n";
}
void addRoom(int floorNo, int roomNo) {
    Floor* temp = floorHead;
    while (temp != NULL && temp->floorNo != floorNo)
        temp = temp->next;
    if (temp == NULL) {
        cout << "Floor not found\n";
        return;
    }
    Room* newRoom = new Room;
    newRoom->roomNo = roomNo;
    newRoom->isBooked = false;
    newRoom->next = temp->roomHead;
    temp->roomHead = newRoom;
    cout << "Room added to floor " << floorNo << endl;
}
void addBooking(int customerId, int priority) {
    Booking* newBooking = new Booking;
    newBooking->customerId = customerId;
    newBooking->priority = priority;
    newBooking->next = NULL;

    if (bookingFront == NULL || priority < bookingFront->priority) {
        newBooking->next = bookingFront;
        bookingFront = newBooking;
    } else {
        Booking* temp = bookingFront;
        while (temp->next != NULL && temp->next->priority <= priority)
            temp = temp->next;
        newBooking->next = temp->next;
        temp->next = newBooking;
    }
    cout << "Booking request added\n";
}
void assignRoom() {
    if (bookingFront == NULL) {
        cout << "No booking requests\n";
        return;
    }
    Floor* f = floorHead;
    while (f != NULL) {
        Room* r = f->roomHead;
        while (r != NULL) {
            if (!r->isBooked) {
                r->isBooked = true;
                History* h = new History;
                h->customerId = bookingFront->customerId;
                h->roomNo = r->roomNo;
                h->next = historyTop;
                historyTop = h;
                cout << "Room " << r->roomNo << " assigned to Customer "
                     << bookingFront->customerId << endl;
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
void viewHistory() {
    if (historyTop == NULL) {
        cout << "No history available\n";
        return;
    }

    History* temp = historyTop;
    while (temp != NULL) {
        cout << "Customer " << temp->customerId
             << " booked Room " << temp->roomNo << endl;
        temp = temp->next;
    }
}
int main() {
    int choice;
    do {
        cout << "\n--- HOTEL MANAGEMENT SYSTEM ---\n";
        cout << "1. Add Floor\n";
        cout << "2. Add Room\n";
        cout << "3. Add Booking Request\n";
        cout << "4. Assign Room\n";
        cout << "5. View Booking History\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) {
            int f;
            cout << "Enter floor number: ";
            cin >> f;
            addFloor(f);
        } 
        else if (choice == 2) {
            int f, r;
            cout << "Enter floor number: ";
            cin >> f;
            cout << "Enter room number: ";
            cin >> r;
            addRoom(f, r);
        }
        else if (choice == 3) {
            int id, p;
            cout << "Customer ID: ";
            cin >> id;
            cout << "Priority (1=VIP, 2=Normal): ";
            cin >> p;
            addBooking(id, p);
        }
        else if (choice == 4) {
            assignRoom();
        }
        else if (choice == 5) {
            viewHistory();
        }
    } while (choice != 0);
    return 0;
}