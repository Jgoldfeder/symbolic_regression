//HOMEWORK 2
//SYMBOLIC REGRESSION PROBLEM
//MECE 4510
//STARTED: 10/11/2022
//John Schlaerth

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <filesystem>
#include <ctime> // for clock
#include <time.h> // for rand
#include <stdlib.h> // for rand
#include <algorithm>
#include <ranges>
#include <random>
#include <bits/stdc++.h> //Swap command?????
using namespace std;
namespace fs = std::filesystem;

const int m = 1000;
const int r = 2;
string data[m][r];
string file_name = "data2022_Gold.txt";

const int fun_len = 100;
const int pop_num = 1000;
const int mp = (pop_num * fun_len);
const int rp = 1;
string ind[fun_len][rp]; // Array for individuals 
string pop[mp][rp];


void read_file(string file_name, string array[m][r])
{
    //OPEN DATA FILE
    ifstream myFile;
    myFile.open(file_name); //OPENS VARIABLE tsp WHICH IS TRAVELING SALESMAN DATA 

    //INITIALIZE COUNT VARIABLE c
    int c = 0; 
    string var;
    // READ IN DATA FROM TXT FILE INTO ARRAY called data[][]
    while(getline(myFile, var))  
    //for (int i=0;i<10;i++)
    {
        istringstream iss(var);
        string decimal_1;
        getline(iss, decimal_1, ',');
        array[c][0]=decimal_1;

        string decimal_2;
        getline(iss, decimal_2, '\n');
        array[c][1]=decimal_2;

        //PRINT ALL VALUES OF data
        cout << decimal_1 << " and " << decimal_2 << " c = " << c << endl;
        c++;
    }
    myFile.close();
    cout << endl;
}

double interpret(string data[m][r],string ind[fun_len][rp],string pop[mp][rp])
{
    for (int i=0;i<fun_len;i++)
    {
        cout << "AAAAAAA";
    }
}


int main()
{

// Initialize Arrays & Constants
string data[m][r];

read_file(file_name, data);

cout << "test";


return 0;

}