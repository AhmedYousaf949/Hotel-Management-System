#include <iostream>
using namespace std;

struct Room {
    int roomNo;
    bool booked;
    bool vip; // true = VIP, false = Normal
    Room* next;
};

struct Floor {
    int floorNo;
    Room* rooms;
    bool vip; // true = VIP floor, false = Normal floor
    Floor* next;
    Floor* prev;
};

struct Booking {
    int customerId;
    int priority; // 1=VIP, 2=Normal
    int floorNo;  // floor customer booked from
    int roomNo;   // room customer booked
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

// Admin adds multiple floors and rooms
void addFloors() {
    int n;
    cout << "How many floors to add? ";
    cin >> n;
    for(int i = 0; i < n; i++) {
        int floorNo, roomsCount;
        char type;
        cout << "Enter floor number: ";
        cin >> floorNo;
        cout << "Is this floor VIP? (y/n): ";
        cin >> type;
        bool vipFloor = (type == 'y' || type == 'Y');

        Floor* f = new Floor{floorNo, NULL, vipFloor, NULL, NULL};
        if(!floorHead) floorHead = f;
        else {
            Floor* t = floorHead;
            while(t->next) t = t->next;
            t->next = f;
            f->prev = t;
        }

        cout << "How many rooms on this floor? ";
        cin >> roomsCount;
        for(int r = 1; r <= roomsCount; r++) {
            Room* room = new Room{r, false, vipFloor, f->rooms};
            f->rooms = room;
        }
    }
}

// Show free rooms on a floor
void showFreeRooms(Floor* f) {
    Room* r = f->rooms;
    cout << "Free rooms on Floor " << f->floorNo << ": ";
    bool any = false;
    while(r) {
        if(!r->booked) {
            cout << r->roomNo << " ";
            any = true;
        }
        r = r->next;
    }
    if(!any) cout << "None";
    cout << endl;
}

// Assign room (Employee)
void assignRoom() {
    if(!bookingFront) {
        cout << "No booking requests\n";
        return;
    }

    int fno;
    cout << "Enter floor number to assign room from: ";
    cin >> fno;

    Floor* f = floorHead;
    while(f && f->floorNo != fno) f = f->next;
    if(!f) {
        cout << "Floor not found\n";
        return;
    }

    showFreeRooms(f);
    Room* r = f->rooms;
    while(r) {
        if(!r->booked) {
            r->booked = true;

            History* h = new History{bookingFront->customerId, r->roomNo, historyTop};
            historyTop = h;

            cout << "Assigned Room " << r->roomNo << " on Floor " << f->floorNo << endl;

            Booking* temp = bookingFront;
            bookingFront = bookingFront->next;
            delete temp;
            return;
        }
        r = r->next;
    }
    cout << "No free rooms on this floor\n";
}

// Undo last booking
void undoBooking() {
    if(!historyTop) {
        cout << "No booking to undo\n";
        return;
    }

    int roomNo = historyTop->roomNo;
    Floor* f = floorHead;
    while(f) {
        Room* r = f->rooms;
        while(r) {
            if(r->roomNo == roomNo) {
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

// View history
void viewHistory() {
    History* h = historyTop;
    while(h) {
        cout << "Customer " << h->customerId << " Room " << h->roomNo << endl;
        h = h->next;
    }
}

// Admin menu
void adminMenu() {
    int c;
    do {
        cout << "\n1 AddFloors 2 View FreeRooms 3 View History 4 Undo 0 Logout: ";
        cin >> c;
        if(c==1) addFloors();
        else if(c==2) {
            Floor* f = floorHead;
            while(f) {
                showFreeRooms(f);
                f=f->next;
            }
        }
        else if(c==3) viewHistory();
        else if(c==4) undoBooking();
    } while(c);
}

// Employee menu
void employeeMenu() {
    int c;
    do {
        cout << "\n1 AssignRoom 2 View FreeRooms 3 Undo 0 Logout: ";
        cin >> c;
        if(c==1) assignRoom();
        else if(c==2) {
            Floor* f = floorHead;
            while(f) {
                showFreeRooms(f);
                f=f->next;
            }
        }
        else if(c==3) undoBooking();
    } while(c);
}

// Admin/Employee login
void adminEmpLogin() {
    char user[20], pass[20];
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    if(match(user,(char*)"admin") && match(pass,(char*)"123")) adminMenu();
    else if(match(user,(char*)"emp") && match(pass,(char*)"123")) employeeMenu();
    else cout << "Invalid login\n";
}

// Customer booking menu
void customerMenu() {
    int id, p;
    cout << "Customer ID: ";
    cin >> id;
    cout << "Priority (1 VIP 2 Normal): ";
    cin >> p;

    Floor* f = floorHead;
    bool booked=false;
    while(f) {
        if(p==1 && f->vip) {
            Room* r = f->rooms;
            while(r) {
                if(!r->booked) {
                    r->booked=true;
                    cout << "Booked VIP Room " << r->roomNo << " on Floor " << f->floorNo << endl;
                    booked=true;
                    break;
                }
                r=r->next;
            }
        } else if(p==2 && !f->vip) {
            Room* r = f->rooms;
            while(r) {
                if(!r->booked) {
                    r->booked=true;
                    cout << "Booked Normal Room " << r->roomNo << " on Floor " << f->floorNo << endl;
                    booked=true;
                    break;
                }
                r=r->next;
            }
        }
        if(booked) break;
        f=f->next;
    }

    if(!booked) cout << "No rooms available for selected type\n";
}

// Main
int main() {
    int choice;
    do {
        cout << "\n--- HOTEL MANAGEMENT SYSTEM ---\n";
        cout << "1 Login (Admin/Employee)\n2 Book Room (Customer)\n0 Exit\nChoice: ";
        cin >> choice;

        if(choice==1) adminEmpLogin();
        else if(choice==2) customerMenu();

    } while(choice!=0);
}