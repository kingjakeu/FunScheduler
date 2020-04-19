#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "ku_cfs.h"

#define TIMESLICE 1000

int timeSlice;
double niceArr[5] = {0.64, 0.8, 1.0, 1.25, 1.5625}; 
Node *current;
Queue *queue;

Node* initNode(pid_t pid, int nice, double vruntime) {
    Node* newNode = (Node *)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->pid = pid;
    newNode->nice = nice;
    newNode->vruntime = vruntime;
    return newNode;
}

int isEmpty(Queue *q) {
    if(q->front == NULL) return 1;
    else return 0;
}

void enQueue(Queue *q, Node *newNode) {
    if(q->front == NULL){
        q->front = newNode;
        return;
    }
    Node *tempNode = q->front;
    Node *prevTempNode = NULL;
    
    while(tempNode != NULL){
        if(newNode->vruntime < tempNode->vruntime){
            if(prevTempNode == NULL){
                q->front = newNode; 
            }else{
                prevTempNode->next = newNode;
            }
            newNode->next = tempNode;
            return;
        }
        prevTempNode = tempNode;
        tempNode = tempNode->next;
    }
    prevTempNode->next = newNode; //inset node at queue's tail. 
}

Node* deQueue(Queue *q) {
    Node* tempNode = q->front;
    
    if(tempNode != NULL){
        q->front = tempNode->next;
        tempNode->next = NULL;
    }
    return tempNode;
}


int calVruntime(Node *n) {
    if(n != NULL){
        if (n->vruntime != 0) {
           n->vruntime = n->vruntime + ((TIMESLICE/1000)*niceArr[n->nice]);
        }
        else{
           n->vruntime = (TIMESLICE/1000)*niceArr[n->nice];
        }
         return n->vruntime;
    }
    return 0;
}

void terminate(){ //reaping children and free queue, node
    while(!isEmpty(queue)){
        Node* del = deQueue(queue);
        kill(del->pid, SIGKILL); //kill childprocess
        free(del); //Node free
    }
    free(queue); //Queue free
}

void handler(int sig){
    if(current == NULL && isEmpty(queue)){
        exit(1);
    }
    
    if(timeSlice == 0){
        kill(current->pid, SIGSTOP);
        enQueue(queue, current);
        terminate();
        exit(1);
    }
    else{
        if(current==NULL){ //first signaling
            current = deQueue(queue);
            kill(current->pid, SIGCONT);
        }
        else{
            kill(current->pid, SIGSTOP);
            calVruntime(current);
            enQueue(queue, current);
            current = deQueue(queue);
            kill(current->pid, SIGCONT);
        }
        timeSlice--;
    }
}

int main(int argc, char *argv[]) {
   
    int processNum[5];
    timeSlice = atoi(argv[6]); //the number of timeslice

    //allocate values
    for (int i = 0; i < 5; i++) {
      processNum[i] = atoi(argv[i+1]);
    }

    //timer
    struct itimerval it_val;
    it_val.it_value.tv_sec = TIMESLICE/1000;
    it_val.it_value.tv_usec = (TIMESLICE*1000)%1000000;
    it_val.it_interval = it_val.it_value;
    //set timer
    setitimer(ITIMER_REAL, &it_val, NULL);

    //create queue and init
    queue = (Queue*)malloc(sizeof(Queue));
    pid_t pid;
    //create process
    char arg[2] = {'A', '\0'};
    for(int i = 0; i < 5; i++){
        for(int j=0; j<processNum[i]; j++){
            pid = fork();
            sleep(1);
            
            if(pid == 0){
                execl("./ku_app","ku_app", arg, (char*) 0);
            }
            
            Node *newNode = initNode(pid, i, 0);
            calVruntime(newNode);
            enQueue(queue, newNode);
            arg[0]++;
        }
    }
    signal(SIGALRM, handler);
    while(1){
        
    }

    return 0;
}
