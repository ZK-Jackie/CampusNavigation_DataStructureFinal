#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "adjgraph.h"

bool CreateAdjGraph(AdjGraph *g, int adj[MaxNum][MaxNum], int nodeNum, int edgeNum) {
	//创建邻接表储存方式
	for (int i = 0; i < nodeNum; ++i) {
		for (int j = nodeNum - 1; j >= 0; --j) {    //欲用头插法，矩阵从后往前遍历
			if (adj[i][j] != 0) {
				//定义一个边和边终点上的结点
				ArcNode *sub = (ArcNode *) malloc(sizeof(ArcNode));
				sub->sn = j;
				sub->weight = adj[i][j];
				//头插法进入邻接表
				sub->next = g->list[i].first;
				g->list[i].first = sub;
			}
		}
	}
	return true;
}


/**
 * @param adjGraph 邻接表式存储的图
 * @param origin 起始结点
 * @param aim 目标结点
 * @param nowLength 当前路径长度
 * @param path 临时路径储存地
 * @param assign 寻找定长路径，0--寻找所有路径，>0--寻找定长路径（不带权）
 * @param visited 标记已访问的结点
 * @param isFind 向前一函数是否找到符合条件的路径
 * @remark 一个普通的DFS，递归实现（有点像树的遍历）
 * */
void getReachableByDFS(AdjGraph *adjGraph, int origin, int aim, int nowLength, int visited[], bool *isFind) {
	//记录当前位置已被访问过
	visited[origin] = 1;
	if((*isFind)){		//【递归出口2】
		return;
	}
	if (origin == aim) {    //【递归出口1】
		//通知前一个函数已找到符合条件的路径
		(*isFind) = true;
		putchar('\n');
		return;
	}
	//DFS
	ArcNode *p = adjGraph->list[origin].first;    //遍历邻接表
	while (p != NULL) {
		int newStart = p->sn;
		if (visited[newStart] != 1) {	//没被访问过
			//一条路走到黑
			getReachableByDFS(adjGraph, newStart, aim, nowLength, visited, isFind);
		}
		//上一条路走完换第二条
		p = p->next;
	}
}

void FindAllPaths(AdjGraph *g, int origin, int aim, int paths[][MaxNum+1]){
	//定义、初始化path和visited数组
	int path[MaxNum];
	int visited[MaxNum];
	for (int i = 0; i < MaxNum; ++i) {
		path[i] = 0;
		visited[i] = 0;
	}
	//初始化paths数组
	for (int i = 0; i < MaxNum+1; ++i) {
		for (int j = 0; j < MaxNum+1; ++j) {
			paths[i][j] = 0;
		}
	}
	//DFS找路
	FindPathsByDFS(g, origin, aim, 0, path, visited, paths);
}

/**
 * @param adjGraph 空的邻接表式存储的图
 * @param origin 起始结点
 * @param destination 目标结点
 * @param nowLength 当前路径长度
 * @param path 临时路径储存地
 * @param assign 寻找定长路径，0--寻找所有路径，>0--寻找定长路径（不带权）
 * @param visited 标记已访问的结点
 * @param isFind 向前一函数是否找到符合条件的路径
 * @param paths (0,0)全部路径数目，(0,n)代表第n个路径中含的结点个数
 * @remark 一个普通的DFS，递归实现（有点像树）
 * */
void FindPathsByDFS(AdjGraph *adjGraph, int origin, int destination, int nowLength, int path[], int visited[], int paths[][MaxNum+1]){
	//更新路径
	path[nowLength] = origin;
	nowLength++;
	//记录当前位置已被访问过
	visited[origin] = 1;

	if(paths[0][0] == 25){
		return;
	}
	if(origin == destination){	//【递归出口】
		paths[0][0]++;//总路径数增加
		paths[0][paths[0][0]] = nowLength;//填充当前路径长度
		for (int i = 0; i < nowLength; ++i) {
			paths[paths[0][0]][i] = path[i];//
		}
		visited[origin] = 0;	//重置状态填充当前路径
		return;
	}

	//DFS
	ArcNode *p = adjGraph->list[origin].first;	//遍历邻接表
	while(p != NULL){
		int newStart = p->sn;
		if(visited[newStart] != 1){
			//一条路走到黑
			FindPathsByDFS(adjGraph, newStart, destination, nowLength, path, visited, paths);
		}
		//上一条路走完换第二条
		p = p->next;
	}
	//重置遍历结果
	visited[origin] = 0;
}

//物理size
bool reverseArr(int *arr, int size){
	int temp;
	for (int i = 0; i < size/2; ++i) {
		temp = arr[i];
		arr[i] = arr[size-i-1];
		arr[size-i-1] = temp;
	}

	return true;
}
//任务：需要修改，nodeNums不稳定，考虑替换成MaxNum
int Dijkstra(int adjMatrix[][MaxNum], int origin, int dest, int nodeNums, int ret[], int retWeight[], int *totalWeights){
	int set[nodeNums];
	int distance[nodeNums];
	int path[nodeNums];

	//1.初始化
	for (int i = 0; i < nodeNums; ++i) {
		//初始化集合
		set[i] = 0;
		//初始化距离
		distance[i] = adjMatrix[origin][i];
		//初始化路径
		if(distance[i] == 0){
			path[i] = -1;
		} else {
			path[i] = origin;
		}
	}
	//起始结点入集合
	set[origin] = 1;

	int minDistance;
	int minPoint;
	//5.每次set进一个，则总共需要循环nodeNums - 1次
	for (int i = 0; i < nodeNums - 1; ++i) {
		//2.找distance中最小且不在集合中的点
		minDistance = INF;
		for (int j = 0; j < nodeNums; ++j) {
			if(distance[j] != 0 && distance[j] < minDistance && set[j] == 0){
				minPoint = j;
				minDistance = distance[j];
			}
		}
		//3.将该点加入集合
		set[minPoint] = 1;
		//4.更新distance（只更新不在set中的结点）
		for (int j = 0; j < nodeNums; ++j) {
			if(set[j] == 0){
				//还需保证新路径的长度小于旧路径的长度
				if(adjMatrix[minPoint][j] != 0 && (distance[minPoint] + adjMatrix[minPoint][j] < distance[j] || distance[j] == 0)){
					distance[j] = distance[minPoint] + adjMatrix[minPoint][j];
					//更新path
					path[j] = minPoint;
				}
			}
		}
	}

	//1.填充路径数组
	//函数返回值是一个有效数据的下标  也就是起点  从该下标向前遍历即为最短路径
	int retSize = 0;
	int cur = dest;
	//从终点向起点填充
	while (cur != origin){
		ret[retSize] = cur;
		cur = path[cur];
		retSize++;
	}
	ret[retSize] = origin;
	retSize++;
	//数组反转
	reverseArr(ret, retSize);
	//2.填充路径长度数组
	int pre = 0;
	for (int i = 0; i < retSize; ++i) {
		retWeight[i] = distance[ret[i]] - pre;
		pre = distance[ret[i]];
	}
	//3.填充总权值
	(*totalWeights) = distance[dest];

	return retSize;
}
