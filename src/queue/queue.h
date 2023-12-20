#include <stdbool.h>

#ifndef CAMPUSNAVIGATION_QUEUE_H
#define CAMPUSNAVIGATION_QUEUE_H
#define QueueVol 25
//队列相关类型（用于BFS）
typedef struct queue {
	int sn;            	//结点数据
	struct queue *next; //下一个
	int spot;           //元素位置
} DataNode;
typedef struct {            //封装队列参数
	DataNode *front;
	DataNode *rear;
} Link;

extern bool EnQueue(Link *link, int data);
extern bool isEmpty(Link *link);
extern DataNode *DeQueue(Link *link);
extern Link *InitQueue(Link **link);
extern bool DestroyQueue(Link *link);
extern int seeFront(Link *link);

#endif //CAMPUSNAVIGATION_QUEUE_H
