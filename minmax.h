#define NUM_OF_FLOORS 5

typedef struct request{
	int floor;
	int priority;
	float time;
}REQ;

typedef struct queue{
	REQ req_queue[NUM_OF_FLOORS];
	int n;
}QUEUE;

void init_queue(QUEUE *pq){
	pq->n=0;
}

int up_enq(QUEUE *pq,REQ rq){
	int son,father;

	if(pq->n==NUM_OF_FLOORS){
		printf("\nRequest at %d ignored\n",rq.floor);
		return 0;
	}

	else{
		son=pq->n;
		father=(son-1)/2;
		while(son>0 && pq->req_queue[father].priority > rq.priority ){
			pq->req_queue[son]=pq->req_queue[father];
			son=father;
			father=(son-1)/2;
		}
		pq->req_queue[son]=rq;
		pq->n++;
		return 1;
	}

}

int deq(QUEUE *pq){
	if(pq->n==0){
		return 0;
	}

	REQ elt;
	int father=0, son;
	elt=pq->req_queue[pq->n-1];
	if(pq->n==1){
		son=-1;
	}
	else{
		son=1;
	}

	if( (pq->n > 2) && (pq->req_queue[2].priority < pq->req_queue[1].priority) ){
		son=son+1;
	}

	while(son>=0 && pq->req_queue[son].priority<elt.priority){
		pq->req_queue[father]=pq->req_queue[son];
		father=son;
		son=2*father+1;

		if((son+1 < pq->n-2) && (pq->req_queue[son+1].priority>pq->req_queue[son].priority)){
			son=son+1;
		}
		if(son>pq->n-2){
			son=-1;
		}
	}
	pq->req_queue[father]=elt;
	pq->n--;

	return 1;
}

void show_top(QUEUE *pq){
	if(pq->n==0){
		printf("\nIdle");
	}
	else{
		printf("\nGoing to floor %d",pq->req_queue[0]);
	}
}
