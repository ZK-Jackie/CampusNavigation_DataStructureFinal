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
} HeadNode;
typedef struct graph {
	HeadNode list[MaxNum];
	char ports[MaxNum][MaxNum];
} AdjGraph;

//图整合
typedef struct {
	AdjGraph *adjGraph;
	int adjMatrix[MaxNum][MaxNum];	//邻接矩阵
	Node nodes[MaxNum];
	FILE *basisFile;
	int nodeNum;    //物理个数
	int edgeNum;    //物理个数
} Graph;

bool CreateAdjGraph(AdjGraph *g, int adj[MaxNum][MaxNum], int nodeNum, int edgeNum);
void getReachableByDFS(AdjGraph *adjGraph, int origin, int aim, int nowLength, int visited[], bool *isFind);
int Dijkstra(int adjMatrix[][MaxNum], int origin, int dest, int ret[], int retWeight[]);
void FindPathsByDFS(AdjGraph *adjGraph, int origin, int destination, int nowLength, int path[], int visited[], int paths[][MaxNum+1]);
void FindAllPaths(AdjGraph *g, int origin, int aim, int paths[][MaxNum+1]);
bool DelAdjNodeInList(HeadNode *head, int aim);
bool UpdateAdjNodeInList(HeadNode *head, int aim, int weight);
bool AddAdjNode(AdjGraph *adj, int srcSn, int resSn, int weight);
#endif //CAMPUSNAVIGATION_ADJGRAPH_H
