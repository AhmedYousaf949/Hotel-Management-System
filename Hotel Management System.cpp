#include <iostream>
using namespace std;
struct Room {
    int roomNo;
    bool booked;
    bool vip;
    int view;  
    int smoke;  
    Room* next;
};

struct Floor {
    int floorNo;
    Room* rooms;
    bool vip;
    Floor* next;
    Floor* prev;
};

struct History {
    int customerId;
    int floorNo;
    int roomNo;
    bool vip;
    int view;   
    int smoke;  
    History* next;
};

Floor* floorHead = NULL;
History* historyTop = NULL;

bool match(char a[], char b[]) {
    int i=0;
    while(a[i] && b[i]){
        if(a[i]!=b[i]) return false;
        i++;
    }
    return a[i]=='\0' && b[i]=='\0';
}
void initHotel() {
    for(int f=1; f<=20; f++){
        bool vipFloor = (f>11);
        Floor* fl = new Floor{f,NULL,vipFloor,NULL,NULL};
        if(!floorHead) floorHead=fl;
        else {
            Floor* t=floorHead;
            while(t->next) t=t->next;
            t->next=fl;
            fl->prev=t;
        }
        int roomCount = vipFloor ? 60 : 80;
        for(int r=1; r<=roomCount; r++){
            int view=1+rand()%2; 
            int smoke=1+rand()%2; 
            if(!vipFloor){ view=0; smoke=0; } 
            Room* rm = new Room{r,false,vipFloor,view,smoke,fl->rooms};
            fl->rooms=rm;
        }
    }
}
int countFreeRooms(Floor* f){
    int cnt=0;
    Room* r=f->rooms;
    while(r){ if(!r->booked) cnt++; r=r->next; }
    return cnt;
}
int countBookedRooms(Floor* f){
    int cnt=0;
    Room* r=f->rooms;
    while(r){ if(r->booked) cnt++; r=r->next; }
    return cnt;
}
void undoBooking() {
    if(!historyTop){ cout<<"No booking to undo\n"; return;}
    int roomNo = historyTop->roomNo;
    int floorNo = historyTop->floorNo;
    Floor* f=floorHead;
    while(f){
        if(f->floorNo==floorNo){
            Room* r=f->rooms;
            while(r){
                if(r->roomNo==roomNo){ r->booked=false; break;}
                r=r->next;
            }
            break;
        }
        f=f->next;
    }
    History* temp=historyTop;
    historyTop=historyTop->next;
    delete temp;
    cout<<"Last booking undone\n";
}
void viewHistory(){
    if(!historyTop){ cout<<"No bookings yet.\n"; return;}
    History* h=historyTop;
    while(h){
        cout<<"Customer "<<h->customerId<<" booked ";
        cout<<(h->vip?"VIP ":"Normal ");
        cout<<"Room "<<h->roomNo<<" on Floor "<<h->floorNo;
        if(h->vip){
            cout<<" [View:"<< (h->view==1?"City":"Garden")<<", ";
            cout<<"Type:"<< (h->smoke==1?"Smoking":"Non-Smoking")<<"]";
        }
        cout<<endl;
        h=h->next;
    }
}
void adminMenu(){
    int c;
    do{
        cout << "\033[90m ";
        cout<<"\n1 View FreeRooms 2 View All Rooms 3 View History 4 Undo 0 Logout: ";
        cout << "\033[0m ";
        cin>>c;
        if(c==1){
            Floor* f=floorHead;
            while(f){ cout<<"Floor "<<f->floorNo<<": "<<countFreeRooms(f)<<" free rooms"<<endl; f=f->next;}
        }
        else if(c==2){
            Floor* f=floorHead;
            while(f){ cout<<"Floor "<<f->floorNo<<": "<<countFreeRooms(f)<<" free, "<<countBookedRooms(f)<<" booked"<<endl; f=f->next;}
        }
        else if(c==3) viewHistory();
        else if(c==4) undoBooking();
    }while(c);
}
void employeeMenu(){
    int c;
    do{
        cout<<"\n1 View FreeRooms 2 Undo Last Booking 0 Logout: ";
        cin>>c;
        if(c==1){
            Floor* f=floorHead;
            while(f){ cout<<"Floor "<<f->floorNo<<": "<<countFreeRooms(f)<<" free rooms"<<endl; f=f->next;}
        }
        else if(c==2) undoBooking();
    }while(c);
}
void adminEmpLogin(){
    char user[20],pass[20];
    cout<<"Username: "; cin>>user;
    cout<<"Password: "; cin>>pass;
    if(match(user,(char*)"admin") && match(pass,(char*)"123")) adminMenu();
    else if(match(user,(char*)"emp") && match(pass,(char*)"123")) employeeMenu();
    else {
        cout << "\033[91m ";
         cout<<"Invalid login\n";
         cout << "\033[0m ";
    }
}
void customerMenu(){
    int id,p;
    cout<<"Customer ID: "; cin>>id;
    cout<<"Priority (1 VIP 2 Normal): "; cin>>p;
    int viewChoice=0, smokeChoice=0;
    if(p==1){
        cout<<"Choose Room View: 1 City View 2 Garden/No View: ";
        cin>>viewChoice;
        cout<<"Choose Room Type: 1 Smoking 2 Non-Smoking: ";
        cin>>smokeChoice;
    }

    Floor* f=floorHead;
    bool booked=false;
    while(f){
        if(p==1 && f->vip){
            Room* r=f->rooms;
            while(r){
                if(!r->booked && r->view==viewChoice && r->smoke==smokeChoice){
                    r->booked=true;
                    booked=true;
                    cout<<"Booked VIP Room "<<r->roomNo<<" on Floor "<<f->floorNo;
                    cout<<" [View:"<< (r->view==1?"City":"Garden")<<", ";
                    cout<<"Type:"<< (r->smoke==1?"Smoking":"Non-Smoking")<<"]\n";
                    History* h=new History{id,f->floorNo,r->roomNo,true,r->view,r->smoke,historyTop};
                    historyTop=h;
                    break;
                }
                r=r->next;
            }
        }
        else if(p==2 && !f->vip){
            Room* r=f->rooms;
            while(r){
                if(!r->booked){
                    r->booked=true;
                    booked=true;
                    cout<<"Booked Normal Room "<<r->roomNo<<" on Floor "<<f->floorNo<<endl;
                    History* h=new History{id,f->floorNo,r->roomNo,false,0,0,historyTop};
                    historyTop=h;
                    break;
                }
                r=r->next;
            }
        }
        if(booked) break;
        f=f->next;
    }
    if(!booked) {
        cout << "\033[91m ";
         cout<<"No rooms available for selected type/preferences\n";
         cout << "\033[0m ";
    }
}

int main(){
    srand(time(0));
    for(int f=1; f<=20; f++){
        bool vipFloor = (f>11);
        Floor* fl = new Floor{f,NULL,vipFloor,NULL,NULL};
        if(!floorHead) floorHead=fl;
        else{
            Floor* t=floorHead;
            while(t->next) t=t->next;
            t->next=fl;
            fl->prev=t;
        }
        int roomCount = vipFloor ? 60 : 80;
        for(int r=1; r<=roomCount; r++){
            int view=1+rand()%2;
            int smoke=1+rand()%2;
            if(!vipFloor){ view=0; smoke=0; }
            Room* rm = new Room{r,false,vipFloor,view,smoke,fl->rooms};
            fl->rooms=rm;
        }
    }

    int choice;
    do{
        cout << "\033[96m ";
        cout<<"\n--- Epstein Hotel ---\n";
        cout << "\033[0m ";
        cout << "\033[92m ";
        cout<<"1 Login (Admin/Employee)\n2 Book Room (Customer)\n0 Exit\nChoice: ";
        cout << "\033[0m ";
        cin>>choice;
        if(choice==1) adminEmpLogin();
        else if(choice==2) customerMenu();
    }while(choice!=0);
}