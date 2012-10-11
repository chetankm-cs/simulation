#include "simulation.h"
/***********************************************************
 * function to read the graph info from the file 
 * *********************************************************/

float time_step;
COLLISION_DATA *collisons=NULL;
int insert(ADJACENCY_LIST **list, int vertex_id,int next_vertex,int weight)
{
    ADJACENCY_LIST *temp;
    temp = (ADJACENCY_LIST *) malloc (sizeof(ADJACENCY_LIST));
    temp->next_node = next_vertex;
    temp->weight = weight;
    temp->edge_id = -1;
    if(list[vertex_id] == NULL)
        temp->next = NULL;
    else
        temp->next = list[vertex_id];

    list[vertex_id]  = temp;
}
int read_graph(GRAPH *g,char * file_path)
{
    FILE *fp;

    ADJACENCY_LIST **list;
    int no_of_vertex,i,weight,vertex_id,next_vertex;
    fp=fopen(file_path,"r");
    if(fp==NULL)
    {
        printf("\nERROR: File not found");
        exit(0);
    }
    fscanf(fp,"%d",&no_of_vertex);
    list =(ADJACENCY_LIST **) malloc(sizeof(ADJACENCY_LIST *)*no_of_vertex);
   
    for (i=0;i<no_of_vertex;i++)
        list[i] = NULL;

    for(i=0;i<no_of_vertex;i++){ 
        fscanf(fp,"%d",&vertex_id);

        while(1)
        {
            fscanf(fp,"%d",&next_vertex);
            if(next_vertex == -1)break;

            fscanf(fp,"%d",&weight);

            insert(list,vertex_id,next_vertex,weight);
        }
   } 
    g->no_of_vertex = no_of_vertex;
    g->graph_data = list;
    fclose(fp);
}
void print_graph(GRAPH *g)
{
    int i;
    ADJACENCY_LIST * temp;
    for(i=0;i<g->no_of_vertex;++i)
    {
        printf("\nVertex %d",i);
        temp =g->graph_data[i];
        while(temp != NULL)
        {
            printf(" %d %d",temp->next_node,temp->weight);
            temp=temp->next;
        }
    }
}

int read_cars(CARS *C,char *filepath)
{
    FILE *fp;
    int no_of_cars,i;
    CAR *temp;
    fp =fopen (filepath,"r");
    if(fp==NULL)
    {
        printf("\nERROR : Cannot open cars file "); exit(0);
    }
    fscanf(fp,"%d",&no_of_cars);
    printf (" no of cars %d",no_of_cars);
    temp = (CAR * )malloc(sizeof(CAR)* no_of_cars);
    for (i=0;i<no_of_cars;++i)
    {
        fscanf(fp,"%s",temp[i].name);
        temp[i].velocity =0;
        fscanf(fp,"%d",&temp[i].start_node);
        fscanf(fp,"%d",&temp[i].end_node);
        fscanf(fp,"%d",&temp[i].endurance);
        fscanf(fp,"%d",&temp[i].velocity);
        temp[i].id = i;
        temp[i].last_node_visited=-1;
        temp[i].current_destination=-1;
        temp[i].path_history=NULL;
        temp[i].no_of_collisons=0;
    }
    C->no_of_cars = no_of_cars;
    C->car_data = temp;
    fclose(fp);

    
}
void print_cars(CARS *c)
{
    int i;
    for ( i=0;i<c->no_of_cars;++i)
    {
        printf("\n%s start_node %d end_node %d endurance %d  velocity %d ",c->car_data[i].name,c->car_data[i].start_node,c->car_data[i].end_node,c->car_data[i].endurance,c->car_data[i].velocity);
    }
}
// this function is to  allocate edge id if edge is bydirectional then same edge 
// id goes to both places in adjacency list .if there are multipal bidirectional 
// or unidirectional edges between two nodes then each edge gets different id and 
// same values goes on both the places in adjacency list. 
int check_edge_id(ADJACENCY_LIST *p,int value,int vertex_id, int weight) 
{
    while(p!=NULL)
    {
        if(p->next_node == vertex_id && p->edge_id == -1 && p->weight == weight)// for edge to be bidirectional its weight should be same on both places in adjacency list.
        {
            p->edge_id = value;
            break;
        }
        p=p->next;
    }
}
int allocate_edge_id(GRAPH *g)  
{
    int i, count =0 ;
    ADJACENCY_LIST *temp;
    for(i=0;i<g->no_of_vertex;++i)
    {
        temp=g->graph_data[i];
        while(temp != NULL)
        {
      void update_car_path_initial(CAR *c)
{
    c->last_node_visited =c->start_node;
   
}      if(temp->edge_id == -1)
            {
                temp->edge_id=count;
                check_edge_id(g->graph_data[temp->next_node],count,i,temp->weight);
                count++;
            }
            temp=temp->next;
        }

    }
    g->no_of_edges = count;
}

void initialise_car_path(CAR *c)
{
        c->last_node_visited =c->start_node; 
        c->path_history = (PATH_INFO *)malloc(sizeof(PATH_INFO));
        c->path_history->enter_time = 0;
        c->path_history->next = NULL;
        c->path_history->path_node = c->start_node;
        c->current_edge = -1;
}
void init(GRAPH *g,CARS *c,EDGES *E)
{
    int i;
    ADJACENCY_LIST *temp,*temp2;
    allocate_edge_id(g);
    E->no_of_edges = g->no_of_edges;
    EDGE * e = (EDGE *)malloc (sizeof(EDGE)*g->no_of_edges);

    for(i=0;i<g->no_of_vertex;++i)
    {
        temp=g->graph_data[i];
        while(temp != NULL)
        {
           e[temp->edge_id].type =1; 
           e[temp->edge_id].u = i;
           e[temp->edge_id].v = temp->next_node;
           e[temp->edge_id].distance = temp->weight;
           e[temp->edge_id].age_history=NULL;
           e[temp->edge_id].no_of_cars = 0;
           e[temp->edge_id].status = 0;
           e[temp->edge_id].max_age = 0;
           e[temp->edge_id].car_list= NULL;
           e[temp->edge_id].last_car_exit_time =0 ;

           temp2 = g->graph_data[temp->next_node];
           while(temp2 != NULL)
           {
               if(temp2->edge_id == temp->edge_id)
                   e[temp->edge_id].type=2;
               temp2 = temp2->next;
           }
           temp = temp->next;
        }
    }
    E->edge_data=e;

    // intialising car _path
     for(i=0;i<c->no_of_cars;i++){
        initialise_car_path((c->car_data) + i);
    }
}
void print_edges(EDGES *e)
{
    int i;
    for(i=0;i<e->no_of_edges;++i)
    {
        printf("\n Edge %d :\n",i);
        printf(" u,v : %d,%d  ",e->edge_data[i].u,e->edge_data[i].v);
        printf(" distance :%d  ",e->edge_data[i].distance);
        printf(" status:%d type:%d \n",e->edge_data[i].status , e->edge_data[i].type);
    }   
}
void remove_car_from_list(EDGE *e,int car_id)
{
    CARS_ON_EDGE *temp,*temp2;
    if(e->car_list->car_id == car_id)
    {
        e->car_list=e->car_list->next;
        return;
    }
    temp = e->car_list;
    temp2= temp->next;
    while(temp2!= NULL)
    {
        if(temp2->car_id == car_id)
        {
            temp->next=temp2->next;
            break;
        }
        temp=temp2;
        temp2=temp2->next;
    }
}
void update_car_path(CAR *c,EDGES *e,long global_tick)
{
    AGE_INFO *temp;
    CARS_ON_EDGE *temp2;
    PATH_INFO *temp3;
    if(c->current_destination != -1)
    {
        // car decided to leave the node in last tick; 
        if(c->path_history->exit_time+1 == global_tick)
        {
            e->edge_data[c->current_edge].status=1;
            e->edge_data[c->current_edge].no_of_cars+=1;
            if(global_tick - e->edge_data[c->current_edge].last_car_exit_time != 1)
            {
                temp = (AGE_INFO *)malloc(sizeof(AGE_INFO));
                temp->idle_start_time = e->edge_data[c->current_edge].last_car_exit_time;
                temp->idle_finish_time=global_tick-1;
                temp->next = e->edge_data[c->current_edge].age_history;
                e->edge_data[c->current_edge].age_history = temp;

                temp2 =(CARS_ON_EDGE *)malloc(sizeof(CARS_ON_EDGE));
                temp2->car_id = c->id;
                temp2->next = e->edge_data[c->current_edge].car_list;
                e->edge_data[c->current_edge].car_list=temp2;
            }
        }

        // car reaches a node
        if((global_tick - c->path_history->exit_time )* time_step* c->velocity >= e->edge_data[c->current_edge].distance)
        {
            //update EDGE :  update last_car_exit_time & endurance & status & no_of_cars
            e->edge_data[c->current_edge].last_car_exit_time = global_tick;
            e->edge_data[c->current_edge].no_of_cars-=1;
            if(e->edge_data[c->current_edge].no_of_cars == 0)e->edge_data[c->current_edge].status=0;
            c->endurance -= e->edge_data[c->current_edge].distance;
            if(c->endurance == 0)c->done =1; // car out of gas 
            // remove car from car_list

            remove_car_from_list((e->edge_data)+(c->current_edge),c->id); 

            //update car path 
            temp3=(PATH_INFO *)malloc(sizeof(PATH_INFO));
            temp3->enter_time=global_tick;
            temp3->exit_time = -1;
            temp3->next = c->path_history;
            c->path_history = temp3;

            c->current_edge = -1;    
        }
    }
}
void detect_collison(EDGE *e,CARS *c,int car1,int car2,long global_tick)
{
    int direction=1;
    int flag =0;
    COLLISION_DATA *temp;
    float dist1,dist2,dist3,dist4;
    if(c->car_data[car1].current_destination == e->u && c->car_data[car2].current_destination ==e->u) direction = 0;
    if(c->car_data[car1].current_destination == e->v && c->car_data[car2].current_destination ==e->v) direction = 0;
        //if velocity variable then load 
    
        dist1 = (global_tick - c->car_data[car1].path_history->exit_time )* time_step* c->car_data[car1].velocity ;
        dist2 = (global_tick - c->car_data[car2].path_history->exit_time )* time_step* c->car_data[car2].velocity ;
       
        dist3 = ((global_tick+1) - c->car_data[car1].path_history->exit_time )* time_step* c->car_data[car1].velocity ;
        dist4 = ((global_tick+1) - c->car_data[car2].path_history->exit_time )* time_step* c->car_data[car2].velocity ;

    if(direction ==0)
    {
        if((dist1-dist2)*(dist3-dist4)<0)//collision detected
            flag=1;
    }
   else
   {
      if((dist1+dist2) < e->distance && (dist3+dist4) >= e->distance) 
          flag=1;
   } 
   if(flag==1)
   {
        temp = (COLLISION_DATA *)malloc(sizeof(COLLISION_DATA));
        temp->car1_id = car1;
            temp->car2_id = car2;
            temp->edge_id = e->id;
            temp->next=collisons;
            temp->time = global_tick +1;
            collisons = temp;
   }
}
void handle_collison(EDGE *e,CARS *c,long global_tick)
{
    CARS_ON_EDGE *coe1,*coe2;
    if(e->no_of_cars == 2)
        detect_collison(e,c,e->car_list->car_id,e->car_list->car_id,global_tick);
    else
    {
        coe1=e->car_list;
        coe2=coe1->next;
        while(coe1->next != NULL)
        {
            while(coe2!=NULL){
                detect_collison(e,c,coe1->car_id,coe2->car_id,global_tick);
                coe2=coe2->next;
            }
            coe1=coe1->next;
        }
    }
}
void update_car_destination(CAR *c,int next_node_id,int next_edge_id,long global_tick)
{
    c->current_destination = next_node_id ;
    c->last_node_visited = c->path_history->path_node;
    c->path_history->exit_time = global_tick;
    c->current_edge = next_edge_id ;
    c->path_history->exit_time =global_tick;
}
void start_simulation(GRAPH *g,CARS *c, EDGES *e )
{
    long global_tick=0;
    int i=0;
    int next_node_id,next_edge_id;
    int done = 0;
    
    //initial make decision
     for(i=0;i<c->no_of_cars;++i)
            if(c->car_data[i].current_destination == -1)   // car at node
            {
                make_decision(g,e,(c->car_data)+i,&next_node_id,&next_edge_id);
                if(!(next_node_id==-1 && next_edge_id==-1))
                    update_car_destination(c->car_data+i,next_node_id,next_edge_id,global_tick);
            }
    
    //simulation loop 
    while(done != 1)
    {
        done =1;
        // collison handling
        for(i=0;e->no_of_edges;++i)
        {
            if(e->edge_data[i].no_of_cars >=2)
            handle_collison((e->edge_data)+i,c,global_tick);
        }
        
        for(i=0;i<c->no_of_cars;++i)
            update_car_path((c->car_data) + i,e,global_tick);

        for(i=0;i<c->no_of_cars;++i)
            if(c->car_data[i].current_destination == -1)   // car at node
            {
                make_decision(g,e,(c->car_data)+i,&next_node_id,&next_edge_id);
                if(!(next_node_id==-1 && next_edge_id==-1))
                    update_car_destination(c->car_data+i,next_node_id,next_edge_id,global_tick);
            }

        for(i=0;i<c->no_of_cars;++i)//check for all car finished traversing 
        {
            if(c->car_data[i].done != 1)
                done = 0;
        }
        ++global_tick;
    }
    printf("\nSimulation Time = %ld ticks \n",global_tick);
}
void print_collisons()
{
    printf("\n===============================\nCollisons\n==================================\n");
    while(collisons != NULL)
    {
        printf("car1:%d car2:%d edge:%d time:%ld\n",collisons->car1_id,collisons->car2_id,collisons->edge_id,collisons->time);
        collisons=collisons->next;
    }
}
void print_edge_history(EDGES *e)
{
    int i;
    AGE_INFO *temp;
    printf("\n===============================\nEdge History\n==================================\n");
    for(i=0;i<e->no_of_edges;++i)
    {
        printf("EDGE : %d\n",i);
        temp= e->edge_data[i].age_history;
        while(temp!=NULL)
        {
            printf("idle_start_time:%ld idle_finish_time:%ld\n",temp->idle_start_time,temp->idle_finish_time);
            temp=temp->next;
        }
    }
}
void print_path_history(CARS *c)
{
    int i;
    PATH_INFO *temp;
    printf("\n===============================\nCar History\n==================================\n");
    for(i=0;i<c->no_of_cars;++i)
    {
        printf("Car : %d\n",i);
        temp=c->car_data[i].path_history;
        while(temp!=NULL)
        {
            printf("path_node:%d enter_time:%ld exit_time=%ld\n",temp->path_node,temp->enter_time,temp->exit_time);
            temp=temp->next;
        }
    }
}
int main(int argc , char * argv[])
{
    if(argc !=4 ){
        printf("\nUSAGE : simuation graph_data car_data time_step");exit(0);}
    GRAPH G; 
    CARS C;
    EDGES E;
    time_step = atof(argv[3]);
    read_graph(&G, argv[1]); 
    read_cars(&C, argv[2]);
    print_graph(&G);
    print_cars(&C);
    init(&G,&C,&E);
    print_edges(&E);
    printf(" \n time_step : %f\n",time_step);
    start_simulation(&G,&C,&E);
    print_collisons();
    print_path_history(&C);
    print_edge_history(&E);
}
