/*
 * Location.h
 *
 *  Created on: 24 Apr 2025
 *      Author: Dongmo
 */

#ifndef LOCATION_H_
#define LOCATION_H_

class Location {
public:
	int index;//Key
	string locationName;
	bool chargerInstalled;
	double chargingPrice;

	void printLocation() const {
		cout << setw (6) << index << setw (20) << locationName << setw (15);
		if(chargerInstalled)
			cout << "yes";
		else
			cout << "no";

		if(chargerInstalled && chargingPrice == 0)
			cout << setw (28) << "free of charge" << endl;
		else if (!chargerInstalled )
			cout << setw (17) << "N/A" << endl;
		else
		   cout << setw(15) << "$" << setprecision(2) << fixed << chargingPrice << "/kWh" << endl;
	}

	bool operator<(const Location& l)  const {
		return chargingPrice < l.chargingPrice;
	}
};



#endif /* LOCATION_H_ */
