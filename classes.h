#ifndef CLASSES_H
#define CLASSES_H

using namespace std;
#include <string>
#include <vector>



class User {
public:


    
    User() = default;
    User(const string& username, const string& id);

 
    string getUsername() const { return username_; }
    string getId() const { return id_; }

    void setUsername(const string& username) { username_ = username; }
    void setId(const string& id) { id_ = id; }

private:
    // Private member variables
    string username_;
    string id_;
};






#endif