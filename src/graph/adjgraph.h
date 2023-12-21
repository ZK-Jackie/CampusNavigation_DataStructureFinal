#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef CAMPUSNAVIGATION_ADJGRAPH_H
#define CAMPUSNAVIGATION_ADJGRAPH_H
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
//图的邻接表式存储所需类型
typedef struct aNode {//边+终边上的结点+结点数据
	int weight;    //权重
	int sn;
	struct aNode *next;
} ArcNode;
typedef struct vNode {
	ArcNode *first;
} headNode;
typedef struct graph {
	headNode list[MaxNum];
	char ports[MaxNum][MaxNum];
} AdjGraph;

//图整合
typedef struct {
	AdjGraph *adjGraph;
	int adjMatrix[MaxNum][MaxNum];
	Node nodes[MaxNum];
	FILE *dependency;
	int nodeNum;    //物理个数
	int edgeNum;    //物理个数
} Graph;

extern bool CreateAdjGraph(AdjGraph *g, int adj[MaxNum][MaxNum], int nodeNum, int edgeNum);
extern void getReachableByDFS(AdjGraph *adjGraph, int origin, int aim, int nowLength, int visited[], bool *isFind);
extern int Dijkstra(int adjMatrix[][MaxNum], int origin, int final, int nodeNums, int ret[], int retWeight[],int *totalWeight);
extern void FindPathsByDFS(AdjGraph *adjGraph, int origin, int destination, int nowLength, int path[], int visited[], int paths[][MaxNum+1]);
extern void FindAllPaths(AdjGraph *g, int origin, int aim, int paths[][MaxNum+1]);

#endif //CAMPUSNAVIGATION_ADJGRAPH_H
