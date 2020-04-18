#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int pcb; //우선순위
   double vruntime; 
   struct Node *next;
}Node;
typedef struct Queue {
   Node *front;
   Node *rear;
   int count; //큐 보관 개수
}Queue;
void initNode(Node *n) {
   n->next = NULL;
   n->pcb = 0;
   n->vruntime = 0;
}
void initQueue(Queue *q) {
   q->front = q->rear = NULL;
   q->count = 0;
}
int isEmpty(Queue *q) {
   if (q->count == 0) {
      return 1;
   }
   else return 0;
}
void enQueue(Queue *q, int pcb) {
   Node *present = (Node *)malloc(sizeof(Node));
   present->pcb = pcb;

   if (isEmpty(q)) {
      q->front = present;
      q->rear = NULL;
   }
   else { //안비어있으면 정렬해서 들어갈거야. 
      
   }
}
int deQueue(Queue *q) {
   if (isEmpty(q)) {
      return 0;
   }
   else { //앞에서부터 뺄거야.
      Node *use = q->front;
      q->front = use->next;
      free(use);
      q->count--;
      return use->pcb;
   }
}

int calVruntime(Node node, int timeSlice, int processNum[]) {
   int nice;
   int vruntime;

   if (node.vruntime != 0) {
      node.vruntime = node.vruntime + timeSlice * (1.25 ^ nice);
   }
}
void init() {

}

void main(int argc, char *argv[]) {
   
   int processNum[5]; //프로세스 개수 담는 배열
   int totalpNum= 0; //프로세스 총 개수
   int timeSlice = atoi(argv[6]); //timeslice 개수
   

   for (int i = 0; i < 5; i++) {
      processNum[i] = atoi(argv[i+1]);
      totalpNum += processNum[i];
   }

   init(); //이때 각 프로세스마다 vruntime 다 초기화세팅. 레디큐에 다 순서대로 있어야돼. 
   
}


