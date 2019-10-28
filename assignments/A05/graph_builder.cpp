///////////////////////////////////////////////////////////////////////////////
//
// Date:             28 October 2019
// Title:            Program 3 - Graphs
// Semester:         Spring 2018
// Course:           CMPS 3013 
// Assignment:       A05
// Author:           Sean Aleman
// Email:            seantaleman@gmail.com
// Files:            graph_builder.cpp, cities,json, Geo.hpp, heap.hpp,
// json.hpp, jsonfacade.hpp
// Description:
// This program makes an array out of the cities json file,
// then it loops through the array in a double for loop so
// that it can calculate distances between cities. It then 
// makes a min heap of each city so that it can 
// arrange the cities in order of which city is closest to the
// current city. Finally the program prints 10 cities and shows //the 10 closest cities to it, along with the distance. 
//
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string.h>
#include "heap.hpp"
#include "jsonfacade.hpp"
#include "json.hpp"
#include "Geo.hpp"

using namespace std;

using json = nlohmann::json;

struct City{
    double Priority, latitude, longitude;
    string Name;

    City(){
        Priority = 0;
        Name = "";
        latitude = 0;
        longitude = 0;
    }
    City(json obj){
        Name = obj["city"];
        Priority = obj["population"];
        latitude = obj["latitude"];
        longitude = obj["longitude"];
    }
};

int main(){
    ofstream outfile;  //opensoutput file named output.txt
    outfile.open("output.txt");
    json obj;
    City** Cities;
    City* temp;
    string filename = "cities.json";
    JsonFacade J(filename);
 int usernumber = 0;
 int enterednumber = 10;
    Heap <City> H(10001,false);

    int size = J.getSize();
    Cities = new City *[size];
    // loads an array of cities with the json data
    for (int i = 0; i < size; i++) 
    {
        obj = J.getNext();
        Cities[i] = new City(obj);
       // C =  Cities[i]->Pop;
        
    }
outfile << "Sean Aleman" << endl; 
// create a heap that holds cities with priority = distance (smaller 
//distance = higher priority)
// loop through cities
// for each city, find closest cities by calculating distance and 
//pushing onto a heap
// get city1 lat and lon for distance calculation
// write out city1's name to output
  for( int i = 0 ; i < 1000 ; i ++ )
  {
   Coordinate tempcoord1 = Coordinate(Cities[i]->latitude, Cities[i]->longitude);
        if (i < 10 || i > size - 11)
        {
            outfile << i +1 << ": " << Cities[i]->Name << endl;
        }
         // get city2 lat and lon for distance calculation
         // calculate distance from city1 to city2 and
    for( int j = 0 ; j < 1000; j++ )
    {
    Coordinate tempcoord2 = Coordinate(Cities[j]->latitude, Cities[j]->longitude);
    Cities[j]->Priority= HaversineDistance(tempcoord1, tempcoord2);
    H.Insert(Cities[j]);
    }

 // loops through the 1000 and only prints city and priority
 // of 10 cities
    while( usernumber < 1000)
    {
      temp = H.Extract();
        if (i < 10 || i >  size-11)
        {
                if (usernumber <=10)
                {
                    //so we wont print same city
                    if (temp->Name != Cities[i]->Name)
                        outfile << "\t" <<usernumber << ")  " << temp->Name << " " << temp->Priority << endl;
                }
        }
        usernumber++;
        temp = NULL;
    }
 usernumber = 0;
  }
    outfile.close();
}
