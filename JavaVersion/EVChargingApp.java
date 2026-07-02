import java.io.*;
import java.util.*;

public class EVChargingApp {
    public static void main(String[] args) {
        EVCharging charging = new EVCharging();
        charging.printLocations();
        charging.printAdjacencyMatrix();
        charging.printLocationsSortedByPrice();
        charging.BFT();

        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("\nChoose a task to run:");
            System.out.println("4. Task 4 : List adjacent charging stations");
            System.out.println("5. Task 5 : Find the nearest charging station (excluding the current location)");
            System.out.println("6. Task 6 : Find the cheapest charging station other than the current location");
            System.out.println("7. Task 7 : Find the cheapest charging station between origin and destination");
            System.out.println("8. Task 8 : Generate charging demands and calculate their charging locations");
            System.out.println("9. Task 9 : Find the best location to build a new charging station");
            System.out.println("0. Quit");
            System.out.print("Your choice: ");

            String input = scanner.nextLine();

            switch (input) {
                case "0":
                    return;
                case "4":
                    System.out.print("\nTask 4: Enter a location name to find adjacent locations with charging stations: ");
                    String loc4 = scanner.nextLine();
                    charging.showAdjacentChargers(loc4);
                    break;
                case "5":
                    System.out.print("\nTask 5: Enter a location name to find the nearest location with a charging station: ");
                    String loc5 = scanner.nextLine();
                    charging.findNearestChargingStation(loc5);
                    break;
                case "6":
                    System.out.print("\nTask 6: Enter your current location name to find the lowest total cost (travel + charging): ");
                    String loc6 = scanner.nextLine();
                    charging.findLowestTotalCostLocation(loc6);
                    break;
                case "7":
                    System.out.print("\nTask 7: Find the cheapest charging station between origin and destination.\n");
                    System.out.print("Input origin location: ");
                    String origin = scanner.nextLine();
                    System.out.print("Input destination location: ");
                    String destination = scanner.nextLine();
                    charging.findCheapestStationOnPath(origin, destination);
                    break;
                case "8":
                    System.out.println("\nTask 8: Generate charging demands and calculate their charging locations:");
                    charging.generateChargingDemandFor100EVs();
                    break;
                case "9":
                    System.out.println("\nTask 9: Find the best location to build a new charging station:");
                    //charging.recommendBestLocationForNewStation();
                    break;
                default:
                    System.out.println("Invalid input. Please choose a valid task number.");
            }
        }
    }
}
