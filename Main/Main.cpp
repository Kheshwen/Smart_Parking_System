#include <iostream>  
#include <iomanip>  
#include <unordered_map> 
#include <string>  
using namespace std;

const int ROWS = 5, COLS = 5;

struct User {
    string id;
    bool isParked;
    int slotRow, slotCol;
};

struct ParkingSlot {
    bool isOccupied = false;
    string username;
};

ParkingSlot parkingLot[ROWS][COLS];  // 5x5 parking lot grid
unordered_map<string, User> userMap;  // Maps user ID to User struct


void displayStylizedGrid() {
    cout << "\n+=============================================+\n";
    cout << "|           SMART PARKING SYSTEM              |\n";
    cout << "+=============================================+\n\n";

    for (int row = 0; row < ROWS; ++row) {
        cout << "      ";
        for (int col = 0; col < COLS; ++col) cout << "+------";
        cout << "+\n       | ";

        for (int col = 0; col < COLS; ++col)
            cout << "P" << setw(2) << row * COLS + col + 1 << "  | ";
        cout << "\n       | ";

		for (int col = 0; col < COLS; ++col)  // get the status of each slot
            cout << "  " << (parkingLot[row][col].isOccupied ? "X" : "O") << "  | ";  
        cout << "\n";
    }

    cout << "       ";
    for (int col = 0; col < COLS; ++col) cout << "+------";
    cout << "+\n\nLegend: O = Available | X = Occupied\n";
    cout << "===============================================\n";

    cout << "\n--- Parking History ---\n";
    for (const auto& entry : userMap) {
        const User& user = entry.second;
        if (user.isParked) {
            cout << "User ID: " << user.id << ", Slot: P" << user.slotRow * COLS + user.slotCol + 1 << "\n";
        }
        else {
        }
    }
    cout << "------------------------\n";
}

bool isParkingAvailable() {
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
			if (!parkingLot[i][j].isOccupied) return true;  // check if any slot is available
    return false;
}

pair <int, int> getSlotCoordinates(int choice) {  // convert slot number to row and column
	return make_pair((choice - 1) / COLS, (choice - 1) % COLS);  // (0-based index)
}

void enterParking(const string & id) { 
	if (userMap.count(id) && userMap[id].isParked) {  // check if user is already parked
        cout << "Error: You are already parked." << endl;
        return;
    }

    if (!isParkingAvailable()) {
        cout << "Sorry, parking is full." << endl;
        return;
    }

    cout << "\nAvailable slots: ";
    for (int i = 0; i < ROWS * COLS; ++i) {
		pair<int, int> coordinates = getSlotCoordinates(i + 1);  // convert slot number to coordinates
        int r = coordinates.first;
        int c = coordinates.second;
		if (!parkingLot[r][c].isOccupied)  // check if the slot is available
            cout << "P" << i + 1 << " ";
    }

    int choice;
	cout << "\nPick a slot number (1-" << ROWS * COLS << "): "; // display available slots
    cin >> choice;

    if (choice < 1 || choice > ROWS * COLS) {
        cout << "Invalid slot number." << endl;  
        return;
    }

	pair<int, int> coordinates = getSlotCoordinates(choice);  // convert slot number to coordinates
    int row = coordinates.first;
    int col = coordinates.second;
    if (parkingLot[row][col].isOccupied) {
        cout << "Error: Slot P" << choice << " is already taken." << endl;
        return;
    }

    parkingLot[row][col].isOccupied = true;
    parkingLot[row][col].username = id;
    userMap[id] = User{ id, true, row, col };  // user

    cout << "Welcome, " << id << "! Your slot is: P" << choice << "\n";
}

void exitParking(const string& id) {
    if (!userMap.count(id) || !userMap[id].isParked) {  
        cout << "Error: You are not currently parked.\n";
        return;
    }

	int r = userMap[id].slotRow, c = userMap[id].slotCol;  // get user's parking slot coordinates
	parkingLot[r][c].isOccupied = false;  // mark the slot as available
	parkingLot[r][c].username.clear();  // clear the username
	userMap[id].isParked = false;  // update user's parked status

    cout << "Goodbye, " << id << "! You exited slot P" << r * COLS + c + 1 << ".\n";
}

void clearScreen() {
system("cls"); 
}


int main() {
    int choice;
    string id;

    do {
        displayStylizedGrid();
        cout << "\n--- Smart Parking System ---\n";
        cout << "1 Enter Parking\n";
        cout << "2 Exit Parking\n";
        cout << "3 Exit Program\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');  
            cout << "Invalid input. Enter a number.\n";
            continue;
        }
        switch (choice) {
        case 1:
			cout << "\n";
            cout << "Enter ID: ";
            cin >> id;
            enterParking(id);
            break;
        case 2:
            cout << "\n";
            cout << "Enter ID: ";
            cin >> id;
            exitParking(id);

            break;
        case 3:
			cout << "\n";
            cout << "Exiting system.";
            return 0;
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }
        clearScreen();
    } while (choice != 3 );

}