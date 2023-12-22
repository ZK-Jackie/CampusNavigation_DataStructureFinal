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

bool EnQueue(Link *link, int data);
bool isQueueable(Link *link);
bool isEmpty(Link *link);
DataNode *DeQueue(Link *link);
Link *InitQueue(Link **link);
bool DestroyQueue(Link *link);
int seeFront(Link *link);

#endif //CAMPUSNAVIGATION_QUEUE_H
