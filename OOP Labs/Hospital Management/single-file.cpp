#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

// ─────────────────────────────────────────────
//  Abstract Base Class: Hospital
// ─────────────────────────────────────────────
class Hospital {
protected:
    string name;
    int    age;
    int    id;

public:
    Hospital(const string& name, int age, int id)
        : name(name), age(age), id(id) {}

    virtual void   display()         const = 0;
    virtual double calculateSalary() const = 0;

    virtual ~Hospital() {}
};

// ─────────────────────────────────────────────
//  Derived Class: Doctor
// ─────────────────────────────────────────────
class Doctor : public Hospital {
private:
    string         specialization;
    double         monthlySalary;
    vector<string> patients;

public:
    Doctor(const string& name, int age, int id,
           const string& specialization, double monthlySalary)
        : Hospital(name, age, id),
          specialization(specialization)
    {
        if (monthlySalary <= 0)
            throw invalid_argument("Doctor salary must be positive.");
        this->monthlySalary = monthlySalary;
    }

    void addPatient(const string& patientName) {
        patients.push_back(patientName);
    }

    double calculateSalary() const override {
        return monthlySalary;
    }

    void display() const override {
        cout << "========================================\n";
        cout << "  [DOCTOR]\n";
        cout << "  Name           : " << name           << "\n";
        cout << "  Age            : " << age            << "\n";
        cout << "  ID             : " << id             << "\n";
        cout << "  Specialization : " << specialization << "\n";
        cout << "  Monthly Salary : $" << calculateSalary() << "\n";
        cout << "  Patients       : ";
        if (patients.empty()) {
            cout << "None";
        } else {
            for (size_t i = 0; i < patients.size(); ++i) {
                cout << patients[i];
                if (i + 1 < patients.size()) cout << ", ";
            }
        }
        cout << "\n========================================\n";
    }
};

// ─────────────────────────────────────────────
//  Derived Class: Nurse
// ─────────────────────────────────────────────
class Nurse : public Hospital {
private:
    string         ward;
    double         monthlySalary;
    vector<string> duties;

public:
    Nurse(const string& name, int age, int id,
          const string& ward, double monthlySalary)
        : Hospital(name, age, id), ward(ward)
    {
        if (monthlySalary <= 0)
            throw invalid_argument("Nurse salary must be positive.");
        this->monthlySalary = monthlySalary;
    }

    void addDuty(const string& duty) {
        duties.push_back(duty);
    }

    double calculateSalary() const override {
        return monthlySalary;
    }

    void display() const override {
        cout << "========================================\n";
        cout << "  [NURSE]\n";
        cout << "  Name           : " << name           << "\n";
        cout << "  Age            : " << age            << "\n";
        cout << "  ID             : " << id             << "\n";
        cout << "  Ward           : " << ward           << "\n";
        cout << "  Monthly Salary : $" << calculateSalary() << "\n";
        cout << "  Duties         : ";
        if (duties.empty()) {
            cout << "None";
        } else {
            for (size_t i = 0; i < duties.size(); ++i) {
                cout << duties[i];
                if (i + 1 < duties.size()) cout << ", ";
            }
        }
        cout << "\n========================================\n";
    }
};

// ─────────────────────────────────────────────
//  Derived Class: Technician
// ─────────────────────────────────────────────
class Technician : public Hospital {
private:
    string         labType;
    double         hourlyRate;
    int            workingHours;
    vector<string> machinesHandled;

public:
    Technician(const string& name, int age, int id,
               const string& labType, double hourlyRate, int workingHours)
        : Hospital(name, age, id), labType(labType)
    {
        if (hourlyRate <= 0)
            throw invalid_argument("Technician hourly rate must be positive.");
        if (workingHours <= 0)
            throw invalid_argument("Technician working hours must be positive.");
        this->hourlyRate    = hourlyRate;
        this->workingHours  = workingHours;
    }

    void addMachine(const string& machineName) {
        machinesHandled.push_back(machineName);
    }

    double calculateSalary() const override {
        return hourlyRate * workingHours;
    }

    void display() const override {
        cout << "========================================\n";
        cout << "  [TECHNICIAN]\n";
        cout << "  Name           : " << name          << "\n";
        cout << "  Age            : " << age           << "\n";
        cout << "  ID             : " << id            << "\n";
        cout << "  Lab Type       : " << labType       << "\n";
        cout << "  Hourly Rate    : $" << hourlyRate   << "\n";
        cout << "  Working Hours  : "  << workingHours << " hrs\n";
        cout << "  Total Salary   : $" << calculateSalary() << "\n";
        cout << "  Machines       : ";
        if (machinesHandled.empty()) {
            cout << "None";
        } else {
            for (size_t i = 0; i < machinesHandled.size(); ++i) {
                cout << machinesHandled[i];
                if (i + 1 < machinesHandled.size()) cout << ", ";
            }
        }
        cout << "\n========================================\n";
    }
};

// ─────────────────────────────────────────────
//  main()
// ─────────────────────────────────────────────
int main() {
    vector<Hospital*> members;

    // ── Create Doctor ──────────────────────────
    Doctor* doc = new Doctor("Dr. Sarah Ahmed", 45, 1001,
                             "Cardiology", 12000.00);
    doc->addPatient("Ali Hassan");
    doc->addPatient("Maria Garcia");
    doc->addPatient("John Smith");
    members.push_back(doc);

    // ── Create Nurse ───────────────────────────
    Nurse* nurse = new Nurse("Emily Clark", 30, 2001,
                             "ICU", 4500.00);
    nurse->addDuty("Administer medications");
    nurse->addDuty("Monitor vital signs");
    nurse->addDuty("Assist in surgeries");
    members.push_back(nurse);

    // ── Create Technician ─────────────────────
    Technician* tech = new Technician("James Wilson", 38, 3001,
                                      "Radiology", 35.00, 160);
    tech->addMachine("MRI Scanner");
    tech->addMachine("CT Scanner");
    tech->addMachine("X-Ray Machine");
    members.push_back(tech);

    // ── Display all members ───────────────────
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║      HOSPITAL MANAGEMENT SYSTEM          ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    for (const Hospital* member : members) {
        member->display();
        cout << "\n";
    }

    // ── Free dynamically allocated memory ─────
    for (Hospital* member : members) {
        delete member;
    }
    members.clear();

    cout << "All records displayed and memory freed successfully.\n";
    return 0;
}
