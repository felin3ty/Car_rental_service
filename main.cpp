#include <iostream>
#include <string>
#include<vector>

using namespace std;

class Vehicles{ //δημιουργουμε αρχικα τις κλασεις που θα χρησιμοποιησουμε    
protected:
    string license_plate;
    string category;
    string fuel_type;
    bool available;
    float daily_cost;

    public:
        Vehicles(string plate , string categ , string fuel , float cost)  // αυτος ειναι ο constructor του Vehicles
        : license_plate(plate) , category(categ) , fuel_type(fuel) ,available(true) ,  daily_cost(cost){}
    
    virtual ~Vehicles() = default; //αυτος ειναι ο destructor

    string getlicense_plate() const{ return license_plate;}
    float getdaily_cost() const{return daily_cost;}
};

