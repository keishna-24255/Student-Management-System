#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Structure to hold Student Information
struct Student {
    int id;
    char name[50];
    int age;
    char course[50];
};

// Function Declarations
void addStudent();
void displayStudents();
void updateStudent();
void deleteStudent();

int main() {
    int choice;

    while (true) {
        cout << "\n====================================";
        cout << "\n     STUDENT MANAGEMENT SYSTEM      ";
        cout << "\n====================================";
        cout << "\n1. Add Student Record";
        cout << "\n2. Display All Students";
        cout << "\n3. Update Student Record";
        cout << "\n4. Delete Student Record";
        cout << "\n5. Exit";
        cout << "\n------------------------------------";
        cout << "\nEnter your choice (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: 
                cout << "\nExiting system. Goodbye!\n"; 
                return 0;
            default: 
                cout << "\nInvalid choice! Please try again.\n";
        }
    }
    return 0;
}

// 1. Function to Add a Student Record to File
void addStudent() {
    Student s;
    ofstream outFile("students.dat", ios::binary | ios::app); 

    if (!outFile) {
        cout << "\nError opening file!";
        return;
    }

    cout << "\nEnter Student ID: ";
    cin >> s.id;
    cin.ignore(); // Clear input buffer
    cout << "Enter Name: ";
    cin.getline(s.name, 50);
    cout << "Enter Age: ";
    cin >> s.age;
    cin.ignore();
    cout << "Enter Course: ";
    cin.getline(s.course, 50);

    outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
    outFile.close();

    cout << "\nRecord added successfully!\n";
}

// 2. Function to Display All Student Records
void displayStudents() {
    Student s;
    ifstream inFile("students.dat", ios::binary);

    if (!inFile) {
        cout << "\nNo records found. Please add a student first!\n";
        return;
    }

    cout << "\n=========================================================\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Age" << setw(15) << "Course" << endl;
    cout << "=========================================================\n";

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        cout << left << setw(10) << s.id 
             << setw(20) << s.name 
             << setw(10) << s.age 
             << setw(15) << s.course << endl;
    }
    cout << "---------------------------------------------------------\n";
    inFile.close();
}

// 3. Function to Update a Record
void updateStudent() {
    int targetId;
    Student s;
    bool found = false;

    fstream file("students.dat", ios::binary | ios::in | ios::out);

    if (!file) {
        cout << "\nError opening file!\n";
        return;
    }

    cout << "\nEnter Student ID to update: ";
    cin >> targetId;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.id == targetId) {
            found = true;
            cout << "\nCurrent Record Details Found. Enter New Details:\n";
            cin.ignore();
            cout << "Enter New Name: ";
            cin.getline(s.name, 50);
            cout << "Enter New Age: ";
            cin >> s.age;
            cin.ignore();
            cout << "Enter New Course: ";
            cin.getline(s.course, 50);

            // Move the file put pointer back to rewrite this specific record
            int pos = -1 * static_cast<int>(sizeof(Student));
            file.seekp(pos, ios::cur);
            
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));
            cout << "\nRecord updated successfully!\n";
            break;
        }
    }

    if (!found) cout << "\nStudent ID not found.\n";
    file.close();
}

// 4. Function to Delete a Record (Using a Temporary Swap File)
void deleteStudent() {
    int targetId;
    Student s;
    bool found = false;

    ifstream inFile("students.dat", ios::binary);
    if (!inFile) {
        cout << "\nError opening file!\n";
        return;
    }

    ofstream tempFile("temp.dat", ios::binary); 

    cout << "\nEnter Student ID to delete: ";
    cin >> targetId;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.id == targetId) {
            found = true; // Skips writing this item to the temporary backup file
        } else {
            tempFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
        }
    }

    inFile.close();
    tempFile.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        cout << "\nRecord deleted successfully!\n";
    } else {
        cout << "\nStudent ID not found.\n";
    }
}