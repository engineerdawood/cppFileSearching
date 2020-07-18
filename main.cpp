// g++ main.cpp -std=c++0x -lpthread -o main && ./main
// commnd to execute thread in cpp

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<streambuf>
#include<string.h>
#include<typeinfo>
#include<thread>

using namespace std;

time_t startT;
time_t endT;

string token = "This";      // Phrase to find
string fileName = "./file.txt";      // file path to find
int parts = 4;              // number of threads and file parts

void searchInString(string test);
ifstream openFile();
void loadInRam(ifstream & file);
void calculateTime();
void searchSingleProcess(ifstream & file);
void searchFourProcess(ifstream & file);



int main(){

    ifstream file = openFile();

    startT = endT = 0.0;
    time(&startT);
    loadInRam(file);
    time(&endT);
    calculateTime();

    cout << "\nPhrase to Find: " << token << endl;

    startT = endT = 0.0;
    time(&startT);
    searchSingleProcess(file);
    time(&endT);
    calculateTime();

    startT = endT = 0.0;
    time(&startT);
    searchFourProcess(file);
    time(&endT);
    calculateTime();


    file.close();
    return 0;
    
}


void searchFourProcess(ifstream & file){

    file.clear();               // to clear and seek back file to start
    file.seekg(0, ios::beg);

    cout << "\nSearching in Four Processes";
    // divide file into 4 equal parts
    string line = "";
    string fileString = "";

    while(getline (file, line)){
        fileString += line;
    }
    
    int str_size = fileString.length();
   
    while (str_size % parts != 0)   // adding a character to make divisible at equal parts
    {
        fileString = fileString + ' ';
        str_size = fileString.length();
    }
    
    string fileStringParts[parts];  //n parts of String
    int partsCounter = -1;      //counter starts from -1 for first array to be 0
    int part_size = str_size / parts;

    for (int i = 0; i< str_size; i++) {     // dividing main string into n number string
        if (i % part_size == 0)
            partsCounter++;
        fileStringParts[partsCounter] +=  fileString[i];
    }

    fileString.erase();

    thread t1(searchInString, fileStringParts[0]);
    thread t2(searchInString, fileStringParts[1]);
    thread t3(searchInString, fileStringParts[2]);
    thread t4(searchInString, fileStringParts[3]);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    fileStringParts[0].erase();
    fileStringParts[1].erase();
    fileStringParts[2].erase();
    fileStringParts[3].erase();


}

// open the file
ifstream openFile(){
    
    ifstream file(fileName);

    if(!file){
        cerr << "Unable to open file path: " << fileName << endl;
        exit(1);   // call system to stop
    }

    return file;
}

// Load in the Ram, basically we are loading in string that's in the RAM
void loadInRam(ifstream & file){

    file.clear();               // to clear and seek back file to start
    file.seekg(0, ios::beg);

    cout << "\nLoading In Ram";

    string line = "";
    string fileString = "";
    while(getline (file, line)){
        fileString += line;
    }
    
    fileString.erase();
}

// search a phrase in single system process
void searchSingleProcess(ifstream & file){

    file.clear();               // to clear and seek back file to start
    file.seekg(0, ios::beg);

    cout << "\nSearching in Single Process";
    string line = "";
    string fileString = "";
    while(getline (file, line)){
        fileString += line;
    }

    searchInString(fileString);

    fileString.erase();
    
}

//Calculating the Time
void calculateTime(){
    double time_taken = double(endT - startT);
    cout << "\nTime taken: " << fixed 
         << time_taken << setprecision(6); 
    cout << " sec" << endl;
}


//search in the string and return the result
void searchInString(string str){

    int M = token.length(); 
    int N = str.length(); 
    int count = 0; 
    
    for (int i = 0; i <= N - M; i++) {  

        int j; 
        for (j = 0; j < M; j++) 
            if (str[i+j] != token[j]) 
                break;

        if (j == M){ 
           count++; 
           j = 0; 
        } 
    }

    cout << "\nStatus: ";
    if(count == 0)
        cout << "Not found";
    else
        cout << "Found " << count << " times";
}