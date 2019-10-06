//https://stackoverflow.com/questions/55116344/how-to-setup-vs-code-for-c-14-c-17
//https://github.com/nlohmann/json

#include "json_helper.cpp"
#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;

/**
 * Temporary priority function that turns a string into an
 * integer by adding together the ascii value of all of its
 * characters. So, longest "name" will have highest priority.
 */
int nameToNumber(string name) {
	int total = 0;
	for (int i = 0; i < name.length(); i++) {
		total += (int)name[i];

	}
	return total;
}
int nameToNumber1(string name)
{
	int total = (int)name.length();
	return total;
}
const static double EarthRadiusKm = 6372.8;

inline double DegreeToRadian(double angle) {
	return M_PI * angle / 180.0;
}

class Coordinate {
public:
	Coordinate(double latitude, double longitude) : myLatitude(latitude), myLongitude(longitude) {}

	double Latitude() const {
		return myLatitude;
	}

	double Longitude() const {
		return myLongitude;
	}

private:
	double myLatitude;
	double myLongitude;
};

double HaversineDistance(const Coordinate &p1, const Coordinate &p2) {

	double latRad1 = DegreeToRadian(p1.Latitude());
	double latRad2 = DegreeToRadian(p2.Latitude());
	double lonRad1 = DegreeToRadian(p1.Longitude());
	double lonRad2 = DegreeToRadian(p2.Longitude());

	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;

	double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
	return 2 * EarthRadiusKm * computation;

}

struct Animal {
	string animal_name;
	long date;
	double latitude;
	double longitude;
	float priority;
	bool validated;
	string version;

	Animal() {
		animal_name = "";
		date = 0;
		latitude = 0.0;
		longitude = 0.0;
		priority = 0.0;
		validated = 0;
		version = "";
	}

	Animal(string name, long _date, double lat, double lon, float pri, bool val, string ver) {
		animal_name = name;
		date = _date;
		latitude = lat;
		longitude = lon;
		priority = pri;
		validated = val;
		version = ver;
	}

	Animal(json j) {
		animal_name = j["animal_name"];
		date = j["date"];
		latitude = j["latitude"];
		longitude = j["longitude"];
		priority = j["priority"];
		validated = j["validated"];
		version = j["version"];
	}
};

class AnimalHelper {
private:
	Animal **Animals;
	JsonHelper *J;
	json obj;

public:

	int size;

	AnimalHelper(string filename) {
		J = new JsonHelper(filename);
		size = J->getSize();
		Animals = new Animal *[size];
		for (int i = 0; i < size; i++) {
			obj = J->getNext();
			Animals[i] = new Animal(obj);
		}
	}

	void PrintAnimals() {

		for (int i = 0; i < size; i++) {

			cout << Animals[i]->animal_name << " "
				<< Animals[i]->latitude << " "
				<< Animals[i]->longitude << " "
				<< Animals[i]->date << " "
				<< Animals[i]->priority << " "
				<< Animals[i]->validated << " "
				<< Animals[i]->version << endl;

		}
	}


	Animal**& ReturnAnimals() {

		return Animals;
	}
};


class Heap {
private:

	Animal **H;       // Pointer to allocate dynamic array
	int Next;     // Next available location
	int MaxSize;  // Max size since were using array
	int HeapSize; // Actual number of items in the array.

	/**
	 * Function IncreaseKey:
	 *      Bubbles element up from given index.
	 *
	 * Params:
	 *     [int] index  - index of item to be increased
	 * Returns
	 *      void
	 */
	void BubbleUp(int i) {
		int p = Parent(i);
		while (p > 0 && H[i] > H[p]) {
			Swap(i, p);
			i = p;
			p = Parent(i);
		}
	}

	/**
	 * Function DecreaseKey:
	 *      Bubbles element down from given index.
	 *
	 * Params:
	 *      [int] index - index of item to be decreased
	 * Returns
	 *      void
	 */
	void BubbleDown(int i) {
		int c = PickChild(i);

		while (c > 0) {
			if (H[i] < H[c]) {
				Swap(i, c);
				i = c;
				c = PickChild(i);
			}
			else {
				c = -1;
			}
		}
	}

	/**
	 * Function Swap:
	 *      Swaps two elements in an array
	 *
	 * Params:
	 *      [int] index1 - index of item to swap with
	 *      [int] index2 - same
	 * Returns
	 *      void
	 */
	void Swap(int p, int i) {
		Animal *temp = H[p];
		H[p] = H[i];
		H[i] = temp;
	}

	/**
	 * Function Parent:
	 *      Returns parent index of a given index
	 *
	 * Params:
	 *      [int]index - index to calculate parent from
	 * Returns
	 *      index [int]
	 */
    int Parent(int i) {
        return int(i / 2);
    }

	/**
	 * Function LeftChild:
	 *      Returns left index of a given index
	 *
	 * Params:
	 *      [int] index - index to calculate child from
	 * Returns
	 *      [int] index - left child index
	 */
	int LeftChild(int i) {
		return i * 2;
	}

	/**
	 * Function RightChild:
	 *      Returns right index of a given index
	 *
	 * Params:
	 *      [int] index - index to calculate child from
	 * Returns
	 *      [int] index - right child index
	 */
	int RightChild(int i) {
		return i * 2 + 1;
	}

	/**
	 * Function PickChild:
	 *      Return index of child to swap with or -1 to not swap.
	 *
	 * Params:
	 *      [int] index - index of parent element
	 * Returns
	 *      [int] index - index to swap with or -1 to not swap
	 */


	int PickChild(int i) {
		if (RightChild(i) >= Next) {    //No right child
			if (LeftChild(i) >= Next) { //No left child
				return -1;
			}
			else { //you have a left no right
				return LeftChild(i);
			}
		}
		else {
			//right child exists
			if (H[RightChild(i)]->priority > H[LeftChild(i)]->priority) {
				return RightChild(i);
			}
			else {
				return LeftChild(i);
			}
		}
	}

public:
	/**
	 * Function Heap:
	 *      Constructor that allocates memory for array and
	 *      inits vars.
	 *
	 * Params:
	 *      void
	 * Returns
	 *      void
	 */
	Heap(int size) {
		H = new Animal*[size];
		Next = 1;
		MaxSize = size;
		HeapSize = 0;
	}
	Heap() {
		H = new Animal*[0];
		Next = 1;
		MaxSize = 0;
		HeapSize = 0;
	}

	/**
	 * Function Insert:
	 *      Insert value into heap.
	 *

	 * Params:
	 *      [int] x - value to be inserted
	 * Returns
	 *      void
	 */
	void Insert(Animal *x) {
		//H = new Animal*[1];
		 H[Next] = x;
		BubbleUp(Next);
		Next++;
		HeapSize++;
	}

	/**
	 * Function Extract:
	 *      Removes top element from heap (whether min or max).
	 *
	 * Params:
	 *      void
	 * Returns
	 *      [int] top_value - top value in the heap (min or max)
	 */
	Animal Extract() {

		//if (Empty()) {
		//	return -1;
		//}

		Animal retval = *H[1];
		H[1] = H[--Next];
		HeapSize--;

		if (HeapSize > 1) {
			BubbleDown(1);
		}

		return retval;
	}

	/**
	 * Function PrintHeap:
	 *      Prints the values currently in the heap array
	 *      based on array location, not heap order
	 *
	 * Params:
	 *      void
	 * Returns
	 *      void
	 */
	void PrintHeap() {
		for (int i = 0; i < HeapSize; i++) {
			cout << *(&H[i]->animal_name) << endl;
			cout << *(&H[i]->priority) << endl << endl;
		}
		cout << endl;
	}

	/**
	 * Function Size:
	 *      Returns the number of items in the heap
	 *
	 * Params:
	 *      void
	 * Returns
	 *      [int] heapSize - size of heap
	 */
	int Size() {
		return Next - 1;
	}

	/**
	 * Function Empty:
	 *      Returns boolean true of array is empty
	 *
	 * Params:
	 *      void
	 * Returns
	 *      [bool] empty - is array empty
	 */
	bool Empty() {
		return Next == 1;
	}

	/**
	 * Function Heapify:
	 *      Creates a heap out of a given array of nums in no specific order.
	 *
	 * Params:
	 *      [int*] array - array of values to heapify
	 *      [int] size - size of array
	 * Returns
	 *      void
	 */
	void Heapify(Animal **&A, int size) {
		int i = size / 2;
		H = A;
		HeapSize = size - 1;

		for (int j = i; j >= 1; j--) {
			BubbleDown(j);
			Next = Next++;
		}
	}
};



int main(int argc, char **argv) {
	AnimalHelper AH("animals.json");
	JsonHelper JH("animals.json");


	//H = new Animal*[1];

	int a = AH.size;
	Heap H(AH.size);
	cout << "Enter Number of Buckets"; 

	int NumberOfBuckets = 0;
	cin >> NumberOfBuckets;

	Heap** Buckets = new Heap*[NumberOfBuckets];

	for (int i = 0; i < NumberOfBuckets; i++) {
		Buckets[i] = new Heap;
	}


	Animal** A = AH.ReturnAnimals();

	for (int i = 0; i < AH.size; i++) {
		double d = 0; 
		double total = 0;
		cout << A[i]->animal_name << endl;
		cout << A[i]->priority << endl;
		//cout << nameToNumber(A[i]->animal_name) << endl;
		//A[i]->priority = nameToNumber(A[i]->animal_name);

		Coordinate a1(A[i]->latitude, (A[i]->longitude));
		bool v = A[i]->validated;
		total = (((6372.8 - d)*(A[i]->priority) )/ nameToNumber1(A[i]->animal_name));
			if (v == 1)
			total = -total;
			A[i]->priority = total;





			int bucket = abs(A[i]->date) % NumberOfBuckets;
			Buckets[bucket]->Insert(A[i]);

		//double d = HaversineDistance(a1, a1);
		//cout << d; 

		//cout << A[i]->animal_name << endl;
		//cout << nameToNumber(A[i]->animal_name) << endl;
		//cout << A[i]->priority << endl;
	}
	cout << endl << endl << endl << endl;
	for (int i = 0; i < AH.size; i++) {
		cout << A[i]->animal_name << endl;
		//cout << nameToNumber(A[i]->animal_name) << endl;
		cout << A[i]->priority << endl;
	}

	//H.PrintHeap();
	H.Heapify(A, AH.size);
	cout << endl << endl << endl << endl << endl;
	H.PrintHeap();
	//cout << &H[1];
	//H.PrintHeap();
	//cout << a;
	
	for (int i = 0; i < NumberOfBuckets; i++) {
		Buckets[i]->Extract();

		cout << A[i]->priority << endl;
		Buckets[i]->Extract();
		Buckets[i]->Extract();
		Buckets[i]->Extract();
		Buckets[i]->Extract();

	}
	
	system("pause");
	//AH.PrintAnimals();
	/*
	 for (int i = 0, i<AH.size, i++ ){

	}
	The priority will be calculated by doing the following :

	L = length of the animal_name
		D = distance(latitude, longitude) is from(33.9137, -98.4934)
		A = adjustor
		V = validated
		Bucket = abs(date) % number_of_buckets

		Priority = (6372.8 - D) * A / L
		*/
	return 0;
}
