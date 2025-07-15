#include <iostream>
#include <string>
#include <vector> // Dynamic user management with vector
using namespace std;

const int ROWS = 4; // 4 rows of parking
const int COLS = 4; // 4 columns of parking

// total parking slot is 16

struct User {       //Function to store user's details 
    string id;
	bool isParked = false; //Track if user is parked
    int slotRow = -1; //Row checking
	int slotCol = -1; //Column checking
};

struct ParkingSlot { //Function to store parking slot details
    bool isOccupied = false;
    string username = "";
};

ParkingSlot parkingLot[ROWS][COLS]; // 2D array for parking slots
vector<User> users; 




void displayStylizedGrid() {    //Func to display parking grid
    cout << "\n+=============================================+\n";
    cout << "|           SMART PARKING SYSTEM              |\n";
    cout << "+=============================================+\n\n";

    int spot = 1;
    for (int row = 0; row < ROWS; ++row) {
        cout << "       +------+------+------+------+------+\n";
        cout << "       | ";
        for (int col = 0; col < COLS; ++col) {
            cout << "P" << spot++ << "  | ";
        }
        cout << "\n       | ";
        spot -= COLS;
        for (int col = 0; col < COLS; ++col) {
            int i = row;
            int j = col;
            cout << (parkingLot[i][j].isOccupied ? "X" : "O") << "   | ";
            ++spot;
        }
        cout << "\n";
    }
    cout << "       +------+------+------+------+------+\n";
    cout << "\nLegend: O = Available | X = Occupied\n";
    cout << "===============================================\n";
}

bool checkAvailability() { //Function to check if parking slots are available
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            if (!parkingLot[i][j].isOccupied)
                return true;
    return false;
}

int findUserIndex(const string& id) { //Function to find user index in vector
    for (int i = 0; i < users.size(); ++i)
        if (users[i].id == id)
            return i;
    return -1;
}

void enterParking(const string& id) { //Function to enter parking
    int index = findUserIndex(id);
    if (index != -1 && users[index].isParked) {
        cout << "Error: You are already parked.\n";
        return;
    }

	if (!checkAvailability()) { // Check if parking is available
        cout << "Sorry, parking is full.\n";
        return;
    }

	for (int i = 0; i < ROWS; ++i) { // Loop through parking slots
        for (int j = 0; j < COLS; ++j) {
            if (!parkingLot[i][j].isOccupied) {
                parkingLot[i][j].isOccupied = true;
                parkingLot[i][j].username = id;

                User newUser = { id, true, i, j };
                if (index == -1) {
                    users.push_back(newUser); // Add new user to vector
                }
                else {
                    users[index] = newUser;
                }

                cout << "Welcome, " << id << "! Your slot is: (" << i << ", " << j << ")\n";
                return;
            }
        }
    }
}

void exitParking(const string& id) { //Function to exit parking
    int index = findUserIndex(id);
    if (index == -1 || !users[index].isParked) {
        cout << "Error: You are not currently parked.\n";
        return;
    }

    int i = users[index].slotRow;
    int j = users[index].slotCol;

    parkingLot[i][j].isOccupied = false;
    parkingLot[i][j].username = "";
    users[index].isParked = false;

    cout << "Goodbye, " << id << "! You have exited from slot (" << i << ", " << j << ").\n";
}

int main() { // Main function to run the parking system
    int choice;
    string id;

    do {
        cout << "\n--- Smart Parking System ---\n";
        cout << "1 Enter Parking\n";
        cout << "2 Exit Parking\n";
        cout << "3 View Parking Grid\n";
        cout << "4 Exit Program\n";
        cout << "Enter no.of choice from above: ";

        if (!(cin >> choice)) { // Input validation for non-integer
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter ID: ";
            cin >> id;
            enterParking(id);
            break;
        case 2:
            cout << "Enter ID: ";
            cin >> id;
            exitParking(id);
            break;
        case 3:
            displayStylizedGrid(); // 
            break;
        case 4:
            cout << "Exiting system.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
