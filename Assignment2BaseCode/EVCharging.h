/*
 * EVCharging.h
 *
 *  Created on: 24 Apr 2025
 *  Author: Dongmo
 */
#include <algorithm>
#include <vector>  

#ifndef EVCHARGING_H_
#define EVCHARGING_H_

class EVCharging {// you may also declare this class as an extension of WeightedGraph
private:
	map<int, Location> locations;//It can also be unordered_map: store location's index -> int and location's information
	int numberOfLocations;
	WeightedGraphType* weightedGraph;
public:
	EVCharging();
	~EVCharging();
	void inputLocations();
	void printLocations();
	void printAdjacencyMatrix();
	void printLocationsSortedByPrice();
	void BFT();
	void showAdjacentChargers(const string& locationName);
	void findNearestChargingStation(const string& locationName);
	void findLowestTotalCostLocation(const string& locationName);
	void findCheapestStationOnPath(const string& originName, const string& destinationName);
	void generateChargingDemandFor100EVs();
	void recommendBestLocationForNewStation();

};

EVCharging::EVCharging() {
	srand(time(0));
	inputLocations();
	weightedGraph = new WeightedGraphType(numberOfLocations);
}

EVCharging::~EVCharging() {
	delete weightedGraph;
}

void EVCharging::inputLocations() {
	ifstream infile;
	char fileName[50] = "Locations.txt";

//	cout << "Enter charging location file name: ";
//	cin >> fileName;
//	cout << endl;

	infile.open(fileName);

	if (!infile) {
		cout << "Cannot open input file." << endl;
		return;
	}

	int locationIndex = 0;

	while (!infile.eof()) {
		Location s;
		string charger;
		string price;
		while (!infile.eof()) {
			getline(infile, s.locationName, ',');
			getline(infile, charger, ',');
			getline(infile, price);
			s.chargerInstalled = (stoi(charger) == 1) ? true : false;
			s.chargingPrice = stod(price);
			s.index = locationIndex;
			locations[locationIndex] = s;
			locationIndex++;
		}
	}

	numberOfLocations = locationIndex;
}

void EVCharging::printLocations() {
	cout << "List of locations and charging information " << endl;
	cout << setw (8) << "Index" << setw (20) << "Location name" << setw (20) <<"Charging station" << setw(20) << "Charging price" << endl;

	map<int, Location>::iterator it = locations.begin();

	for (; it != locations.end(); it++) {
		it->second.printLocation();
	}

    cout << endl;
}

void EVCharging::printAdjacencyMatrix() {
	cout << "Adjacency matrix (0 means no direct connection, non-zero value represents the distance of adjacent locations)\n" << endl;
	cout << setw(13) << " ";
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(13) << locations[i].locationName;
	}
	cout << fixed << endl;
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(13) << locations[i].locationName;
		for (int j = 0; j < numberOfLocations; j++) {
			cout << setw(13) << fixed << (weightedGraph->getWeight(i,j) == DBL_MAX ? 0.0 : weightedGraph->getWeight(i,j));
		}
		cout << endl;
	}
}

void EVCharging::printLocationsSortedByPrice() {
    vector<Location> sortedStations;

    for (auto it = locations.begin(); it != locations.end(); ++it) {
        if (it->second.chargerInstalled) {
            sortedStations.push_back(it->second);
        }
    }

    sort(sortedStations.begin(), sortedStations.end());

    cout << "\nLocations with charging stations sorted by charging price (ascending):" << endl;
    cout << setw(8) << "Index" << setw(20) << "Location name" << setw(20) << "Charging station" << setw(20) << "Charging price" << endl;

    for (const auto& loc : sortedStations) {
        loc.printLocation();
    }

    cout << endl;
}

void EVCharging::BFT() {
    cout << "The list of locations with breadth-first traversal:" << endl;

    vector<int> bftResult = weightedGraph->breadthFirstTraversal();

    for (int i = 0; i < bftResult.size(); i++) {
        cout << locations[bftResult[i]].locationName << endl;
    }
}

void EVCharging::showAdjacentChargers(const string& locationName) {
    int index = -1;
    for (auto& entry : locations) {
        if (entry.second.locationName == locationName) {
            index = entry.first;
            break;
        }
    }

    if (index == -1) {
        cout << "Location not found." << endl;
        return;
    }

    list<int> adjList = weightedGraph->getAdjancencyList(index);

    bool hasCharger = false;
    for (int adjIndex : adjList) {
        if (locations[adjIndex].chargerInstalled) {
            if (!hasCharger) {
                cout << "Adjacent locations with charging stations:" << endl;
                hasCharger = true;
            }
            cout << "- " << locations[adjIndex].locationName << endl;
        }
    }

    if (!hasCharger) {
        cout << "NONE" << endl;
    }
}

void EVCharging::findNearestChargingStation(const string& locationName) {
    int srcIndex = -1;
    for (auto& entry : locations) {
        if (entry.second.locationName == locationName) {
            srcIndex = entry.first;
            break;
        }
    }

    if (srcIndex == -1) {
        cout << "Location not found." << endl;
        return;
    }

    vector<double> distances(numberOfLocations, DBL_MAX);
    vector<bool> visited(numberOfLocations, false);
    distances[srcIndex] = 0;

    for (int count = 0; count < numberOfLocations - 1; count++) {
        double minDist = DBL_MAX;
        int u = -1;
        for (int i = 0; i < numberOfLocations; i++) {
            if (!visited[i] && distances[i] < minDist) {
                minDist = distances[i];
                u = i;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        list<int> neighbors = weightedGraph->getAdjancencyList(u);
        for (int v : neighbors) {
            double weight = weightedGraph->getWeight(u, v);
            if (!visited[v] && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }

    int nearestIndex = -1;
    double minDistance = DBL_MAX;

    for (int i = 0; i < numberOfLocations; i++) {
        if (i != srcIndex && locations[i].chargerInstalled && distances[i] < minDistance) {
            minDistance = distances[i];
            nearestIndex = i;
        }
    }

    if (nearestIndex != -1) {
        cout << "Nearest location with a charging station is: " << locations[nearestIndex].locationName << endl;
    } else {
        cout << "No other location with a charging station found." << endl;
    }
}

void EVCharging::findLowestTotalCostLocation(const string& locationName) {
	int srcIndex = -1;
	for (auto& entry : locations) {
		if (entry.second.locationName == locationName) {
			srcIndex = entry.first;
			break;
		}
	}

	if (srcIndex == -1) {
		cout << "Location not found." << endl;
		return;
	}

	vector<double> distances(numberOfLocations, DBL_MAX);
	vector<bool> visited(numberOfLocations, false);
	distances[srcIndex] = 0;

	for (int count = 0; count < numberOfLocations - 1; count++) {
		double minDist = DBL_MAX;
		int u = -1;
		for (int i = 0; i < numberOfLocations; i++) {
			if (!visited[i] && distances[i] < minDist) {
				minDist = distances[i];
				u = i;
			}
		}
		if (u == -1) break;
		visited[u] = true;

		list<int> neighbors = weightedGraph->getAdjancencyList(u);
		for (int v : neighbors) {
			double weight = weightedGraph->getWeight(u, v);
			if (!visited[v] && distances[u] + weight < distances[v]) {
				distances[v] = distances[u] + weight;
			}
		}
	}

	double kWh = rand() % 51 + 10;
	double travelRate = 0.28;

	double minTotalCost = DBL_MAX;
	int bestLocation = -1;

	for (int i = 0; i < numberOfLocations; i++) {
		if (i != srcIndex && locations[i].chargerInstalled && distances[i] != DBL_MAX) {
			double travelCost = distances[i] * 2 * travelRate;
			double chargingCost = kWh * locations[i].chargingPrice;
			double totalCost = travelCost + chargingCost;

			if (totalCost < minTotalCost) {
				minTotalCost = totalCost;
				bestLocation = i;
			}
		}
	}

	if (bestLocation != -1) {
		cout << "Best location to charge: " << locations[bestLocation].locationName << endl;
		cout << "Distance (one-way): " << distances[bestLocation] << " km" << endl;
		cout << "Charging price: $" << locations[bestLocation].chargingPrice << "/kWh" << endl;
		cout << "Energy needed: " << kWh << " kWh" << endl;
		cout << "Total cost (travel + charging): $" << fixed << setprecision(2) << minTotalCost << endl;
	} else {
		cout << "No charging station found." << endl;
	}
}

void EVCharging::findCheapestStationOnPath(const string& originName, const string& destinationName) {
    int origin = -1, destination = -1;
    for (auto& entry : locations) {
        if (entry.second.locationName == originName) origin = entry.first;
        if (entry.second.locationName == destinationName) destination = entry.first;
    }

    if (origin == -1 || destination == -1) {
        cout << "Invalid origin or destination.\n";
        return;
    }

    srand(time(0));
    double kWh = rand() % 51 + 10;
    double travelRate = 0.28;

    vector<double> distFromOrigin(numberOfLocations, DBL_MAX);
    vector<bool> visited(numberOfLocations, false);
	vector<int> prevFromOrigin(numberOfLocations, -1);
    distFromOrigin[origin] = 0;

    for (int i = 0; i < numberOfLocations - 1; i++) {
        double minDist = DBL_MAX;
        int u = -1;
        for (int j = 0; j < numberOfLocations; j++) {
            if (!visited[j] && distFromOrigin[j] < minDist) {
                minDist = distFromOrigin[j];
                u = j;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        for (int v : weightedGraph->getAdjancencyList(u)) {
            double weight = weightedGraph->getWeight(u, v);
            if (!visited[v] && distFromOrigin[u] + weight < distFromOrigin[v]) {
                distFromOrigin[v] = distFromOrigin[u] + weight;
				prevFromOrigin[v] = u;
            }
        }
    }

    double minTotalCost = DBL_MAX;
    int bestStation = -1;
	vector<int> bestPathFromOrigin;
    vector<int> bestPathToDestination;

    for (int i = 0; i < numberOfLocations; i++) {
        if (i == origin || !locations[i].chargerInstalled || distFromOrigin[i] == DBL_MAX) continue;

        vector<double> distToDest(numberOfLocations, DBL_MAX);
        vector<bool> visitedToDest(numberOfLocations, false);
		vector<int> prevToDest(numberOfLocations, -1);
        distToDest[i] = 0;

        for (int j = 0; j < numberOfLocations - 1; j++) {
            double minDist = DBL_MAX;
            int u = -1;
            for (int k = 0; k < numberOfLocations; k++) {
                if (!visitedToDest[k] && distToDest[k] < minDist) {
                    minDist = distToDest[k];
                    u = k;
                }
            }

            if (u == -1) break;
            visitedToDest[u] = true;

            for (int v : weightedGraph->getAdjancencyList(u)) {
                double weight = weightedGraph->getWeight(u, v);
                if (!visitedToDest[v] && distToDest[u] + weight < distToDest[v]) {
                    distToDest[v] = distToDest[u] + weight;
					prevToDest[v] = u;
                }
            }
        }

        if (distToDest[destination] == DBL_MAX) continue;

        double travelToStation = distFromOrigin[i] * travelRate;
        double travelToDest = distToDest[destination] * travelRate;
        double chargingCost = kWh * locations[i].chargingPrice;
        double totalCost = travelToStation + chargingCost + travelToDest;

        if (totalCost < minTotalCost) {
            minTotalCost = totalCost;
            bestStation = i;


            bestPathFromOrigin.clear();
            for (int at = i; at != -1; at = prevFromOrigin[at])
                bestPathFromOrigin.push_back(at);
            reverse(bestPathFromOrigin.begin(), bestPathFromOrigin.end());

            bestPathToDestination.clear();
            for (int at = destination; at != -1; at = prevToDest[at])
                bestPathToDestination.push_back(at);
            reverse(bestPathToDestination.begin(), bestPathToDestination.end());
        }
    }

    cout << "\nCharging amount (randomly generated): " << kWh << "kWh" << endl;
    if (bestStation != -1) {
        cout << "The cheapest charging station is: " << locations[bestStation].locationName << ".\n";
        cout << "Charging cost = $" << kWh * locations[bestStation].chargingPrice << endl;
        cout << "Travel cost = $" << fixed << setprecision(2) << minTotalCost - kWh * locations[bestStation].chargingPrice << endl;
        cout << "Total cost = $" << fixed << setprecision(2) << minTotalCost << endl;

        cout << "\nTravel Path from " << originName << " to " << destinationName << ":\n";
        for (int idx : bestPathFromOrigin) {
            if (idx == bestStation)
                cout << locations[idx].locationName << " (Charge here) -> ";
            else
                cout << locations[idx].locationName << " -> ";
        }
        for (int j = 1; j < bestPathToDestination.size(); j++) {
            cout << locations[bestPathToDestination[j]].locationName;
            if (j != bestPathToDestination.size() - 1)
                cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "No valid charging station found on the path." << endl;
    }
}

void EVCharging::generateChargingDemandFor100EVs() {
    double travelRate = 0.28;
	double totalChargingCost = 0.0;
    vector<string> locationNames;
    for (auto& loc : locations) {
        locationNames.push_back(loc.second.locationName);
    }

    cout << left << setw(15) << "Vehicle ID"
         << setw(25) << "Origin"
         << setw(20) << "Destination"
         << setw(18) << "Charging Amount"
         << setw(25) << "Best Charging Station"
         << "Charging Cost" << endl;

    for (int vehicleID = 1; vehicleID <= 100; ++vehicleID) {
        string originName, destName;

        do {
            originName = locationNames[rand() % locationNames.size()];
            destName = locationNames[rand() % locationNames.size()];
        } while (originName == destName);

        double kWh = rand() % 51 + 10;

        int origin = -1, destination = -1;
        for (auto& entry : locations) {
            if (entry.second.locationName == originName) origin = entry.first;
            if (entry.second.locationName == destName) destination = entry.first;
        }

        vector<double> distFromOrigin(numberOfLocations, DBL_MAX);
        vector<bool> visited(numberOfLocations, false);
        distFromOrigin[origin] = 0;

        for (int i = 0; i < numberOfLocations - 1; i++) {
            double minDist = DBL_MAX;
            int u = -1;
            for (int j = 0; j < numberOfLocations; j++) {
                if (!visited[j] && distFromOrigin[j] < minDist) {
                    minDist = distFromOrigin[j];
                    u = j;
                }
            }
            if (u == -1) break;
            visited[u] = true;

            for (int v : weightedGraph->getAdjancencyList(u)) {
                double weight = weightedGraph->getWeight(u, v);
                if (!visited[v] && distFromOrigin[u] + weight < distFromOrigin[v]) {
                    distFromOrigin[v] = distFromOrigin[u] + weight;
                }
            }
        }

        string bestStationName = "";
        double minTotalCost = DBL_MAX;

        for (int i = 0; i < numberOfLocations; ++i) {
            if (i == origin || !locations[i].chargerInstalled || distFromOrigin[i] == DBL_MAX)
                continue;

            vector<double> distToDest(numberOfLocations, DBL_MAX);
            vector<bool> visitedToDest(numberOfLocations, false);
            distToDest[i] = 0;

            for (int j = 0; j < numberOfLocations - 1; j++) {
                double minDist = DBL_MAX;
                int u = -1;
                for (int k = 0; k < numberOfLocations; k++) {
                    if (!visitedToDest[k] && distToDest[k] < minDist) {
                        minDist = distToDest[k];
                        u = k;
                    }
                }
                if (u == -1) break;
                visitedToDest[u] = true;

                for (int v : weightedGraph->getAdjancencyList(u)) {
                    double weight = weightedGraph->getWeight(u, v);
                    if (!visitedToDest[v] && distToDest[u] + weight < distToDest[v]) {
                        distToDest[v] = distToDest[u] + weight;
                    }
                }
            }

            if (distToDest[destination] == DBL_MAX) continue;

            double travelToStation = distFromOrigin[i] * travelRate;
            double travelToDest = distToDest[destination] * travelRate;
            double chargingCost = kWh * locations[i].chargingPrice;
            double totalCost = travelToStation + travelToDest + chargingCost;

            if (totalCost < minTotalCost) {
                minTotalCost = totalCost;
                bestStationName = locations[i].locationName;
            }
        }

        cout << left << setw(15) << vehicleID
             << setw(25) << originName
             << setw(20) << destName
             << setw(18) << fixed << setprecision(0) << kWh
             << setw(25) << (bestStationName == "" ? "NONE" : bestStationName)
             << fixed << setprecision(2) << (bestStationName == "" ? 0.0 : minTotalCost) << endl;

		totalChargingCost += minTotalCost;
    }

	cout << fixed << setprecision(2) << "Total charging cost = $" << totalChargingCost << endl;
}

void EVCharging::recommendBestLocationForNewStation() {
    double travelRate = 0.28;
    int numVehicles = 100;

    struct Vehicle {
        int id;
        int origin;
        int destination;
        double kWh;
    };

    vector<string> locationNames;
    for (auto& entry : locations)
        locationNames.push_back(entry.second.locationName);

    //generate vehicles
    vector<Vehicle> vehicles;
    for (int i = 0; i < numVehicles; ++i) {
        int origin, dest;
        do {
            origin = rand() % numberOfLocations;
            dest = rand() % numberOfLocations;
        } while (origin == dest);
        double kWh = rand() % 51 + 10;
        vehicles.push_back({i + 1, origin, dest, kWh});
    }

    //calculate best charging for a list of vehicles
    auto calculateAllocations = [&](int newStation = -1, double& totalCost, vector<string>& stationNames, vector<double>& costs) {
        totalCost = 0;
        stationNames.clear();
        costs.clear();

        for (const auto& v : vehicles) {
            vector<double> distFromOrigin(numberOfLocations, DBL_MAX);
            vector<bool> visited(numberOfLocations, false);
            distFromOrigin[v.origin] = 0;

            for (int i = 0; i < numberOfLocations - 1; i++) {
                double minDist = DBL_MAX;
                int u = -1;
                for (int j = 0; j < numberOfLocations; j++) {
                    if (!visited[j] && distFromOrigin[j] < minDist) {
                        minDist = distFromOrigin[j];
                        u = j;
                    }
                }
                if (u == -1) break;
                visited[u] = true;

                for (int vtx : weightedGraph->getAdjancencyList(u)) {
                    double weight = weightedGraph->getWeight(u, vtx);
                    if (!visited[vtx] && distFromOrigin[u] + weight < distFromOrigin[vtx])
                        distFromOrigin[vtx] = distFromOrigin[u] + weight;
                }
            }

            double minCost = DBL_MAX;
            string bestStation = "NONE";

            for (int i = 0; i < numberOfLocations; ++i) {
                bool hasCharger = locations[i].chargerInstalled || (i == newStation);
                if (!hasCharger || i == v.origin || distFromOrigin[i] == DBL_MAX) continue;

                vector<double> distToDest(numberOfLocations, DBL_MAX);
                vector<bool> visitedToDest(numberOfLocations, false);
                distToDest[i] = 0;

                for (int i2 = 0; i2 < numberOfLocations - 1; i2++) {
                    double minDist = DBL_MAX;
                    int u = -1;
                    for (int j = 0; j < numberOfLocations; j++) {
                        if (!visitedToDest[j] && distToDest[j] < minDist) {
                            minDist = distToDest[j];
                            u = j;
                        }
                    }
                    if (u == -1) break;
                    visitedToDest[u] = true;

                    for (int vtx : weightedGraph->getAdjancencyList(u)) {
                        double weight = weightedGraph->getWeight(u, vtx);
                        if (!visitedToDest[vtx] && distToDest[u] + weight < distToDest[vtx])
                            distToDest[vtx] = distToDest[u] + weight;
                    }
                }

                if (distToDest[v.destination] == DBL_MAX) continue;

                double travelCost = (distFromOrigin[i] + distToDest[v.destination]) * travelRate;
                double chargeCost = (i == newStation) ? 0 : v.kWh * locations[i].chargingPrice;
                double total = travelCost + chargeCost;

                if (total < minCost) {
                    minCost = total;
                    bestStation = locations[i].locationName;
                }
            }

            stationNames.push_back(bestStation);
            costs.push_back(minCost);
            totalCost += minCost;
        }
    };

    //initial allocation
    double originalTotal;
    vector<string> originalStations;
    vector<double> originalCosts;

    calculateAllocations(-1, originalTotal, originalStations, originalCosts);

    //free station at each non-station location
    int bestLoc = -1;
    double bestTotal = originalTotal;

    for (int i = 0; i < numberOfLocations; ++i) {
        if (locations[i].chargerInstalled) continue;
        double testTotal;
        vector<string> tempS;
        vector<double> tempC;
        calculateAllocations(i, testTotal, tempS, tempC);

        if (testTotal < bestTotal) {
            bestTotal = testTotal;
            bestLoc = i;
        }
    }

    cout << "\nTask 9: The recommended location for a new free charging station is " << locations[bestLoc].locationName << ".\n\n";

    cout << "The original charging allocation:\n";
    cout << left << setw(12) << "Vehicle Id"
         << setw(20) << "Origin"
         << setw(20) << "Destination"
         << setw(18) << "Charging Amount"
         << setw(25) << "Best charging at"
         << "Charging cost\n";

    for (int i = 0; i < numVehicles; ++i) {
        cout << left << setw(12) << vehicles[i].id
             << setw(20) << locations[vehicles[i].origin].locationName
             << setw(20) << locations[vehicles[i].destination].locationName
             << setw(18) << vehicles[i].kWh
             << setw(25) << originalStations[i]
             << fixed << setprecision(3) << originalCosts[i] << "\n";
    }

    double newTotal;
    vector<string> newStations;
    vector<double> newCosts;
    calculateAllocations(bestLoc, newTotal, newStations, newCosts);

    cout << "\nThe charging allocation with the new installed free charging station:\n";
    cout << left << setw(12) << "Vehicle Id"
         << setw(20) << "Origin"
         << setw(20) << "Destination"
         << setw(18) << "Charging Amount"
         << setw(25) << "Best charging at"
         << "Charging cost\n";

    for (int i = 0; i < numVehicles; ++i) {
        cout << left << setw(12) << vehicles[i].id
             << setw(20) << locations[vehicles[i].origin].locationName
             << setw(20) << locations[vehicles[i].destination].locationName
             << setw(18) << vehicles[i].kWh
             << setw(25) << newStations[i]
             << fixed << setprecision(3) << newCosts[i] << "\n";
    }

    cout << "\nAverage saving of charging cost per vehicle = $" 
         << fixed << setprecision(4) << ((originalTotal - newTotal) / numVehicles)
         << " = (" << fixed << setprecision(2) << originalTotal << " - " << newTotal << ") / 100\n";
}


#endif /* EVCHARGING_H_ */
