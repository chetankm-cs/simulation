#include "simulation.h"
typedef struct stack_node
{
	int key;
    int edge_id;
	struct stack_node * next;
}stack;

int pop(stack **s)
{
	int key;
	if(*s == NULL) return -1;

	else
	{
		key = (*s)->key;
		(*s)= (*s)->next;
		return key;
	}
}
int push(stack **s, int key,int edge_id)
{
	stack * temp;
	temp =(stack *) malloc (sizeof(stack));
	if(temp == NULL)return 0;
	temp->key= key;
    temp->edge_id=edge_id;
	temp->next = *s;
	*s= temp;
	return 1;
}
int is_instack(stack **S, int key,int edge_id)
{
	stack *temp;
	temp= *S;
	while(temp!=NULL)
	{
		if(temp->key == key && temp->edge_id== edge_id)return 1;
		temp =temp->next;
	}
	return 0;
}
void dfs_visit(GRAPH *g, stack *S, int v,int d, int *done,int edge_id)
{
    if(*done ==1)return;
	push(&S,v,edge_id);

    ADJACENCY_LIST *temp;
    temp = g->graph_data[v];
	while(temp != NULL)
	{
        if(temp->next_node == d) {*done=1;
            push(&S,d,temp->edge_id);
            return ;}
		dfs_visit(g,S,temp->next_node,d,done,temp->edge_id);
	}
	pop(&S);
}
void make_decision(GRAPH *g,EDGES *e,CAR *c,int *next_node_id,int *next_edge_id)
{
    stack *s;
	s=NULL;
    int done = 0;
    *next_node_id = -1;
    *next_edge_id = -1;

    if(c->path_history->path_node == c->end_node)
        return;
    
    dfs_visit(g,s,c->path_history->path_node,c->end_node,&done,-1);
    while(s->next->next != NULL)
    {
        s=s->next;
    }
    if(c->endurance < e->edge_data[s->edge_id].distance)return;
    
        *next_node_id = s->key;
        *next_edge_id = s->edge_id;
}
