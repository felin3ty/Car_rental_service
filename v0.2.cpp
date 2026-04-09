#include <iostream>
#include <string>
#include<vector>

using namespace std;

class Vehicles{ //δημιουργουμε αρχικα τις κλασεις που θα χρησιμοποιησουμε    
private:
    string license_plate;
    string model_type;
    string category;
    string fuel_type;
    bool available;
    float daily_cost;

    public:
        Vehicles(string plate , string model , string categ , string fuel , float cost)  // αυτος ειναι ο constructor του Vehicles
        : license_plate(plate) ,model_type(model) ,  category(categ) , fuel_type(fuel) ,available(true) ,  daily_cost(cost){}
    
    virtual ~Vehicles() = default; //αυτος ειναι ο destructor

    string getlicense_plate() const{ return license_plate;}
    float getdaily_cost() const{return daily_cost;}

    virtual void printDetails() const{ //εδω κανουμε το προγραμμα να εμφανιζει τις πληροφοριες του καθε αμαξιου
        cout<<"Vehicle: " << model_type << " ["  << license_plate << "] - " << daily_cost << "$/day\n"; 
    }  
};


class Customers{//κλαση με τα χαρακτηριστικα για τους πελατες
    private:
        string customer_name;
        int customer_id;

    public:
        Customers(int id , string name)
        : customer_id(id) , customer_name(name){}

    string getName() const{return customer_name;}
    int getID() const{return customer_id;}
};


class Reservation{//κλαση για τις κρατησεις
    protected:
        int Reservation_number;
        Customers* Customer;    //δεικτες για το ποιος πελατης θελει ποιο οχημα
        Vehicles* Vehicle;
        time_t Start_Date; //ημερομηνιες για την κρατηση
        time_t End_Date;
        bool Reservation_Status = true;

        public:
            Vehicles* getVehicle() const { return Vehicle; }
            Reservation(int number , Customers* c , Vehicles* v , time_t start , time_t end)
            :Reservation_number(number) , Customer(c) , Vehicle(v) , Start_Date(start) , End_Date(end){}
            
            bool double_booking(time_t new_start , time_t new_end) const{ //Εδω ελεγχει αμα εχει γινει διπλοκρατηση
                if(Reservation_Status == false){ //αν το στατους της κρατησης ειναι φαλσ τοτε δεν δημιουργειται διπλοκρατηση
                    return false;
                }
                else{
                    return(new_start<End_Date) && (new_end>Start_Date); //εδω ελεγχει τις ημερομηνιες και βλεπει αμα θα δημιουργηθει διπλοκρατηση και αμα γινει επιστεφει Α ή Ψ 
                }
            }
};

class RentalContract: public Reservation{ //κληρονομικοτητα απο την πανω καλση 
    private:
        time_t return_date;
        bool has_insurance;
    
    public:
        RentalContract(int number , Customers* c , Vehicles* v , time_t start , time_t end , time_t return_d , bool insurance )// χρησιμοποοιυμε πρωτα τον constructor του Reservation
        : Reservation(number , c , v ,  start , end) , return_date(return_d) , has_insurance(insurance){}

        float Total_Cost() const{ //υπολογισμος του τελικου κοστους
            int booked_days = End_Date - Start_Date;
            if(End_Date - Start_Date == 0){
                booked_days = 1;
            }

        float total = booked_days * Vehicle->getdaily_cost();
        
        if(has_insurance == true){//ελεγχει αμα εχει ασφαλεια και χρεωνει 12 τηβν μερα
            total += (booked_days * 12.0);
        }

        if(return_date > End_Date){ // ελεγχει αμα το επεστρεψε αργοτερα απο το κανονικο
            int late_days = return_date - End_Date;
            float extra = Vehicle->getdaily_cost() * 2.0;
            total +=(late_days * extra);
            cout << "Προστεθηκε προστιμο γιατι το οχημα καθυστερησε " << late_days << "μερες\n"; 
        }
    }
};


class RentalAgency{ //κλαση για την εταιρια μας
    private: 
        vector<Vehicles*> total_vehicles; //συνολικος αριθμος οχηματων που διαθετουμε
        vector<Customers*>total_customers;//οι πελατες της εταιριας
        vector<Reservation>total_reservations;//οι κρατησεις που εχουν γινει

    public://εδω προσθετουμε οχηματα για την εταιρια μας
        void add_vehicle(Vehicles* v){
            total_vehicles.push_back(v);
        }
    
    void add_customers(Customers* c){
        total_customers.push_back(c);
    }        
    

    bool make_reservation(int reserv_numb , Customers* c , Vehicles* v , time_t start , time_t end){//ΣΥΝΑΡΤΗΣΗ ΓΙΑ ΤΙΣ ΚΡΑΤΗΣΕΙΣ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        for(const Reservation& res:total_reservations){

        }
    }
};
