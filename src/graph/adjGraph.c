#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MaxIntro 50
#define MaxNum 25
#define NameSize 20
#define INF 888
typedef struct node {
	char data[NameSize];
	double pos_x;
	double pos_y;
	char intro[MaxIntro];
	bool isValid;
} Node;
//ͼ���ڽӱ�ʽ�洢��������
typedef struct aNode {//��+�ձ��ϵĽ��+�������
	int weight;    //Ȩ��
	int sn;
	struct aNode *next;
} ArcNode;
typedef struct vNode {
	ArcNode *first;
} HeadNode;
typedef struct graph {
	HeadNode list[MaxNum];
	char ports[MaxNum][MaxNum];
} AdjGraph;
//ͼ����
typedef struct {
	AdjGraph *adjGraph;
	int adjMatrix[MaxNum][MaxNum];
	Node nodes[MaxNum];
	FILE *basisFile;
	int nodeNum;    //�������
	int edgeNum;    //�������
} Graph;

bool CreateAdjGraph(AdjGraph *g, int adj[MaxNum][MaxNum], int nodeNum, int edgeNum) {
	//�����ڽӱ��淽ʽ
	for (int i = 0; i < nodeNum; ++i) {
		for (int j = nodeNum - 1; j >= 0; --j) {    //����ͷ�巨������Ӻ���ǰ����
			if (adj[i][j] != 0) {
				//����һ���ߺͱ��յ��ϵĽ��
				ArcNode *sub = (ArcNode *) malloc(sizeof(ArcNode));
				sub->sn = j;
				sub->weight = adj[i][j];
				//ͷ�巨�����ڽӱ�
				sub->next = g->list[i].first;
				g->list[i].first = sub;
			}
		}
	}
	return true;
}

void FindPathsByDFS(AdjGraph *adjGraph, int origin, int destination, int nowLength, int path[], int visited[], int paths[][MaxNum+1]){
	//����·��
	path[nowLength] = origin;
	nowLength++;
	//��¼��ǰλ���ѱ����ʹ�
	visited[origin] = 1;

	if(paths[0][0] == 25){
		return;
	}
	if(origin == destination){	//���ݹ���ڡ�
		paths[0][0]++;//��·��������
		paths[0][paths[0][0]] = nowLength;//��䵱ǰ·������
		for (int i = 0; i < nowLength; ++i) {
			paths[paths[0][0]][i] = path[i];//
		}
		visited[origin] = 0;	//����״̬��䵱ǰ·��
		return;
	}

	//DFS
	ArcNode *p = adjGraph->list[origin].first;	//�����ڽӱ�
	while(p != NULL){
		int newStart = p->sn;
		if(visited[newStart] != 1){
			//һ��·�ߵ���
			FindPathsByDFS(adjGraph, newStart, destination, nowLength, path, visited, paths);
		}
		//��һ��·���껻�ڶ���
		p = p->next;
	}
	//���ñ������
	visited[origin] = 0;
}
void getReachableByDFS(AdjGraph *adjGraph, int origin, int aim, int nowLength, int visited[], bool *isFind) {
	//��¼��ǰλ���ѱ����ʹ�
	visited[origin] = 1;
	if((*isFind)){		//���ݹ����2��
		return;
	}
	if (origin == aim) {    //���ݹ����1��
		//֪ͨǰһ���������ҵ�����������·��
		(*isFind) = true;
		putchar('\n');
		return;
	}
	//DFS
	ArcNode *p = adjGraph->list[origin].first;    //�����ڽӱ�
	while (p != NULL) {
		int newStart = p->sn;
		if (visited[newStart] != 1) {	//û�����ʹ�
			//һ��·�ߵ���
			getReachableByDFS(adjGraph, newStart, aim, nowLength, visited, isFind);
		}
		//��һ��·���껻�ڶ���
		p = p->next;
	}
}
void FindAllPaths(AdjGraph *g, int origin, int aim, int paths[][MaxNum+1]){
	//���塢��ʼ��path��visited����
	int path[MaxNum];
	int visited[MaxNum];
	for (int i = 0; i < MaxNum; ++i) {
		path[i] = 0;
		visited[i] = 0;
	}
	//��ʼ��paths����
	for (int i = 0; i < MaxNum+1; ++i) {
		for (int j = 0; j < MaxNum+1; ++j) {
			paths[i][j] = 0;
		}
	}
	//DFS��·
	FindPathsByDFS(g, origin, aim, 0, path, visited, paths);
}

bool reverseArr(int *arr, int size){//����size
	int temp;
	for (int i = 0; i < size/2; ++i) {
		temp = arr[i];
		arr[i] = arr[size-i-1];
		arr[size-i-1] = temp;
	}

	return true;
}

int Dijkstra(int adjMatrix[][MaxNum], int origin, int dest, int ret[], int retWeight[]){
	int nodeNums = MaxNum;	//�п��ܴ��ڷ�������¼ӵ㽻����״̬����ʱnodeNums��
	int set[nodeNums];
	int distance[nodeNums];
	int path[nodeNums];
	//1.��ʼ��
	for (int i = 0; i < nodeNums; ++i) {
		//��ʼ������
		set[i] = 0;
		//��ʼ������
		distance[i] = adjMatrix[origin][i];
		//��ʼ��·��
		if(distance[i] == 0){
			path[i] = -1;
		} else {
			path[i] = origin;
		}
	}
	//��ʼ����뼯��
	set[origin] = 1;
	int minDistance;
	int minPoint;
	//5.ÿ��set��һ�������ܹ���Ҫѭ��nodeNums - 1��
	for (int i = 0; i < nodeNums - 1; ++i) {
		//2.��distance����С�Ҳ��ڼ����еĵ�
		minDistance = INF;
		for (int j = 0; j < nodeNums; ++j) {
			if(distance[j] != 0 && distance[j] < minDistance && set[j] == 0){
				minPoint = j;
				minDistance = distance[j];
			}
		}
		//3.���õ���뼯��
		set[minPoint] = 1;
		//4.����distance��ֻ���²���set�еĽ�㣩
		for (int j = 0; j < nodeNums; ++j) {
			if(set[j] == 0){
				//���豣֤��·���ĳ���С�ھ�·���ĳ���
				if(adjMatrix[minPoint][j] != 0 && (distance[minPoint] + adjMatrix[minPoint][j] < distance[j] || distance[j] == 0)){
					distance[j] = distance[minPoint] + adjMatrix[minPoint][j];
					//����path
					path[j] = minPoint;
				}
			}
		}
	}
	//1.���·������
	//��������ֵ��һ����Ч���ݵ��±�  Ҳ�������  �Ӹ��±���ǰ������Ϊ���·��
	int retSize = 0;
	int cur = dest;
	//���յ���������
	while (cur != origin){
		ret[retSize] = cur;
		cur = path[cur];
		retSize++;
	}
	ret[retSize] = origin;
	retSize++;
	//���鷴ת
	reverseArr(ret, retSize);
	//2.���·����������
	int pre = 0;
	for (int i = 0; i < retSize; ++i) {
		retWeight[i] = distance[ret[i]] - pre;
		pre = distance[ret[i]];
	}
	return retSize;
}

bool DelAdjNodeInList(HeadNode *head, int aim){
	//����ɾ�����
	ArcNode *pre = head->first;
	ArcNode *p = NULL;
	if(pre != NULL){	//�쳣���һ������Ϊ��
		p = pre->next;
	} else{
		return false;
	}
	if(pre->sn == aim){	//�Ƿ����������һ������Ŀ��
		head->first = p;
		free(pre);
		pre = NULL;
		return true;
	}
	//һ���������
	while(p != NULL){
		if(p->sn == aim){
			pre->next = p->next;
			free(p);
			p = NULL;
			return true;
		}
		pre = p;
		p = p->next;
	}
	return false;
}
bool UpdateAdjNodeInList(HeadNode *head, int aim, int weight){
	ArcNode *pre = head->first;
	//һ���������
	while(pre != NULL){
		if(pre->sn == aim){
			pre->weight = weight;
			return true;
		}
		pre = pre->next;
	}
	return false;
}
bool AddAdjNode(AdjGraph *adj, int srcSn, int resSn, int weight){
	//˫����ӽ��
	//����/��ʼ�����
	ArcNode *src = (ArcNode *)malloc(sizeof (ArcNode));
	src->sn = srcSn;
	src->weight = weight;
	src->next = NULL;
	ArcNode *res = (ArcNode *)malloc(sizeof (ArcNode));
	res->sn = resSn;
	res->weight = weight;
	res->next = NULL;

	//ͷ�� src --> res  ��  res --> src
	res->next = adj->list[srcSn].first;
	adj->list[srcSn].first = res;

	src->next = adj->list[resSn].first;
	adj->list[resSn].first = src;

	return true;
}
