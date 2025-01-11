#include <vector>
#include <string>
#include <algorithm> 
#include <random>
#include <chrono>


const int NUMBER_OF_STATUSES = 3; 
using namespace std;


class User;  
class Status;  // Add this
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
    Telecom_operater(const string& username, const string& id) 
        : username_(username), id_(id) {}

    string getUsername() const { return username_; }
    string getId() const { return id_; }

    void setUsername(const string& username) { username_ = username; }
    void setId(const string& id) { id_ = id; }

    // User management methods
    void addUser(User* user);
    void removeUser(User* user);
    const vector<User*>& getUsers() const { return users_; }

private:
    string username_;
    string id_;
    vector<User*> users_;  // sacuva pointer na usere, da ne cuva dirketno citave objekte, previse memorije uzima
};

class User {
public:

    // konstruktor. omogucava one to many relationship sa operaterima. Svkai user ima operatera, svkai operater ima vise usera
    User() = default;
    // koristi pointer da direktno uzme operatera na kojeg je assigned ovaj specifican user

    // mora projveriti postoji lli uopste operater, ako postoji koristi addUser, add user ocekuje usera, u ovom slucaju to je "this(ovaj specifican objekat)"
    User(const string& name, const string& id, Telecom_operater* operater)
        : name_(name), id_(id), operator_(operater) {
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


