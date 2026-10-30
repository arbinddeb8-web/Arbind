#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// Base class
class Crop {
protected:
    string name, season;
    double area, yieldPerAcre;
    static int count;

public:
    Crop(string n, double a, string s, double y) {
        name = n;
        area = a;
        season = s;
        yieldPerAcre = y;
        count++;
    }

    virtual ~Crop() { count--; }

    double totalYield() const {
        return area * yieldPerAcre;
    }

    string getName() const {
        return name;
    }

    static int getCount() {
        return count;
    }

    bool operator>(const Crop& other) const {
        return totalYield() > other.totalYield();
    }

    virtual void show() const {
        cout << "\nCrop name: " << name
             << "\nArea: " << area << " acres"
             << "\nSeason: " << season
             << "\nYield per acre: " << yieldPerAcre << " kg"
             << "\nTotal yield: " << totalYield() << " kg\n";
    }

    virtual void save(ofstream& out) const = 0;
};

int Crop::count = 0;

// Cereal crop
class Cereal : public Crop {
    string fertilizer;
    int irrigation;

public:
    Cereal(string n, double a, string s, double y,
           string f, int i)
        : Crop(n, a, s, y) {
        fertilizer = f;
        irrigation = i;
    }

    void show() const override {
        cout << "\n--- CEREAL CROP ---";
        Crop::show();
        cout << "Fertilizer: " << fertilizer
             << "\nIrrigation: " << irrigation << " times/week\n";
    }

    void save(ofstream& out) const override {
        out << "CEREAL\n" << name << '\n' << area << '\n'
            << season << '\n' << yieldPerAcre << '\n'
            << fertilizer << '\n' << irrigation << '\n';
    }
};

// Vegetable crop
class Vegetable : public Crop {
    string fertilizer;
    int irrigation;

public:
    Vegetable(string n, double a, string s, double y,
              string f, int i)
        : Crop(n, a, s, y) {
        fertilizer = f;
        irrigation = i;
    }

    void show() const override {
        cout << "\n--- VEGETABLE CROP ---";
        Crop::show();
        cout << "Fertilizer: " << fertilizer
             << "\nIrrigation: " << irrigation << " times/week\n";
    }

    void save(ofstream& out) const override {
        out << "VEGETABLE\n" << name << '\n' << area << '\n'
            << season << '\n' << yieldPerAcre << '\n'
            << fertilizer << '\n' << irrigation << '\n';
    }
};

// Farming advice
void showAdvice(int type) {
    if (type == 1) {
        cout << "\nCereal farming advice:\n"
             << "1. Use good quality seeds.\n"
             << "2. Control weeds and diseases.\n"
             << "3. Avoid excess irrigation.\n";
    } else if (type == 2) {
        cout << "\nVegetable farming advice:\n"
             << "1. Water plants regularly.\n"
             << "2. Check plants for pests.\n"
             << "3. Harvest at the right time.\n";
    } else {
        cout << "Invalid advice option.\n";
    }
}

// Add a crop
void addCrop(vector<Crop*>& crops, int type) {
    string name, season, fertilizer;
    double area, yield;
    int irrigation;

    cin.ignore();
    cout << "Enter crop name: ";
    getline(cin, name);

    cout << "Enter area in acres: ";
    cin >> area;

    cin.ignore();
    cout << "Enter season: ";
    getline(cin, season);

    cout << "Enter yield per acre (kg): ";
    cin >> yield;

    cin.ignore();
    cout << "Enter fertilizer type: ";
    getline(cin, fertilizer);

    cout << "Enter irrigation times per week: ";
    cin >> irrigation;

    if (name.empty() || season.empty() || fertilizer.empty() ||
        area <= 0 || yield <= 0 || irrigation < 0) {
        cout << "Invalid input. Crop was not added.\n";
        return;
    }

    if (type == 1)
        crops.push_back(new Cereal(name, area, season, yield,
                                   fertilizer, irrigation));
    else
        crops.push_back(new Vegetable(name, area, season, yield,
                                      fertilizer, irrigation));

    cout << "Crop added successfully.\n";
}

// Display all crops
void displayCrops(const vector<Crop*>& crops) {
    if (crops.empty()) {
        cout << "No crop records available.\n";
        return;
    }

    for (int i = 0; i < (int)crops.size(); i++) {
        cout << "\nRecord " << i + 1;
        crops[i]->show();
    }
}

// Compare two crops
void compareCrops(const vector<Crop*>& crops) {
    if (crops.size() < 2) {
        cout << "Add at least two crops first.\n";
        return;
    }

    for (int i = 0; i < (int)crops.size(); i++)
        cout << i + 1 << ". " << crops[i]->getName() << '\n';

    int a, b;
    cout << "Enter first crop number: ";
    cin >> a;
    cout << "Enter second crop number: ";
    cin >> b;

    if (a < 1 || b < 1 || a > (int)crops.size() ||
        b > (int)crops.size() || a == b) {
        cout << "Invalid crop numbers.\n";
        return;
    }

    Crop* first = crops[a - 1];
    Crop* second = crops[b - 1];

    cout << fixed << setprecision(2);
    cout << first->getName() << " yield: "
         << first->totalYield() << " kg\n";
    cout << second->getName() << " yield: "
         << second->totalYield() << " kg\n";

    if (*first > *second)
        cout << first->getName() << " has higher yield.\n";
    else if (*second > *first)
        cout << second->getName() << " has higher yield.\n";
    else
        cout << "Both crops have equal yield.\n";
}

// Save crop information
void saveCrops(const vector<Crop*>& crops) {
    ofstream out("crops.txt");

    if (!out) {
        cout << "Unable to open file.\n";
        return;
    }

    for (const Crop* crop : crops)
        crop->save(out);

    cout << "Crop data saved successfully.\n";
}

// Load crop information
void loadCrops(vector<Crop*>& crops) {
    ifstream in("crops.txt");

    if (!in) {
        cout << "File not found. Save crop data first.\n";
        return;
    }

    string type, name, season, fertilizer;
    double area, yield;
    int irrigation, loaded = 0;

    while (getline(in, type)) {
        getline(in, name);
        in >> area;
        in.ignore();
        getline(in, season);
        in >> yield;
        in.ignore();
        getline(in, fertilizer);
        in >> irrigation;
        in.ignore();

        if (type == "CEREAL")
            crops.push_back(new Cereal(name, area, season, yield,
                                       fertilizer, irrigation));
        else if (type == "VEGETABLE")
            crops.push_back(new Vegetable(name, area, season, yield,
                                           fertilizer, irrigation));
        else
            continue;

        loaded++;
    }

    cout << loaded << " crop record(s) loaded.\n";
}

int main() {
    vector<Crop*> crops;
    int choice;

    do {
        cout << "\n===== SMART FARM MANAGEMENT SYSTEM =====\n"
             << "1. Add cereal crop\n"
             << "2. Add vegetable crop\n"
             << "3. Display all crops\n"
             << "4. Compare two crops\n"
             << "5. Get farming advice\n"
             << "6. Save crop data\n"
             << "7. Load crop data\n"
             << "8. Display total crop count\n"
             << "9. Exit\n"
             << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addCrop(crops, 1);
                break;

            case 2:
                addCrop(crops, 2);
                break;

            case 3:
                displayCrops(crops);
                break;

            case 4:
                compareCrops(crops);
                break;

            case 5: {
                int type;
                cout << "1. Cereal advice\n2. Vegetable advice\n";
                cout << "Choose option: ";
                cin >> type;
                showAdvice(type);
                break;
            }

            case 6:
                saveCrops(crops);
                break;

            case 7:
                loadCrops(crops);
                break;

            case 8:
                cout << "Total crop objects: " << Crop::getCount() << '\n';
                break;

            case 9:
                cout << "Thank you for using Smart Farm Management System!\n";
                break;

            default:
                cout << "Invalid choice. Select 1 to 9.\n";
        }

    } while (choice != 9);

    for (Crop* crop : crops)
        delete crop;

    return 0;
}
