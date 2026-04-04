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

template <typename T2>
class LinkedList {
private:
	struct Node {
		T2* data;
		Node* next;
		Node(T2* d) : data(d), next(nullptr) {}
	};

	Node* head;
	int size;

public:
	LinkedList() : head(nullptr), size(0) {}

	~LinkedList() {
		clear();
	}

	void insertFront(T2* item) {
		Node* newNode = new Node(item);
		newNode->next = head;
		head = newNode;
		size++;
	}

	void insertBack(T2* item) {
		Node* newNode = new Node(item);

		if (!head) {
			head = newNode;
		}
		else {
			Node* current = head;
			while (current->next)
				current = current->next;
			current->next = newNode;
		}
		size++;
	}

	void remove(int index) {
		if (index < 0 || index >= size)
			throw std::out_of_range("Invalid index");

		Node* temp = head;

		if (index == 0) {
			head = head->next;
		}
		else {
			Node* prev = nullptr;
			for (int item = 0; item < index; item++) {
				prev = temp;
				temp = temp->next;
			}
			prev->next = temp->next;
		}

		delete temp->data;
		delete temp;
		size--;
	}

	int search(const string& location) const {
		Node* current = head;
		int index = 0;

		while (current) {
			if (current->data->getLocation() == location)
				return index;
			current = current->next;
			index++;
		}
		return -1;
	}

	void set(int index, T2* value) {
		if (index < 0 || index >= size)
			throw std::out_of_range("Index out of bounds");

		Node* current = head;
		for (int item = 0; item < index; item++)
			current = current->next;

		current->data = value;
	}

	T2* get(int index) const {
		if (index < 0 || index >= size)
			throw std::out_of_range("Index out of bounds");

		Node* current = head;
		for (int item = 0; item < index; item++)
			current = current->next;

		return current->data;
	}

	int getSize() const {
		return size;
	}

	void clear() {
		Node* current = head;
		while (current) {
			Node* temp = current;
			current = current->next;
			delete temp->data;
			delete temp;
		}
		head = nullptr;
		size = 0;
	}

	class Iterator {
	private:
		Node* current;
	public:
		Iterator(Node* start) : current(start) {}

		bool hasNext() const {
			return current != nullptr;
		}

		void next() {
			if (current) current = current->next;
		}

		T2* getData() const {
			if (!current) throw std::runtime_error("Invalid iterator");
			return current->data;
		}
	};

	Iterator begin() const {
		return Iterator(head);
	}
};

template <typename T>
class Manager {
private:
	LinkedList<T> items;

public:
	Manager(int initialCapacity = 10) {}

	~Manager() {}

	void addTrip(T* trip) {
		items.insertBack(trip);
	}

	Manager& operator+=(T* trip) {
		addTrip(trip);
		return *this;
	}

	void removeTrip(int index) {
		items.remove(index);
	}

	Manager& operator-=(int index) {
		removeTrip(index);
		return *this;
	}

	void printTrips() const {
		auto it = items.begin();
		int i = 1;

		while (it.hasNext()) {
			SetConsoleTextAttribute(hConsole, 13);
			cout << "Birding Trip #" << i++ << endl;
			SetConsoleTextAttribute(hConsole, 7);

			it.getData()->Print();
			it.next();
		}
	}

	int getSize() const {
		return items.getSize();
	}

	T* operator[](int index) const {
		return items.get(index);
	}

	int countTripsRecursiveHelper(int index) const {
		if (index >= items.getSize()) return 0;
		return 1 + countTripsRecursiveHelper(index + 1);
	}

	int countTripsRecursive() const {
		return countTripsRecursiveHelper(0);
	}

	int Sequentialsearch(const string& targetLocation) const {
		return items.search(targetLocation);
	}

	void Bubblesort() {
		int n = items.getSize();
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (items.get(j)->getBirdCount() > items.get(j + 1)->getBirdCount()) {
					T* temp = items.get(j);
					items.set(j, items.get(j + 1));
					items.set(j + 1, temp);
				}
			}
		}
	}

	int Binarysearch(double targetBird) const {
		int first = 0;
		int last = items.getSize() - 1;

		while (first <= last) {
			int mid = (first + last) / 2;
			double midBird = items.get(mid)->getBirdCount();

			if (midBird == targetBird) return mid;
			else if (midBird > targetBird) last = mid - 1;
			else first = mid + 1;
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

// ===== ASSIGNMENT 11 ADDITION =====
// Stack class implemented using a linked list
// WHY linked list? Because the existing program already uses a linked list pattern
// and a linked list based stack has no size limit unlike an array based stack
// Used to track trip history so the user can undo the last added trip
template <typename T>
class TripStack {
private:
	// ===== ASSIGNMENT 11 ADDITION =====
	// Node structure for the stack
	struct StackNode {
		T* data;        // pointer to the trip
		StackNode* next; // pointer to the next node
		StackNode(T* d) : data(d), next(nullptr) {}
	};
	StackNode* top;  // pointer to the top of the stack
	int size;        // number of items in the stack
public:
	// Constructor — initializes empty stack
	TripStack() : top(nullptr), size(0) {}
	// Destructor — cleans up all nodes
	// NOTE: does NOT delete the trip data — Manager owns the trips
	~TripStack() {
		while (!isEmpty()) {
			StackNode* temp = top;
			top = top->next;
			delete temp;  // delete node only not the trip data
		}
	}
	// Push — adds a trip pointer to the top of the stack
	void push(T* trip) {
		StackNode* newNode = new StackNode(trip);
		newNode->next = top;
		top = newNode;
		size++;
	}
	// Pop — removes the top trip pointer from the stack
	// throws exception if stack is empty
	void pop() {
		if (isEmpty())
			throw std::out_of_range("Stack is empty — cannot pop");
		StackNode* temp = top;
		top = top->next;
		delete temp;  // delete node only not the trip data
		size--;
	}
	// Peek — returns the top trip pointer without removing it
	// throws exception if stack is empty
	T* peek() const {
		if (isEmpty())
			throw std::out_of_range("Stack is empty — cannot peek");
		return top->data;
	}
	// isEmpty — returns true if stack has no items
	bool isEmpty() const {
		return top == nullptr;
	}
	// getSize — returns number of items in the stack
	int getSize() const {
		return size;
	}
};

// ===== ASSIGNMENT 11 ADDITION =====
// Queue class implemented using a linked list
// WHY linked list? Because a linked list queue has no size limit
// and enqueue/dequeue are both O(1) with front and back pointers
// Used as a pending trip queue — trips waiting to be reviewed
template <typename T>
class TripQueue {
private:
	// ===== ASSIGNMENT 11 ADDITION =====
	// Node structure for the queue
	struct QueueNode {
		T* data;         // pointer to the trip
		QueueNode* next; // pointer to the next node
		QueueNode(T* d) : data(d), next(nullptr) {}
	};
	QueueNode* front;  // pointer to front of queue — dequeue from here
	QueueNode* back;   // pointer to back of queue — enqueue here
	int size;          // number of items in the queue
public:
	// Constructor — initializes empty queue
	TripQueue() : front(nullptr), back(nullptr), size(0) {}
	// Destructor — cleans up all nodes
	// NOTE: does NOT delete the trip data — Manager owns the trips
	~TripQueue() {
		while (!isEmpty()) {
			QueueNode* temp = front;
			front = front->next;
			delete temp;  // delete node only not the trip data
		}
	}
	// Enqueue — adds a trip pointer to the back of the queue
	void enqueue(T* trip) {
		QueueNode* newNode = new QueueNode(trip);
		if (back == nullptr) {
			// queue is empty — new node is both front and back
			front = newNode;
			back = newNode;
		}
		else {
			// point current back to new node then update back
			back->next = newNode;
			back = newNode;
		}
		size++;
	}
	// Dequeue — removes the front trip pointer from the queue
	// throws exception if queue is empty
	void dequeue() {
		if (isEmpty())
			throw std::out_of_range("Queue is empty — cannot dequeue");
		QueueNode* temp = front;
		front = front->next;
		if (front == nullptr)
			back = nullptr;  // queue is now empty
		delete temp;  // delete node only not the trip data
		size--;
	}
	// Front — returns the front trip pointer without removing it
	// throws exception if queue is empty
	T* getFront() const {
		if (isEmpty())
			throw std::out_of_range("Queue is empty — cannot get front");
		return front->data;
	}
	// isEmpty — returns true if queue has no items
	bool isEmpty() const {
		return front == nullptr;
	}
	// getSize — returns number of items in the queue
	int getSize() const {
		return size;
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

TEST_CASE("Linked List Addition/Deletion & Invalid Index Tests") {
	Manager<Distancetime> manager;

	CHECK(manager.getSize() == 0);
	CHECK(manager.countTripsRecursive() == 0);

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

TEST_CASE("Linked List Recursive Trip Count & Sequential Search Test") {
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

TEST_CASE("Bubble Sort & Binary Search Test") {
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

TEST_CASE("Iterator Test & Different Insertion Tests") {
	LinkedList<Distancetime> list;

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Kensington");

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake St. Clair");

	list.insertFront(b1);
	list.insertBack(b2);

	auto it = list.begin();

	CHECK(it.hasNext());
	CHECK(it.getData()->getLocation() == "Kensington");

	it.next();
	CHECK(it.getData()->getLocation() == "Lake St. Clair");
}

// ===== ASSIGNMENT 11 ADDITION =====
// Stack Tests
TEST_CASE("Stack push and peek") {
	TripStack<Distancetime> stack;

	// test isEmpty on empty stack
	CHECK(stack.isEmpty() == true);
	CHECK(stack.getSize() == 0);

	// create trips — stack does NOT own these so we manage them manually
	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Kensington");
	b1->setTime(60);
	b1->setBirdsseen(10);

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake St. Clair");
	b2->setTime(30);
	b2->setBirdsseen(5);

	// push first trip
	stack.push(b1);
	CHECK(stack.isEmpty() == false);
	CHECK(stack.getSize() == 1);
	CHECK(stack.peek()->getLocation() == "Kensington");

	// push second trip — should now be on top
	stack.push(b2);
	CHECK(stack.getSize() == 2);
	CHECK(stack.peek()->getLocation() == "Lake St. Clair");

	// clean up trips manually since stack does not own them
	delete b1;
	delete b2;
}

TEST_CASE("Stack pop removes top item") {
	TripStack<Distancetime> stack;

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Kensington");

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake St. Clair");

	stack.push(b1);
	stack.push(b2);

	// pop top item — Lake St. Clair should be removed
	stack.pop();
	CHECK(stack.getSize() == 1);
	CHECK(stack.peek()->getLocation() == "Kensington");

	// pop last item
	stack.pop();
	CHECK(stack.isEmpty() == true);

	// clean up
	delete b1;
	delete b2;
}

TEST_CASE("Stack throws on pop from empty stack") {
	TripStack<Distancetime> stack;
	// popping from empty stack should throw
	CHECK_THROWS_AS(stack.pop(), std::out_of_range);
}

TEST_CASE("Stack throws on peek from empty stack") {
	TripStack<Distancetime> stack;
	// peeking empty stack should throw
	CHECK_THROWS_AS(stack.peek(), std::out_of_range);
}

// ===== ASSIGNMENT 11 ADDITION =====
// Queue Tests
TEST_CASE("Queue enqueue and getFront") {
	TripQueue<Distancetime> queue;

	// test isEmpty on empty queue
	CHECK(queue.isEmpty() == true);
	CHECK(queue.getSize() == 0);

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Kensington");
	b1->setTime(60);
	b1->setBirdsseen(10);

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake St. Clair");
	b2->setTime(30);
	b2->setBirdsseen(5);

	// enqueue first trip
	queue.enqueue(b1);
	CHECK(queue.isEmpty() == false);
	CHECK(queue.getSize() == 1);
	CHECK(queue.getFront()->getLocation() == "Kensington");

	// enqueue second trip — front should still be Kensington
	queue.enqueue(b2);
	CHECK(queue.getSize() == 2);
	CHECK(queue.getFront()->getLocation() == "Kensington");

	// clean up
	delete b1;
	delete b2;
}

TEST_CASE("Queue dequeue removes front item") {
	TripQueue<Distancetime> queue;

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("Kensington");

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Lake St. Clair");

	queue.enqueue(b1);
	queue.enqueue(b2);

	// dequeue front item — Kensington should be removed
	queue.dequeue();
	CHECK(queue.getSize() == 1);
	CHECK(queue.getFront()->getLocation() == "Lake St. Clair");

	// dequeue last item
	queue.dequeue();
	CHECK(queue.isEmpty() == true);

	// clean up
	delete b1;
	delete b2;
}

TEST_CASE("Queue throws on dequeue from empty queue") {
	TripQueue<Distancetime> queue;
	// dequeuing from empty queue should throw
	CHECK_THROWS_AS(queue.dequeue(), std::out_of_range);
}

TEST_CASE("Queue throws on getFront from empty queue") {
	TripQueue<Distancetime> queue;
	// getting front of empty queue should throw
	CHECK_THROWS_AS(queue.getFront(), std::out_of_range);
}

TEST_CASE("Queue FIFO order test") {
	TripQueue<Distancetime> queue;

	Birdsseen* b1 = new Birdsseen();
	b1->setLocation("First");

	Birdsseen* b2 = new Birdsseen();
	b2->setLocation("Second");

	Birdsseen* b3 = new Birdsseen();
	b3->setLocation("Third");

	// enqueue three trips
	queue.enqueue(b1);
	queue.enqueue(b2);
	queue.enqueue(b3);

	// dequeue should come out in same order — First In First Out
	CHECK(queue.getFront()->getLocation() == "First");
	queue.dequeue();
	CHECK(queue.getFront()->getLocation() == "Second");
	queue.dequeue();
	CHECK(queue.getFront()->getLocation() == "Third");
	queue.dequeue();
	CHECK(queue.isEmpty() == true);

	// clean up
	delete b1;
	delete b2;
	delete b3;
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

	// ===== ASSIGNMENT 11 ADDITION =====
	// Stack to track trip history — used to undo last added trip
	TripStack<Distancetime> tripHistory;

	// ===== ASSIGNMENT 11 ADDITION =====
	// Queue to hold trips pending review
	TripQueue<Distancetime> pendingReview;

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

			// ===== ASSIGNMENT 11 ADDITION =====
			// push trip pointer onto history stack
			// stack does not own the trip — manager does
			tripHistory.push(trip);

			// ===== ASSIGNMENT 11 ADDITION =====
			// enqueue trip pointer into pending review queue
			// queue does not own the trip — manager does
			pendingReview.enqueue(trip);

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

			// ===== ASSIGNMENT 11 ADDITION =====
			// push trip pointer onto history stack
			tripHistory.push(trip);

			// ===== ASSIGNMENT 11 ADDITION =====
			// enqueue trip pointer into pending review queue
			pendingReview.enqueue(trip);

			printSummary(*trip);
		}

		cout << "Would you like to add another birding trip? (Y/N)" << endl;
		cin >> moreTrips;
		cin.ignore();

	} while (moreTrips == 'y' || moreTrips == 'Y');

	cout << "\nAll Birding Trips:" << endl;
	manager.printTrips();
	cout << "Total trips taken: " << manager.countTripsRecursive() << endl;

	// ===== ASSIGNMENT 11 ADDITION =====
	// Show last added trip using stack peek
	if (!tripHistory.isEmpty()) {
		cout << "\nLast trip added (from stack history):" << endl;
		tripHistory.peek()->Print();
	}

	// ===== ASSIGNMENT 11 ADDITION =====
	// Process pending review queue — show all trips in order they were added
	cout << "\nTrips pending review (queue order):" << endl;
	while (!pendingReview.isEmpty()) {
		pendingReview.getFront()->Print();
		pendingReview.dequeue();
	}

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