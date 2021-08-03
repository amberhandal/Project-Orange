#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <map>

using namespace std;
using namespace chrono;

struct flight
{
    //flight object initializer
    string origin, destination, originCity, destinationCity;
    unsigned int passengers, seats, flights, distance, flyDate, originPopulation, destinationPopulation;

    flight(string origin = "", string destination = "", string originCity = "", string destinationCity = "", unsigned int passengers = 0, unsigned int seats = 0, unsigned int flights = 0, unsigned int distance = 0, unsigned int flyDate = 0, unsigned int originPopulation = 0, unsigned int destinationPopulation = 0);
    void printInfo(ostream &outFile);
    void printInfoNeatly(ostream &outFile);
};

//functions implemented
void merge(vector<flight> &flightInfo, int start, int mid, int end, string option);
void mergeSort(vector<flight> &flightInfo, int start, int end, string option);
string neatDate(int date);
vector<flight> CompressList(vector<flight> &list);
void FirstLetterPrint(vector<flight> &list);
void countingSort_S(vector<flight> &fl, int place);
void countingSort(vector<flight> &fl, int place);
void radixSort(vector<flight> &fl, bool Num_Str);

int main()
{
    //reads the file of flight data
    ifstream inFile("flight_edges.tsv");
    string currentLine;

    auto start = steady_clock::now();
    auto end = steady_clock::now();
    duration<double> elapsed_seconds = end - start;

    string origin, destination, originCity, destinationCity;
    unsigned int passengers, seats, flights, distance, flyDate, originPopulation, destinationPopulation;
    string convertTemp;

    // used to organize data by date
    map<unsigned int, vector<flight> > allFlights;
    // used to store data that we want to use, as you may not want to use all 3 million flights
    vector<flight> mostFlights;

    start = steady_clock::now();
    while (getline(inFile, currentLine))
    {
        //parses through the flight data row by row and pushes it into a vector in the map
        istringstream stream(currentLine);
        getline(stream, origin, '\t');
        getline(stream, destination, '\t');
        getline(stream, originCity, '\t');
        getline(stream, destinationCity, '\t');
        getline(stream, convertTemp, '\t');
        passengers = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        seats = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        flights = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        distance = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        flyDate = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        originPopulation = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        destinationPopulation = stoi(convertTemp);

        allFlights[flyDate].push_back(flight(origin, destination, originCity, destinationCity, passengers, seats, flights, distance, flyDate, originPopulation, destinationPopulation));
    }

    inFile.close();
    end = steady_clock::now();
    elapsed_seconds = end - start;

    cout << flush;
    cout << "Took " << elapsed_seconds.count() << " seconds to order data into map." << flush << endl;
    cout << flush;

    start = steady_clock::now();
    ofstream out("flights.txt", ofstream::trunc);

    for (auto i = allFlights.begin(); i != allFlights.end(); i++)
    {
        //this was used to originally sort the data by flyDate and determine which of the ~3,000,000 flights we would use
        for (int j = 0; j < i->second.size(); j++)
        {
            i->second[j].printInfo(out);
        }
    }

    out.close();
    end = steady_clock::now();
    elapsed_seconds = end - start;

    cout << "Took " << elapsed_seconds.count() << " seconds to create file with flights sorted by date." << endl;

    string yearSelection = "0";
    bool tryYear = true;

    while (tryYear)
    {
        //checks for valid number
        tryYear = false;

        cout << "What year would you like the data to include up to? (Min: 1990, Max: 2009)" << endl;

        getline(cin, yearSelection);
        bool isNumber = true;

        for (char a : yearSelection)
        {
            if (!isdigit(a))
                isNumber = false;
        }

        if (!isNumber || stoi(yearSelection) < 1990 || stoi(yearSelection) > 2009)
        {
            cout << "Enter valid year." << endl;
            tryYear = true;
        }
    }

    flyDate = 0;
    inFile.open("flights.txt");
    int k = 0;

    while (getline(inFile, currentLine) && flyDate <= (stoi(yearSelection) + 1) * 100 && k < 2293271)
    {
        k++;
        istringstream stream(currentLine);
        getline(stream, origin, '\t');
        getline(stream, destination, '\t');
        getline(stream, originCity, '\t');
        getline(stream, destinationCity, '\t');
        getline(stream, convertTemp, '\t');
        passengers = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        seats = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        flights = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        distance = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        flyDate = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        originPopulation = stoi(convertTemp);
        getline(stream, convertTemp, '\t');
        destinationPopulation = stoi(convertTemp);

        mostFlights.push_back(flight(origin, destination, originCity, destinationCity, passengers, seats, flights, distance, flyDate, originPopulation, destinationPopulation));
    }

    cout << "Loaded " << mostFlights.size() << " unique flights into the current data!" << endl;
    //2nd list since our sorts directly alter the list
    vector<flight> mostFlights2 = mostFlights; 
    inFile.close();

    string selection;

    while (selection != "5")
    {
        cout << "\n_________________________________________________________________________________________________________" << endl;
        //print out most popular city to travel to based on number of flights to the city
        cout << "\n1. Search for the most popular city" << endl;
        cout << "2. Search for the destination with the most number of flights traveled to in a single month" << endl;
        cout << "3. Change the year the data includes up to" << endl;
        cout << "4. Change the data to a singular year" << endl;
        cout << "5. Quit" << endl;
        getline(cin, selection);

        if (selection == "1")
        {
            start = steady_clock::now();
            mergeSort(mostFlights, 0, mostFlights.size() - 1, selection);
            end = steady_clock::now();
            elapsed_seconds = end - start;

            string tempCity = mostFlights[0].destinationCity;
            int tempMax = 0;
            //this will be the total number of flights
            int currMax = 0;
            //this will be the printed city
            string currCity;

            for (int i = 0; i < mostFlights.size(); i++)
            {
                if (mostFlights[i].destinationCity == tempCity)
                {
                    tempMax += mostFlights[i].flights;
                }
                else
                {
                    if (tempMax > currMax)
                    {
                        currMax = tempMax;
                        currCity = tempCity;
                    }
                    tempCity = mostFlights[i].destinationCity;
                    tempMax = mostFlights[i].flights;
                }
            }
            if (tempMax > currMax)
            {
                //in the case the data only has one destination
                currMax = tempMax;
                currCity = tempCity;
            }
            //comparison here: radixSort
            auto start2 = steady_clock::now();

            //sorts alphabetically
            radixSort(mostFlights2, false);
            //if a cummulative number of passengers is required
            vector<flight> compList = CompressList(mostFlights2);
            //gets alphabetical list and resorts it based on number of passengers by month.
            radixSort(compList, true);

            auto end2 = steady_clock::now();
            duration<double> elapsed_seconds2 = end2 - start2;

            cout << "The most popular city is " << currCity << " with a total of " << currMax << " flights to it!" << endl;

            cout << "\n\nTook " << elapsed_seconds.count() << " seconds to merge sort." << endl;
            cout << "Took " << elapsed_seconds2.count() << " seconds to radix sort." << endl;
        }

        else if (selection == "2")
        {
            start = steady_clock::now();
            mergeSort(mostFlights, 0, mostFlights.size() - 1, selection);

            end = steady_clock::now();
            elapsed_seconds = end - start;

            auto start2 = steady_clock::now();

            //sorts alphabetically
            radixSort(mostFlights2, true);

            auto end2 = steady_clock::now();
            duration<double> elapsed_seconds2 = end2 - start2;

            cout << "\n\nTook " << elapsed_seconds.count() << " seconds to merge sort." << endl;
            cout << "Took " << elapsed_seconds2.count() << " seconds to radix sort." << endl;

            mostFlights[mostFlights.size() - 1].printInfoNeatly(cout);
        }

        else if (selection == "3")
        {
            mostFlights.clear();
            yearSelection = "0";
            tryYear = true;

            while (tryYear)
            {
                //ensures a valid number
                tryYear = false;
                cout << "What year would you like the data to include up to? (Min: 1990, Max: 2009)" << endl;
                getline(cin, yearSelection);
                bool isNumber = true;

                for (char a : yearSelection)
                {
                    if (!isdigit(a))
                        isNumber = false;
                }

                if (!isNumber || stoi(yearSelection) < 1990 || stoi(yearSelection) > 2009)
                {
                    cout << "Enter valid year.\n"
                         << endl;
                    tryYear = true;
                }
            }

            flyDate = 0;
            inFile.open("flights.txt");
            int j = 0;

            while (getline(inFile, currentLine) && flyDate <= (stoi(yearSelection) + 1) * 100 && j < 2293671)
            {
                j++;
                istringstream stream(currentLine);

                getline(stream, origin, '\t');
                getline(stream, destination, '\t');
                getline(stream, originCity, '\t');
                getline(stream, destinationCity, '\t');
                getline(stream, convertTemp, '\t');
                passengers = stoi(convertTemp);
                getline(stream, convertTemp, '\t');
                seats = stoi(convertTemp);
                getline(stream, convertTemp, '\t');
                flights = stoi(convertTemp);
                getline(stream, convertTemp, '\t');
                distance = stoi(convertTemp);
                getline(stream, convertTemp, '\t');
                flyDate = stoi(convertTemp);
                getline(stream, convertTemp, '\t');
                originPopulation = stoi(convertTemp);
                getline(stream, convertTemp, '\t');
                destinationPopulation = stoi(convertTemp);

                mostFlights.push_back(flight(origin, destination, originCity, destinationCity, passengers, seats, flights, distance, flyDate, originPopulation, destinationPopulation));
            }
            cout << "\nLoaded " << mostFlights.size() << " unique flights into the current data!" << endl;
            inFile.close();
        }

        else if (selection == "4")
        {
            mostFlights.clear();
            yearSelection = "0";
            tryYear = true;

            while (tryYear)
            {
                tryYear = false;
                cout << "What year would you like the data to be? (Min: 1990, Max: 2009)" << endl;
                getline(cin, yearSelection);
                bool isNumber = true;

                for (char a : yearSelection)
                {
                    if (!isdigit(a))
                        isNumber = false;
                }

                if (!isNumber || stoi(yearSelection) < 1990 || stoi(yearSelection) > 2009)
                {
                    cout << "Enter valid year.\n"
                         << endl;
                    tryYear = true;
                }
            }

            for (auto i : allFlights)
            {
                if (i.first / 100 == stoi(yearSelection))
                {
                    for (int j = 0; j < i.second.size(); j++)
                    {
                        mostFlights.push_back(i.second[j]);
                    }
                }
            }

            cout << "\nLoaded " << mostFlights.size() << " unique flights into the current data!" << endl;
        }
    }
}

flight::flight(string origin, string destination, string originCity, string destinationCity, unsigned int passengers, unsigned int seats, unsigned int flights, unsigned int distance, unsigned int flyDate, unsigned int originPopulation, unsigned int destinationPopulation)
{
    this->origin = origin, this->destination = destination, this->originCity = originCity, this->destinationCity = destinationCity;
    this->passengers = passengers, this->seats = seats, this->flights = flights, this->distance = distance, this->flyDate = flyDate, this->originPopulation = originPopulation, this->destinationPopulation = destinationPopulation;
}

void flight::printInfo(ostream &outFile)
{
    outFile << origin << "\t" << destination << "\t" << originCity << "\t" << destinationCity << "\t" << passengers << "\t" << seats << "\t" << flights << "\t" << distance << "\t" << flyDate << "\t" << originPopulation << "\t" << destinationPopulation << endl;
}

void flight::printInfoNeatly(ostream &outFile)
{
    string flyDate = neatDate(this->flyDate);
    outFile << "\nOrigin: " << origin << "\nDestination: " << destination << "\nOrigin City: " << originCity << "\nDestination City: " << destinationCity << "\nPassengers: " << passengers << "\nSeats: " << seats << "\nFlights: " << flights << "\nDistance: " << distance << "\nFly Date: " << flyDate << "\nOrigin Population: " << originPopulation << "\nDestination Population: " << destinationPopulation << endl;
}

void merge(vector<flight> &flightInfo, int start, int mid, int end, string option)
{
    //merges the divided vector upon sorting each element in its rightful place
    int i, j, k, maxLeft, maxRight;

    //finds max elements of left and right sides
    maxLeft = mid - start + 1, maxRight = end - mid;
    //left and right subvectors
    vector<flight> left, right;

    for (i = 0; i < maxLeft; i++)
    {
        left.push_back(flightInfo[start + i]);
    }

    for (j = 0; j < maxRight; j++)
    {
        right.push_back(flightInfo[mid + 1 + j]);
    }

    i = 0, j = 0, k = start;
    bool varToCompare;

    while (i < maxLeft && j < maxRight)
    {
      //compares elements
        if (option == "1")
        {
            varToCompare = left[i].destinationCity <= right[j].destinationCity;
        }
        else if (option == "2")
        {
            varToCompare = left[i].passengers <= right[j].passengers;
        }
        if (varToCompare)
        {
            flightInfo[k] = left[i];
            i++;
            k++;
        }
        else
        {
            flightInfo[k] = right[j];
            j++;
            k++;
        }
    }

    //swaps elements
    while (i < maxLeft)
    {
        flightInfo[k] = left[i];
        i++;
        k++;
    }

    while (j < maxRight)
    {
        flightInfo[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(vector<flight> &flightInfo, int start, int end, string option)
{
    //recursively divides into two until vector is split into n elements
    int mid = 0;
    if (start < end)
    {
        mid = start + (end - start) / 2;

        mergeSort(flightInfo, start, mid, option);

        mergeSort(flightInfo, mid + 1, end, option);

        merge(flightInfo, start, mid, end, option);
    }
}

string neatDate(int date)
{
    //lists the date of the flights in a more readable format
    string answer;
    string dateString = to_string(date);
    string temp = dateString.substr(dateString.size() - 2);
    if (temp == "01")
        answer = "January, ";
    else if (temp == "02")
        answer = "February, ";
    else if (temp == "03")
        answer = "March, ";
    else if (temp == "04")
        answer = "April, ";
    else if (temp == "05")
        answer = "May, ";
    else if (temp == "06")
        answer = "June, ";
    else if (temp == "07")
        answer = "July, ";
    else if (temp == "08")
        answer = "August, ";
    else if (temp == "09")
        answer = "September, ";
    else if (temp == "10")
        answer = "October, ";
    else if (temp == "11")
        answer = "November, ";
    else if (temp == "12")
        answer = "December, ";
    answer.append(dateString.substr(0, 4));

    return answer;
}

vector<flight> CompressList(vector<flight> &list)
{
    //puts all unique flights with its summed up number of flights and passengers into a vector
    vector<flight> sortedList;
    string tempExit = list.at(0).destination;
    unsigned int tempFlights = 0;
    unsigned int tempPass = 0;
    for (int i = 0; i < list.size(); i++)
    {
        if (tempExit.compare(list.at(i).destination) != 0)
        {
            sortedList.push_back(flight("", list.at(i - 1).destination, "", list.at(i - 1).destinationCity, tempPass, 0, tempFlights, 0, 0, 0, list.at(i - 1).destinationPopulation));
            tempExit = list.at(i).destination;
            tempFlights = list.at(i).flights;
            tempPass = list.at(i).passengers;
        }
        else
        {
            tempFlights += list.at(i).flights;
            tempPass += list.at(i).passengers;
        }
    }
    return sortedList;
}

int findMaxNum(vector<int> &list)
{
    //determines the number of iterations and significant digits the radix sort will do
    int max = list.at(0);
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i) > max)
        {
            max = list.at(i);
        }
    }
    return max;
}

int findMaxIntLength(vector<flight> &list)
{
    int max = list.at(0).passengers;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).passengers > max)
        {
            max = list.at(i).passengers;
        }
    }
    return max;
}

void countingSort_S(vector<flight> &fl, int place)
{
    //sorts lexographically with numbers in the mix 0-9 with 0 having priority and 9 being weakest
    //known a-z is 26 | 0-9 = 10 == 36
    int const max = 36;
    int count[max] = {0};

    for (int i = 0; i < max; i++)
    {
        count[i] = 0;
    }

    vector<flight> out(fl.size());

    for (int i = 0; i < fl.size(); i++)
    {
        if (isdigit(fl.at(i).destination[place]))
        {
            count[fl.at(i).destination[place] - 22]++;
        }
        else
        {
            //increments count of char at index
            count[fl.at(i).destination[place] - 'A']++;
        } 
    }     

    for (int i = 1; i < max; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = fl.size() - 1; i >= 0; i--)
    { 
        //to go foward through the original list, increment number of count since a number already exists for where we put it
        if (isdigit(fl.at(i).destination[place]))
        {
            out[count[fl.at(i).destination[place] - 22] - 1] = fl.at(i);
            count[fl.at(i).destination[place] - 22]--;
        }
        else
        {
            out[count[fl.at(i).destination[place] - 'A'] - 1] = fl.at(i); 
            //decreases count so [#, 1] next 1 would be at #
            count[fl.at(i).destination[place] - 'A']--;                   
        }                                                                
    }

    for (int i = 0; i < fl.size(); i++)
    {
        fl.at(i) = out.at(i);
    }
}


void countingSort(vector<flight> &fl, int place)
{ 
    //This sort looks is number based so it sorts on a specific number
    //0-9 = 10
    int const max = 10; 
    int count[max] = {0};
    for (int i = 0; i < max; i++)
    { 
        //just in case it is not 0
        count[i] = 0;
    }

    vector<flight> out(fl.size());

    for (int i = 0; i < fl.size(); i++)
    {
        //increments count of char at index
        count[(fl.at(i).passengers / place) % 10]++; 
    }

    for (int i = 1; i < max; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = fl.size() - 1; i >= 0; i--)
    { 
        out[count[(fl.at(i).passengers / place) % 10] - 1] = fl.at(i);
        count[(fl.at(i).passengers / place) % 10]--;
    }

    for (int i = 0; i < fl.size(); i++)
    {
        fl.at(i) = out.at(i);
    }
}

void radixSort(vector<flight> &fl, bool Num_Str)
{
    //find max length of string (already known 3)
    int max = findMaxIntLength(fl);
    if (Num_Str)
    {
        //if it's a number-based sort then use max as reference
        for (int place = 1; (max / place) > 0; place *= 10)
        {   
            //sort based on significant digit                         
            countingSort(fl, place); 
        }
    }
    else
    {
        //if it's a string look from right to left of the string index (significant digit)
        for (int i = 2; i >= 0; i--)
        {                     
            //then go down to the left and continue sort lexographically.     
            countingSort_S(fl, i); 
        }
    }
}