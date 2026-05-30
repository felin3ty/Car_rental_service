#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;

class Vehicles{ //δημιουργουμε αρχικα τις κλασεις που θα χρησιμοποιησουμε    
protected:
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
            int getReservation_number() const { return Reservation_number; }
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
            int booked_days = (End_Date - Start_Date)/86400;
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

        return total;
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
            cout<<"Το οχημα προστεθηκε επιτυχως\n";
        }
        
        void show_cars() const {
        if(total_vehicles.empty()) {
            cout << "Ο στολος ειναι αδειος.\n";
            return;
        }
        for(auto v : total_vehicles) {
            v->printDetails();
        }
        }

        void delete_vehicles(string plate){
        for(auto it = total_vehicles.begin(); it != total_vehicles.end(); ++it){
            if((*it)->getlicense_plate() == plate){
                delete *it; 
                total_vehicles.erase(it); 
                cout << " [V] Το οχημα διαγραφηκε!\n";
                return;
            }
        }
                cout << " [X] Σφαλμα: Δεν βρεθηκε οχημα με πινακιδα " << plate << "\n";
                }
        
        void add_customers(Customers* c){
            total_customers.push_back(c);
        } 
        void show_customers() const {
        if(total_customers.empty()) {
            cout << "Δεν υπάρχουν πελάτες.\n";
            return;
        }
        }
        void load_data(){
            string skip;
            ifstream v_file("vehicles.txt");
            if(v_file.is_open()){
                string lp , m , cat , f;
                float cost;

                while(v_file >> skip >> lp >> skip >> m >> skip >> cat >> skip >> f >> skip >> cost >> skip >> skip >> skip){
                    total_vehicles.push_back(new Vehicles(lp , m , cat ,f, cost));
                }
                v_file.close();
            }
            ifstream c_file("customers.txt");
            if(c_file.is_open()){
                int id;
                string name;

                while(c_file >> skip >> id >> skip >> name){
                    total_customers.push_back(new Customers(id , name));
                }
                c_file.close();
            }
        }
        void show_reservations() const {
        if(total_reservations.empty()) {
            cout << "Δεν υπάρχουν αιτήσεις για κράτηση.\n";
            return;
        }
        }
        
            bool make_reservation(int reserv_numb , Customers* c , Vehicles* v , time_t start , time_t end){//ΣΥΝΑΡΤΗΣΗ ΓΙΑ ΤΙΣ ΚΡΑΤΗΣΕΙΣ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            for(const Reservation& res:total_reservations){
                if(res.getVehicle() == v){// ελεγχει αμα υπαρχει προβλημα για την κρατηση
                    if(res.double_booking(start , end) == true){
                        cout<< "το οχημα ειναι ηδη κρατημενο αυτες τις ημερομηνιες\n";
                        return false;
                    }
                }
            }
            Reservation new_res(reserv_numb , c , v , start , end); //εδω αμα δεν υπαρχει υεμα γνεται η κρατηση
            total_reservations.push_back(new_res);
            cout<<"Εγινε κρατηση με επιτυχια΄\n";
            return true;
        }
        void delete_customers(string id){
            if(total_customers.empty()) {
                cout << "Δεν υπάρχουν πελάτες για διαγραφή.\n";

        }
    }

    void delete_reservation(int res_num){
        if(total_reservations.empty()){
            cout<<"Δεν υπαρχουν κρατησεις για διαγραφη\n";
            return;
        }
        for(int i = 0; i<total_reservations.size(); i++){
           if (total_reservations[i].getReservation_number() == res_num){
                total_reservations.erase(total_reservations.begin() +i);
                cout<<"Η κρατηση εχει διαγραφει \n";

            }
        }
    }
    };

time_t string_to_time(const string& date_temp){
    struct tm t ={0};
    int year , month , day;
    if(sscanf(date_temp.c_str() , "%d-%d-%d" ,  &year , &month , &day) == 3){
        t.tm_year = year-1900;
        t.tm_mon = month;
        t.tm_mday = day;
        return mktime(&t);
    }
} 

int main(){
    RentalAgency agency;
    agency.load_data();
    fstream vehicles("vehicles.txt", ios::app | ios::in);
    fstream tempv("temp_vehicles.txt", ios::out); //αυτό το αρχείο το χρησιμοποιούμε για να διαγράφουμε οχήματα από το αρχικό αρχείο μας
    fstream customers("customers.txt", ios::app | ios::in);
    fstream tempc("temp_customers.txt", ios::out); //αυτό το αρχείο το χρησιμοποιούμε για να διαγράφουμε πελάτες από το αρχικό αρχείο μας
    fstream reservations("reservations.txt", ios::app | ios::in);
    fstream tempr("temp_reservations.txt", ios::out); //αυτό το αρχείο το χρησιμοποιούμε για να διαγράφουμε κρατήσεις από το αρχικό αρχείο μας

    int choice1=0 , choice2=0 , choice3=0, choice4=0, choice5=0, yn; //αυτες οι μεταβλητες χρησιμοποιουνται για τα μενου επιλογων
    do{
        choice2 =0; choice3=0; choice4 = 0; choice5=0;
        cout<<"ΜΕΝΟΥ ΕΠΙΛΟΓΩΝ\n"; //τα μενου δεν τα εξηγω δεν χρεαιζεραι
        cout<<"1. Είσοδος Διαχειριστή\n";
        cout<<"2. Είσοδος Πελάτη\n";
        cout<<"3. Έξοδος\n";
        cin>> choice1;


        if(choice1 == 1){
            cout << "\n=== ΜΕΝΟΥ ΔΙΑΧΕΙΡΙΣΤΗ ===\n";
            cout << "1. Διαχείριση Στόλου\n";
            cout << "2. Διαχείριση Πελατών\n";
            cout << "3. Διαχείριση Κρατήσεων\n";
            cout << "4. Έξοδος\n";
            cin >> choice2;
        
            if(choice2 == 1){
            cout << "\n=== ΜΕΝΟΥ ΔΙΑΧΕΙΡΙΣΗΣ ΣΤΟΛΟΥ ===\n";
            cout << "1. Προβολή Στόλου\n";
            cout << "2. Προσθήκη Νέου Οχήματος\n";
            cout << "3. Διαγραφή Οχήματος (Πινακίδα)\n";
            cout << "4. Έξοδος\n";
            cout << "Επιλογή: ";
            cin >> choice3;
            }

                      if(choice3 == 1){
                    agency.show_cars();   
                    cout << vehicles.rdbuf();
                }
                     else if(choice3 == 2){
                    string lp, m, cat, f; 
                    float c; 

                    cout << "Πινακιδα: (Παρακαλείστε να χρησιμοποίησετε την μορφή ΧΧΧ-0000)\n";
                    cin >> lp;
                    cin.ignore(10000, '\n');
                    if(lp.length() !=8){
                        cout << "Λάθος μορφή πινακίδας. Παρακαλώ εισάγετε ξανά.\n";
                        }
                        while(lp.length() !=8){
                            cout << "Πινακιδα: (Παρακαλείστε να χρησιμοποίησετε την μορφή ΧΧΧ-0000)\n";
                            cin >> lp;
                            cin.ignore(10000, '\n');
                        }
                    cout << "Μοντελο:\n";
                    cin >> m;
                    cin.ignore(10000, '\n');
                    cout << "Κατηγορια:\n";
                    cin >> cat;
                    cin.ignore(10000, '\n');
                    cout << "Καυσιμο:\n";
                    cin >> f;
                    cin.ignore(10000, '\n');
                    cout << "Κοστος ανα μερα:\n";
                    cin >> c;
                    cin.ignore(10000, '\n');
                    cout << "Είστε σίγουροι ότι θέλετε να προσθέσετε το όχημα; (1: Ναι, 2: Όχι)\n";
                    cin >> yn;
                    if(yn == 1){

                    agency.add_vehicle(new Vehicles(lp, m, cat, f, c));
                    vehicles << "Πινακίδα: " << lp << " Μοντέλο: " << m << " Κατηγορία: " << cat << " Καύσιμο: " << f << " Κόστος: " << c << " € την ημέρα" << endl; //εδώ αποθηκεύουμε τα οχήματα στο αρχείο μας
                }
                else if(yn != 1){
                    cout << "Η προσθήκη του οχήματος ακυρώθηκε.\n";
                    return 1;
                }
            }
                    else if(choice3 == 3){
                    string plate_to_delete;
                     cout << "Δώστε την πινακίδα του οχήματος για διαγραφή: ";
                     cin >> plate_to_delete;

   
                     agency.delete_vehicles(plate_to_delete);

                     vehicles.close(); 
                     tempv.close();    
                     ifstream infile("vehicles.txt");
                     ofstream outfile("temp_vehicles.txt");
                     string line;
                     bool found_in_file = false;

                     while (getline(infile, line)) {
        
                     if (line.find(plate_to_delete) == string::npos) {
                     outfile << line << "\n";
                     } else {
                      found_in_file = true; 
                    }
                    }

                      infile.close();
                     outfile.close();

                      remove("vehicles.txt");
                     rename("temp_vehicles.txt", "vehicles.txt");

                       vehicles.open("vehicles.txt", ios::app | ios::in);

                      if(found_in_file) {
                      cout << " [V] Το όχημα διαγράφηκε και από το αρχείο επιτυχώς!\n";
                      } else {
                        cout << " [X] Το όχημα δεν βρέθηκε στο αρχείο.\n";
                    }
                    }
                    
                
                     else if(choice3 == 4){
                    cout<<"Το προγραμμα τερματίστηκε με επιτυχια";
                    return 1;
                }
            
            
            if(choice2 == 2){
                    cout << "=== ΜΕΝΟΥ ΔΙΑΧΕΙΡΙΣΗΣ ΠΕΛΑΤΩΝ ===\n";
                    cout << "1. Προβολή Πελατών\n";
                    cout << "2. Προσθήκη Νέου Πελάτη\n";
                    cout << "3. Διαγραφή Πελάτη (ID)\n";
                    cout << "4. Έξοδος\n";
                    cout << "Επιλογή: ";
                    cin >> choice4;

                    if(choice4 == 1){
                        agency.show_customers();
                        cout << customers.rdbuf();
                    }
                    else if(choice4 == 2){
                        float id;
                        string name;

                        cout << "ID Πελάτη:\n";
                        cin >> id;
                        cout << "Όνομα Πελάτη:\n";
                        cin >> name;

                        agency.add_customers(new Customers(id, name));
                        customers << "ID: " << id << " Όνομα: " << name << endl; //εδώ αποθηκεύουμε τους πελάτες στο αρχείο μας
                    }
                    else if(choice4 == 3){
                        string id_to_delete;
                        cout << "Δώστε το ID του πελάτη για διαγραφή: ";
                        cin >> id_to_delete;
                        
                        agency.delete_customers(id_to_delete);

                        customers.close();
                        tempc.close();
                        ifstream infile("customers.txt");
                        ofstream outfile("temp_customers.txt");
                        string line;
                        bool found_in_file = false;
                        
                        while (getline(infile, line)) {
                            if (line.find(id_to_delete) == string::npos) {
                                outfile << line << "\n";
                            } else {
                                found_in_file = true; 
                            }
                        }
                        infile.close();
                        outfile.close();

                        remove("customers.txt");
                        rename("temp_customers.txt", "customers.txt");

                        customers.open("customers.txt", ios::app | ios::in);
                        if(found_in_file) {
                            cout << " [V] Ο πελάτης διαγράφηκε και από το αρχείο επιτυχώς!\n";
                        } else {
                            cout << " [X] Ο πελάτης δεν βρέθηκε στο αρχείο.\n";
                        }
                    }
                    else if(choice4 == 4){
                        cout<<"Το πρόγραμμα τερματίστηκε με επιτυχία";
                        return 1;
                    }
                }
            if(choice2 == 3){
                    cout << "=== ΜΕΝΟΥ ΔΙΑΧΕΙΡΙΣΗΣ ΚΡΑΤΗΣΕΩΝ ===\n";
                    cout << "1. Προβολή Κρατήσεων\n";
                    cout << "2. Δημιουργία Νέας Κράτησης\n";
                    cout <<"3.Εκδοση Αποδειξης\n";
                    cout << "4. Ακύρωση Κράτησης\n";
                    cout << "5. Έξοδος\n";
                    cout << "Επιλογή: ";
                    cin >> choice5;

                    if(choice5 == 1){
                          agency.show_reservations();
                    }
                    else if(choice5 == 2){
                        int reserv_numb;
                        int id;
                        int asfaleia_choice;
                        bool asfaleia = false;
                        string lp;
                        Customers* c;
                        Vehicles* v;
                        time_t start;
                        time_t end;
                        cout << "Αριθμός Κράτησης:\n";
                        cin >> reserv_numb;
                        cout << "ID Πελάτη:\n";
                        cin >> id;
                        cout << "Πινακίδα Οχήματος:\n";
                        cin >> lp;
                        cout << "Ημερομηνία Έναρξης (YYYY-MM-DD):\n";
                        ////when u lowk want to rape ur hb 
                        cout << "Ημερομηνία Λήξης (YYYY-MM-DD):\n";
                        ////SYSTHMA MERAS REE
                       
                       // agency.make_reservation() (??????)
                    }
                    if(choice5 == 3){
                        string start_temp , end_temp , return_temp;
                        float daily_price , total;
                        int asfaleia_choice;
                        bool asfaleia;
                        cout << "\n=== ΕΠΙΣΤΡΟΦΗ ΟΧΗΜΑΤΟΣ & ΕΚΔΟΣΗ ΑΠΟΔΕΙΞΗΣ ===\n";
                        cout << "Ημερομηνία Έναρξης της κράτησής σας (YYYY-MM-DD): ";
                        cin >> start_temp;
                        cout << "Ημερομηνία που ΕΠΡΕΠΕ να το επιστρέψετε (YYYY-MM-DD): ";
                        cin >> end_temp;
                        cout << "Ημερομηνία Πραγματικής Επιστροφής (YYYY-MM-DD): ";
                        cin >> return_temp;
                        cout << "Ημερήσιο Κόστος του οχήματος που είχατε ($): ";
                        cin >> daily_price;
                        cout << "Είχατε επιλέξει ασφάλεια; (1: Ναι, 2: Όχι): ";
                        cin >> asfaleia_choice;
                        if(asfaleia_choice == 1){
                            asfaleia = true;
                        }
                        time_t start = string_to_time(start_temp);
                        time_t end = string_to_time(end_temp);
                        time_t return_d = string_to_time(return_temp);
                        Vehicles temp_car("TEMP", "TEMP", "TEMP", "TEMP", daily_price); //to kanoyme mono gia na exoyme apetheias to daily price(dp)
                        RentalContract contract(101, nullptr, &temp_car, start, end, return_d, asfaleia);
                        total = contract.Total_Cost();
                        cout<<"ΣΥΝΟΛΙΚΟ ΠΟΣΟ" , total;
                        
                    }
                    else if(choice5 == 4){
                        int reserv_to_cancel;
                        cout << "Δώστε τον αριθμό της κράτησης για ακύρωση: ";
                        cin >> reserv_to_cancel;

                        agency.delete_reservation(reserv_to_cancel);

                        reservations.close();
                        tempr.close();
                        ifstream infile("reservations.txt");
                        ofstream outfile("temp_reservations.txt");
                        string line;
                        bool found_in_file = false;
                        while (getline(infile, line)) {
                            if (line.find(to_string(reserv_to_cancel)) == string::npos) {
                                outfile << line << "\n";
                            } else {
                                found_in_file = true; 
                            }
                        }
                        infile.close();
                        outfile.close();
                        remove("reservations.txt");
                        rename("temp_reservations.txt", "reservations.txt");
                        reservations.open("reservations.txt", ios::app | ios::in);
                        if(found_in_file) {
                            cout << " [V] Η κράτηση ακυρώθηκε και από το αρχείο επιτυχώς!\n";
                        } else {
                            cout << " [X] Η κράτηση δεν βρέθηκε στο αρχείο.\n";
                        }
                    }
                    else if(choice5 == 5){
                        cout<<"Το πρόγραμμα τερματίστηκε με επιτυχία";
                        return 1;
                    }
                }
            }
            
                
         if(choice1 == 2){
            cout << "=== ΣΥΣΤΗΜΑ ΕΝΟΙΚΙΑΣΗΣ ΟΧΗΜΑΤΩΝ ===\n";
            cout << "1. Εμφανιση Διαθεσιμων Οχηματων\n";
            cout << "2. Δημιουργια Νεας Κρατησης\n";
            cout << "3. Προβολή Ιστορικού Κρατήσεων\n";
            cout << "4. Εξοδος\n";
            cout << "Επιλεξτε ενεργεια (0-3): ";
            cin >> choice5;

            if(choice5 == 1){
                agency.show_cars();
                cout << vehicles.rdbuf();
            }
            if(choice5 == 2){
                int reserv_numb;
                int id;
                string lp;
                Customers* c;
                Vehicles* v;
                string start_temp;
                string end_temp;
                cout << "Αριθμός Κράτησης:\n";
                cin >> reserv_numb;
                cout << "ID Πελάτη:\n";
                cin >> id;
                cout << "Πινακίδα Οχήματος:\n";
                cin >> lp;
                cout << "Ημερομηνία Έναρξης (YYYY-MM-DD):\n";
                cin >> start_temp;
                time_t start = string_to_time(start_temp);
                cout << "Ημερομηνία Λήξης (YYYY-MM-DD):\n";
                cin >> end_temp;
                time_t end = string_to_time(end_temp);
            }
        }
    }while(choice1 != 3); 
return 0;
}   
