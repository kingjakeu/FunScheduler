#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#define TIMESLICE 1000

int counter = 0;
typedef struct Node {
    int pid;
    char letter;
    double vruntime;
    struct Node *next;
}Node;

typedef struct Queue {
    Node *front;
}Queue;

Node* initNode(int pid, char letter, double vruntime) {
    Node* newNode = (Node *)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->pid = pid;
    newNode->letter = letter;
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
    prevTempNode->next = newNode;
}

Node* deQueue(Queue *q) {
    Node* tempNode = q->front;
    
    if(tempNode != NULL){
        q->front = tempNode->next;
    }
    tempNode->next = NULL;
    return tempNode;
}

void func(int sig){
    printf("timer alarm\n");
    counter++;
    if(counter == 3){
        
    }
}

int main(int argc, char *argv[]) {
    struct itimerval it_val;
    it_val.it_value.tv_sec = TIMESLICE/1000;
    it_val.it_value.tv_usec = (TIMESLICE*1000) %1000000;
    it_val.it_interval = it_val.it_value;
    
    //signal handler
    struct sigaction sa;
    sa.sa_handler = DoStuff;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    //timer set
    if(setitimer(ITIMER_REAL, &it_val, NULL) ==-1){
        perror("error calling setitimer()");
        exit(1);
    }
    
    signal(SIGALRM, func);
    
    while(1);
    
    return 0;
}
