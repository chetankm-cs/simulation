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
    int path_node;
    struct path_info * next;
}PATH_INFO;

// To hold the age history info of a edge (idle time )
typedef struct age_info{
    long idle_start_time;  // starting of an idle period
    long idle_finish_time; // finishing of an idle period
    struct age_info * next;// history of the last idle period
} AGE_INFO;

typedef struct cars_on_edge
{
    int car_id;
    struct cars_on_edge * next;
}CARS_ON_EDGE;

// To hold the information of an edge 
typedef struct edge {
    int id;
    int u,v;
    int distance;
    struct age_info * age_history;  // idle history of path
    long max_age;                   // maximum idle time
    int status;                     // busy or idle
    int no_of_cars;                 // no of cars currently traversing the edge
    int type;                       //type=1 means unidirectional edge and type=2 means bidirectional edge .
    int last_car_exit_time;
    CARS_ON_EDGE *car_list;
}EDGE;

// TO hold the information of a car 
typedef struct car 
{
    char name[10];
    int current_destination;    
    int last_node_visited;
    int current_edge;
    int velocity;
    int endurance;
    int id;
    int no_of_collisons;
    int start_node;
    int end_node;
    int done;
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

typedef struct edges
{
    int no_of_edges;
    EDGE * edge_data;
}EDGES;

typedef struct collision_data
{
    int car1_id;
    int car2_id;
    int edge_id;
    long time ;
    struct collision_data * next;
}COLLISION_DATA;
// function declarations
void make_decision(GRAPH *g,EDGES *e,CAR *c,int *next_node_id,int *next_edge_id);
