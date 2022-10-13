//HOMEWORK 1
//"TRAVELING SALESMAN" PROBLEM
//MECE 4510
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
//fstream file;
//ofstream file; //for writing to files (I think...?)

// INITIALIZE DATA ARRAY and TEMP ARRAY
const int m = 1000; // NUMBER OF COLUMNS OF TXT DATA FILE (1000)
const int r = 4; // NUMBER OF ROWS OF TXT DATA FILE
const int tot_pop = 15; // 100 --> can be changed --> 10
const int pop = 15000; // m * population number = 100,000 --> 10,000
string data[m][r];
string temp_data[m][r];
double dist[m][r];
double env[pop][r];
double pdist[tot_pop];
const string tsp = "tsp.txt";
//const string tsp = "evolution_23.txt";

// INITIALIZE TIME ARRAY
const int iteration_count = 400; //HOW MANY ITERATIONS IN SEARCH FUNCTION
const int step_threshold = 20; //HOW MANY TIMES PER IMPROVEMENT WILL DATA BE RECORDED
string time_array[iteration_count][r];

// INITIALIZE TARGETS
const double target_threshold = 20.0; // STOPPING CONDITION FOR SHORTEST LENGTH

// -------------
// BASIC FUNCTIONS
// -------------
// Including: rand_int, get_digits, get_string_digits, create_file, euc_dist, get_dist, total_dist
// -------------

// GET RANDOM NUMBER -- INCOMPLETE
int rand_int(int n) // int n = 0 to (n-1)
{
    //srand(time(0));
    
    /*
    time_t nTime;
    srand((unsigned) time(&nTime));
    */

    //Create array
    //int r_array[100]; 
    /*
   for(int i = 0; i < 100; i++)
   {
        srand(time(0));
        r_array[i] = rand() % n;
   }
    */
    //int c = rand() % 100;
    //int random = r_array[c];
 
   /*
   srand(time(0));
   int random;
   std::vector<int> v(n);
   std::random_device rd;
   std::iota(v.begin(), v.end(), 1);
   std::ranges::shuffle(v,rd);
    */
   
   int random;
   random = rand() % n; // int n = 0 to (n-1)

    return(random);
}

// GET NUMBER OF DIGITS IN AN INTEGER
int get_digits(int x)
{
    int length = 1;
    while ( x /= 10 )
    {
        length++;
    }
    //cout << "\n\n" << "Length of int: " << length << "\n\n";
    return length;
}
// GET NUMBER OF CHARACTERS IN A STRING
int get_string_digits(string x)
{
        int i = 0;
        while (x[i])
        {
            i++;
        }
        return i;
}

//CREATE NEW BLANK FILE (inputs: base name of file, directory, type, and file number (ie file_23))
string create_file(string sub_file_name,string file_name,string file_type,int file_number,string data1[m][r])
{
    //string sub_file_name = "test_data";
    //string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    //string file_type = ".csv";
    int name_length = get_string_digits(sub_file_name); //Finds length of subfile
    int dir_length = get_string_digits(file_name); //Finds length of directory
    file_name.replace(dir_length, name_length,sub_file_name); //idk
    //cout << file_name; //TEST OUTPUT
    //int b = 100;
    dir_length = get_string_digits(file_name); //Find directory length
    file_name.replace(dir_length,get_digits(file_number),to_string(file_number)); // Add file number to directory
    file_name = file_name + file_type; // Add file type to directory name
    //cout << file_name; //TEST OUTPUT
    ofstream file;
    //CREATES FILE
    file.open(file_name, ios::out); 
    for (int i = 0; i < m; i++)
    {
        file<<data1[i][0]<<", "<<data1[i][1]<<", "<<data1[i][2]<<'\n';
    }
    file.close();

    cout << endl << "<<< NEW FILE PRINTED >>>" << endl;

    return file_name;

    // NOTE!!!!!
    // file_name is the directory location
    // temp_name is the updated file name (base name + number + file type)
}

string create_time_file(string sub_file_name,string file_name,string file_type,int file_number,string time_array[iteration_count][r])
{
    //string sub_file_name = "test_data";
    //string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    //string file_type = ".csv";
    int name_length = get_string_digits(sub_file_name); //Finds length of subfile
    int dir_length = get_string_digits(file_name); //Finds length of directory
    file_name.replace(dir_length, name_length,sub_file_name); //idk
    //cout << file_name; //TEST OUTPUT
    //int b = 100;
    dir_length = get_string_digits(file_name); //Find directory length
    file_name.replace(dir_length,get_digits(file_number),to_string(file_number)); // Add file number to directory
    file_name = file_name + file_type; // Add file type to directory name
    //cout << file_name; //TEST OUTPUT
    ofstream file;
    //CREATES FILE
    file.open(file_name, ios::out); 
    for (int i = 0; i < iteration_count; i++)
    {
        file<<time_array[i][0]<<", "<<time_array[i][1]<<", "<<time_array[i][2]<<'\n';
    }
    file.close();

    cout << endl << "<<< NEW FILE PRINTED >>>" << endl;

    return file_name;

    // NOTE!!!!!
    // file_name is the directory location
    // temp_name is the updated file name (base name + number + file type)
}

//UNUSED WRITE FILE
void write_file(string data[m][r],double dist[m][r],string temp_data[m][r],string file_name,string temp_name, string dir)
{
    /*
    const int size = 5;
  double x[] = {1,2,3,4,5}; 

  ofstream file;
  file.open(file_name);
    file << "This is a line.\n";
    file << "This is another line.\n";
    for(int count = 0; count < size; count ++)
    {
        file << x[count] << " " ;
    file.close();
    }
    */

    // "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    std::string path = dir+file_name;
        for (const auto & entry : fs::directory_iterator(path))
        {
            std::cout << entry.path() << std::endl;

        }   

}

//CALCULATES EUCLIDIAN DISTANCE BETWEEN 2 POINTS
double euc_dist(double x1, double y1, double x2, double y2)
{
    double d = 0;
    d = sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
    return d;  
}

// TOTAL DISTANCE BETWEEN POINTS
double total_dist(string data1[m][r])
{
    double d = 0;
    for (int i = 0; i < m; i++)
    {
        d = d + std::stod(data1[i][2]);
        //cout << endl << data[i][2]<<endl;
    }
    return d;
}

// GET DISTANCE BETWEEN ALL POINTS IN ARRAY & ADD IT TO 3RD COLUMN OF DATA ARRAY
// Returns sum of all distances
double get_dist(string data1[m][r])
{ 
    double dist_sum;
    /*expected val = 0.016281
    cout << endl << " " << endl << std::stof(data[5][1])*std::stof(data[4][1]); 
    cout << dist[0][0];
   */
    /*
    string sub_file_name = "random_data";
    string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    string file_type = ".txt";
    */
   
   for (int i = 0; i < m; i++)
   {
            double x1 = std::stod(data1[i][0]);
            double y1 = std::stod(data1[i][1]);
            //cout << data[i][j] << " ";
            if (i != m-1)
            {
                double x2 = std::stod(data1[i+1][0]);
                double y2 = std::stod(data1[i+1][1]);
                data1[i][2] = to_string(euc_dist(x1,y1,x2,y2));
                //(((x2-x2)*(x2-x2))+((y2-y1)*(y2-y1)))^0.5;
            }
            else
            {
                double x2 = std::stod(data1[0][0]);
                double y2 = std::stod(data1[0][1]);
                data1[i][2] = to_string(euc_dist(x1,y1,x2,y2));
            }
            
       // cout << data1[i][2] << endl;
   }

    dist_sum = total_dist(data1);

    return dist_sum;
}

//FINDS EUC DIST IN DATA BETWEEN two index points
double single_dist(string data1[m][r],int i,int j)
{
    double x1;
    double x2;
    double y1;
    double y2;
    double d;

            x1 = std::stod(data1[i][0]);
            y1 = std::stod(data1[i][1]);
            //cout << data[i][j] << " ";
            x2 = std::stod(data1[j][0]);
            y2 = std::stod(data1[j][1]);
            d = euc_dist(x1,y1,x2,y2);

    return d;

}


//SWAPS TWO LINES IN AN ARRAY
void swap_val(string data1[m][r],int i1,int i2)
{
    string temp_x1 = "";
    string temp_y1 = "";
    string temp_x2 = "";
    string temp_y2 = "";

    temp_x1 = data1[i1][0];
    temp_y1 = data1[i1][1];
    temp_x2 = data1[i2][0];
    temp_y2 = data1[i2][1];

    data1[i1][0] = temp_x2;
    data1[i1][1] = temp_y2;
    data1[i2][0] = temp_x1;
    data1[i2][1] = temp_y1;

}

// COPYS DATA2 TO DATA1
void copy_to_array(string data1[m][r],string data2[m][r])
{
    for(int i=0;i<m;i++)
    {
        //data2 imprints onto data1 
        //data2 = data array
        //data1 = temp_data array
        data1[i][0] = data2[i][0];
        data1[i][1] = data2[i][1];
        data1[i][2] = data2[i][2];
        data1[i][3] = data2[i][3];
    }
}

// -------------
// BIG FUNCTIONS
// -------------
// Including: Rand search, hill climber, genetic algorithm
// -------------

//HILL CLIMBER (NOT RAND SEARCH!!!!)
double rand_search(string data[m][r],string temp_data[m][r],string time_array[iteration_count][r],double target_distance,int step_threshold,int iteration_count)
{   
    clock_t time_req2;
    time_req2 = clock();

    //Initialize time array
    float time_rec; //variable that time is copied to
    //string time_array[iteration_count][3];
    for(int i = 0; i < iteration_count; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            time_array[i][j] = "0";
        }
    }


    //rand_search file directory
    string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";

    int c = 0; //Counting variable
    int r; //For random variable
    double d0; //Distance value for comparison
    double dt; //difference between d0 and d

    get_dist(data); //Update distances for array
    double d = total_dist(data); 
    
    //while(d>target_distance)
    for(int q = 0; q < iteration_count; q++)
    {
        d0 = total_dist(data);
        d = d0;
        copy_to_array(temp_data,data);
        for(int i = 0; i < m; i++)
        {
            r = rand_int(m);
            swap_val(temp_data,i,r);
            get_dist(temp_data);
            d0 = total_dist(temp_data);
            if (d0<d) //CHANGE THIS < TO CONVERT TO LONGEST PATH!!!!!!!!!!!!
            {
                copy_to_array(data,temp_data);
            }
            else
            {
                copy_to_array(temp_data,data);
            }
            dt = abs(d0-d);
        }
        get_dist(data);
        d = total_dist(data);
        copy_to_array(temp_data,data);

        clock_t time_temp;
        time_temp = clock() - time_req2;
        cout << "\n\n" << " Iteration "<< c << " Time = " << (float)time_temp/CLOCKS_PER_SEC << " seconds" ;
        cout << "\n Iteration " << c << " Distance = " << d << endl;

        //TIME ARRAY CALCULATIONS
        time_rec = (float)time_temp/CLOCKS_PER_SEC;
        time_array[c][0] = to_string(c);
        time_array[c][1] = to_string(time_rec);
        time_array[c][2] = to_string(d);

        c++; //COUNT EVALUATION

        //if (dt > step_threshold)
        if (c%step_threshold == 0)
        {
            create_file("rand_data_plot",file_name,".txt",c,data);
            
        }

    }

    d = total_dist(data);

    cout << "\n Final Length: " << d << endl;

    time_req2 = clock() - time_req2;
    cout << "\n\n" << "Random Search Runtime = " << (float)time_req2/CLOCKS_PER_SEC << " seconds" ;

    create_time_file("rand_time_plot",file_name,".txt",1,time_array);

    //CREATE TIME ARRAY FILE
    //create_file("rand_data",file_name,".txt",1,data); 
    return d;
}

//RAND SEARCH (NOT HILL CLIMBER!!!!!)
double hill_climber(string data[m][r],string temp_data[m][r],string time_array[iteration_count][r],double target_distance,int step_threshold,int iteration_count)
{  

    clock_t time_req2;
    time_req2 = clock();
    float time_rec;

    string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    double dist_initial;
    double dist_final = 500.0;
    int r;
    
    int c = 0;
    //int i; // TAKE THIS OUT IF FOR LOOP!!!!!!!!!!!!!!!
    //while (dist_final > 75)
    for(int i=0;i<iteration_count;i++) 
    {
        copy_to_array(temp_data,data); // temp_data = data
        get_dist(temp_data);
        dist_initial = total_dist(temp_data);
        //copy_to_array(temp_data,data); //temp_data = data
        for(int j=0;j<m;j++)    
        {
            r = rand_int(m);
            swap_val(temp_data,j,r);
        }
        get_dist(temp_data);
        dist_final = total_dist(temp_data);
        if (dist_initial > dist_final)  // make > for decrease
        {
            copy_to_array(data,temp_data);
        
        //i++; // TAKE THIS OUT IF FOR LOOP!!!!!!!!


        //TIME ARRAY CALCULATIONS
        clock_t time_temp;
        time_temp = clock() - time_req2;
        time_array[c][0] = to_string(c);
        time_array[c][1] = to_string((float)time_temp/CLOCKS_PER_SEC);
        time_array[c][2] = to_string(dist_initial);

        cout << "\n\n" << " Iteration "<< c << " Time = " << (float)time_temp/CLOCKS_PER_SEC << " seconds" ;
        cout << endl << dist_initial <<endl;
        
        if (c%step_threshold == 0)
        {
            create_file("rand_data_plot",file_name,".txt",c,data);
            
        }
        c++;
        }
    }

    get_dist(data);
    dist_final = total_dist(data);

    cout << "\n Final Length: " << dist_final << endl;

    time_req2 = clock() - time_req2;
    cout << "\n\n" << "Random Search Runtime = " << (float)time_req2/CLOCKS_PER_SEC << " seconds" ;

    create_time_file("rand_time_plot",file_name,".txt",1,time_array);

    return dist_final;
}

//EVOLUTIONARY ALGORITHM
double evolution(double pdist[tot_pop],double env[pop][r],string data[m][r],string temp_data[m][r],string time_array[iteration_count][r],int step_threshold,int iteration_count)
{

    // SECTION 0: TIME INITIALIZATION AND FILE WORK
    clock_t time_req2;
    time_req2 = clock();
    float time_rec;
    string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    int c = 0;

    //SECTION A: INITIALIZE VARS AND ARRAYS
    //INITIALIZE ENV ARRAY, WHICH STORES TOTAL GENETIC POPULATION
    double d; // for total dist of highest 
    int r; //for random int generation
    int r1;
    int r2;

    double temp_x1; //SECTION B variables
    double temp_y1;
    double temp_x2;
    double temp_y2;

    int index; //indexes
    int index2;
    int index3;
    int rindex;
    int n; //free variable

    

    for(int i=0;i<tot_pop;i++) //Fill dist with zeros
    {
        pdist[i] = 0;
    } 

    


    for(int i=0;i<tot_pop;i++) //Fill each pop member j+(m*i) with corresponding data value
    {
        for(int j=0;j<m;j++)
        {
            index = j+(i*m); // j+(i*m) --> (i*m) is scaling factor to reach different population members
            env[index][0] = std::stod(data[j][0]);
            env[index][1] = std::stod(data[j][1]);
            //cout << endl << " j: " << j << " i: " << i << " Index: " << index << endl; //test code
        }
    }

    


    //SECTION B: RANDOMIZE POPULATION
    // Initialize random population
    for(int i=0;i<tot_pop;i++) 
    {
         for(int j=0;j<m;j++)    
        {
            // Generates random number
            r = rand_int(m);
            index = j+(i*m); //index
            rindex = r+(i*m); // index for random

            //grabs values
            temp_x1 = env[index][0];
            temp_y1 = env[index][1];
            temp_x2 = env[rindex][0];
            temp_y2 = env[rindex][1];

            //swaps values
            env[index][0] = temp_x2;
            env[index][1] = temp_y2;
            env[rindex][0] = temp_x1;
            env[rindex][1] = temp_y1;
        }
    }

    



//Note: begin grand genetic loop here:

    for (int w=0;w<iteration_count;w++) //evolve for (iteration_count) number of iterations
    {
        //SECTION C: EVAL DIST
        // Evaluate distance of each population member and store into dist array
        for(int s=0;s<tot_pop;s++) //evaluate distance
        {
            pdist[s]=0; // reset distance array
            for (int f = 0; f < m; f++)
            {
                index = f+(s*m);
                double x1 = env[index][0]; // find first data point
                double y1 = env[index][1];
                if ((f) != (m-1))
                {
                    double x2 = env[index+1][0]; //compare to next data point in array
                    double y2 = env[index+1][1];
                    pdist[s] = pdist[s] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                }
                else
                {
                    double x2 = env[(s*m)][0]; // if data 1 is at end of array, 
                    double y2 = env[(s*m)][1]; // it will loop back to the begining
                    pdist[s] = pdist [s] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                }
            }
        }










        //SECTION D: BUBBLE SORT
        for(int i=0;i<tot_pop-1;i++) //swap values from smallest to largest in env
        {                        // pop-1 because sorting algorithm stops before end
            for(int j=0;j<tot_pop-1;j++)
            {
                //SECTION C: RE-EVAL DIST
                // Evaluate distance of each population member and store into dist array
                for(int s=0;s<tot_pop;s++) //evaluate distance
                {
                    pdist[s]=0; // reset distance array
                    for (int f = 0; f < m; f++)
                    {
                        index = f+(s*m);
                        double x1 = env[index][0]; // find first data point
                        double y1 = env[index][1];
                        if ((f) != (m-1))
                        {
                            double x2 = env[index+1][0]; //compare to next data point in array
                            double y2 = env[index+1][1];
                            pdist[s] = pdist[s] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                        }
                        else
                        {
                            double x2 = env[(s*m)][0]; // if data 1 is at end of array, 
                            double y2 = env[(s*m)][1]; // it will loop back to the begining
                            pdist[s] = pdist [s] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                        }
                    }
                }

                // CHANGE THIS FOR SHORTEST/LONGEST DISTANCE!!! <------------------
                if ((pdist[j])>(pdist[j+1])) // SORTING (> means sorting for shortest length)
                // ^^^^^^^^^^^^^^^^^^^^^
                //      ^^^^^^^^^^^^^
                {
                    for (int k = 0; k < m; k++)
                    {
                        index = k+(j*m);
                        index2 = k+((j+1)*m);
                        temp_x1 = env[index][0]; 
                        temp_y1 = env[index][1];
                        temp_x2 = env[index2][0]; 
                        temp_y2 = env[index2][1];

                        env[index][0] = temp_x2;
                        env[index][1] = temp_y2;
                        env[index2][0] = temp_x1;
                        env[index2][1] = temp_y1;


                        

                    }
                }
            }
        }

        


        //SECTION F: RANDOM MUTATION 
        //PLAY AROUND WITH THESE VARIABLES
        const int mutation_number = 10; //number of mutations that will occur in entire population
        for(int i=0;i<mutation_number+1;i++)
        {
            //choose which member of the population will have mutation
            index = rand_int(tot_pop)*m; //index constant for particular individual in pop
            r1 = rand_int(m)+index; //index of mutation gene 1
            r2 = rand_int(m)+index; //index of mutation gene 2 (both normalized to location in env)

            //SWAP RANDOM POINTS IN AN INDIVIDUAL IN POPULATION
            temp_x1 = env[r1][0]; 
            temp_y1 = env[r1][1];
            temp_x2 = env[r2][0]; 
            temp_y2 = env[r2][1];

            env[r1][0] = temp_x2;
            env[r1][1] = temp_y2;
            env[r2][0] = temp_x1;
            env[r2][1] = temp_y1;

        }



        // SECTION E: KILL/MATE/SHUFFLEREPLACE POPULATION
        // KILL HALF OF POPULATION (tot_pop/2) --> begining index = (tot_pop/2)-1 
        // tot_pop/2 <---- you can potentially change /2 to something else
        int p = 2; //fraction constant of population that will be eliminated
        //int p = rand_int(5);


        //RANDOM SHUFFLE --> ALLOWS FOR 'BAD' GENES TO PROPEGATE
        r1 = rand_int(tot_pop);
        r2 = rand_int(tot_pop);
        for(int k=0;k<m;k++)
        {
            temp_x1 = env[k+(m*r1)][0]; 
            temp_y1 = env[k+(m*r1)][1];
            temp_x2 = env[k+(m*r2)][0]; 
            temp_y2 = env[k+(m*r2)][1];

            env[k+(m*r1)][0] = temp_x2;
            env[k+(m*r1)][1] = temp_y2;
            env[k+(m*r2)][0] = temp_x1;
            env[k+(m*r2)][1] = temp_y1;
        }

        for(int i=((tot_pop/p));i<tot_pop;i++) //check this <------------
        //for(int j=0;j<((tot_pop/p));j++)
        {
            //I TOOK SECOND LOOP OUT BC IDK WHAT IT DOES...!!!
            //for(int j=0;j<((tot_pop/p));j++) //check this <------------
            //for(int i=((tot_pop/p));i<tot_pop;i++)
            //{
                //generate indexes 
                //of father, mother, and child
                //index constants = pop # * m --> index + i = normalized index for part of array
                r1 = rand_int(tot_pop/p); //random parent selection from top 50% of parents
                index = r1*m; // index constant of parent 1 / father
                if (r1 == ((tot_pop/p)-1)) //ensures index doesnt go out of bounds
                {
                    index2 = (r1-1)*m; //index constant of parent 2 / mother
                }
                else
                {
                    index2 = (r1+1)*m; //index constant of parent 2 / mother
                }
                index3 = i*m; //index constant index of child

                //COPY index and index2 to temp arrays
                //in order for genetic shuffle to not lose information...
                //...the parent's genetic information must be shuffled to...
                //...ensure information isn't lost
                double father[m][2];
                double mother[m][2];
                for (int k = 0; k < m; k++) 
                {
                    father[k][0] = env[index+k][0];
                    father[k][1] = env[index+k][1];
                    mother[k][0] = env[index2+k][0];
                    mother[k][1] = env[index2+k][1];
                }

                r1 = rand_int(m/4); //breakpoint 1 --> where first split of genes begins
                r2 = rand_int(m/4) + r1; //breakpoint 2
                for (int k = 0; k < r1; k++) //PARENT 1 GENES
                {
                    env[index3+k][0] = father[k][0];
                    env[index3+k][1] = father[k][1];

                    for (int l = 0; l < m; l++) 
                    {
                        if (father[k][0] == mother[l][0])
                        {
                            if (father[k][1] == mother[l][1])
                            {
                                //SWAP VARIABLE IN OTHER PARENT ARRAY
                                temp_x1 = mother[l][0]; 
                                temp_y1 = mother[l][1];
                                temp_x2 = mother[k][0]; 
                                temp_y2 = mother[k][1];

                                mother[l][0] = temp_x2;
                                mother[l][1] = temp_y2;
                                mother[k][0] = temp_x1;
                                mother[k][1] = temp_y1;
                            }
                        }
                    }

                }
                for (int k = r1; k < r2; k++) //PARENT 2 GENES
                {
                    env[index3+k][0] = mother[k][0];
                    env[index3+k][1] = mother[k][1];

                    for (int l = 0; l < m; l++) 
                        {
                            if (mother[k][0] == father[l][0])
                            {
                                if (mother[k][1] == father[l][1])
                                {
                                    //SWAP VARIABLE IN OTHER PARENT ARRAY
                                    temp_x1 = father[l][0]; 
                                    temp_y1 = father[l][1];
                                    temp_x2 = father[k][0]; 
                                    temp_y2 = father[k][1];

                                    father[l][0] = temp_x2;
                                    father[l][1] = temp_y2;
                                    father[k][0] = temp_x1;
                                    father[k][1] = temp_y1;
                                }
                            }
                        }
                }
                for (int k = r2; k < m; k++) //PARENT 1 GENES
                {
                    env[index3+k][0] = father[k][0];
                    env[index3+k][1] = father[k][1];

                    for (int l = 0; l < m; l++) 
                    {
                        if (father[k][0] == mother[l][0])
                        {
                            if (father[k][1] == mother[l][1])
                            {
                                //SWAP VARIABLE IN OTHER PARENT ARRAY
                                temp_x1 = mother[l][0]; 
                                temp_y1 = mother[l][1];
                                temp_x2 = mother[k][0]; 
                                temp_y2 = mother[k][1];

                                mother[l][0] = temp_x2;
                                mother[l][1] = temp_y2;
                                mother[k][0] = temp_x1;
                                mother[k][1] = temp_y1;
                            }
                        }
                    }

                }

           // }
        }

            





        


            




        //n++;
        n = 1;
        if (n == 1)
        {
        //SECTION F: DELIBERATE MUTATION 
        //PLAY AROUND WITH THESE VARIABLES
        int del_mutation_number = 1000; //number of mutations that will occur in entire population
        //del_mutation_number = del_mutation_number + 250;
        for(int i=0;i<del_mutation_number+1;i++)
        {
            //choose which member of the population will have mutation
            int r0 = rand_int(tot_pop);
            index = r0*m; //index constant for particular individual in pop
            r1 = rand_int(m)+index; //index of mutation gene 1
            r2 = rand_int(m)+index; //index of mutation gene 2 (both normalized to location in env)

            // EVALUATE DISTANCE OF INDIVIDUAL r0 before mutation
            pdist[r0]=0; // reset distance array
            for (int f = 0; f < m; f++)
            {
                index = f+(r0*m);
                double x1 = env[index][0]; // find first data point
                double y1 = env[index][1];
                if ((f) != (m-1))
                {
                    double x2 = env[index+1][0]; //compare to next data point in array
                    double y2 = env[index+1][1];
                    pdist[r0] = pdist[r0] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                }
                else
                {
                    double x2 = env[(r0*m)][0]; // if data 1 is at end of array, 
                    double y2 = env[(r0*m)][1]; // it will loop back to the begining
                    pdist[r0] = pdist [r0] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                }
            }
            double d_initial = pdist[r0];


            //SWAP RANDOM POINTS IN AN INDIVIDUAL IN POPULATION
            temp_x1 = env[r1][0]; 
            temp_y1 = env[r1][1];
            temp_x2 = env[r2][0]; 
            temp_y2 = env[r2][1];

            env[r1][0] = temp_x2;
            env[r1][1] = temp_y2;
            env[r2][0] = temp_x1;
            env[r2][1] = temp_y1;

            
            // evaluate distance after mutation
            pdist[r0]=0; // reset distance array
            for (int f = 0; f < m; f++)
            {
                index = f+(r0*m);
                double x1 = env[index][0]; // find first data point
                double y1 = env[index][1];
                if ((f) != (m-1))
                {
                    double x2 = env[index+1][0]; //compare to next data point in array
                    double y2 = env[index+1][1];
                    pdist[r0] = pdist[r0] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                }
                else
                {
                    double x2 = env[(r0*m)][0]; // if data 1 is at end of array, 
                    double y2 = env[(r0*m)][1]; // it will loop back to the begining
                    pdist[r0] = pdist [r0] + euc_dist(x1,y1,x2,y2); //store total distance into dist array
                }
            }
            double d_final = pdist[r0];

            //swap mutation back if it doesn't improve individual
            if (d_final > d_initial) // swap mutation back  <<------- > for shortest path, < for longest path
            //^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^
            {
                temp_x1 = env[r1][0]; 
                temp_y1 = env[r1][1];
                temp_x2 = env[r2][0]; 
                temp_y2 = env[r2][1];

                env[r1][0] = temp_x2;
                env[r1][1] = temp_y2;
                env[r2][0] = temp_x1;
                env[r2][1] = temp_y1;
            }
            

        }
        }

            //DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //NUMBER 4
            /*
            for (int i=0;i<m;i++)
            {
                temp_data[i][0] = to_string(env[i+((tot_pop-1)*m)][0]);
                temp_data[i][1] = to_string(env[i+((tot_pop-1)*m)][1]);
                temp_data[i][2] = to_string(env[i+((tot_pop-1)*m)][2]);
                temp_data[i][3] = to_string(env[i+((tot_pop-1)*m)][3]);
            }
            create_file("evolution_debug_",file_name,".txt",c,temp_data);
            c++;
            */
            //DEBUG ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^




    //TIME ARRAY CALCULATIONS
            clock_t time_temp;
            time_temp = clock() - time_req2;
            time_array[c][0] = to_string(c);
            time_array[c][1] = to_string((float)time_temp/CLOCKS_PER_SEC);
            time_array[c][2] = to_string(pdist[0]);

        cout << "Iteration #: " << c << " Path Length: " << pdist[0] << " Time: " << (float)time_temp/CLOCKS_PER_SEC << " seconds" << "\n\n";
        
        for (int b=0;b<m;b++)
        {
            data[b][0] = to_string(env[b][0]);
            data[b][1] = to_string(env[b][1]);
            data[b][2] = to_string(env[b][2]);
        }

        if (c%step_threshold == 0)
        {
            create_file("evolution_",file_name,".txt",c/step_threshold,data);   
        }
        c++;


        

    }
    
    create_time_file("evolution_time_",file_name,".txt",c,time_array);
    create_file("evolution_points_",file_name,".txt",c,data);
    //d++; // DELETE AFTER DONE
    //cout << endl <<d;
    return pdist[0];

}





















// MAIN PROGRAM ***************************************************************************
// ****************************************************************************************
int main()
{

srand(time(0));
// -----------------
// INITIALIZING ZONE
// -----------------

    //TOTAL PROGRAM TIME MEASUREMENT -- STARTS HERE
    clock_t time_req;
    time_req = clock();

    // INITIALIZE DATA ARRAY
    //const int m = 10; // NUMBER OF COLUMNS OF TXT DATA FILE
    //const int r = 2; // NUMBER OF ROWS OF TXT DATA FILE
    string data[m][r];

    //OPEN DATA FILE
    ifstream myFile;
    myFile.open(tsp); //OPENS VARIABLE tsp WHICH IS TRAVELING SALESMAN DATA 

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
        data[c][0]=decimal_1;

        string decimal_2;
        getline(iss, decimal_2, '\n');
        data[c][1]=decimal_2;

        //PRINT ALL VALUES OF data
        //cout << decimal_1 << " and " << decimal_2 << endl;
        c++;
    }
    myFile.close();
    cout << endl;
    
    // COMPUTE INITIAL DISTANCE DATA AND ADD IT TO data[][] ARRAY
    get_dist(data); //TEST (WORKS AND CONFIRMED BY CALCULATOR)
    //cout << data[3][2];
    double d;
    d = total_dist(data);

// ------------
// TESTING ZONE
// ------------


    /*
    for (int i=0;i<10;i++)
    {
        for (int j=0;j<2;j++)
        {
            //cout << "(" << i << "," << j << ")" << " ";
            //cout << data[i][j];
        }
    }
    */
   
   // test to see if array is actually reading correct value

   /*
   bool i=true;
   while(i == true)
   {
    double a = 0;
    a = std::stof(data[5][0])*std::stof(data[4][0]);
    cout << data[5][0] << endl;
    cout << a;
    cout<<endl<<endl<<endl;
    cout << m * r; // testing to see if global variables work
    i=false;
   }
   */



    //WRITE FILE CODE DRAFT 1 -- WORKING 
    /*
    string sub_file_name = "sample";
    string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    string file_type = ".csv";
    int name_length = get_string_digits(sub_file_name);
    int dir_length = get_string_digits(file_name);
    file_name.replace(dir_length, name_length,sub_file_name); // 120 = length to filename
    cout << file_name;
    int b = 100;
    dir_length = get_string_digits(file_name);
    file_name.replace(dir_length,get_digits(b),to_string(b)); // 120 = length to filename
    file_name = file_name + file_type;
    cout << file_name;
    file.open(file_name, ios::out);
    */

    //create_file() TEST 1 --  WORKING
    /*
    string sub_file_name = "doobis_data";
    string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    string file_type = ".csv";
    int file_number = 34567;
    create_file(sub_file_name,file_name,file_type,file_number);
*/

    //create file(test 2) to return string -- WORKING --THIS ONE!!!!!!!!!!!
    
    /*
    string sub_file_name = "doobis_data";
    string file_name = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510\\RAND SEARCH\\";
    string file_type = ".txt";
    int file_number = 123456;
    */

    //(TEST FOR MULTIPLE FILES) - THIS ONEEE!!!!!!!!
    /*
    for (int i=0;i<3;i++)
    {
        string doobis = create_file(sub_file_name,file_name,file_type,file_number);
        file_number++;
    }
    */
    //string doobis = create_file(sub_file_name,file_name,file_type,file_number,data);
    //cout << "\n \n here ya go: \n" << doobis << endl; //TEST OUTPUT
    

    /* THIS ONE!!!!
    cout << data[3][3];
    double d;
    d = total_dist(data);
    cout << "\n\n\n\n" << "total distance is: " << d << "\n\n\n";
    */


    // NOTE!!!!!
    // file_name is the directory location
    // temp_name is the updated file name (base name + number + file type)

    //Write file function experiment
    /*
    
    string temp_name = sub_file_name + to_string(file_number) + file_type;
    string dir = "C:\\Users\\johns\\Desktop\\ACADEMIC NOTES\\EVOLUTIONARY COMPUTATION\\C++ Code\\HW1_MECE4510";
    string foldr = "\\RAND SEARCH";
    string updated_dir = dir+foldr;
    write_file(data, dist, temp_data,file_name,temp_name,updated_dir);
    */

// RAND NUMBER EXPERIMENT TEST
/*
for (int i = 0; i < 10; i++)
{
   c = rand_int(1000);
    cout << "\n\n\n da numbah: " << c << "\n";
}
*/

/*
cout << "\n\n" << std::stod(data[5][0])<< "\n\n" ;
cout << "\n\n" << std::stod(data[5][1])*2.0<< "\n\n" ;
*/

copy_to_array(temp_data,data);
int search_type;
cout << "Random Search (1), Hill Climber (2), or Evolutionary Search (3)? ";
cin >> search_type;


if (search_type == 1)
{
    rand_search(data,temp_data,time_array,target_threshold,step_threshold,iteration_count);
}
if (search_type == 2)
{
    hill_climber(data,temp_data,time_array,target_threshold,step_threshold,iteration_count);
}
if (search_type == 3)
{
    //evolution(data,temp_data,time_array,target_threshold,10.0,iteration_count);
    evolution(pdist,env,data,temp_data,time_array,step_threshold,iteration_count);
}


// --------------
// END OF PROGRAM 
// --------------

    //TOTAL PROGRAM TIME MEASUREMENT -- ENDS HERE
    time_req = clock() - time_req;
    cout << "\n\n\n\n" << "Program Total Runtime: " << (float)time_req/CLOCKS_PER_SEC << " seconds" ;

    //rand_int(m);

   return 0;
}