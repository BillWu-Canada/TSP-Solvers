#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cmath>
#include <time.h>


using namespace std;


int swap_num = 0;

// testing function
void print_vector(vector<string> v) {
    //cout << "size of nei: " << neighbor.size() << endl;
    cout << "the path is: " ;
    for (int i=0; i<v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}



// store coordinates in map
void store_map(string line, string &city, double x, double y, map< string, vector<double> > &xy_map) {
    
    char *token;
    char *myline = const_cast<char*>(line.c_str());
    token = strtok(myline, " ");
    
    for (int i=0; token != NULL; i++) {
        ////cout << token << endl;
        if (i==0) { city = token;}
        else if (i==1) { x = atof(token);}
        else if (i==2) { y = atof(token);}
        else {cout << "can't be this." << endl;}
        token = strtok(NULL, " ");
    }
    
    
    xy_map[city].push_back(x);
    xy_map[city].push_back(y);
    
}


// testing function in order to print the content of map
void print_map(map< string, vector<double> > &xy_map) {
    
    for (map< string, vector<double> >::const_iterator it = xy_map.begin(); it != xy_map.end(); it++) {
        cout << "city name: " << it->first << " " << "x: " << (it->second)[0] << " " << "y: " << (it->second)[1] << endl;
    }
    
}


// calculate distance between two cities
double distance_between(string from, string to, map< string, vector<double> > xy_map) {
    double from_x = xy_map[from][0];
    double from_y = xy_map[from][1];
    
    double to_x = xy_map[to][0];
    double to_y = xy_map[to][1];
    
    double distance = sqrt(pow( (from_x - to_x), 2) + pow( (from_y - to_y), 2));
    
    return distance;
    
}



// calculate the current travel plan's total path length
double path_length(vector<string> path_queue, map< string, vector<double> > xy_map) {
    
    double distance = 0;
    
    for (int i=0; i<path_queue.size()-1; i++) {
        distance += distance_between(path_queue[i], path_queue[i+1], xy_map);
    }
    
    return distance;
}



// calculate the probability for a bad move
string SA_value(double T, double old_dis, double new_dis) {
    
    double x = -(new_dis-old_dis) / T;
    double SA = exp(x);
    SA = SA * 100;
    
    
    int ind = rand() % 101;
    
    
    if (ind <= SA) {
        return "jump_to_new";
    }
    else {
        return "stay_same";
    }
    
}



// swap two cities and compare new path and old path
void swap_and_compare(vector<string> &path_queue, int first, int second, map< string, vector<double> > xy_map, double T) {
    
    double old_dis = path_length(path_queue, xy_map);
    
    string first_city = path_queue[first];
    string second_city = path_queue[second];
    
    path_queue[first] = second_city;
    path_queue[second] = first_city;
    swap_num++;

    double new_dis = path_length(path_queue, xy_map);
    
    if (new_dis <= old_dis) {
        // new distance is better, move to new solution (done at previous part).
    }
    
    else {
        string result = SA_value(T, old_dis, new_dis);
        if (result == "jump_to_new") {
            // already jump to the new path
        }
        else if (result == "stay_same") {
            // use old dis
            path_queue[first] = first_city;
            path_queue[second] = second_city;
        }
        else {
            cout << "this line should not be appear !!!" << endl;
        }
    }
}



// main function of the local search
void local_search(vector<string> &path_queue, map< string, vector<double> > xy_map, int city_num) {
    int upper = (int) path_queue.size()-2;
    int first = 0;
    int second = 0;
    double decrease_rate = 0.002;
   
    for ( double T=200000; T>1; T=T*(1-decrease_rate) ) {
        
        first = (rand() % upper) + 1;
        second = (rand() % upper) + 1;
        
        swap_and_compare(path_queue, first, second, xy_map, T);
        
    }
    
}






int main() {
    map< string, vector<double> > xy_map;
    
    int i=0;
    int city_num;
    string line;
    
    vector<string> path_queue;
    
    
    while(getline(cin,line)) {
        if(i==0) {
            city_num = atoi(line.c_str());
            //cout <<"city_num:" <<city_num << endl;
            i++;
        }
        else {
            
            string city;
            double x;
            double y;
            
            store_map(line, city, x, y, xy_map);
            path_queue.push_back(city);
            
        }
    }
    path_queue.push_back("A");
    
    srand (time(NULL));  // initial random seed  

    
    // base case handler
    int path_size = (int)path_queue.size();
    if (path_size==1) {
        cout << "it is impossible that the path size is 1 !!!" << endl;
    }
    else if (path_size==2) {
        // only one city
        cout << "the optimal length of path is: " << 0 << endl;
        cout << "the path is: " << "A" << endl;
    }
    
    else {
      local_search(path_queue, xy_map, city_num);
    
      double my_path_length;
      my_path_length = path_length(path_queue, xy_map);
      cout << "the optimal length of path is: " << my_path_length << endl;
    
      print_vector(path_queue);


      //cout << "swap_num is: " << swap_num << endl;
    }
    
    
    
}



