#include "simulation.h"
/***********************************************************
 * function to read the graph info from the file 
 * *********************************************************/

float time_step;
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
}void update_car_path_initial(CAR *c)
{
    c->last_node_visited =c->start_node;
   
}void update_car_path_initial(CAR *c)
{
    c->last_node_visited =c->start_node;
   
}
int check_edge_id(ADJACENCY_LIST *p,int value,int vertex_id, int weight) // this function is to  allocate edge id if edge is bydirectional then same edge id goes to both places in adjacency list .if there are multipal bidirectional or unidirectional edges between two nodes then each edge gets different id and same values goes on both the places in adjacency list. 
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

void update_car_path(CAR *c)
{

}
void start_simulation(GRAPH *g,CARS *c, EDGES *e )
{
    long global_tick=0;
    int i=0;
    int next_node_id,next_edge_id;
    int done = 0;
    for(i=0;i<c->no_of_cars;i++)
        c->car_data[i].last_node_visited =c->car_data[i].start_node; 
      
    while(done != 1){
        done =1;
        for(i=0;i<c->no_of_cars;++i)
            update_car_path((c->car_data) + i);
        for(i=0;i<c->no_of_cars;++i)
            if(car_at_node((c->car_data)+i))
            {
                make_decision(g,e,(c->car_data)+i,&next_node_id,&next_edge_id);
                if(!(next_node_id==-1 && next_edge_id==-1))
                    update_car_destination(g,e,c->car_data+i,next_node_id,next_edge_id);
            }

        for(i=0;i<c->no_of_cars;++i)
        {
            if(c->car_data[i].done != 1)
                done = 0;

        }
        ++global_tick;
    }

}


int main(int argc , char * argv[])
{
    
    if(argc !=4 ){
        printf("\nUSAGE : simuation file_name time_step");exit(0);}
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
    printf(" \n time_step : %f",time_step);
    start_simulation(&G,&C,&E);
}
