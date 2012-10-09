#include "simulation.h"
/***********************************************************
 * function to read the graph info from the file 
 * *********************************************************/
int insert(ADJACENCY_LIST **list, int vertex_id,int next_vertex,int weight)
{
    ADJACENCY_LIST *temp;
    temp = (ADJACENCY_LIST *) malloc (sizeof(ADJACENCY_LIST));
    temp->next_node = next_vertex;
    temp->weight = weight;
    if(list[vertex_id] == NULL)
        temp->next == NULL;
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
      //  fscanf(fp,"%s",name);
        temp[i].velocity =0;
       // fscanf(fp,"%d",temp[i].start_node);
       // fscanf(fp,"%d",temp[i].end_node);
       // fscanf(fp,"%d",temp[i].endurance);
        temp[i].id = i;
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
        printf("\nstart_node %d end_node %d ",c->car_data[i].start_node,c->car_data[i].end_node);
    }
}
int main(int argc , char * argv[])
{
    if(argc !=4 ){
        printf("\nUSAGE : simuation file_name time_step");exit(0);}
    GRAPH G; 
    CARS C;
    read_graph(&G, argv[1]); 
    read_cars(&C, argv[2]);
    print_graph(&G);
   //print_cars(&C);

}
