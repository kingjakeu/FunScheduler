#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "ku_cfs.h"
#define TIMESLICE 1000

// static variables
List *que;
Node *old = NULL;
Node *current;
int ts;

int main (int argc, char *argv[]){
	// integers for loops
	int i, j;
	// nice temp values
	int nice=0;
	// string for argument for processes
	char alpha = 'A';
	char arg[2] = {alpha, '\0'};
	// timer 
	struct itimerval it_val;
	//signal handler
	struct sigaction sa;
	sa.sa_handler = DoStuff;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	//List *que;
	que = (List*)malloc(sizeof(struct List));
	initList(que);
	pid_t pid;
	//set the timer
	it_val.it_value.tv_sec = TIMESLICE/1000;
	it_val.it_value.tv_usec = (TIMESLICE*1000) %1000000;
	it_val.it_interval = it_val.it_value;
	
	//timer set
	if(setitimer(ITIMER_REAL, &it_val, NULL) ==-1){
		perror("error calling setitimer()");
		exit(1);
	}
	// exception if num of args below 7
	if(argc != 7){
		printf("error: num of args not enough\n");
		exit(1);
	}
	//num of timeslices
	ts = atoi(argv[6]);

	//create processes
	for(i=1; i<argc-1; i++){
		int temp = atoi(argv[i]);
		for(j=0; j<temp; j++){
			arg[0] = alpha;
			pid = fork();

			switch(pid){
				case -1:
					printf("fork failed\n");
					break;
				case 0:
					execl("./ku_app","ku_app",arg, (char*) 0);
					printf("exec failed\n");
				//default:
					//wait((int*) 0);
				}
			nice = i-3;
			insertNode(que,pid,nice,0);
			alpha++;
		}
	}
	
	//set sigaction for ALRM
	if(sigaction(SIGALRM,&sa,NULL)==-1){
		perror("catch sigalrm errrr");
	}
	
	while(1){
		pause();
	}
	

}
//------function for linked list ---------------//
// list initiate
void initList(List *list){
	list->head = NULL;
	list->tail = NULL;
	//printf("list created\n");
}
//make node
Node* createNode(pid_t pid, int nice, int vrtime){
	Node *n = (Node*)malloc(sizeof(struct Node));
	n->pid = pid;
	n->nice = nice;
	n->vrtime = vrtime;
	n->nextNode = NULL;
	//printf("Node created\n");
	return n;
}
//check empty 
int isEmpty(List *l, Node *new){
	if((l->head == NULL) && (l->tail == NULL)){
		l->head = new;
		l->tail = new;
		return 0;
	}else{
		//printf("d: %d",l->head->pid);
		return 1;
	}
}
// add last 
void addLast(List *l, Node *new){
	l->tail->nextNode = new;
	l->tail = new;
}
// add first
void addFirst(List *l, Node *new){
	new->nextNode = l->head;
	l->head = new;
}
// add mid somewhere
void addNode(Node *target, Node *new){
	new->nextNode = target->nextNode;
	target->nextNode = new;
}
//search Node
Node *searchNode(List *l, pid_t pid){
	Node *temp = l->head;
	while(temp != NULL){
		if(temp->pid == pid){
				break;
		}
		temp = temp->nextNode;
	}
	return temp;
}
// print list ;
void printList(List *l){
	if(l->head == NULL && l->tail == NULL){
		printf("error : empty list\n");
	}else{
		Node *temp = l->head;
		while(temp != NULL){
			printf("pid: %d ",temp->pid);
			temp = temp->nextNode;
		}
		printf("\n");
	}
}
// insert node;
void insertNode(List *l, pid_t p, int nice, int vrtime){
	Node *new = createNode(p,nice,vrtime);
	Node *target;

	if(isEmpty(l,new)){
		addLast(l,new);
	}
	//printList(l);
}
// insert node;
void pushNode(List *l, Node *new){
	Node *target;
	if(isEmpty(l,new)){
		if(l->head->vrtime > new->vrtime){
			addFirst(l,new);
		}else{
			target = l->head;
			while(target->nextNode != NULL){
					if(target->nextNode->vrtime > new->vrtime){
						addNode(target,new);
						break;
					}
					//printf("t :%p n: %p\n",target,target->nextNode);
					target = target->nextNode;
			}
			if(target == l->tail){
				addLast(l,new);
			}
		}
	}
	//printList(l);
}
// pop node from que 
Node* popNode(List *l){
	Node *temp = l->head;
	if(temp->nextNode != NULL){
		l->head = temp->nextNode;
		temp->nextNode = NULL;
	}else{
		l->head = NULL;
		l->tail = NULL;
	}
	//printList(l);
	//printf("pop n push\n");
	return temp;
}
//--------list end ------------------------------//

// power calculation instead of using math.h
float power(float base, int exp){
	float result = 1, i=0;
	if(exp == 0 ){
		result = 1;
	}else if(exp > 0){
		for(i=0; i<exp; i++){
			result = result *base;
		}
	}else if(exp < 0){
		for(i=exp; i<0; i++){
			result = result / base;
		}
	}
	return result;
}
// calculate vrtime for each process;
void calcVrtime(Node *temp){
	temp->vrtime = temp->vrtime + (TIMESLICE*power(1.25,temp->nice));
	printf("vr: %d\n",temp->vrtime);
}
// signal act
void DoStuff(int sig){
	printf("timer went off\n");
	if(ts ==0){
		printf("time slices done\n");
		if(que->head == NULL){
			kill(old->pid,SIGSTOP);
		}
		exit(1);
	}else if(old == NULL){
		current = popNode(que);
		old = current;
		kill(current->pid,SIGCONT);
		ts--;
	}else{
		kill(old->pid,SIGSTOP);
		calcVrtime(old);
		pushNode(que,old);
		current = popNode(que);
		kill(current->pid,SIGCONT);
		old = current;
		ts--;
	}
	
	//calcVrtime(current);
}

