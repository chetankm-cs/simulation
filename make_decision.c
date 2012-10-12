#include "simulation.h"
typedef struct stack_node
{
	int key;
    int edge_id;
	struct stack_node * next;
}stack;
stack *s;
int pop()
{
	int key;
	if(s == NULL) return -1;

	else
	{

		key = s->key;
		s= s->next;
    //    printf(" Poping %d\n ",key);
		return key;
	}
}
int push(int key,int edge_id)
{
   // printf("Inserting vertex=%d edge_id %d\n",key,edge_id);
	stack * temp;
	temp =(stack *) malloc (sizeof(stack));
	if(temp == NULL)return 0;
	temp->key= key;
    temp->edge_id=edge_id;
	temp->next = s;
	s= temp;
	return 1;
}
int is_instack(int key,int edge_id)
{
	stack *temp;
	temp=s; 
	while(temp!=NULL)
	{
		if(temp->key == key )return 1;
		temp =temp->next;
	}
	return 0;
}
void print_stack ( ) // prints the loop by accessing the stack till the loop is printed
{
	stack * temp ;
	temp =s ;
    printf("\nStack :");
	while (temp!=NULL)
	{
		printf("%d ,",temp->key);
		temp=temp->next;
	}
    printf("\n");
}
int is_in_path_history(CAR *c,int node_id)
{
    PATH_INFO *temp;
    temp=c->path_history;
    while(temp != NULL)
    {
        if(temp->path_node == node_id)return 1;
        temp=temp->next;
    }
    return 0;
}
void dfs_visit(GRAPH *g,CAR *c, int v,int d, int *done,int edge_id)
{
    //print_stack();
    if(*done ==1)return;
	push(v,edge_id);
   // printf("dfs_visitvis %d %d %d\n",v,d,edge_id);
    ADJACENCY_LIST *temp;
    temp = g->graph_data[v];
	while(temp != NULL)
	{
        if(temp->next_node == d) 
        {
     //       printf("found t %d\n",temp->next_node);
            *done=1;
            push(d,temp->edge_id);
            //getchar();
            return ;
        }
        if(is_instack(temp->next_node,temp->edge_id)==0 && is_in_path_history(c,temp->next_node)==0)
     		dfs_visit(g,c,temp->next_node,d,done,temp->edge_id);
              if(*done==1)return;
        temp=temp->next;
	}
	pop();
}
void make_decision(GRAPH *g,EDGES *e,CAR *c,int *next_node_id,int *next_edge_id)
{
    if(c->done == 1)return;
	s=NULL;
    int done = 0;
    *next_node_id = -1;
    *next_edge_id = -1;
    if(DEBUG)
        printf("\nLOG:car %d s: %d t : %d \n",c->id,c->path_history->path_node,c->end_node);
    if(c->path_history->path_node == c->end_node)
    {
        c->done=1;
        return;
    }
    
    dfs_visit(g,c,c->path_history->path_node,c->end_node,&done,-1);
    if(s==NULL)printf("\n suck\n ");
    //print_stack(s);
    while(s->next->next != NULL)
    {
        s=s->next;
    }
    if(c->endurance < e->edge_data[s->edge_id].distance)
    {
        c->done =1;
        return;
    }
        printf("node %d edge %d\n",s->key,s->edge_id); 
        *next_node_id = s->key;
        *next_edge_id = s->edge_id;
}
