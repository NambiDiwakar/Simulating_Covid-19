#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define NOV 10000		//maximum no.of vertices

typedef struct node{	//defining struct node 
    int key;			//vertex id
    int stat;			//indicating whether its susceptible,infected or recovered
    int inf_time;		//0-SUSCEPTIBLE,1-INFECTED,2-RECOVERED
    int rec_time;		//recovery snd infected times
    struct node*next;	//pointer to the next node
}node;

typedef struct list{    //defining struct list for creating lists
	int key;			//contains the value
	struct list* next;
}list;

typedef struct priority{//defining struct priority queue
	int time;			//time when the event occurs
	char status;		//status of the event indicatin t for transmit and r for recovery
	int vertex_index;	//vertex id
	struct priority* next;//pointer to next node
}priority;

list *S=NULL;

list *R=NULL;			//global lists used for storing the vertices which are

list *I=NULL;			//either infected or susceptible or recovered and as well as stamps

list *times=NULL;

priority *queue=NULL;   //priority queue pointer

node* array[NOV];		//array for adjacency list of graph each index +1 indicating the vertex

int day_count=0;

int prev_count=0;		//counts for printing the output

int c_l;

node* new_node(int d){				//creates the node with require key
	node *temp=(node*)(malloc(sizeof(node)));
	temp->key=d;
    temp->next=NULL;
    return temp;
}

list* new_index(int d){				//creates new node in the list
	list *temp=(list*)(malloc(sizeof(list)));
	temp->key=d;
	temp->next=NULL;
	return temp;
}

priority* new_priority(int t,char type,int vertex){ //creates new event
	priority * temp=(priority*)(malloc(sizeof(priority)));
	temp->time=t;
	temp->status=type;
	temp->vertex_index=vertex;
	temp->next=NULL;
	return temp;
}

void GenerateRandGraphs(int e)		//generating random graph
{
	int i, j, edge[e][2], count;
 
	i = 0;
	// Build a connection between two random vertex by iterating through edges
	while(i < e)
	{					

		edge[i][0] = rand()%(NOV)+1;
		edge[i][1] = rand()%(NOV)+1;
		i++;
	}

	for(i = 0; i < NOV; i++)
	{
	    node *root=NULL;
	    root=new_node(i+1);			//used for getting the adjacency list
	    node *temp;
		count = 0;
		for(j = 0; j < e; j++)
		{
			if(edge[j][0] == i+1)
			{
			        temp=root;
			        while(temp->next!=NULL){
			            temp=temp->next;
			        }
			        temp->next=new_node(edge[j][1]);
				count++;
			}
			else if(edge[j][1] == i+1)
			{
			        // root=new_node(edge[j][0]);
			        temp=root;
			        while(temp->next!=NULL){
			            temp=temp->next;
			        }
			        temp->next=new_node(edge[j][0]);
				// printf("%d ",edge[j][0]);
				count++;
			}
		}
		array[i]=root;				//storing the adjacency list along the vertex as root and their neighbors
	}	
}

int coin_toss(double p){			//simulating biased coin toss
	// srand(time(0));
	int count=0;
	int flag=1;
	double r;
	while(flag){
		r =  (1.0*rand()) /(RAND_MAX); //generates random number between 0 and 1
		 if(r>p){
		 	count++;
		 }
		 else{
		 	flag=0;
		 }
	}
	return count;					//returns the no of days after u get first head
}

void create_initial_S(){			//creates the initial susceptible list
	int i=0;
	for(i=0;i<NOV;i++){
		if(S==NULL){				//when the list is empty
			S=new_index(i+1);
		}
		else{
			list *temp;
			temp=S;
			while(temp->next!=NULL){
				temp=temp->next;
			}
			temp->next=new_index(i+1);
		}
		array[i]->stat=0;			//assigning all nodes susceptible
		array[i]->inf_time=1024;	//some random number indicating infinite time greater than t_max
	}

}

void push_priority(priority** head,int t,int vertex,char type){ //pushing the event into priority queue using time
	priority *temp=*head;
	priority *temp1=new_priority(t,type,vertex);				//creates th event
	if((*head)==NULL){											//when its empty
		*head=temp1;
	}
	else{
		if((*head)->time>t){
			temp1->next=*head;
			*head=temp1;
		}
		else{
			while((temp)->next!=NULL&&(temp)->next->time<t){	//inserts at correct position
				(temp)=(temp)->next;
			}
			temp1->next = temp->next;
			temp->next = temp1;
		}
	}
}

priority* pop_priority(priority** head){					//pops the top most element
	
	priority *temp;
	if(*head==NULL){
		return NULL;
	}
	else {
		temp=*head;
		*head=temp->next;
		return temp;
	}
}

void list_push(list **l,int v){				//pushes the element into list at the end of list
	list *temp;
	if(*l == NULL){
		list *new_l = new_index(v);
		*l = new_l;
	}

	else{
		temp=*l;
		while((temp)->next!=NULL){
			temp=(temp)->next;
		}
		(temp)->next=new_index(v);
	}
}
void list_pop(list **l,int v){				//pops the elemnt with v from the list
	list *temp;
	list *prev;
	temp=*l;
	while(temp->key!=v){
		prev=temp;
		temp=temp->next;
	}
	if(temp!=*l){
		prev->next=temp->next;
		free(temp);		
	}
	else{
		*l=temp->next;
		free(temp);
	}

}

int minimum(int a,int b,int  c){		//finds the minimum element in a,b,c
	int min=a;
	if(a>b){
		min=b;
	}
	if(min>c){
		min=c;
	}
	return min;
}
int count_list(list *l){			//gives the count of the list 
	list *temp3;
	temp3=l;
	int c=0;
	while(temp3!=NULL){
		c++;
		temp3=temp3->next;
	}
	return c;
}

void print_output(){			//prints the output in counts of various lists
	int n=prev_count;
	int c1=count_list(S);
	int c2=count_list(I);
	int c3=count_list(R);
	for(n;n<day_count;n++){
		printf("Day %d:\t ",c_l);
		printf("\tSusecptible count: %d",c1);
		printf("\t\tInfected count: %d",c2);
		printf("\t\tRecovered count:  %d\n",c3);
		c_l++;
	}
}

void find_trans_SIR(int t,double p,int v1,int v2,int t_max){ //transmits to the neighbors
	int inf_t;												//v1 is source and v2 is destination
	if(array[v2-1]->stat==0){
		inf_t=t+coin_toss(p);								//no of days ,infected times
		if(inf_t<minimum(array[v1-1]->rec_time,array[v2-1]->inf_time,t_max)){//transmits if only its infected time is less than max time and source time			
			push_priority(&queue,inf_t,v2,'t');			//pushes the event into queue
			array[v2-1]->inf_time=inf_t;
		}
	}
}

void rec_SIR(int u,int t){				//the node gets recovered
	prev_count=day_count;
	day_count=t;
	if(day_count!=prev_count){
		print_output();
	}
	list_push(&R,u);					//pushes into recovered list
	list_pop(&I,u);						//pops from infected list
	list_push(&times,t);				//pushes the time stamp
	array[u-1]->stat=2;					//changes to recover mode
}

void trans_SIR(int v,int t,double p1,double p2,int t_max){
	prev_count=day_count;				//used for printing the output
	day_count=t;
	if(day_count!=prev_count){
		print_output();
	}
	list_pop(&S,v);						//pops the node from S
	list_push(&I,v);					//pushs into I
	list_push(&times,t);				//pushes the time stamp into times
	array[v-1]->stat=1;					//changes the state to infected mode
	array[v-1]->rec_time=t+coin_toss(p2);
	if(array[v-1]->rec_time<t_max){
		push_priority(&queue,array[v-1]->rec_time,v,'r');		
	}
	
	node *temp;
	temp=array[v-1];					//iterates through neighbors
	while((temp)!=NULL){
		
		if((temp)->next!=NULL){
			
			find_trans_SIR(t,p1,v,(temp)->next->key,t_max);
		}
		(temp)=(temp)->next;
	}

}

void SIR(double p1,double p2,list* initial_infecteds){
	int i=0;
	list *temp5;
	temp5=initial_infecteds;				//iterates through initial infecteds list
	while(temp5!=NULL){
		array[temp5->key-1]->inf_time=0;
		push_priority(&queue,0,temp5->key,'t');
		temp5=temp5->next;
		
	}
	while(queue!=NULL){						//iterates through the events in priority q
		priority *temp2;
		temp2=pop_priority(&queue);
		if((temp2)->status=='t'){			//checks the status whether its transmit or not				
			if(array[((temp2)->vertex_index)-1]->stat==0){//checks whether it is susceptible or not
				trans_SIR((temp2)->vertex_index,(temp2)->time,0.5,0.2,300);
			}
		}
		else{								//it will get recovered
			rec_SIR((temp2)->vertex_index,(temp2)->time);
		}
	}
}

int main()
{
	srand(time(0));				//for generating different random numbers
	int n, i ,e;
	double trans_prob,rec_prob;
	trans_prob=0.5;				//transmission and recovery probability
	rec_prob=0.2;
    e=rand()%3000;				//generates random no of edges between 0 and 3000
    // printf("edge:%d,\n",e);
    GenerateRandGraphs(e);      //creates the graph
    double rand_num;
    create_initial_S();
    list *initial_infecteds=NULL;
    for(i=0;i<NOV;i++){			//creates a list of initial infecteds
    	if(array[i]->stat==0){
    		 rand_num=(1.0*rand()) /(RAND_MAX);//generates number between 0 & 1
    		 if(rand_num<0.1){	//used a random probability of 0.1 to generate initial infecteds
    		 	if(initial_infecteds==NULL){

    		 		initial_infecteds=new_index(i+1);
    		 	}
    		 	else{
    		 		list *temp4;
    		 		temp4=initial_infecteds;
    		 		while(temp4->next!=NULL){
    		 			temp4=temp4->next;
    		 		}
    		 		temp4->next=new_index(i+1);
    		 	}
    		 }
    	}
    }
    SIR(trans_prob,rec_prob,initial_infecteds);
	printf("Day %d:\t ",c_l);							//prints the last day status
	printf("\tSusecptible count: %d",count_list(S));
	printf("\t\tInfected count: %d",count_list(I));
	printf("\t\tRecovered count:  %d\n",count_list(R));
}