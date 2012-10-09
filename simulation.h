#include <stdio.h>
#include <stdlib.h>
struct node{
    int id;
    char name[20];
};
struct path_info{
    long enter_time;
    long exit_time;
    struct node * wait_node;
    struct wait_info * next;
};
struct age_info{
    long idle_start_time;
    long idle_finish_time;
    struct age_info * next;
};
struct edge {
    int id;
    struct age_info * age_history;
    long max_age;
    int status;
    int no_of_cars;
};
struct car 
{
    int current_destination;
    int last_node_visited;
    int velocity;
    int endurance;
    int id;
    int no_of_collisons;
    int start_node;
    int end_node;
    struct path_info * path_history;
};
