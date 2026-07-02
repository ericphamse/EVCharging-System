import java.util.*;
import java.io.*;

class EVCharging {
    Map<Integer, Location> locations = new HashMap<>();
    int numberOfLocations;
    WeightedGraphType weightedGraph;

    public EVCharging() {
        inputLocations();
        weightedGraph = new WeightedGraphType(numberOfLocations);
    }

    void inputLocations() {
        try (Scanner infile = new Scanner(new File("Locations.txt"))) {
            int locationIndex = 0;
            while (infile.hasNextLine()) {
                String line = infile.nextLine();
                String[] parts = line.split(",");
                Location s = new Location();
                s.locationName = parts[0];
                s.chargerInstalled = Integer.parseInt(parts[1]) == 1;
                s.chargingPrice = Double.parseDouble(parts[2]);
                s.index = locationIndex;
                locations.put(locationIndex++, s);
            }
            numberOfLocations = locationIndex;
        } catch (Exception e) {
            System.out.println("Cannot open input file.");
        }
    }

    void printLocations() {
        System.out.println("List of locations and charging information ");
        System.out.printf("%8s%20s%20s%20s\n", "Index", "Location name", "Charging station", "Charging price");
        for (Location loc : locations.values()) {
            loc.printLocation();
        }
        System.out.println();
    }

    void printAdjacencyMatrix() {
        System.out.println("Adjacency matrix (0 means no direct connection, non-zero value represents the distance of adjacent locations)\n");
        System.out.printf("%13s", "");
        for (int i = 0; i < numberOfLocations; i++) {
            System.out.printf("%13s", locations.get(i).locationName);
        }
        System.out.println();
        for (int i = 0; i < numberOfLocations; i++) {
            System.out.printf("%13s", locations.get(i).locationName);
            for (int j = 0; j < numberOfLocations; j++) {
                double weight = weightedGraph.getWeight(i, j);
                System.out.printf("%13.2f", weight == Double.MAX_VALUE ? 0.0 : weight);
            }
            System.out.println();
        }
    }

    void printLocationsSortedByPrice() {
        List<Location> sortedStations = new ArrayList<>();
        for (Location loc : locations.values()) {
            if (loc.chargerInstalled) {
                sortedStations.add(loc);
            }
        }
        Collections.sort(sortedStations);
        System.out.println("\nLocations with charging stations sorted by charging price (ascending):");
        System.out.printf("%8s%20s%20s%20s\n", "Index", "Location name", "Charging station", "Charging price");
        for (Location loc : sortedStations) {
            loc.printLocation();
        }
        System.out.println();
    }

    void BFT() {
        System.out.println("The list of locations with breadth-first traversal:");
        List<Integer> bftResult = weightedGraph.breadthFirstTraversal();
        for (int index : bftResult) {
            System.out.println(locations.get(index).locationName);
        }
    }

    void showAdjacentChargers(String locationName) {
        int index = -1;
        for (Map.Entry<Integer, Location> entry : locations.entrySet()) {
            if (entry.getValue().locationName.equals(locationName)) {
                index = entry.getKey();
                break;
            }
        }
        if (index == -1) {
            System.out.println("Location not found.");
            return;
        }
        List<Integer> adjList = weightedGraph.getAdjancencyList(index);
        boolean hasCharger = false;
        for (int adjIndex : adjList) {
            if (locations.get(adjIndex).chargerInstalled) {
                if (!hasCharger) {
                    System.out.println("Adjacent locations with charging stations:");
                    hasCharger = true;
                }
                System.out.println("- " + locations.get(adjIndex).locationName);
            }
        }
        if (!hasCharger) {
            System.out.println("NONE");
        }
    }

    void findNearestChargingStation(String locationName) {
        int srcIndex = getLocationIndex(locationName);
        if (srcIndex == -1) {
            System.out.println("Location not found.");
            return;
        }

        double[] distances = new double[numberOfLocations];
        boolean[] visited = new boolean[numberOfLocations];
        Arrays.fill(distances, Double.MAX_VALUE);
        distances[srcIndex] = 0;

        for (int count = 0; count < numberOfLocations - 1; count++) {
            int u = getMinDistanceIndex(distances, visited);
            if (u == -1) break;
            visited[u] = true;
            for (int v : weightedGraph.getAdjancencyList(u)) {
                double weight = weightedGraph.getWeight(u, v);
                if (!visited[v] && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                }
            }
        }

        int nearestIndex = -1;
        double minDistance = Double.MAX_VALUE;
        for (int i = 0; i < numberOfLocations; i++) {
            if (i != srcIndex && locations.get(i).chargerInstalled && distances[i] < minDistance) {
                minDistance = distances[i];
                nearestIndex = i;
            }
        }

        if (nearestIndex != -1) {
            System.out.println("Nearest location with a charging station is: " + locations.get(nearestIndex).locationName);
        } else {
            System.out.println("No other location with a charging station found.");
        }
    }

    int getLocationIndex(String name) {
        for (Map.Entry<Integer, Location> entry : locations.entrySet()) {
            if (entry.getValue().locationName.equals(name)) {
                return entry.getKey();
            }
        }
        return -1;
    }

    int getMinDistanceIndex(double[] dist, boolean[] visited) {
        double min = Double.MAX_VALUE;
        int index = -1;
        for (int i = 0; i < dist.length; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                index = i;
            }
        }
        return index;
    }

    void findLowestTotalCostLocation(String locationName) {
        int srcIndex = getLocationIndex(locationName);
        if (srcIndex == -1) {
            System.out.println("Location not found.");
            return;
        }

        double[] distances = new double[numberOfLocations];
        boolean[] visited = new boolean[numberOfLocations];
        Arrays.fill(distances, Double.MAX_VALUE);
        distances[srcIndex] = 0;

        for (int count = 0; count < numberOfLocations - 1; count++) {
            int u = getMinDistanceIndex(distances, visited);
            if (u == -1) break;
            visited[u] = true;
            for (int v : weightedGraph.getAdjancencyList(u)) {
                double weight = weightedGraph.getWeight(u, v);
                if (!visited[v] && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                }
            }
        }

        double kWh = new Random().nextInt(51) + 10;
        double travelRate = 0.28;
        double minTotalCost = Double.MAX_VALUE;
        int bestLocation = -1;

        for (int i = 0; i < numberOfLocations; i++) {
            if (i != srcIndex && locations.get(i).chargerInstalled && distances[i] != Double.MAX_VALUE) {
                double travelCost = distances[i] * 2 * travelRate;
                double chargingCost = kWh * locations.get(i).chargingPrice;
                double totalCost = travelCost + chargingCost;
                if (totalCost < minTotalCost) {
                    minTotalCost = totalCost;
                    bestLocation = i;
                }
            }
        }

        if (bestLocation != -1) {
            System.out.println("Best location to charge: " + locations.get(bestLocation).locationName);
            System.out.println("Distance (one-way): " + distances[bestLocation] + " km");
            System.out.println("Charging price: $" + locations.get(bestLocation).chargingPrice + "/kWh");
            System.out.println("Energy needed: " + kWh + " kWh");
            System.out.printf("Total cost (travel + charging): $%.2f\n", minTotalCost);
        } else {
            System.out.println("No charging station found.");
        }
    }

    void findCheapestStationOnPath(String originName, String destinationName) {
        int origin = getLocationIndex(originName);
        int destination = getLocationIndex(destinationName);

        if (origin == -1 || destination == -1) {
            System.out.println("Invalid origin or destination.");
            return;
        }

        Random rand = new Random();
        double kWh = rand.nextInt(51) + 10;
        double travelRate = 0.28;

        double[] distFromOrigin = new double[numberOfLocations];
        Arrays.fill(distFromOrigin, Double.MAX_VALUE);
        boolean[] visited = new boolean[numberOfLocations];
        int[] prevFromOrigin = new int[numberOfLocations];
        Arrays.fill(prevFromOrigin, -1);
        distFromOrigin[origin] = 0;

        for (int i = 0; i < numberOfLocations - 1; i++) {
            int u = getMinDistanceIndex(distFromOrigin, visited);
            if (u == -1) break;
            visited[u] = true;
            for (int v : weightedGraph.getAdjancencyList(u)) {
                double weight = weightedGraph.getWeight(u, v);
                if (!visited[v] && distFromOrigin[u] + weight < distFromOrigin[v]) {
                    distFromOrigin[v] = distFromOrigin[u] + weight;
                    prevFromOrigin[v] = u;
                }
            }
        }

        double minTotalCost = Double.MAX_VALUE;
        int bestStation = -1;
        List<Integer> bestPathFromOrigin = new ArrayList<>();
        List<Integer> bestPathToDestination = new ArrayList<>();

        for (int i = 0; i < numberOfLocations; i++) {
            if (i == origin || !locations.get(i).chargerInstalled || distFromOrigin[i] == Double.MAX_VALUE)
                continue;

            double[] distToDest = new double[numberOfLocations];
            Arrays.fill(distToDest, Double.MAX_VALUE);
            boolean[] visitedToDest = new boolean[numberOfLocations];
            int[] prevToDest = new int[numberOfLocations];
            Arrays.fill(prevToDest, -1);
            distToDest[i] = 0;

            for (int j = 0; j < numberOfLocations - 1; j++) {
                int u = getMinDistanceIndex(distToDest, visitedToDest);
                if (u == -1) break;
                visitedToDest[u] = true;
                for (int v : weightedGraph.getAdjancencyList(u)) {
                    double weight = weightedGraph.getWeight(u, v);
                    if (!visitedToDest[v] && distToDest[u] + weight < distToDest[v]) {
                        distToDest[v] = distToDest[u] + weight;
                        prevToDest[v] = u;
                    }
                }
            }

            if (distToDest[destination] == Double.MAX_VALUE) continue;

            double travelToStation = distFromOrigin[i] * travelRate;
            double travelToDest = distToDest[destination] * travelRate;
            double chargingCost = kWh * locations.get(i).chargingPrice;
            double totalCost = travelToStation + chargingCost + travelToDest;

            if (totalCost < minTotalCost) {
                minTotalCost = totalCost;
                bestStation = i;

                bestPathFromOrigin.clear();
                for (int at = i; at != -1; at = prevFromOrigin[at])
                    bestPathFromOrigin.add(0, at);

                bestPathToDestination.clear();
                for (int at = destination; at != -1; at = prevToDest[at])
                    bestPathToDestination.add(0, at);
            }
        }

        System.out.println("\nCharging amount (randomly generated): " + kWh + "kWh");
        if (bestStation != -1) {
            System.out.println("The cheapest charging station is: " + locations.get(bestStation).locationName);
            System.out.printf("Charging cost = $%.2f\n", kWh * locations.get(bestStation).chargingPrice);
            System.out.printf("Travel cost = $%.2f\n", minTotalCost - kWh * locations.get(bestStation).chargingPrice);
            System.out.printf("Total cost = $%.2f\n", minTotalCost);

            System.out.println("\nTravel Path from " + originName + " to " + destinationName + ":");
            for (int idx : bestPathFromOrigin) {
                if (idx == bestStation)
                    System.out.print(locations.get(idx).locationName + " (Charge here) -> ");
                else
                    System.out.print(locations.get(idx).locationName + " -> ");
            }
            for (int j = 1; j < bestPathToDestination.size(); j++) {
                System.out.print(locations.get(bestPathToDestination.get(j)).locationName);
                if (j != bestPathToDestination.size() - 1)
                    System.out.print(" -> ");
            }
            System.out.println();
        } else {
            System.out.println("No valid charging station found on the path.");
        }
    }

    void generateChargingDemandFor100EVs() {
        double travelRate = 0.28;
        List<String> locationNames = new ArrayList<>();
        for (Location loc : locations.values()) {
            locationNames.add(loc.locationName);
        }

        System.out.printf("%-15s%-25s%-20s%-18s%-25s%s\n",
                "Vehicle ID", "Origin", "Destination", "Charging Amount",
                "Best Charging Station", "Charging Cost");

        for (int vehicleID = 1; vehicleID <= 100; ++vehicleID) {
            String originName, destName;
            Random rand = new Random();
            do {
                originName = locationNames.get(rand.nextInt(locationNames.size()));
                destName = locationNames.get(rand.nextInt(locationNames.size()));
            } while (originName.equals(destName));

            double kWh = rand.nextInt(51) + 10;
            int origin = getLocationIndex(originName);
            int destination = getLocationIndex(destName);

            double[] distFromOrigin = new double[numberOfLocations];
            boolean[] visited = new boolean[numberOfLocations];
            Arrays.fill(distFromOrigin, Double.MAX_VALUE);
            distFromOrigin[origin] = 0;

            for (int i = 0; i < numberOfLocations - 1; i++) {
                int u = getMinDistanceIndex(distFromOrigin, visited);
                if (u == -1) break;
                visited[u] = true;
                for (int v : weightedGraph.getAdjancencyList(u)) {
                    double weight = weightedGraph.getWeight(u, v);
                    if (!visited[v] && distFromOrigin[u] + weight < distFromOrigin[v]) {
                        distFromOrigin[v] = distFromOrigin[u] + weight;
                    }
                }
            }

            String bestStationName = "NONE";
            double minTotalCost = Double.MAX_VALUE;

            for (int i = 0; i < numberOfLocations; ++i) {
                if (i == origin || !locations.get(i).chargerInstalled || distFromOrigin[i] == Double.MAX_VALUE)
                    continue;

                double[] distToDest = new double[numberOfLocations];
                boolean[] visitedToDest = new boolean[numberOfLocations];
                Arrays.fill(distToDest, Double.MAX_VALUE);
                distToDest[i] = 0;

                for (int j = 0; j < numberOfLocations - 1; j++) {
                    int u = getMinDistanceIndex(distToDest, visitedToDest);
                    if (u == -1) break;
                    visitedToDest[u] = true;
                    for (int v : weightedGraph.getAdjancencyList(u)) {
                        double weight = weightedGraph.getWeight(u, v);
                        if (!visitedToDest[v] && distToDest[u] + weight < distToDest[v]) {
                            distToDest[v] = distToDest[u] + weight;
                        }
                    }
                }

                if (distToDest[destination] == Double.MAX_VALUE) continue;

                double travelToStation = distFromOrigin[i] * travelRate;
                double travelToDest = distToDest[destination] * travelRate;
                double chargingCost = kWh * locations.get(i).chargingPrice;
                double totalCost = travelToStation + travelToDest + chargingCost;

                if (totalCost < minTotalCost) {
                    minTotalCost = totalCost;
                    bestStationName = locations.get(i).locationName;
                }
            }

            System.out.printf("%-15d%-25s%-20s%-18.0f%-25s%.2f\n",
                    vehicleID, originName, destName, kWh,
                    bestStationName, bestStationName.equals("NONE") ? 0.0 : minTotalCost);
        }
    }

    /*void recommendBestLocationForNewStation() {
        double travelRate = 0.28;
        int numVehicles = 100;
        Random rand = new Random();

        class Vehicle {
            int id, origin, destination;
            double kWh;
            Vehicle(int id, int origin, int destination, double kWh) {
                this.id = id;
                this.origin = origin;
                this.destination = destination;
                this.kWh = kWh;
            }
        }

        List<Vehicle> vehicles = new ArrayList<>();
        for (int i = 0; i < numVehicles; i++) {
            int origin, dest;
            do {
                origin = rand.nextInt(numberOfLocations);
                dest = rand.nextInt(numberOfLocations);
            } while (origin == dest);
            double kWh = rand.nextInt(51) + 10;
            vehicles.add(new Vehicle(i + 1, origin, dest, kWh));
        }

        class AllocationResult {
            double totalCost;
            List<String> stationNames;
            List<Double> costs;
        }

        AllocationResult calculateAllocations(int newStation, List<Vehicle> vehicles) {
            AllocationResult result = new AllocationResult();
            result.totalCost = 0;
            result.stationNames = new ArrayList<>();
            result.costs = new ArrayList<>();

            for (Vehicle v : vehicles) {
                double[] distFromOrigin = new double[numberOfLocations];
                Arrays.fill(distFromOrigin, Double.MAX_VALUE);
                boolean[] visited = new boolean[numberOfLocations];
                distFromOrigin[v.origin] = 0;

                for (int i = 0; i < numberOfLocations - 1; i++) {
                    int u = getMinDistanceIndex(distFromOrigin, visited);
                    if (u == -1) break;
                    visited[u] = true;
                    for (int adj : weightedGraph.getAdjancencyList(u)) {
                        double weight = weightedGraph.getWeight(u, adj);
                        if (!visited[adj] && distFromOrigin[u] + weight < distFromOrigin[adj]) {
                            distFromOrigin[adj] = distFromOrigin[u] + weight;
                        }
                    }
                }

                double minCost = Double.MAX_VALUE;
                String bestStation = "NONE";

                for (int i = 0; i < numberOfLocations; i++) {
                    boolean hasCharger = locations.get(i).chargerInstalled || (i == newStation);
                    if (!hasCharger || i == v.origin || distFromOrigin[i] == Double.MAX_VALUE) continue;

                    double[] distToDest = new double[numberOfLocations];
                    Arrays.fill(distToDest, Double.MAX_VALUE);
                    boolean[] visitedToDest = new boolean[numberOfLocations];
                    distToDest[i] = 0;

                    for (int i2 = 0; i2 < numberOfLocations - 1; i2++) {
                        int u = getMinDistanceIndex(distToDest, visitedToDest);
                        if (u == -1) break;
                        visitedToDest[u] = true;
                        for (int adj : weightedGraph.getAdjancencyList(u)) {
                            double weight = weightedGraph.getWeight(u, adj);
                            if (!visitedToDest[adj] && distToDest[u] + weight < distToDest[adj]) {
                                distToDest[adj] = distToDest[u] + weight;
                            }
                        }
                    }

                    if (distToDest[v.destination] == Double.MAX_VALUE) continue;

                    double travelCost = (distFromOrigin[i] + distToDest[v.destination]) * travelRate;
                    double chargeCost = (i == newStation) ? 0 : v.kWh * locations.get(i).chargingPrice;
                    double total = travelCost + chargeCost;

                    if (total < minCost) {
                        minCost = total;
                        bestStation = locations.get(i).locationName;
                    }
                }

                result.totalCost += minCost;
                result.stationNames.add(bestStation);
                result.costs.add(minCost);
            }

            return result;
        }

        AllocationResult original = calculateAllocations(-1, vehicles);

        int bestLoc = -1;
        double bestTotal = original.totalCost;

        for (int i = 0; i < numberOfLocations; i++) {
            if (locations.get(i).chargerInstalled) continue;
            AllocationResult test = calculateAllocations(i, vehicles);
            if (test.totalCost < bestTotal) {
                bestTotal = test.totalCost;
                bestLoc = i;
            }
        }

        System.out.println("\nTask 9: The recommended location for a new free charging station is " + locations.get(bestLoc).locationName + ".\n");

        System.out.printf("%-12s%-20s%-20s%-18s%-25s%s\n",
                "Vehicle Id", "Origin", "Destination", "Charging Amount", "Best charging at", "Charging cost");
        for (int i = 0; i < numVehicles; ++i) {
            Vehicle v = vehicles.get(i);
            System.out.printf("%-12d%-20s%-20s%-18.0f%-25s%.3f\n",
                    v.id, locations.get(v.origin).locationName, locations.get(v.destination).locationName,
                    v.kWh, original.stationNames.get(i), original.costs.get(i));
        }

        AllocationResult updated = calculateAllocations(bestLoc, vehicles);

        System.out.println("\nThe charging allocation with the new installed free charging station:");
        System.out.printf("%-12s%-20s%-20s%-18s%-25s%s\n",
                "Vehicle Id", "Origin", "Destination", "Charging Amount", "Best charging at", "Charging cost");
        for (int i = 0; i < numVehicles; ++i) {
            Vehicle v = vehicles.get(i);
            System.out.printf("%-12d%-20s%-20s%-18.0f%-25s%.3f\n",
                    v.id, locations.get(v.origin).locationName, locations.get(v.destination).locationName,
                    v.kWh, updated.stationNames.get(i), updated.costs.get(i));
        }

        double averageSaving = (original.totalCost - updated.totalCost) / numVehicles;
        System.out.printf("\nAverage saving of charging cost per vehicle = $%.4f = (%.2f - %.2f) / 100\n",
                averageSaving, original.totalCost, updated.totalCost);
    }*/
}