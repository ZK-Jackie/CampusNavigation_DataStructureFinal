/*环形队列函数，都基于link操作*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define QueueVol 25
//队列相关类型（用于BFS）
typedef struct queue {
	int sn;            	//结点数据
	struct queue *next; //下一个
	int spot;           //元素位置
} DataNode;
typedef struct {
	DataNode *front;
	DataNode *rear;
} Link;//封装队列参数

bool EnQueue(Link *link, int data) {
	bool isQueueable = ((link->rear->spot + 1) % (QueueVol + 1)) != (link->front->spot);
	if (isQueueable) {
		link->rear->next->sn = data;
		link->rear = link->rear->next;
		return true;
	} else {
		return false;
	}
}
bool isQueueable(Link *link){
	return ((link->rear->spot + 1) % (QueueVol + 1)) != (link->front->spot);
}
bool isEmpty(Link *link) {
	return link->rear == link->front;
}
DataNode *DeQueue(Link *link) {
	if (isEmpty(link)) {
		return NULL;
	} else {
		link->front->sn = -88;
		link->front = link->front->next;
		return link->front;
	}
}
Link *InitQueue(Link **link) {
	DataNode *queue;
	queue = (DataNode *) malloc(sizeof(DataNode));
	queue->sn = -99;
	queue->next = NULL;
	queue->spot = 0;
	*link = (Link *) malloc(sizeof(Link));
	(*link)->front = queue;
	(*link)->rear = (*link)->front;
	DataNode *temp = queue;
	for (int i = 1; i <= QueueVol; i++) {        //构造环形队列
		DataNode *sub = (DataNode *) malloc(sizeof(DataNode));
		temp->next = sub;
		sub->next = queue;
		sub->sn = -88;
		sub->spot = i;
		temp = sub;
	}
	return *link;
}
bool DestroyQueue(Link *link) {
	DataNode *freer = link->rear->next;
	DataNode *p = freer->next;
	while (p != link->rear) {
		free(freer);
		freer = NULL;
		freer = p;
		p = p->next;
	}
	free(freer);
	freer = NULL;
	free(p);
	p = NULL;
	free(link);
	link = NULL;
	return true;
}
int seeFront(Link *link){
	return link->front->next->sn;
}