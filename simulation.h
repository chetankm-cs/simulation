#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    int id;             // id of a vertex of grap
}NODE;

// To hold the path history of a car (path + wait time)
typedef struct path_info{
    long enter_time;    // time of entering a node      
    long exit_time;     // time of leaving a node
    struct node * path_node;
    struct path_info * next;
}PATH_INFO;

// To hold the age history info of a edge (idle time )
typedef struct age_info{
    long idle_start_time;  // starting of an idle period
    long idle_finish_time; // finishing of an idle period
    struct age_info * next;// history of the last idle period
} AGE_INFO;

// To hold the information of an edge 
typedef struct edge {
    int id;
    int u,v;
    int distance;
    struct age_info * age_history;  // idle history of path
    long max_age;                   // maximum idle time
    int status;                     // busy or idle
    int no_of_cars;                 // no of cars currently traversing the edge
}EDGE;

// TO hold the information of a car 
typedef struct car 
{
    char name[10];
    int current_destination;    
    int last_node_visited;
    int velocity;
    int endurance;
    int id;
    int no_of_collisons;
    int start_node;
    int end_node;
    struct path_info * path_history;
}CAR;

typedef struct adjacency_list
{
    int next_node;
    int edge_id;
    int weight;
    struct adjacency_list * next;    
}ADJACENCY_LIST;

typedef struct graph
{
    int no_of_vertex;
    int no_of_edges;
    struct adjacency_list ** graph_data;

}GRAPH;

typedef struct cars
{
    int no_of_cars;
    CAR *car_data;
}CARS;
// function declarations
void init();
int read_graph(GRAPH *g,char * file_path);
