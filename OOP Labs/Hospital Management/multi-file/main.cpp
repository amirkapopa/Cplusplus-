#include <iostream>
#include <vector>
#include "Hospital.h"
#include "Doctor.h"
#include "Nurse.h"
#include "Technician.h"

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

    for (const Hospital* member : members)
        member->display(), cout << "\n";

    // ── Free dynamically allocated memory ─────
    for (Hospital* member : members)
        delete member;
    members.clear();

    cout << "All records displayed and memory freed successfully.\n";
    return 0;
}
