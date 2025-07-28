#include <iostream>  
#include <sstream>
#include <iomanip>  
#include <unordered_map> 
#include <string>  
#include <ctime>
using namespace std;

const int ROWS = 5, COLS = 5;

struct User {
    string id;
    bool isParked;
    int slotRow, slotCol;
	time_t entryTime;  
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
        cout << "+\n      | ";

        for (int col = 0; col < COLS; ++col)
            cout << "P" << setw(2) << row * COLS + col + 1 << "  | ";
        cout << "\n      | ";

		for (int col = 0; col < COLS; ++col)  // get the status of each slot
            cout << "  " << (parkingLot[row][col].isOccupied ? "X" : "O") << "  | ";  
        cout << "\n";
    }

    cout << "      ";
    for (int col = 0; col < COLS; ++col) cout << "+------";
    cout << "+\n\nLegend: O = Available | X = Occupied\n";
    cout << "===============================================\n";
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
	userMap[id] = User{ id, true, row, col, time(0) };  

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

void viewParkedUsers() {

    cout << "\n+=======================================================================+\n";
    cout << "|                     CURRENTLY PARKED USERS                            |\n";
    cout << "+=======================================================================+\n";
    cout << left << setw(15) << "User ID" << setw(10) << "Slot" << setw(30) << "Parked Time" << "Duration\n";
    cout << "+-----------------------------------------------------------------------+\n";

    bool any = false;
    for (const auto& entry : userMap) {
        const User& user = entry.second;
        if (user.isParked) {
            int slotNum = user.slotRow * COLS + user.slotCol + 1;

            // Format parked time
            char timeStr[26];
            ctime_s(timeStr, sizeof(timeStr), &user.entryTime);
            timeStr[strlen(timeStr) - 1] = '\0';  // remove newline

            // Calculate duration
            time_t now = time(0);
            int durationSeconds = static_cast<int>(difftime(now, user.entryTime));
            int hours = durationSeconds / 3600;
            int minutes = (durationSeconds % 3600) / 60;

            // Format duration nicely
            ostringstream durationStr;
            durationStr << hours << "h " << minutes << "m";

            // Print formatted row
            cout << left << setw(15) << user.id << setw(10) << ("P" + to_string(slotNum)) << setw(30) << timeStr << durationStr.str() << "\n";

            any = true;
        }
    }

    if (!any)
        cout << "No users are currently parked.\n";

    cout << "+=======================================================================+\n";
}

void searchUserSlot(const string& id) {
    if (!userMap.count(id)) {
        cout << "User ID not found in records.\n";
        return;
    }

    const User& user = userMap[id];
    if (user.isParked) {
        cout << "User " << id << " is parked at slot P" << user.slotRow * COLS + user.slotCol + 1 << ".\n";
    }
    else {
        cout << "User " << id << " is registered but not currently parked.\n";
    }
}

void displaySummary() {
    int occupied = 0, available = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (parkingLot[i][j].isOccupied) occupied++;
            else available++;
        }
    }
    cout << "\n--- Parking Slot Summary ---\n";
    cout << "Total Slots: " << ROWS * COLS << "\n";
    cout << "Occupied   : " << occupied << "\n";
    cout << "Available  : " << available << "\n";
    cout << "------------------------------\n";
}

void waitUserInput() {
	cout << "\nPress Enter to continue...";
	cin.ignore();
	cin.get();  
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
		cout << "3 View Parked Users\n";
        cout << "4 Search User Parking Info\n";
        cout << "5 View Slot Summary\n";
        cout << "99 Exit Program\n";
        cout << "\nEnter choice: ";

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
            clearScreen();
			displayStylizedGrid();
			viewParkedUsers();
			waitUserInput();
            break;

        case 4:
            clearScreen();
            displayStylizedGrid();
            cout << "\nEnter ID to search: ";
            cin >> id;
            searchUserSlot(id);
            waitUserInput();
            break;

        case 5:
            clearScreen();
			displayStylizedGrid();
			displaySummary();
			waitUserInput();
            break;

		case 99:
            clearScreen();
            cout << "\n";
            cout << "Exiting program.....\n";
            cout << "\n";
            return 0;

        default:
            cout << "Invalid choice. Try again.\n";
        }
        clearScreen();
    } while (choice != 99 );

}