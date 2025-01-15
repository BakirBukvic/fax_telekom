#include <vector>
#include <string>
#include <algorithm> 
#include <random>
#include <chrono>
#include <iostream>
#include <functional>

const int NUMBER_OF_STATUSES = 3; 
using namespace std;


class User;  
class Status;  
class Telecom_operater;

using std::string;
using std::vector;
using std::random_device;


class Status {
public:
    Status() : status_1(false), status_2(false), status_3(false) {}

    // Getters
    bool getStatus1() const { return status_1; }
    bool getStatus2() const { return status_2; }
    bool getStatus3() const { return status_3; }

    // Setters
    void setStatus1(bool val) { status_1 = val; }
    void setStatus2(bool val) { status_2 = val; }
    void setStatus3(bool val) { status_3 = val; }

    // metoda da promijeni status 50/50 sansa 
    bool switchStatus(int statusNumber) {
        // koristi random biblioteku, dadne neki broj
        // ako je taj broj neparan onda se mijenja status

        // zasto sve ovo? jer mora napraviti novi seed. Ako se kompajlira programr broj ce uvijek biti isti
        // ovaj kod uzme trenutno vrijeme i onda taj borj sluzi kao seed.
        // bez ovog su uvijek isti brojevi

        random_device rd;
        mt19937 gen(rd() + chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<> dis(0, 1);
   

       
        if (dis(gen)){
            switch(statusNumber) {
                case 1:
                    status_1 = !status_1;
                    break;
                case 2:
                    status_2 = !status_2;
                    break;
                case 3:
                    status_3 = !status_3;
                    break;
                default:
                    return false;
            }
            return true;
        }
        return false;
    }

private:
    bool status_1;
    bool status_2;
    bool status_3;
};





class Telecom_operater {
public:

    Telecom_operater() = default;


    Telecom_operater(const string& username, const string& password) 
        : username_(username) {
        id_ = "OP" + to_string(++next_id_); // napravi OP pa bro sledeci u inkrementaciji
        password_hash_ = hashPassword(password);
    }


    string getUsername() const { return username_; }
    string getId() const { return id_; }

    // size_t garantuje da ce objekat imati dovoljno prostora u memoriji
    size_t hashPassword(const string& password) const {
        return hash<string>{}(password);
    }

    bool checkPassword(const string& input_password) const {
        return hashPassword(input_password) == password_hash_;
    }


    void setUsername(const string& username) { username_ = username; }
    void setId(const string& id) { id_ = id; }

    // User management methods
    void addUser(User* user);
    void removeUser(User* user);
    const vector<User*>& getUsers() const { return users_; }

private:
    static int next_id_;
    string username_;
    string id_;
    vector<User*> users_;  // sacuva pointer na usere, da ne cuva dirketno citave objekte, previse memorije uzima
    size_t password_hash_; 
};

class User {
public:

    // konstruktor. omogucava one to many relationship sa operaterima. Svkai user ima operatera, svkai operater ima vise usera
    User() = default;
    // koristi pointer da direktno uzme operatera na kojeg je assigned ovaj specifican user

    // mora projveriti postoji lli uopste operater, ako postoji koristi addUser, add user ocekuje usera, u ovom slucaju to je "this(ovaj specifican objekat)"
    User(const string& name, Telecom_operater* operater)
        : name_(name), operator_(operater) {
        id_ = "USER" + to_string(++next_id_);
        if (operater) operater->addUser(this);
        // Initialize with 3 default statuses


        for (int i = 0; i < NUMBER_OF_STATUSES; i++) {
            statuses_.push_back(new Status());
        }
    }


// destruktor. Ako se izbrise user, izbrisi i asocirane statuse. preventira memory leak :)
    ~User() {
    for (Status* status : statuses_) {
        delete status;
    }
}
    


    string getName() const { return name_; }
    string getId() const { return id_; }
    Telecom_operater* getOperator() const { return operator_; }

     Status* getStatus(int index) const {  // Add this getter
        if (index >= 0 && index < statuses_.size()) {
            return statuses_[index];
        }
        return nullptr;
    }

    void setOperator(Telecom_operater* op);

private:
    string name_;
    string id_;
    Telecom_operater* operator_;
    vector<Status*> statuses_;
    static int next_id_;
};








// zasto inline? da bi ostavio sve funkcije (metode) u header fajlu.
// moras ih poslije zvati u cpp fajlu, ovdje je samo deklaracija. 

inline void Telecom_operater::addUser(User* user) {
    users_.push_back(user);
}

inline void Telecom_operater::removeUser(User* user) {
    users_.erase(
        remove(users_.begin(), users_.end(), user),
        users_.end()
    );
}

inline void User::setOperator(Telecom_operater* op) {
    if (operator_) {
        operator_->removeUser(this);
    }
    operator_ = op;
    if (op) {
        op->addUser(this);
    }
}





class Terminal {



public:
    Terminal() : current_operator_(nullptr) {}


    // main loop


    void run() {
        int choice;
        bool logged_in = false;

        // login forced
        while (!logged_in) {
            displayLoginMenu();
            cin >> choice;
            
            switch(choice) {
                case 1: 
                    createOperator(); 
                    break;
                case 2: 
                    logged_in = login(); 
                    break;
                case 0: 
                    cout << "Exiting...\n"; 
                    return;
                default: 
                    cout << "Invalid choice.\n";
            }
        }   

        // main menu
        while (true) {
            displayMainMenu();
            cin >> choice;
            
            switch(choice) {
                case 1: createUser(); break;
                case 2: displayUsers(); break;
                case 3: manageStatuses(); break;
                case 4: 
                    cout << "Logging out...\n";
                    return;
                default: 
                    cout << "Invalid choice.\n";
            }
        }
    }


    void printOperatorName() const{
        cout<<"Operator: "<< current_operator_->getUsername();
        cout<<"    "<<"ID: "<<current_operator_->getId()<<endl;


    }


  



    bool login() {
            string id, password;
            cout << "Enter operator ID: ";
            cin >> id;
            cout << "Enter password: ";
            cin >> password;

           
            
            current_operator_ = findOperator(id);
            if(current_operator_ && current_operator_->checkPassword(password)) {
                cout << "Logged in as: " << current_operator_->getUsername() << "\n";
                return true;
            }
            cout << "Login failed.\n";
            return false;
        }


bool isOperatorUsernameTaken(const string& username) const {
        for(const auto& op : operators_) {
            if(op->getUsername() == username) {
                return true;
            }
        }
        return false;
    }




    void createOperator() {
                string username, id, password;
                cout << "Enter operator username: ";
                cin >> username;

            // ne da dalje ako username vec postoji
                if(isOperatorUsernameTaken(username)) {
            cout << "Error: Username '" << username << "' is already taken.\n";
            return;
        }


                cout << "Enter operator password: ";
                cin >> password;
                

                auto new_operator = new Telecom_operater(username, password);
                operators_.push_back(new_operator);


                cout << "Operator created successfully with ID: " << new_operator->getId() << endl;
                }

    void createUser() {
        system("cls");
        if (current_operator_)printOperatorName();
        else cout<<"How did you get here withouth logging in?";
        string name, id;
        cout << "Enter user name: ";
        cin >> name;

       
        
        
        users_.push_back(new User(name, current_operator_));

        cout << "User created successfully.\n";
    }

    void displayUsers() const {
        system("cls");
        
        if (current_operator_){
            printOperatorName();}
        else cout<<"How did you get here withouth logging in?";
        int x;
        cout << "\n=== Users List for Operator " << current_operator_->getUsername() << " ===\n";
        for(const auto user : current_operator_->getUsers()) {
            cout << "ID: " << user->getId() 
                 << " | Name: " << user->getName() << "\n";
        }
        cout<<endl;

        cout<< "enter any value to continue"<<endl;
        cin >> x;

            }

    void manageStatuses() {
        system("cls");
        if (current_operator_)printOperatorName();
        else cout<<"How did you get here withouth logging in?";
        string id;
        cout << "Enter user ID: ";
        cin >> id;
        
        auto user = findUser(id);
       if(user && user->getOperator() == current_operator_) {
        int choice;
        do {
            system("cls");
            printOperatorName();
            cout << "\nUser: " << user->getName() << "\n\n";
            
            // Print status table
            cout << "=====================================\n";
            cout << "Status 1 | Status 2 | Status 3\n";
            cout << "=====================================\n";
            cout << "   " << user->getStatus(0)->getStatus1() 
                 << "     |    " << user->getStatus(1)->getStatus1()
                 << "     |    " << user->getStatus(2)->getStatus1() << "\n";
            cout << "=====================================\n\n";
            
            cout << "1-3. Switch Status\n";
            cout << "0. Exit to Main Menu\n";
            cout << "Choice: ";
            cin >> choice;
            
            if(choice >= 1 && choice <= 3) {
                user->getStatus(choice-1)->switchStatus(1);
            }
        } while(choice != 0);
        
        cout << "\nPress any key to continue...";
        cin.ignore();
        cin.get();



        } else {
            cout << "User not found or unauthorized.\n";
        }
    }

    ~Terminal() {
        for(auto user : users_) delete user;
        for(auto op : operators_) delete op;
    }






private:
    vector<User*> users_;
    vector<Telecom_operater*> operators_;
    Telecom_operater* current_operator_;
    
    Telecom_operater* findOperator(const string& id) {
        for(auto op : operators_)
            if(op->getId() == id) return op;
        return nullptr;
    }
    
    User* findUser(const string& id) {
        for(auto user : users_)
            if(user->getId() == id) return user;
        return nullptr;
    }
    
    void displayLoginMenu() const {
        cout << "\n=== Login Menu ===\n"
             << "1. Create Operator\n"
             << "2. Login\n"
             << "0. Exit\n"
             << "Choice: ";
    }

    void displayMainMenu() const {
        system("cls");
        if (current_operator_)printOperatorName();
        else cout<<"How did you get here withouth logging in?";
       
        cout << "\n=== Main Menu ===\n"
             << "1. Create User\n"
             << "2. Display Users\n"
             << "3. Manage Statuses\n"
             << "4. Logout\n"
             << "Choice: ";
    }
};


