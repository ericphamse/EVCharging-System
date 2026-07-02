#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <queue>
#include <map>
#include <iomanip>

using namespace std;

#include "Location.h"
#include "WeightedGraph.h"
#include "EVCharging.h"

int main() {
	EVCharging charging;
	charging.printLocations();
	charging.printAdjacencyMatrix();
	charging.printLocationsSortedByPrice();
	charging.BFT();

	/*string input;

    cout << "\nTask 4: Enter a location name to find adjacent locations with charging stations: ";
    getline(cin, input);
    charging.showAdjacentChargers(input);

	cout << "\nTask 5: Enter a location name to find the nearest location with a charging station: ";
    getline(cin, input);
    charging.findNearestChargingStation(input);

	cout << "\nTask 6: Enter your current location name to find the lowest total cost (travel + charging): ";
    getline(cin, input);
    charging.findLowestTotalCostLocation(input);

	string origin, destination;
	cout << "\nTask 7: Find the cheapest charging station between origin and destination.\n";
	cout << "Input a location: ";
	getline(cin, origin);
	cout << "Input a location: ";
	getline(cin, destination);
	charging.findCheapestStationOnPath(origin, destination);

	charging.generateChargingDemandFor100EVs();

	charging.recommendBestLocationForNewStation();*/

    while (true) {
        cout << "\nChoose a task to run:\n";
        cout << "4. Task 4 : List adjacent charging stations\n";
        cout << "5. Task 5 : Find the nearest charging station (excluding the current location)\n";
        cout << "6. Task 6 : Find the cheapest charging station other than the current location\n";
        cout << "7. Task 7 : Find the cheapest charging station between origin and destination\n";
        cout << "8. Task 8 : Generate charging demands and calculate their charging locations\n";
        cout << "9. Task 9 : Find the best location to build a new charging station\n";
        cout << "0. Quit\n";
        cout << "Your choice: ";

        string input;
        getline(cin, input);

        if (input == "0") {
            break;
        } else if (input == "4") {
            string location;
            cout << "\nTask 4: Enter a location name to find adjacent locations with charging stations: ";
            getline(cin, location);
            charging.showAdjacentChargers(location);
        } else if (input == "5") {
            string location;
            cout << "\nTask 5: Enter a location name to find the nearest location with a charging station: ";
            getline(cin, location);
            charging.findNearestChargingStation(location);
        } else if (input == "6") {
            string location;
            cout << "\nTask 6: Enter your current location name to find the lowest total cost (travel + charging): ";
            getline(cin, location);
            charging.findLowestTotalCostLocation(location);
        } else if (input == "7") {
            string origin, destination;
            cout << "\nTask 7: Find the cheapest charging station between origin and destination.\n";
            cout << "Input origin location: ";
            getline(cin, origin);
            cout << "Input destination location: ";
            getline(cin, destination);
            charging.findCheapestStationOnPath(origin, destination);
        } else if (input == "8") {
            cout << "\nTask 8: Generate charging demands and calculate their charging locations:\n";
            charging.generateChargingDemandFor100EVs();
        } else if (input == "9") {
            cout << "\nTask 9: Find the best location to build a new charging station:\n";
            charging.recommendBestLocationForNewStation();
        } else {
            cout << "Invalid input. Please choose a valid task number.\n";
        }
	}
	
	return 0;
}
