//This is a new program designed to take in a user's stats for their birding.
//It takes in their location, the duration of their trip, the distance they traveled, how they birded, how many birds they saw, and the hourly average.

// ********** DOCTEST STATEMENTS **********
#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

// ********** MEMORY LEAK STATEMENTS **********
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// ********** INCLUDE STATEMENTS **********
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <vector>

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//********** Enum To Determine What Style of Birding **********
enum _Type {
	TRAIL,
	CAR,
	PARK
};

//********** PRINT TABLE FUNCTION (NOT USED CURRENTLY) **********
//void printTable(const string& location, int time, double distance, _Type type, double birdsObserved, double hourlyAverage);

//********** Base Class To Determine Time, Distance, Location, and Type of Birding **********
class Distancetime {
private:
	_Type type;
	int _time;
	double _distance;
protected:
	string _location;
public:
	Distancetime() {
		_time = 0;
		_distance = 0.0;
		type = TRAIL;
	}

	virtual void Abstractfunction() = 0;

	void setLocation(const string& location) {
		_location = location;
	}
	void setTime(int time) {
		_time = time;
	}
	void setDistance(double distance) {
		_distance = distance;
	}
	void setType(_Type birdingtype) {
		type = birdingtype;
	}

	string getLocation() const {
		return _location;
	}
	int getTime() const {
		return _time;
	}
	double getDistance() const {
		return _distance;
	}
	_Type getType() const {
		return type;
	}

	virtual void printOneLine(ostream& os) const {
		os << "[Type: ";
		switch (type) {
		case TRAIL: os << "Trail"; break;
		case CAR: os << "Car"; break;
		case PARK: os << "Park"; break;
		}

		os << ", Location: " << _location
			<< ", Time(min): " << _time
			<< ", Distance(mi): " << _distance;
	}

	friend ostream& operator<<(ostream& os, const Distancetime& obj) {
		obj.printOneLine(os);
		return os;
	}

	virtual void Print() {
		cout << "Type of Birding: ";
		switch (type) {
		case _Type::TRAIL: cout << "Trail Birding\n"; break;
		case _Type::CAR: cout << "Car Birding\n"; break;
		case _Type::PARK: cout << "Park Birding\n"; break;
		}
		cout << "Location Birding: " << _location << endl;
		cout << "Minutes Spent Birding: " << _time << endl;
		cout << "Miles Traveled: " << _distance << endl;
	}

	virtual ~Distancetime() {

	};

	virtual double getBirdCount() const = 0;
};

//********** Composition Class 1 To Determine Bird Average **********
class Hourlybirds {
private:
	double _hourlybirds;
protected:

public:
	void Division(double birdsseen, int time) {
		_hourlybirds = birdsseen / (time / 60.0);
	}

	double getDivision() const {
		return _hourlybirds;
	}

	void printDivision() const {
		cout << "Birds Seen per Hour: " << _hourlybirds << endl;
	}
};

//********** Derived Class 1 To Determine How Many Birds Seen **********
class Birdsseen : public Distancetime {
private:
	double _birdsseen;
	int _birdspecies;
protected:

public:
	Hourlybirds hourlybirds;

	void Abstractfunction() override {
		cout << "Birdsseen overrides this function!" << endl;
	}

	void setBirdsseen(double birdsseen) {
		_birdsseen = birdsseen;
		hourlybirds.Division(_birdsseen, getTime());
	}

	void setBirdspecies(int birdspecies) {
		_birdspecies = birdspecies;
	}

	void setTime(int time) {
		Distancetime::setTime(time);
		hourlybirds.Division(_birdsseen, getTime());
	}
	double getBirdsseen() const {
		return _birdsseen;
	}

	int getBirdspecies() const {
		return _birdspecies;
	}

	double getBirdCount() const override {
		return _birdsseen;
	}

	bool operator== (const Birdsseen& other) const {
		return (_birdsseen == other._birdsseen && _birdspecies == other._birdspecies);
	}

	void printOneLine(ostream& os) const override {
		Distancetime::printOneLine(os);
		os << ", Birds: " << _birdsseen
			<< ", Species: " << _birdspecies
			<< ", Birds/hr: " << hourlybirds.getDivision()
			<< "]";
	}

	virtual void Print() {
		Distancetime::Print();
		cout << "Birds Observed: " << _birdsseen << endl;
		cout << "Different Bird Species: " << _birdspecies << endl;
		hourlybirds.printDivision();
	}
};


//********** Derived Class 2 To Determine if Birding Was Fun Despite No Birds**********
class Nobirds : public Distancetime {
private:
	string _fun;
protected:

public:

	void Abstractfunction() override {
		cout << "Nobirds overrides this function!" << endl;
	}

	void setFun(const string& fun) {
		_fun = fun;
	}

	string getFun() const {
		return _fun;
	}

	double getBirdCount() const override {
		return 0.0;
	}

	void printOneLine(ostream& os) const override {
		Distancetime::printOneLine(os);
		os << ", Birds: 0"
			<< ", Fun: " << _fun
			<< "]";
	}

	virtual void Print() {
		Distancetime::Print();
		cout << "Birds Observed: 0" << endl;
		cout << "Did You Have Fun Regardless: " << _fun << endl;
	}
};

template <typename T>
class Manager {
private:
	vector<T*> items;
protected:

public:
	Manager(int initialCapacity = 10) {

	}

	~Manager() {
		for (auto item : items) {
			delete item;
		}
	}

	void addTrip(T* trip) {
		items.push_back(trip);
	}

	Manager& operator+=(T* trip) {
		this->addTrip(trip);
		return *this;
	}

	void removeTrip(int index) {
		if (index < 0 || index >= items.size()) {
			throw std::out_of_range("Invalid index for removal");
		}
		delete items[index];
		items.erase(items.begin() + index);
	}

	Manager& operator-=(int index) {
		this->removeTrip(index);
		return *this;
	}

	void printTrips() const {
		for (int item = 0; item < items.size(); item++) {
			SetConsoleTextAttribute(hConsole, 13);
			cout << "Birding Trip #" << (item + 1) << endl;
			SetConsoleTextAttribute(hConsole, 7);
			items[item]->Print();
		}
	}

	int getSize() const {
		return items.size();
	}

	T* operator[](int index) const {
		if (index < 0 || index >= items.size()) {
			throw std::out_of_range("Index out of bounds");
		}
		return items[index];
	}

	int countTripsRecursive(int index = 0) const {
		if (index >= items.size()) {
			return 0;
		}
		return 1 + countTripsRecursive(index + 1);
	}

	int Sequentialsearch(const string& targetLocation) const {
		for (int item = 0; item < items.size(); item++) {
			if (items[item]->getLocation() == targetLocation) {
				return item;
			}
		}
		return -1;
	}

	void Bubblesort() {
		for (int item = 0; item < items.size() - 1; item++) {
			for (int item2 = 0; item2 < items.size() - item - 1; item2++) {
				if (items[item2]->getBirdCount() > items[item2 + 1]->getBirdCount()) {
					T* temp = items[item2];
					items[item2] = items[item2 + 1];
					items[item2 + 1] = temp;
				}
			}
		}
	}

	int Binarysearch(double targetBird) const {
		int first = 0;
		int last = items.size() - 1;

		while (first <= last) {
			int mid = (first + last) / 2;
			double midBird = items[mid]->getBirdCount();

			if (midBird == targetBird) {
				return mid;
			}
			else if (midBird > targetBird) {
				last = mid - 1;
			}
			else {
				first = mid + 1;
			}
		}
		return -1;
	}
};

template <typename T>
void printSummary(const T& trip, std::ostream& os = std::cout) {
	os << trip << endl;
}

class Enterzero {
private:
	string message;
public:
	Enterzero() {
		message = "Please input a number larger than 0:";
	}
	Enterzero(string str) {
		message = str;
	}
	string what() {
		return message;
	}
};

//********** DOCTEST UNIT TESTS **********
#ifdef _DEBUG

TEST_CASE("Division Tests") {
	Hourlybirds hourlybirds;
	hourlybirds.Division(15, 60);
	CHECK(hourlybirds.getDivision() == doctest::Approx(15.0));

	hourlybirds.Division(5, 120);
	CHECK(hourlybirds.getDivision() == doctest::Approx(2.5));
}

TEST_CASE("Birdsseen Test Using Hourlybirds") {
	Birdsseen birdsseen;
	birdsseen.setLocation("Lake St. Clair");
	birdsseen.setTime(120);
	birdsseen.setBirdsseen(20);

	CHECK(birdsseen.getLocation() == "Lake St. Clair");
	CHECK(birdsseen.getTime() == 120);
	CHECK(birdsseen.getBirdsseen() == doctest::Approx(20.0));
	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(10.0));
}

TEST_CASE("Birdsseen/Hourlybirds Test When Time Changes") {
	Birdsseen birdsseen;
	birdsseen.setBirdsseen(12);
	birdsseen.setTime(60);

	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(12.0));

	birdsseen.setTime(120);

	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(6.0));
}

TEST_CASE("Nobirds Tests") {
	Nobirds nobirds;
	nobirds.setLocation("Belle Isle");
	nobirds.setTime(45);
	nobirds.setDistance(1.2);
	nobirds.setFun("Yes");

	CHECK(nobirds.getLocation() == "Belle Isle");
	CHECK(nobirds.getTime() == 45);
	CHECK(nobirds.getFun() == "Yes");
}

TEST_CASE("Print Tests") {
	Birdsseen birdsseen;
	birdsseen.setLocation("Gallup Park");
	birdsseen.setTime(30);
	birdsseen.setDistance(0.5);
	birdsseen.setType(PARK);
	birdsseen.setBirdsseen(5);

	CHECK(birdsseen.getLocation() == "Gallup Park");
	CHECK(birdsseen.getTime() == 30);
	CHECK(birdsseen.getDistance() == doctest::Approx(0.5));
	CHECK(birdsseen.getType() == PARK);
	CHECK(birdsseen.getBirdsseen() == doctest::Approx(5));
	CHECK(birdsseen.hourlybirds.getDivision() == doctest::Approx(10.));
}

TEST_CASE("Virtual Function Test") {
	Distancetime* ptr1 = new Birdsseen();
	Distancetime* ptr2 = new Nobirds();

	ptr1->Abstractfunction();
	ptr2->Abstractfunction();

	ptr1->Print();
	ptr2->Print();

	delete ptr1;
	delete ptr2;
}

TEST_CASE("Overload Addition/Deletion & Invalid Index Tests") {
	Manager<Distancetime> manager(2);

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Lake A"); b1->setTime(60); b1->setBirdsseen(5);
	manager += (b1);

	Nobirds* n1 = new Nobirds();
	n1->setLocation("Park B"); n1->setTime(30); n1->setFun("Yes");
	manager += (n1);

	CHECK(manager.getSize() == 2);

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake C"); b2->setTime(45); b2->setBirdsseen(3);
	manager += (b2);

	CHECK(manager.getSize() == 3);

	manager -= (1);
	CHECK(manager.getSize() == 2);

	CHECK_THROWS_AS(manager -= 5, std::out_of_range);
	CHECK_THROWS_AS(manager -= -1, std::out_of_range);
}

TEST_CASE("Overload operator== Test") {
	Birdsseen b1;
	Birdsseen b2;
	Birdsseen b3;

	b1.setBirdsseen(10);
	b1.setBirdspecies(3);

	b2.setBirdsseen(10);
	b2.setBirdspecies(3);

	CHECK(b1 == b2);

	b3.setBirdsseen(5);
	b3.setBirdspecies(5);

	CHECK_FALSE(b2 == b3);
}

TEST_CASE("Overload operator<< Test Birdsseen") {
	Birdsseen b;
	b.setLocation("Lake St. Clair");
	b.setTime(60);
	b.setDistance(2.0);
	b.setType(CAR);
	b.setBirdsseen(10);
	b.setBirdspecies(3);

	std::ostringstream oss;
	oss << b;

	CHECK(oss.str() ==
		"[Type: Car, Location: Lake St. Clair, Time(min): 60, Distance(mi): 2, Birds: 10, Species: 3, Birds/hr: 10]");
}

TEST_CASE("Overload operator<< Test Nobirds") {
	Nobirds b;
	b.setLocation("Kensington");
	b.setTime(120);
	b.setDistance(0.5);
	b.setType(TRAIL);
	b.setFun("Yes");

	std::ostringstream oss;
	oss << b;

	CHECK(oss.str() ==
		"[Type: Trail, Location: Kensington, Time(min): 120, Distance(mi): 0.5, Birds: 0, Fun: Yes]");
}

TEST_CASE("Overload operator[] & Class Template Test & Invalid Index Test") {
	Manager<Distancetime> manager;

	Birdsseen* b = new Birdsseen();
	b->setLocation("Test Location");
	b->setTime(60);
	b->setBirdsseen(10);

	manager.addTrip(b);

	
	CHECK(manager[0] != nullptr);

	CHECK_THROWS_AS(manager[1], std::out_of_range);
	CHECK_THROWS_AS(manager[-1], std::out_of_range);
}

TEST_CASE("Function Template Test") {
	Birdsseen b;
	b.setLocation("Lake St. Clair");
	b.setTime(60);
	b.setDistance(2.0);
	b.setType(CAR);
	b.setBirdsseen(10);
	b.setBirdspecies(3);

	std::ostringstream oss;

	printSummary(b, oss);

	CHECK(oss.str() ==
		"[Type: Car, Location: Lake St. Clair, Time(min): 60, Distance(mi): 2, Birds: 10, Species: 3, Birds/hr: 10]\n");
}

TEST_CASE("Recursive Trip Count + Search Test") {
	Manager<Distancetime> manager;

	CHECK(manager.countTripsRecursive() == 0);

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Kensington");
	b1->setTime(60);
	b1->setBirdsseen(120);

	Nobirds* n1 = new Nobirds();
	n1->setLocation("Lake");
	n1->setTime(30);
	n1->setFun("Yes");

	manager += b1;
	manager += n1;

	CHECK(manager.countTripsRecursive() == 2);

	CHECK(manager.Sequentialsearch("Kensington") == 0);
	CHECK(manager.Sequentialsearch("Lake") == 1);

	CHECK(manager.Sequentialsearch("Stony Creek") == -1);
}

TEST_CASE("Bubble Sort + Binary Search Test") {
	Manager<Distancetime> manager;

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Kensington");
	b1->setBirdsseen(20);

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake");
	b2->setBirdsseen(5);

	Nobirds* n1 = new Nobirds();
	n1->setLocation("Stony");

	manager += b1;
	manager += b2;
	manager += n1;

	manager.Bubblesort();

	CHECK(manager[0]->getLocation() == "Stony");
	CHECK(manager[1]->getLocation() == "Lake");
	CHECK(manager[2]->getLocation() == "Kensington");

	CHECK(manager.Binarysearch(20) == 2);
	CHECK(manager.Binarysearch(50) == -1);
}

#else

//********** MAIN FUNCTION **********

int main() {
	string location;
	int time;
	double distance;
	int typechoice;
	double birdsobserved;
	int speciesofbirds;
	string fun;
	char moreTrips = 'y';
	char removeTrips = 'y';
	int wantedTrip;

	Manager<Distancetime> manager;

	SetConsoleTextAttribute(hConsole, 13);
	cout << "Welcome to the Bird Stat Tracker" << endl;
	SetConsoleTextAttribute(hConsole, 7);

	do {
		cout << "Where did you go birding?" << endl;
		getline(cin, location);

		cout << "How many minutes did you go birding?" << endl;
		cin >> time;

		do {
			try {
				if (time <= 0)
				throw Enterzero();
			}
			catch (Enterzero enterzero) {
				cout << enterzero.what() << endl;
				cin >> time;
			}
		} while (time <= 0);

		cout << "How many miles did you travel while birding?" << endl;
		cin >> distance;

		cout << "What type of birding did you do? (0 = Trail, 1 = Car, 2 = Park): " << endl;
		cin >> typechoice;

		cout << "How many birds did you see?" << endl;
		cin >> birdsobserved;

		cout << "How many different species of birds did you observe?" << endl;
		cin >> speciesofbirds;
		cin.ignore();

		_Type birdingtype;
		switch (typechoice) {
		case 0: birdingtype = TRAIL; break;
		case 1: birdingtype = CAR; break;
		case 2: birdingtype = PARK; break;
		default: birdingtype = TRAIL; break;
		}

		if (birdsobserved > 0) {
			Birdsseen* trip = new Birdsseen();
			trip->setLocation(location);
			trip->setTime(time);
			trip->setDistance(distance);
			trip->setType(birdingtype);
			trip->setBirdsseen(birdsobserved);
			trip->setBirdspecies(speciesofbirds);

			manager += trip;
			printSummary(*trip);
		}
		else {
			cout << "Did you still have fun and enjoy the nature? (Yes/No): " << endl;
			getline(cin, fun);

			Nobirds* trip = new Nobirds();
			trip->setLocation(location);
			trip->setTime(time);
			trip->setDistance(distance);
			trip->setType(birdingtype);
			trip->setFun(fun);

			manager += trip;
			printSummary(*trip);
		}

		cout << "Would you like to add another birding trip? (Y/N)" << endl;
		cin >> moreTrips;
		cin.ignore();

	} while (moreTrips == 'y' || moreTrips == 'Y');

	cout << "\nAll Birding Trips:" << endl;
	manager.printTrips();
	cout << "Total trips taken: " << manager.countTripsRecursive() << endl;

	do {
		cout << "Would you like to remove a birding trip? (Y/N)" << endl;
		cin >> removeTrips;
		cin.ignore();

		if (removeTrips == 'y' || removeTrips == 'Y') {
			cout << "Which trip would you like to remove?" << endl;
			cin >> wantedTrip;
			cin.ignore();
			
			try {
				manager -= (wantedTrip - 1);
			}
			catch (std::out_of_range& exception) {
				cout << exception.what() << endl;
			}
		}

	} while (removeTrips == 'y' || removeTrips == 'Y');

	cout << "\nUpdated Birding Trips:" << endl;
	manager.printTrips();
	cout << "Total trips taken: " << manager.countTripsRecursive() << endl;

	_CrtDumpMemoryLeaks();

	return 0;
}
#endif
/*
void printTable(const string& location, int time, double distance, _Type type, double birdsObserved, double hourlyAverage) {
	cout << fixed << setprecision(2);

	SetConsoleTextAttribute(hConsole, 13);
	cout << setw(32) << setfill('*') << "" << setfill(' ') << endl;
	SetConsoleTextAttribute(hConsole, 7);

	cout << "   Location: " << setw(16) << location << endl;
	cout << "   Minutes Birding: " << setw(9) << time << endl;
	cout << "   Miles Traveled: " << setw(10) << distance << endl;
	cout << "   Birds Seen: " << setw(14) << birdsObserved << endl;
	cout << "   Average Birds: " << setw(11) << hourlyAverage << endl;

	SetConsoleTextAttribute(hConsole, 13);
	cout << setw(32) << setfill('*') << "" << endl;
	SetConsoleTextAttribute(hConsole, 7);

	ofstream outFile("report.txt");
	outFile << setw(30) << setfill('*') << "" << setfill(' ') << endl;
	outFile << "   Location: " << location << endl;
	outFile << "   Minutes Birding: " << time << endl;
	outFile << "   Miles Traveled: " << distance << endl;
	outFile << "   Birds Seen: " << birdsObserved << endl;
	outFile << "   Avg Birds/Hour: " << hourlyAverage << endl;
	outFile << setw(30) << setfill('*') << "" << endl;
	outFile.close();
}
*/