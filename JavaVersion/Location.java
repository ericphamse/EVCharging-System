import java.io.*;
import java.util.*;

class Location implements Comparable<Location> {
    int index;
    String locationName;
    boolean chargerInstalled;
    double chargingPrice;

    void printLocation() {
        System.out.printf("%6d%20s%15s", index, locationName, chargerInstalled ? "yes" : "no");
        if (chargerInstalled && chargingPrice == 0)
            System.out.printf("%28s\n", "free of charge");
        else if (!chargerInstalled)
            System.out.printf("%17s\n", "N/A");
        else
            System.out.printf("%15s%.2f/kWh\n", "$", chargingPrice);
    }

    @Override
    public int compareTo(Location l) {
        return Double.compare(this.chargingPrice, l.chargingPrice);
    }
}