#include <iostream>
using namespace std;

struct Room {
    int roomNo;
    bool booked;
    bool vip; 
    Room* next;
};

struct Floor {
    int floorNo;
    Room* rooms;
    bool vip; 
    Floor* next;
    Floor* prev;
};

struct Booking {
    int customerId;
    int priority; 
    int floorNo;  
    int roomNo;   
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
    int i=0;
    while(a[i] && b[i]){
        if(a[i]!=b[i]) return false;
        i++;
    }
    return a[i]=='\0' && b[i]=='\0';
}

// Initialize hotel with 20 floors and rooms
void initHotel() {
    for(int f=1; f<=20; f++){
        bool vipFloor = (f>11); // floors 12-20 VIP
        Floor* fl = new Floor{f, NULL, vipFloor, NULL, NULL};
        if(!floorHead) floorHead=fl;
        else{
            Floor* t=floorHead;
            while(t->next) t=t->next;
            t->next=fl;
            fl->prev=t;
        }

        int roomCount = vipFloor ? 60 : 80; 
        for(int r=1; r<=roomCount; r++){
            Room* rm = new Room{r, false, vipFloor, fl->rooms};
            fl->rooms = rm;
        }
    }
}

// Count free rooms on a floor
int countFreeRooms(Floor* f){
    int cnt=0;
    Room* r=f->rooms;
    while(r){ if(!r->booked) cnt++; r=r->next; }
    return cnt;
}

// Count booked rooms on a floor
int countBookedRooms(Floor* f){
    int cnt=0;
    Room* r=f->rooms;
    while(r){ if(r->booked) cnt++; r=r->next; }
    return cnt;
}

// Assign room (Employee)
void assignRoom() {
    if(!bookingFront){
        cout<<"No booking requests\n";
        return;
    }

    int fno;
    cout<<"Enter floor number to assign room from: ";
    cin>>fno;

    Floor* f = floorHead;
    while(f && f->floorNo != fno) f=f->next;
    if(!f){cout<<"Floor not found\n"; return;}

    int freeRooms = countFreeRooms(f);
    if(freeRooms==0){ cout<<"No free rooms on this floor\n"; return; }
    cout<<"Number of free rooms on Floor "<<f->floorNo<<": "<<freeRooms<<endl;

    Room* r = f->rooms;
    while(r){
        if(!r->booked){
            r->booked=true;
            History* h = new History{bookingFront->customerId, r->roomNo, historyTop};
            historyTop=h;
            cout<<"Assigned Room "<<r->roomNo<<" on Floor "<<f->floorNo<<endl;
            Booking* temp = bookingFront;
            bookingFront=bookingFront->next;
            delete temp;
            return;
        }
        r=r->next;
    }
}

// Undo last booking
void undoBooking() {
    if(!historyTop){cout<<"No booking to undo\n"; return;}
    int roomNo = historyTop->roomNo;
    Floor* f=floorHead;
    while(f){
        Room* r=f->rooms;
        while(r){
            if(r->roomNo==roomNo){ r->booked=false; break; }
            r=r->next;
        }
        f=f->next;
    }
    History* temp=historyTop;
    historyTop=historyTop->next;
    delete temp;
    cout<<"Last booking undone\n";
}

// View history
void viewHistory(){
    History* h=historyTop;
    while(h){
        cout<<"Customer "<<h->customerId<<" Room "<<h->roomNo<<endl;
        h=h->next;
    }
}

// Admin menu
void adminMenu(){
    int c;
    do{
        cout<<"\n1 View FreeRooms 2 View All Rooms 3 View History 4 Undo 0 Logout: ";
        cin>>c;
        if(c==1){
            Floor* f=floorHead;
            while(f){
                cout<<"Floor "<<f->floorNo<<": "<<countFreeRooms(f)<<" free rooms"<<endl;
                f=f->next;
            }
        }
        else if(c==2){
            Floor* f=floorHead;
            while(f){
                cout<<"Floor "<<f->floorNo<<": "<<countFreeRooms(f)<<" free, "<<countBookedRooms(f)<<" booked"<<endl;
                f=f->next;
            }
        }
        else if(c==3) viewHistory();
        else if(c==4) undoBooking();
    }while(c);
}

// Employee menu
void employeeMenu(){
    int c;
    do{
        cout<<"\n1 AssignRoom 2 View FreeRooms 3 Undo 0 Logout: ";
        cin>>c;
        if(c==1) assignRoom();
        else if(c==2){
            Floor* f=floorHead;
            while(f){
                cout<<"Floor "<<f->floorNo<<": "<<countFreeRooms(f)<<" free rooms"<<endl;
                f=f->next;
            }
        }
        else if(c==3) undoBooking();
    }while(c);
}

// Admin/Employee login
void adminEmpLogin(){
    char user[20], pass[20];
    cout<<"Username: "; cin>>user;
    cout<<"Password: "; cin>>pass;

    if(match(user,(char*)"admin") && match(pass,(char*)"123")) adminMenu();
    else if(match(user,(char*)"emp") && match(pass,(char*)"123")) employeeMenu();
    else cout<<"Invalid login\n";
}

// Customer booking
void customerMenu(){
    int id,p;
    cout<<"Customer ID: "; cin>>id;
    cout<<"Priority (1 VIP 2 Normal): "; cin>>p;

    Floor* f=floorHead;
    bool booked=false;
    while(f){
        if(p==1 && f->vip){
            Room* r=f->rooms;
            while(r){ if(!r->booked){ r->booked=true; booked=true; cout<<"Booked VIP Room "<<r->roomNo<<" on Floor "<<f->floorNo<<endl; break;} r=r->next;}
        }
        else if(p==2 && !f->vip){
            Room* r=f->rooms;
            while(r){ if(!r->booked){ r->booked=true; booked=true; cout<<"Booked Normal Room "<<r->roomNo<<" on Floor "<<f->floorNo<<endl; break;} r=r->next;}
        }
        if(booked) break;
        f=f->next;
    }
    if(!booked) cout<<"No rooms available for selected type\n";
}

// Main
int main(){
    initHotel();
    int choice;
    do{
        cout<<"\n--- HOTEL MANAGEMENT SYSTEM ---\n";
        cout<<"1 Login (Admin/Employee)\n2 Book Room (Customer)\n0 Exit\nChoice: ";
        cin>>choice;

        if(choice==1) adminEmpLogin();
        else if(choice==2) customerMenu();

    }while(choice!=0);
}