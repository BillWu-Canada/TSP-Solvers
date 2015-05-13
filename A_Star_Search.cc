#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


int node_num = 0;


struct c_node {
    string c_name;
    float h;
    float g;
    float f;
    vector<string> neighbor;
    vector<string> path;
    
};


float distance_between(string target_name, string child_name, map<string, float> x_map, map<string, float> y_map);


bool min_first(int i, int j) {return (i<j); }


// edge_calculate is used to calculate all edge in the graph and put them in queue.
void edge_calculate(vector<float> &edge_queue, map<string, float> x_map, map<string, float> y_map) {
    int i = 0;
    int ind = 0;
    
    for (map<string, float>::const_iterator it_x = x_map.begin(); it_x != x_map.end(); it_x++) {
        for (map<string, float>::const_iterator it_y = x_map.begin(); it_y != x_map.end(); it_y++) {
            
            float distance = distance_between(it_x->first, it_y->first, x_map, y_map);
            
            if (ind>=i && distance != 0) {
                edge_queue.push_back(distance);
            }
            ind++;
        }
        i++;
        ind = 0;
    }
    
}



// the heuristic function
float heuristic(struct c_node target, vector<float> edge_queue) {
    int nei_num = (int)target.neighbor.size();
    float return_val = 0;
    
    for (int i=0; i<nei_num; i++) {
        return_val += edge_queue[i];
    }
    
    return return_val;
    
}



// initial neighbor of the start city A
void init_neighbor_of_A(vector<string> &neighbor, map<string, float> x_map) {
    
    for (map<string, float>::const_iterator it = x_map.begin(); it != x_map.end(); it++) {
        if (it->first != "A") {
            //cout << "neighbor is: " << it->first << endl;
            neighbor.push_back(it->first);
        }
    }
}


// testing function, no related to the implementation
void print_vector(vector<string> neighbor) {
    //cout << "size of nei: " << neighbor.size() << endl;
    for (int i=0; i<neighbor.size(); i++) {
        //cout << "nei's name: " << neighbor[i] << endl;
    }
}


// testing function as above
void print_float_vector(vector<float> v) {
    for (int i=0; i<v.size(); i++) {
        cout << v[i] << endl;
    }
}


// print the path length and the optimal path.
void print_result(struct c_node node) {
    cout << endl;
    cout << "THE OPTIMAL PATH LENGTH: " << node.g << endl;
    
    cout << "Therefore, the path is: ";
    for (int i=0; i<(node.path).size(); i++) {
        cout << node.path[i] << " ";
    }
    
    cout << endl;
}



// store the coordinates of cities into the map
void store_map(string line, string city, float x, float y, map<string, float> &x_map, map<string, float> &y_map) {
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
    
    x_map[city] = x;
    y_map[city] = y;
    
}



// testing function in order to print the map to check the correctness
void print_map(map<string, float> x_map, map<string, float> y_map) {
    for (map<string, float>::const_iterator it = x_map.begin(); it != x_map.end(); it++) {
        //cout << "x_map is: " << it->first << " " << it->second << endl;
        
    }
    
    for (map<string, float>::const_iterator it = y_map.begin(); it != y_map.end(); it++) {
        //cout << "y_map is: " << it->first << " " << it->second << endl;
        
    }
    
}




// find the path has minimum f in the search_queue
int min_f(vector<struct c_node> search_queue) {
    int min_pos = 0;
    for (int i=0; i<search_queue.size(); i++) {
        if ( (search_queue.at(i)).f <= (search_queue.at(min_pos)).f ) {
            min_pos = i;
        }
    }
    
    return min_pos;
}



// calculate the distance between two cities
float distance_between(string target_name, string child_name, map<string, float> x_map, map<string, float> y_map){
    
    float parent_city_x = x_map[target_name];
    float parent_city_y = y_map[target_name];
    
    float child_city_x = x_map[child_name];
    float child_city_y = y_map[child_name];
    
    float distance = sqrt(pow( (parent_city_x-child_city_x), 2) + pow( (parent_city_y-child_city_y), 2));
    return distance;
    
}



// calculate the child's neighbor
void child_neighbor(vector<string> parent_nei, struct c_node &child) {
    for (int i=0; i<parent_nei.size(); i++) {
        if (parent_nei[i] != child.c_name) {
            (child.neighbor).push_back(parent_nei[i]);
        }
    }
}



// copy parent city's path to child.
void push_parent_path(struct c_node &child, struct c_node parent) {
    for (int i=0; i<parent.path.size(); i++) {
        child.path.push_back(parent.path[i]);
    }
}



// store city's neighbors to the search_queue
void push_neighbors(vector<struct c_node> &search_queue, struct c_node target_city, map<string, float> x_map, map<string, float> y_map, vector<float> edge_queue) {
    
    
    for (int i=0; i<target_city.neighbor.size(); i++) {
        struct c_node child;
        child.c_name =  (target_city.neighbor).at(i);
        
        child_neighbor(target_city.neighbor, child);
        
        child.h = heuristic(child, edge_queue);
        float distance = distance_between(target_city.c_name, child.c_name, x_map, y_map);
        child.g = target_city.g + distance;
        child.f = child.h + child.g;
        
        
        push_parent_path(child, target_city);
        
        (child.path).push_back(child.c_name);
        
        node_num++;
        
        search_queue.push_back(child);
        
    }
    
}



// pop parent node out of search queue
void pop_node(vector<struct c_node> &search_queue, int pos_to_pop) {
    search_queue.erase(search_queue.begin()+pos_to_pop);
}




// main function of the A* search
void A_star_search(vector<struct c_node> &search_queue, map<string, float> x_map, map<string, float> y_map, vector<float> edge_queue) {
    
    while (true) {
        int pos_to_pop = min_f(search_queue);
        
        if ( search_queue.at(pos_to_pop).neighbor.size() == 0 ) {
            if ( search_queue.at(pos_to_pop).path.back() == "A" ) {
                print_result(search_queue.at(pos_to_pop));
                return;
            }
            else {
                string parent_cname = ((search_queue.at(pos_to_pop)).path).back();
                
                (search_queue.at(pos_to_pop)).g = (search_queue.at(pos_to_pop)).g + distance_between(parent_cname, "A", x_map, y_map);
                (search_queue.at(pos_to_pop)).f = (search_queue.at(pos_to_pop)).g + 0;
                ((search_queue.at(pos_to_pop)).path).push_back("A");
                continue;
            }
        }
        
        push_neighbors(search_queue, search_queue.at(pos_to_pop), x_map, y_map, edge_queue);
        pop_node(search_queue, pos_to_pop);
        
        
        
    }

}



int main() {
    map<string, float> x_map;
    map<string, float> y_map;
    
    int i=0;
    int city_num;
    string line;
       

    
    while(getline(cin,line)) {
        if(i==0) {
            city_num = atoi(line.c_str());
            //cout <<"city_num:" <<city_num << endl;
            i++;
        }
        else {
            
            string city;
            float x;
            float y;
            
            store_map(line, city, x, y, x_map, y_map);
            
        }
    }
    

    vector<struct c_node> search_queue;
    struct c_node A;
    vector<float> edge_queue;
    
    edge_calculate(edge_queue, x_map, y_map);
    
    sort(edge_queue.begin(), edge_queue.end(), min_first);
    

    
    A.c_name = "A";
    A.h = 0;
    A.g = 0;
    A.f = A.h + A.g;
    A.path.push_back("A");
    
    init_neighbor_of_A(A.neighbor, x_map);  
    
    search_queue.push_back(A);
    
    A_star_search(search_queue, x_map, y_map, edge_queue);
    

}






