#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <time.h>

/* 
// Summary:

//    & when used with a variable declaration: "reference to"
//    Example: int &ra = a;
//    "ra is a reference to a"

//    * when used with a variable declaration: "pointer to"
//    Example: int *pa;
//    "pa is a pointer to an integer"

//    & when used with an already declared variable: "address of"
//    Example: &a;
//    "address of a"

//    * when used with an already declared pointer: "dereference"
//    Example: std::cout << *pa << std::endl;
//    "print the underlying value of a"
*/

using namespace std;

double const PI = 3.14159265358979323846;
double const R = 6473; // Km
string const LHR = "LHR";
string const EDI = "EDI";

void process_line(string& line, vector<string>* fields) {
	string token;
	istringstream string_stream(line);

	while (getline(string_stream, token, ',')) {
		fields->push_back(token);
	}
}

void convert_to_double(string& string_value, double* return_value) {
	stringstream convert_stream(string_value);

	if (!(convert_stream >> *return_value)) {
		return_value = 0;
	}
}

void degree_to_radians(double* degrees, double* radians) {
	*radians = (2 * PI * (*degrees / 360));
}

void calculate_distance(double* orig_lat_degrees, double* orig_long_degrees, double *dest_lat_degrees, double *dest_long_degrees, double* distance) {
	/*var R = 6371e3; // metres
	var φ1 = lat1.toRadians();
	var φ2 = lat2.toRadians();
	var Δφ = (lat2 - lat1).toRadians();
	var Δλ = (lon2 - lon1).toRadians();

	var a = Math.sin(Δφ / 2) * Math.sin(Δφ / 2) +
	Math.cos(φ1) * Math.cos(φ2) *
	Math.sin(Δλ / 2) * Math.sin(Δλ / 2);
	var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

	var d = R * c;*/

	double orig_lat_radians = 0;
	double orig_long_radians = 0;
	double dest_lat_radians = 0;
	double dest_long_radians = 0;

	double delta_lat_degrees;
	double delta_long_degrees;
	double delta_lat_radians;
	double delta_long_radians;

	double temp_a;
	double temp_c;


	delta_lat_degrees = *dest_lat_degrees - *orig_lat_degrees;

	degree_to_radians(orig_lat_degrees, &orig_lat_radians);
	degree_to_radians(dest_lat_degrees, &dest_lat_radians);

	degree_to_radians(&delta_lat_degrees, &delta_lat_radians);
	degree_to_radians(&delta_long_degrees, &delta_long_radians);

	temp_a = sin(delta_lat_radians / 2) * sin(delta_lat_radians / 2) +
		cos(orig_lat_radians) * cos(dest_lat_radians) *
		sin(delta_long_radians / 2) * sin(delta_long_radians / 2);
	
	temp_c = 2 * atan2(sqrt(temp_a), sqrt(1 - temp_a));

	*distance = R * temp_c;
}


void match_all_combinations(vector<string>* codes, vector<double>* v_lats, vector<double>* v_longs) {
	string left_code;
	string right_code;
	int length = codes->size();
	int i = 0;
	int j = 0;
	int total_loops = 0;
	double orig_lat_radians = 0;
	double orig_long_radians = 0;
	double orig_lat_degrees = 0;
	double orig_long_degrees = 0;

	double dest_lat_radians = 0;
	double dest_long_radians = 0;
	double dest_lat_degrees = 0;
	double dest_long_degrees = 0;

	double distance;

	clock_t start;
	clock_t end;

	start = clock();
	for (i = 0; i < length; i++) {
		left_code = codes->at(i);
		orig_lat_degrees = v_lats->at(i);
		orig_long_degrees = v_longs->at(i);

		for (j = 0; j < length; j++) {
			if (j != i) {
				right_code = codes->at(j);
				total_loops++;

				dest_lat_degrees = v_lats->at(j);
				dest_long_degrees = v_longs->at(j);

				calculate_distance(&orig_lat_degrees, &orig_long_degrees, &dest_lat_degrees, &dest_long_degrees, &distance);

				/*if (left_code == LHR && right_code == EDI) {
					cout << left_code << " to " << right_code << ": " << distance << '\n';
				}*/
			}
		}
	}
	cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000)  << '\n';

	cout << "total loops: " << total_loops;
}

int main(int argc, char* args[]) {
	const int NORMAL_IATA_IDX = 4;
	const int WHEN_N_IATA_IDX = 5;

	const int NORMAL_LAT_IDX = 6;
	const int WHEN_N_LAT_IDX = 7;

	const int NORMAL_LONG_IDX = 7;
	const int WHEN_N_LONG_IDX = 8;


	string iata_code;
	string lat_string;
	string long_string;
	double latitude;
	double longitude;
	string line;
	vector<string> v_parsed_fields;
	vector<string> v_iata_codes;
	vector<double> v_latitudes;
	vector<double> v_longitudes;
	ifstream airportDat;

	int n = 0; 
	//const int LIMIT = 10000;

	airportDat.open("E:\\IT\\airports-extended.dat");

	while(!airportDat.eof() /*&& n < LIMIT*/) {
		getline(airportDat, line);
		process_line(line, &v_parsed_fields);

		if (v_parsed_fields.size() > 1) {
			iata_code = v_parsed_fields[NORMAL_IATA_IDX];
			lat_string = v_parsed_fields[NORMAL_LAT_IDX];
			long_string = v_parsed_fields[NORMAL_LONG_IDX];
			if (iata_code != "\\N" && iata_code.length() == 5) {
				//cout << parsed_fields[0] << "- " <<  iata_code << '\n';
				iata_code = iata_code.substr(1, 3);
				v_iata_codes.push_back(iata_code);

				convert_to_double(lat_string, &latitude);
				convert_to_double(long_string, &longitude);

				v_latitudes.push_back(latitude);
				v_longitudes.push_back(longitude);

				//cout << lat_string << "       " << long_string << "             " << (latitude + longitude) << '\n';
			}
		}
		// Clears vector.
		v_parsed_fields.clear();
		n++;
	}

	
	airportDat.close();
	cout << "Number of locations: " << v_iata_codes.size() << '\n';

	match_all_combinations(&v_iata_codes, &v_latitudes, &v_longitudes);

	return 0;
}