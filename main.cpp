#include <iostream>
#include "classes.h"

using namespace std;

int main() {
    
    Telecom_operater* ht = new Telecom_operater("HTnet", "ht001");
    User* user1 = new User("John", "u001", ht);

    cout << "User: " << user1->getName() << endl;
    cout << "\nInitial Status Values:" << endl;
    for (int i = 0; i < NUMBER_OF_STATUSES; i++) {
        Status* status = user1->getStatus(i); 
        if (status) {
            cout << "Status " << (i+1) << ":" << endl;
            cout << "Status1: " << status->getStatus1() << endl;
            cout << "Status2: " << status->getStatus2() << endl;
            cout << "Status3: " << status->getStatus3() << endl;
            cout << "-------------------" << endl;
        }
    }

   
    cout << "\nTrying to switch statuses..." << endl;
    for (int i = 0; i < NUMBER_OF_STATUSES; i++) {
        Status* status = user1->getStatus(i);
        if (status) {
            bool changed = status->switchStatus(1);
            cout << "Status " << (i+1) << " switch attempt: " 
                 << (changed ? "changed" : "unchanged") << endl;
        }
    }

  
    delete user1;  
    delete ht;

    return 0;
}