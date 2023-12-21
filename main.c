#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <conio.h>
#include "utils.h"
#include "queue.h"
#include "adjgraph.h"
//初始化相关
bool InitProgram(Graph **graph);
bool InitNode(Graph *graph, char *str);
bool InitEdge(int adjMatrix[][MaxNum], AdjGraph *adjGraph, char *str, int *total);
bool DestroyProgram();
//文件相关
bool ReadGraphFile(Graph *g);	bool ReturnGraphFile(Graph *g);
bool ToPicFile(Graph *graph);
bool CreateBat();
//小工具
bool isConnected(Graph *g, char *origin, char *dest);
int getSn(Graph *g, char *name);
bool PrintPageHead();
/*情景问题解决函数*/
//信息相关
void menu(Graph *g);
bool SelectMap(Graph *g);
bool AddBuildingInfo(Graph *g);
bool ReachableTest(Graph *g);
bool SelectBuilding(Graph *g);
bool ProgramSetting();
bool UpdateRoadInfo(Graph *g, char *origin);
bool UpdateBuildingInfo(Graph *g, char *origin);
//建筑规划相关
bool AddBuilding(Graph *g, char *name, bool isLocated, ...);	bool AddAdjNode(AdjGraph *adj, int srcSn, int resSn, int weight);
bool DelBuilding(Graph *g, char *name, bool isDelRoad);			bool DelAdjNodeInList(headNode *head, int aim);//删建筑结点要用
bool UpdateBuilding(Graph *g, char *name, bool isRename, bool isRelocated, ...);	bool UpdateAdjNodeInList(headNode *head, int aim, int weight);
bool AddRoad(Graph *g, char *origin, char *end, int length, bool isSetPorts, ...);
bool DelRoad(Graph *g, char *origin, char *end);
bool UpdateRoad(Graph *g, char *origin, char *end, int newLength, bool isSetPorts, ...);
//路径相关
bool Navigate(Graph *g);
int FShortestPaths(Graph *g, int origin, int dest, int path[], int pathWeight[]);
bool isReachable(Graph *g, char *origin, char *dest);
/*一些全局变量*/
//空位队列
Link *emptySpot;
//一一对应序列
int basisSn[MaxNum];
//自行记住nodejs是否打开
bool isNodeAlive = false;
typedef struct {
	bool Save_Temp_After_Quit;
	char Png_File_Name[32];
	char Webpic1_File_Name[32];
	char Webpic2_File_Name[32];
	char Basis_File_Name[32];
	short Web_Mode;
	char Dot_File_Name[32];
	char Js_File_Name[32];
	bool Update_Pic_In_Time;
}Setting;
Setting settings;
int main() {
	Graph *graph;
	InitProgram(&graph);

	while(1){
		menu(graph);
	}

	return 0;
}
void menu(Graph *g){
	//0.定义相关变量
	int code;
	//打印头
	PrintPageHead();
	printf("\n\t1.查看当前地图\t");
	printf("\t2.查询建筑信息\t\n");
	printf("\t3.可达性校验\t");
	printf("\t4.导航路线规划\t\n");
	printf("\t5.应用设置\t");
	printf("\t6.退出程序\n\n");
	printf("\t\t请输入操作代码：");
	scanf_s("%c", &code);
	getchar();//消耗多余换行符号

	switch (code) {
		case '1':
			//用户自选操作，返回主菜单
			while(!SelectMap(g));
			printf("\n操作结束，准备回到主页.");
			Sleep(2000);
			break;
		case '2':
			//选建筑，返回主菜单
			while(!SelectBuilding(g));
			printf("\n操作结束，准备回到主页.");
			Sleep(2000);
			break;
		case '3':
			//可达性校验，返回主菜单
			while(!ReachableTest(g));
			printf("\n操作结束，准备回到主页.");
			Sleep(2000);
			break;
		case '4':
			//导航，返回主菜单
			while(!Navigate(g));
			printf("\n路线规划结束~按任意键回到主菜单.\n");
			system("pause");
			break;
		case '5':
			//设置程序，返回主菜单
			ProgramSetting();
			printf("\n应用设置结束.");
			Sleep(2000);
			break;
		case '6':
		case 'n':
		case 'N':
			ReturnGraphFile(g);
			//退出程序
			DestroyProgram();
			printf("\t\t\t再见！");
			Sleep(1500);
			exit(0);
		default:
			printf("无法识别的操作码！");
			Sleep(2000);
	}
	system("cls");
}
bool MakeStatistic(Graph *g){
	//清屏，输出页面头和数据
	system("cls");
	PrintPageHead();
	printf("\n当前校内共有建筑/标志性地点： %d 个\n", g->nodeNum);
	printf("\n当前校内共有道路： %d 条\n", g->edgeNum);
	return true;
}
bool SelectMap(Graph *g){
	//0.清屏，输出文件头，打印新菜单
	system("cls");
	PrintPageHead();
	printf("查看当前地图..\n\n");
	printf("1.查看当前地图信息\t");
	printf("2.增加校园建筑\n");
	printf("3.查看地图数据统计\t");
	printf("4.返回上一级菜单\n\n");
	printf("请输入操作代码：");
	//1.定义并初始化变量
	int code;
	char startPicCmd[50];
	memset(startPicCmd, 0, sizeof(startPicCmd));
	strcat(startPicCmd, "start ");
	//2.读取用户信息
	scanf_s("%c",&code);
	getchar();
	//3.分析用户信息，分流
	switch (code) {
		case '1':	//查看地图
			//新生成文件，根据设置确定打开方式
			ToPicFile(g);
			if(settings.Web_Mode){
				if(settings.Web_Mode == 1){	//网页1-本地网页
					strcat(startPicCmd, settings.Webpic1_File_Name);
				}else if(settings.Web_Mode == 2){	//网页2-vue-cli网页
					strcat(startPicCmd, settings.Webpic2_File_Name);
					if(!isNodeAlive){	//nodejs没开才开
						system("start nav.bat");
						isNodeAlive = true;
						printf("\n请稍后...（为了给node.js足够的启动时间，等待8秒）");
						Sleep(8000);
					}
				}
			}else{	//本地编译器
				strcat(startPicCmd, settings.Png_File_Name);
			}
			//打开查阅的文件，除了网页2都是静态的
			system(startPicCmd);
			printf("\n\t图片已打开~按任意键返回..\n");
			system("pause");
			return false;
		case '2':	//增加建筑
			while(!AddBuildingInfo(g));
			printf("\n操作结束，准备返回上一级页面..");
			Sleep(2000);
			return false;
		case '3':	//统计
			MakeStatistic(g);
			printf("\n统计结束~按任意键回到上一级页面..\n");
			system("pause");
			return false;
		case '4':
		case 'n':
		case 'N':
			break;
		default:
			printf("\n无效的操作码！\n");
			Sleep(2000);
			return false;
	}
	return true;
}
bool AddBuildingInfo(Graph *g){
	//0.清屏，输出页面头
	system("cls");
	PrintPageHead();
	printf("增加校园建筑...（输入“n”返回上一级菜单）\n");
	//1.定义相关参数并初始化，预计最多占27格
	char input[30];
	memset(input, 0, sizeof (input));
	char name[NameSize];
	memset(name, 0, sizeof (name));
	double x, y;
	//1.读取用户输入
	printf("\n请输入新建筑信息：（建筑名最多为8个中文字符且不可含数字符号；坐标均为正数，仅支持一位小数；符号均为英文符号）\n格式：【新建筑名:x坐标,y坐标】，缺省的元素无需输入冗余的符号\n");
	scanf_s("%s", input);
	getchar();
	//2.分析用户输入
	if(input[0] == 'n' || input[0] == 'N'){
		return true;
	}
	//2.1处理input数组
	char *str1 = strtok(input,":");
	char *str2 = strtok(NULL,":");
	bool located = false;
	//2.2非法性校验：非空校验，重复性校验在下一个函数中
	if(str1 == NULL){
		printf("\n错误：数据输入失效！(231)\n");
		Sleep(1000);
		return false;
	}else if(strlen(str1) > NameSize){
		printf("\n错误：建筑名称过长！(235)\n");
		Sleep(1000);
		return false;
	}
	//2.3解析用户输入
	if(str2 == NULL){		//情况1：给定建筑名，不指定坐标
		strcpy(name, str1);
	}else if(str2 != NULL){		//情况2：给定建筑名，给定坐标
		strcpy(name, str1);
		x = parseDouble(strtok(str2,","));
		y = parseDouble(strtok(NULL,","));
		located = true;
	}
	//3.参数递交及页面跳转
	if(AddBuilding(g, name, located, x, y)){
		printf("\n建筑添加成功！\n");
		Sleep(1500);
		ToPicFile(g);
	}
	return false;
}
bool ReachableTest(Graph *g){
	//0.清屏，输出页面头
	system("cls");
	PrintPageHead();
	printf("可达性校验..\n");
	//1.读取用户信息
	char *origin;
	char *dest;
	//1.1读取信息	//1.2返回上一级
	printf("请输入起始地：");
	scanf_s("%s", origin);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	printf("请输入目的地：");
	scanf_s("%s", dest);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	//1.4非法性校验
	if(origin == NULL || dest == NULL){
		printf("\n错误：数据输入失效！(279)\n");
		Sleep(1000);
		return false;
	}
	if(strlen(origin) > NameSize || strlen(dest) > NameSize){
		printf("\n错误：建筑名称有误！(284)\n");
		Sleep(1000);
		return false;
	}
	if(getSn(g,origin) == -1){
		printf("\n错误：起始建筑/地点不存在！(289)\n");
		Sleep(1000);
		return false;
	}else if(getSn(g,dest) == -1){
		printf("\n错误：终点建筑/地点不存在！(293)\n");
		Sleep(1000);
		return false;
	}
	//2.判断可达		//3.反馈
	if(isReachable(g, origin, dest)){
		printf("可达！");
	}else{
		printf("不可达！");
	}
	Sleep(2000);
	return false;
}
bool InitProgram(Graph **graph) {
	//0.程序设置初始化
	//0.1清空
	memset(settings.Basis_File_Name, 0, sizeof(settings.Basis_File_Name));
	memset(settings.Png_File_Name, 0, sizeof(settings.Png_File_Name));
	memset(settings.Dot_File_Name, 0, sizeof(settings.Dot_File_Name));
	memset(settings.Js_File_Name, 0, sizeof(settings.Js_File_Name));
	memset(settings.Webpic1_File_Name, 0, sizeof(settings.Webpic1_File_Name));
	memset(settings.Webpic2_File_Name, 0, sizeof(settings.Webpic2_File_Name));
	//0.2赋值
	strcpy(settings.Basis_File_Name, "basis.txt");	//默认读取basis.txt
	settings.Save_Temp_After_Quit = true;	//默认不清除缓存
	settings.Update_Pic_In_Time = true;		//默认自动更新图
	strcpy(settings.Png_File_Name, "navigation.png");
	strcpy(settings.Webpic1_File_Name, "./graphviz_lite1/index.html");
	strcpy(settings.Webpic2_File_Name, "http://localhost:22334/");
	settings.Web_Mode = 0;	//默认打开方式为本地打开
	strcpy(settings.Dot_File_Name, "navigation.dot");
	strcpy(settings.Js_File_Name, "./graphviz_lite1");
	//1.初始化图
	(*graph) = malloc(sizeof(Graph));
	(*graph)->dependency = fopen(settings.Basis_File_Name, "r");
	(*graph)->edgeNum = 0;
	(*graph)->nodeNum = 0;
	memset((*graph)->adjMatrix, 0, sizeof((*graph)->adjMatrix));
	//2.初始化结点所有值
	for (int i = 0; i < MaxNum; ++i) {
		memset((*graph)->nodes[i].data, 0, sizeof((*graph)->nodes[i].data));
		(*graph)->nodes[i].isValid = false;
		(*graph)->nodes[i].pos_x = -1.0;
		(*graph)->nodes[i].pos_y = -1.0;
		memset((*graph)->nodes[i].intro, 0, sizeof((*graph)->nodes[i].intro));
	}
	//3.初始化图的邻接表式存法
	(*graph)->adjGraph = (AdjGraph *) malloc(sizeof(AdjGraph));
	for (int i = 0; i < MaxNum; ++i) {
		(*graph)->adjGraph->list[i].first = NULL;
	}
	memset((*graph)->adjGraph->ports, 0, sizeof((*graph)->adjGraph->ports));

	//4.初始化空闲队列
	emptySpot = InitQueue(&emptySpot);
	for (int i = 0; i < MaxNum; ++i) {
		EnQueue(emptySpot,i);
	}

	//5.填充邻接表
	ReadGraphFile(*graph);
	//6.创造图
	CreateAdjGraph((*graph)->adjGraph, (*graph)->adjMatrix, (*graph)->nodeNum, (*graph)->edgeNum);
	return true;
}
bool ToPicFile(Graph *graph) {
	//有可能是JS，也有可能是dot
	FILE *navFile = NULL;
	if(settings.Web_Mode){	//网页模式：JS情况下，多写一些
		if(settings.Web_Mode == 1){	//网页模式1给定数据并生成get方法
			navFile = fopen("./graphviz_lite1/navigation.js", "w");
		}else if(settings.Web_Mode == 2){//网页模式2直接export无多余方法
			navFile = fopen("./graphviz_lite2/src/navigation.js", "w");
			fprintf(navFile, "export ");
		}
		fprintf(navFile, "var nav = ' ");
	}else{	//本地模式：DOT情况下
		navFile = fopen("navigation.dot", "w");
	}

	if (navFile == NULL) {
		printf("error!");
		return false;
	}
	//文件头
	fprintf(navFile, "graph G{"
					 "layout=fdp;"
					 "node[shape=rect, fontname=KaiTi];"
					 "splines=curved;");
	//定义点
	for (int i = 0; i < graph->nodeNum; ++i) {
		if(graph->nodes[i].isValid){
			if(graph->nodes[i].pos_x == -1 || graph->nodes[i].pos_y == -1){
				fprintf(navFile, "%d[label=\"%s\"];", i, GbkToUtf8(graph->nodes[i].data));
			}else{
				fprintf(navFile, "%d[label=\"%s\", pos=\"%.2f,%.2f!\"];", i, GbkToUtf8(graph->nodes[i].data), graph->nodes[i].pos_x,
						graph->nodes[i].pos_y);
			}
		}
	}
	int weight;
	char tail, head;
	//定义边：临界矩阵只读上三角区域
	for (int i = 0; i < MaxNum; ++i) {
		for (int j = i + 1; j < MaxNum; ++j) {
			weight = graph->adjMatrix[i][j];
			if (weight > 0) {
				fprintf(navFile, "%d--%d[label=%d", i, j, weight);
				if ((tail = graph->adjGraph->ports[i][j]) != 0) {    //上三角是tailports值
					fprintf(navFile, ",tailport=%c", tail);
				}
				if ((head = graph->adjGraph->ports[j][i]) != 0) {    //下三角是headports值
					fprintf(navFile, ",headport=%c", head);
				}
				fprintf(navFile, "];");
			}
		}
	}
	fprintf(navFile, "}");
	if(settings.Web_Mode){
		fprintf(navFile, " ';\n");
		if(settings.Web_Mode == 1){	//网页模式1多写一个get方法
			fprintf(navFile,"\nfunction getNav(){return nav;}");
		}
	}
	fclose(navFile);
	//生成dot文件
	if(!settings.Web_Mode){	//本地模式：DOT情况下，生成图
		system("dot -Tpng navigation.dot -o navigation.png");
	}
	return true;
}
bool ReadGraphFile(Graph *g) {
	//1.定义变量
	char tempStr[30];
	FILE *reader = g->dependency;	/*文件操作*/
	if (reader == NULL) {
		/*无法打开文件*/
		system("title Campus Navigation System");
		return false;
	}else{
		/*正常打开文件*/
		system("title Campus Navigation System - \"basis.txt\"");
	}
	//2.文件读取
	while (!feof(reader)) {    /*当没有读取到文件末端，持续在循环体中工作*/
		fscanf(reader, "%s", tempStr);    /*读取一行数据存入temp后，文件光标会自动移动至下一行*/
		//3.分流：有冒号说明为结点定义；有减号说明为边定义
		if(strchr(tempStr, ':') != NULL){
			InitNode(g, tempStr);
		}else if(strchr(tempStr, '-') != NULL){
			InitEdge(g->adjMatrix, g->adjGraph, tempStr, &g->edgeNum);
		}else{
			//有可能有问题
		}
	}
	fclose(reader);
	return true;
}
bool InitNode(Graph *graph, char *str) {
	//1.定义变量，sn号，建筑名，坐标位置，可选坐标指定
	int sn;
	char *name = NULL;
	char *xy = NULL;
	double x = -1.0, y = -1.0;
	bool isLocated = false;
	//2.分割、处理字符串：sn，名字，坐标
	sn = parseInt(strtok(str,":"));
	name = strtok(NULL,":");
	xy = strtok(NULL,":");
	if(xy != NULL){
		isLocated = true;
		x = parseDouble(strtok(xy,","));
		y = parseDouble(strtok(NULL,","));
	}
	//3.转换编码（外存UTF8->内存GBK）
	name = Utf8ToGbk(name);

	//程序会重新下发新的sn号，由于后续要用，与用户给的basisSn不同，则记下来
	basisSn[seeFront(emptySpot)] = sn;
	//添加建筑
	AddBuilding(graph,name,isLocated, x, y);

	return true;
}
bool InitEdge(int adjMatrix[][MaxNum], AdjGraph *adjGraph, char *str, int *edgeNum) {
	(*edgeNum)++;
	//1.定义变量并初始化
	int sn1, sn2, weight;
	char *tempPorts = NULL;
	//2.分割字符串：双sn，权值，ports
	sn1 = parseInt(strtok(str, "-"));
	sn2 = parseInt(strtok(NULL, "-"));
	weight = parseInt(strtok(NULL, "-"));
	tempPorts = strtok(NULL, "-");
	//3.二次加工：双sn和权值校验，ports检验
	if(sn1 == -1 || sn2 == -1 || weight == -1){
		//报错
	}

	int portSize = 0;
	if(tempPorts != NULL){
		portSize = (int)strlen(tempPorts);
	}
	//4.填充矩阵
	adjMatrix[sn1][sn2] = weight;
	adjMatrix[sn2][sn1] = weight;
	switch (portSize) {
		case 3:
			adjGraph->ports[sn2][sn1] = tempPorts[2];
		case 1:
			adjGraph->ports[sn1][sn2] = tempPorts[0];
			break;
		case 2:
			adjGraph->ports[sn2][sn1] = tempPorts[1];
			break;
		default:
			break;
	}
	return true;
}
bool AddBuilding(Graph *g, char *name, bool isLocated, ...){
	//0.建筑名校验
	int dup = 0;
	//队空说明无再可用的SN，则无法添加新的结点
	if(isEmpty(emptySpot)){
		printf("\n错误：地图空间不足，添加失败！(519)\n");
		Sleep(1000);
		return false;
	}else if( (dup = getSn(g, name)) != -1){
		printf("\n错误：已存在同名建筑/地点，添加失败！(523)\n");
		Sleep(1000);
		return false;
	}
	//0.1坐标定义、赋值及校验
	double x = -1.0, y = -1.0;
	if(isLocated){
		//获取形参列表
		va_list vaList;
		//参数初始化
		va_start(vaList,2);
		x = va_arg(vaList,double);
		y = va_arg(vaList,double);
		va_end(vaList);
		//参数校验
		if(x < 0 || y < 0){
			printf("\n错误：坐标输入格式有误，添加失败！(539)\n");
			Sleep(1000);
			return false;
		}
		if(x > 14 || y > 15){
			printf("\n错误：坐标超过校园范围，添加失败！(544)\n");
			Sleep(1000);
			return false;
		}
	}
	//1.可用下标队列出队
	int newNodeSn = DeQueue(emptySpot)->sn;
	//2.填充结点名字信息/位置信息
	strcpy(g->nodes[newNodeSn].data, name);
	g->nodes[newNodeSn].isValid = true;
	g->nodes[newNodeSn].pos_x = x;
	g->nodes[newNodeSn].pos_y = y;
	//3.更新结点数
	g->nodeNum++;

	return true;
}
int getSn(Graph *g, char *name){
	//找，没找到返回Sn
	int ret = -1;
	for (int i = 0; i < MaxNum; ++i) {
		if(strcmp(g->nodes[i].data,name) == 0 && g->nodes[i].isValid){
			ret = i;
			break;
		}
	}
	return ret;
}
bool DelBuilding(Graph *g, char *name, bool isDelRoad){
	//删除建筑：1.清除邻接表中的所有信息；2.邻接图所有结点断联；3.清除结点表中的信息；4.sn入队
	int concernSn[MaxNum];
	int concern = 0;
	//1.找结点sn
	int nodeSn = getSn(g, name);
	//没找到，退出
	if(nodeSn == -1){
		printf("\n错误：建筑不存在！(580)\n");
		Sleep(1000);
		return false;
	}
	//2.清空相关信息
	//2.1邻接表
	//目标要被清空的行
	headNode *aimRow = &g->adjGraph->list[nodeSn];
	//检测目标是否为空
	ArcNode *first = g->adjGraph->list[nodeSn].first;
	while(first != NULL){
		//0.记下关联信息
		concernSn[concern++] = first->sn;
		//1.删关联行
		DelAdjNodeInList(&g->adjGraph->list[first->sn], nodeSn);
		//2.删自己
		DelAdjNodeInList(aimRow, first->sn);
		//3.重新更新为第一个
		first = g->adjGraph->list[nodeSn].first;
	}
	//2.2邻接矩阵和port矩阵清除关联信息。若保留路径，互相复制粘贴路径后再清除，并选择性补路
	for (int i = 0; i < concern; ++i) {
		if(!isDelRoad){//不删路，重新计算路线后再删除
			for (int j = 0; j < concern; ++j) {//遍历concern数组
				//对于不相邻的两点加新边，调整port补路，相邻者不管
				if(!isConnected(g, g->nodes[concernSn[i]].data, g->nodes[concernSn[j]].data)){
					//对于权：两条路径的权值相加
					//对于ports，保留开口，开口为0的忽略
					AddRoad(g, g->nodes[concernSn[i]].data, g->nodes[concernSn[j]].data, g->adjMatrix[nodeSn][concernSn[i]]+g->adjMatrix[nodeSn][concernSn[j]] ,true, g->adjGraph->ports[concernSn[i]][nodeSn], g->adjGraph->ports[concernSn[j]][nodeSn]);
				}
			}
		}
		g->adjMatrix[nodeSn][concernSn[i]] = 0;
		g->adjMatrix[concernSn[i]][nodeSn] = 0;
		g->adjGraph->ports[nodeSn][concernSn[i]] = 0;
		g->adjGraph->ports[concernSn[i]][nodeSn] = 0;
	}
	//2.3清空点信息
	memset(g->nodes[nodeSn].data, 0, sizeof(g->nodes[nodeSn].data));
	g->nodes[nodeSn].pos_x = -1.0;
	g->nodes[nodeSn].pos_y = -1.0;
	g->nodes[nodeSn].isValid = false;
	//3.新sn可用，入队
	EnQueue(emptySpot,nodeSn);
	//4.输出提示信息
	printf("\n建筑/地点删除成功！\n");
	return true;
}
bool AddAdjNode(AdjGraph *adj, int srcSn, int resSn, int weight){
	//双向添加结点
	//创建/初始化结点
	ArcNode *src = (ArcNode *)malloc(sizeof (ArcNode));
	src->sn = srcSn;
	src->weight = weight;
	src->next = NULL;
	ArcNode *res = (ArcNode *)malloc(sizeof (ArcNode));
	res->sn = resSn;
	res->weight = weight;
	res->next = NULL;

	//头插 src --> res  ；  res --> src
	res->next = adj->list[srcSn].first;
	adj->list[srcSn].first = res;

	src->next = adj->list[resSn].first;
	adj->list[resSn].first = src;

	return true;
}
bool DelAdjNodeInList(headNode *head, int aim){
	//单向删除结点
	ArcNode *pre = head->first;
	ArcNode *p = NULL;
	if(pre != NULL){	//异常情况一，链表为空
		p = pre->next;
	} else{
		return false;
	}
	if(pre->sn == aim){	//非凡情况二，第一个就是目标
		head->first = p;
		free(pre);
		pre = NULL;
		return true;
	}
	//一般情况，找
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
bool UpdateAdjNodeInList(headNode *head, int aim, int weight){
	ArcNode *pre = head->first;
	//一般情况，找
	while(pre != NULL){
		if(pre->sn == aim){
			pre->weight = weight;
			return true;
		}
		pre = pre->next;
	}
	return false;
}
bool UpdateBuilding(Graph *g, char *name, bool isRename, bool isRelocated, ...){
	char str[MaxIntro];
	memset(str, 0, sizeof(str));
	bool isIntro = false;
	//0.数据规范性校验1
	if(strlen(name) > NameSize){
		printf("\n错误：建筑名称过长！(694)\n");
		Sleep(1000);
		return false;
	}
	//2.更新建筑：改名/该位置即可
	int nodeSn = getSn(g, name);
	//2.1校验2
	if(nodeSn == -1){
		printf("\n错误：未找到目标建筑！(702)\n");
		Sleep(1000);
		return false;
	}
	//3.改简介
	printf("请输入建筑简介信息：（不超过25个中文，跳过请提交“n”）\n");
	scanf_s("%s", str);
	getchar();
	if(!(str == NULL || str[0] == 'n' || str[0] == 'N')){
		if(strlen(str) < MaxIntro){
			strcpy(g->nodes[nodeSn].intro, str);
			isIntro = true;
		}else{
			printf("\n错误：简介内容过长！(715)\n");
			Sleep(1000);
			return false;
		}
	}
	if(isRename || isRelocated || isIntro){
		//获取形参列表
		va_list vaList;
		//参数初始化
		if(isRename && isRelocated){
			va_start(vaList,3);
		}else if(!isRename){
			va_start(vaList,2);
		} else {
			va_start(vaList,1);
		}
		//改名
		char *newName;
		if(isRename){
			newName = va_arg(vaList, char*);
			if(strlen(newName) > NameSize){
				printf("\n错误：建筑名称过长！(736)\n");
				Sleep(1000);
				return false;
			}
		}
		//改位置
		double x, y;
		if(isRelocated){
			x = va_arg(vaList,double);
			y = va_arg(vaList,double);
			if(x > 14 || y > 15){
				printf("\n错误：坐标超过校园范围，添加失败！(747)\n");
				Sleep(1000);
			}
			if(x < 0 || y < 0){
				printf("\n错误：坐标输入格式有误，添加失败！(751)\n");
				Sleep(1000);
			}
		}
		if(isRename){
			strcpy(g->nodes[nodeSn].data, newName);
		}
		if(isRelocated){
			g->nodes[nodeSn].pos_x = va_arg(vaList,double);
			g->nodes[nodeSn].pos_y = va_arg(vaList,double);
		}
		va_end(vaList);

		printf("\n修改成功！\n");
		return true;
	}
	printf("\n未发生修改\n");
	return false;
}
bool AddRoad(Graph *g, char *origin, char *end, int length, bool isSetPorts, ...){
	//加路：1.找两个点的下标；2.邻接矩阵双向加值；2.1有路者不可操作；3.链表加两个结点；
	int sn1 = getSn(g,origin);
	int sn2 = getSn(g,end);
	if(sn1 == -1 && sn2 == -1){
		printf("\n错误：未找到指定建筑/地点！(775)\n");
		Sleep(1000);
		return false;
	}
	//2.处理邻接矩阵
	g->adjMatrix[sn1][sn2] = length;
	g->adjMatrix[sn2][sn1] = length;
	//如果指定了方向
	if(isSetPorts){
		//获取形参列表
		va_list vaList;
		//参数初始化
		va_start(vaList,2);
		//矩阵填参数
		char ch1, ch2;
		ch1 = (char)va_arg(vaList,int);
		ch2 = (char)va_arg(vaList,int);
		if(ch1 != 0)	//对于0，不管
			g->adjGraph->ports[sn1][sn2] = ch1;
		if(ch2 != 0)
			g->adjGraph->ports[sn2][sn1] = ch2;
		va_end(vaList);
	}
	//3.链表加结点，双向都加
	AddAdjNode(g->adjGraph,sn1,sn2,length);

	return true;
}
bool DelRoad(Graph *g, char *origin, char *end){
	//删路：1.找两个点的下标；2.邻接矩阵双向加值；2.1没路者不可操作；3.链表删两个结点；
	int sn1 = getSn(g, origin);
	int sn2 = getSn(g, end);
	if(sn1 == -1 && sn2 == -1){
		printf("\n错误：未找到指定建筑/地点！(808)\n");
		Sleep(1000);
		return false;
	}
	//矩阵双向删值
	g->adjMatrix[sn1][sn2] = 0;
	g->adjMatrix[sn2][sn1] = 0;
	//矩阵port双向删值
	g->adjGraph->ports[sn1][sn2] = 0;
	g->adjGraph->ports[sn2][sn1] = 0;
	//删结点
	DelAdjNodeInList(&g->adjGraph->list[sn1],sn2);
	DelAdjNodeInList(&g->adjGraph->list[sn2],sn1);
	return true;
}
bool UpdateRoad(Graph *g, char *origin, char *end, int newLength, bool isSetPorts, ...){
	//更新路：1.找两个点的下标；2.1改权：邻接矩阵双向改值；若改道：删两个路，加两个路
	int sn1 = getSn(g, origin);
	int sn2 = getSn(g, end);
	if(sn1 == -1 && sn2 == -1){
		printf("\n错误：未找到指定建筑/地点！(828)\n");
		Sleep(1000);
		return false;
	}
	//2.处理邻接矩阵
	g->adjMatrix[sn1][sn2] = newLength;
	g->adjMatrix[sn2][sn1] = newLength;
	//3.改链表
	UpdateAdjNodeInList(&g->adjGraph->list[sn1],sn2,newLength);
	UpdateAdjNodeInList(&g->adjGraph->list[sn2],sn1,newLength);
	//4.标记开口
	if(isSetPorts){
		//获取形参列表
		va_list vaList;
		//参数初始化
		va_start(vaList,2);
		//矩阵填参数
		g->adjGraph->ports[sn1][sn2] = va_arg(vaList,int);
		g->adjGraph->ports[sn2][sn1] = va_arg(vaList,int);
		va_end(vaList);
	}
	return true;
}
bool isReachable(Graph *g, char *origin, char *dest){
	//无需多言
	int startSn = getSn(g,origin);
	int finalSn = getSn(g, dest);

	int visited[MaxNum];
	for (int i = 0; i < MaxNum; ++i) {
		visited[i] = 0;
	}
	bool isFind = false;
	//DFS
	getReachableByDFS(g->adjGraph, startSn, finalSn, 0, visited, &isFind);

	return isFind;
}
int FShortestPaths(Graph *g, int origin, int dest, int path[], int pathWeight[]){
	int totalWeight = 0;
	int pathSize;
	pathSize = Dijkstra(g->adjMatrix, origin, dest, g->nodeNum, path, pathWeight, &totalWeight);
	return pathSize;
}
bool Navigate(Graph *g){
	//0.清屏，输出文件头
	system("cls");
	PrintPageHead();
	printf("导航路线规划..（输入“n”返回上一级菜单）\n");
	PrintPageHead();
	//1.读取用户信息
	char *origin;
	char *dest;
	//1.1读取信息
	again:
	printf("请输入起始地：");
	scanf_s("%s", origin);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	printf("请输入目的地：");
	scanf_s("%s", dest);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	if(origin == NULL || dest == NULL){
		printf("\n错误：数据输入失效！(896)\n");
		Sleep(1000);
		return false;
	}
	//1.2转成sn
	int sn1 = getSn(g,origin);
	int sn2 = getSn(g, dest);
	//1.3非法校验
	if(sn1 == -1 || sn2 == -1){
		if(sn1 == -1){
			printf("\n错误：建筑 %s 不存在！(906)\n", origin);
		}
		if(sn2 == -1){
			printf("\n错误：建筑 %s 不存在！(909)\n", dest);
		}
		Sleep(1000);
		return false;
	}
	//2.拿到最小路径（包括起点和终点）
	int path[MaxNum];
	int weight[MaxNum];
	int length = FShortestPaths(g, sn1, sn2, path, weight);
//	//3.拿到全部路径
//	int paths[MaxNum+1][MaxNum+1];
//	FindAllPaths(g->adjGraph, sn1, sn2, paths);
	//4.输出路径
	int totalWeight = 0;
	for (int i = 0; i < length; i++) {
		printf("%s", g->nodes[path[i]].data);
		if(i != length-1){
			printf("<-%d->",weight[i+1]);
			totalWeight += weight[i+1];
		}
	}
	printf("\n全程距离: %d\n", totalWeight);
	printf("以上是为您查询到的从 %s 到 %s 的最短路径及全程距离\n", origin, dest);
	PrintPageHead();
	goto again;

	return false;
}
bool isConnected(Graph *g, char *origin, char *dest){
	int sn1 = getSn(g,origin);
	int sn2 = getSn(g, dest);
	ArcNode *p = g->adjGraph->list[sn1].first;
	while(p != NULL){
		if(p->sn == sn2){
			return true;
		}
		p = p->next;
	}
	return false;
}
bool SelectBuilding(Graph *g){
	//0.清屏，打印页面头
	system("cls");
	PrintPageHead();
	printf("查询建筑信息..（输入“n”返回上一级菜单）\n");
	//1.读取用户信息
	char origin[NameSize];
	char destination[NameSize];
	//1.0初始化数组
	memset(origin, 0, sizeof (origin));
	memset(destination,0,sizeof (destination));
	//1.1读取信息
	printf("请输入需查询的目标建筑名称：");
	scanf_s("%s",origin);
	getchar();//消耗多余换行符
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	//1.2转成sn
	int sn1 = getSn(g, origin);
	//1.3非法性校验
	if(sn1 == -1){
		printf("没有找到目标建筑/地点“%s”！",origin);
		Sleep(1500);
		return false;
	}
	//2.在邻接表中处理并展示
	system("cls");
	PrintPageHead();
	printf("建筑/地点信息...\n");
	printf("建筑名称：%s\n", origin);
	printf("SN：%d\n", sn1);
	if(g->nodes[sn1].pos_x == -1 || g->nodes[sn1].pos_y == -1){
		printf("位置坐标：（未定）\n");
	}else{
		printf("位置坐标：%.2lf,%.2lf\n",g->nodes[sn1].pos_x, g->nodes[sn1].pos_y);
	}
	printf("建筑/地点介绍：%s\n", g->nodes[sn1].intro);
	printf("与其他建筑道路连接情况：\n");
	ArcNode *p = g->adjGraph->list[sn1].first;
	//2.1遍历邻接链表
	if(p == NULL){
		printf("\t（无）");
	}
	while(p != NULL){
		printf("    %s<-%d->%s\n", origin, p->weight, g->nodes[p->sn].data);
		p = p->next;
	}
	putchar('\n');//优化观感
	//3.下一步操作
	PrintPageHead();
	printf("1.修改该建筑信息\t");
	printf("2.修改相关道路信息\n");
	printf("3.删除该建筑/地点\t");
	printf("4.返回上一级\n");
	printf("请输入操作代码：");
	int code;
	scanf_s("%c",&code);
	getchar();
	PrintPageHead();
	switch (code) {
		case '1':
			UpdateBuildingInfo(g, origin);
			ToPicFile(g);
			printf("操作完成，正在返回上一级页面..");
			Sleep(1500);
			break;
		case '2':
			UpdateRoadInfo(g, origin);
			ToPicFile(g);
			printf("操作完成，正在返回上一级页面..");
			Sleep(1500);
			break;
		case '3':
			printf("是否删除相关道路信息？（Y/N）");
			char confirm;
			scanf_s("%c",&confirm);
			getchar();//消耗多余换行
			if(confirm == 'Y' || confirm == 'y'){
				DelBuilding(g, origin, true);
			}else if(confirm == 'N' || confirm == 'n'){
				DelBuilding(g, origin, false);
			} else{
				printf("\n无效的操作码！\n");
				Sleep(1000);
			}
			ToPicFile(g);
			break;
		case '4':
		case 'n':
		case 'N':
			printf("返回上一级页面，请稍后...");
			Sleep(1500);
			return false;
		default:
			printf("无效的操作码！");
	}
	return true;
}
bool UpdateRoadInfo(Graph *g, char *origin){
	//1.相关数据定义及初始化
	char input[30];
	int portsSize = 0;
	char port1 = 0, port2 = 0;
	memset(input, 0, sizeof (input));
	//1.读取用户输入
	printf("\n请输入新道路信息："
		   "（请选中目的建筑；路径长度仅能为正整数；符号均为英文符号；开口方向n/s/w/e均为小写字母）\n"
		   "格式：【建筑名（不可含数字符号字符）-新长度-道路起点开口方向,道路终点开口方向】，缺省的元素无需输入冗余的符号\n"
		   "%s<->", origin);
	scanf_s("%s", input);
	getchar();
	//2.分析用户输入
	if(input[0] == 'n' || input[0] == 'N'){
		return true;
	}
	int length;
	//2.1处理input数组
	char *str1 = strtok(input,"-");	//目的地
	char *str2 = strtok(NULL,"-");	//长度
	char *str3 = strtok(NULL,"-");	//开口方向
	//2.2非法性检验1
	if(str1 == NULL){
		printf("\n错误：数据输入失效！(1072)\n");
		Sleep(1000);
		return false;
	} else if(strlen(str1) > NameSize){
		printf("\n错误：建筑名称过长！(1076)\n");
		Sleep(1000);
		return false;
	}
	//2.3文本转换
	length = parseInt(str2);
	//2.3.1处理port
	if(str3 != NULL){
		portsSize = (int)strlen(str3);
		switch (portsSize) {
			case 3:
				port2 = toLowerDir(str3[2]);
			case 1:
				port1 = toLowerDir(str3[0]);
				break;
			case 2:
				port2 = toLowerDir(str3[1]);
				break;
			default:
				break;
		}
	}
	//2.4非法性校验
	if(getSn(g, str1) == -1){
		printf("\n错误：输入的目标建筑不存在！(1100)\n");
		Sleep(1000);
		return false;
	}
	//3.判断分流
	if(isConnected(g, origin, str1)){
		if(length > 0){	//情况1：边存在，修改权值
			UpdateRoad(g, origin, str1, length, portsSize, port1, port2);
		}else{	//情况2：边存在，删除边
			DelRoad(g, origin, str1);
		}
	}else{	//情况3：边不存在，增加边
		AddRoad(g, origin, str1, length, portsSize, port1, port2);
	}
	return true;
}
bool UpdateBuildingInfo(Graph *g, char *origin){
	//0.变量初始化
	char input[30];
	memset(input, 0, sizeof (input));
	char *newName;
	double x, y;
	//1.读取用户输入【预计用户最多能输入25个字（包括\0）】
	printf("\n请输入新建筑信息：（建筑名最多为8个中文字符；坐标均为正数，仅支持一位小数；符号均为英文符号）\n格式：【新名字（不可含数字符号字符）:新x坐标,新y坐标】，缺省的元素无需输入冗余的符号，输入“n”放弃修改该条目\n");
	scanf_s("%s", input);
	getchar();
	//2.分析用户输入
	//2.1处理input数组
	char *str1 = strtok(input,":");
	char *str2 = strtok(NULL,":");
	bool rename = false, relocated = false, remain = false;
	//2.2非法性校验1
	if(str1 == NULL){
		printf("\n错误：数据输入失效！(1133)\n");
		Sleep(1000);
		return false;
	}
	//2.3分流分析
	if(str1[0] == 'n' || str1[0] == 'N'){	//情况4：不修改，想改简介
		remain = true;
		goto intro;
	}
	if(str2 == NULL && strchr(str1,',') != NULL){	//情况1：不给建筑名，只指定坐标
		x = parseDouble(strtok(str1,","));
		y = parseDouble(strtok(NULL,","));
		relocated = true;
	}else if(str2 == NULL){		//情况2：给定建筑名，不指定坐标
		newName = str1;
		rename = true;
	}else if(str2 != NULL && strchr(str1,',') != NULL ){		//情况3：给定建筑名，给定坐标
		newName = str1;
		rename = true;
		x = parseDouble(strtok(str2,","));
		y = parseDouble(strtok(NULL,","));
		relocated = true;
	}else{
		//2.2非法性校验
		printf("\n错误：输入格式错误！(1153)\n");
		Sleep(1000);
		return false;
	}
	intro:
	//3.多种情况下的不同参数列表
	if(rename && relocated){	//情况1：重命名，重定位
		UpdateBuilding(g, origin, true, true, newName, x, y);
	}else if(rename && !relocated){	//情况2：重命名
		UpdateBuilding(g, origin, true, false, newName);
	}else if(!rename && relocated){	//情况3：重定位
		UpdateBuilding(g, origin, false, true, x, y);
	}else if(remain){
		UpdateBuilding(g, origin, false, false);
	}
	return true;
}
bool CreateBat(){
	FILE *batFile = fopen("nav.bat", "w");
	if(batFile == NULL){
		printf("error!");
		return false;
	}
	//隐藏bat
	fprintf(batFile, "@echo off\nif \"%%1\" == \"h\" goto begin\nmshta vbscript:createobject(\"wscript.shell\").run(\"\"\"%%~nx0\"\"h\",0)(window.close)&&exit\n");
	fprintf(batFile, ":begin\n");
	//补充bat
	fprintf(batFile, "cd ./graphviz_lite2\n");
	fprintf(batFile, "set startDir=%%cd%%\n");
	fprintf(batFile, "npm run serve\n");
	fclose(batFile);
	return true;
}
bool ProgramSetting(){
	setCursorVisible(false);
	//0.输入页面头
	PrintPageHead();
	printf("\nCampus Navigation System 校园导航系统应用设置..\n");
	printf("键盘按下相应设置项数字可对设置进行修改\n\n");
	//1.当前设置展示
	printf("1.退出程序后保留缓存文件\t当前为：");
	if(settings.Save_Temp_After_Quit){
		printf("保留\n");
	}else{
		printf("不保留\n");
	}
	printf("2.生成的本地无向图文件名\t当前为：%s\n", settings.Png_File_Name);
	printf("3.无向图基础文件地址\t\t当前为：%s\n", settings.Basis_File_Name);
	printf("4.看图模式\t\t\t当前为：");
	if(settings.Web_Mode == 1) {
		printf("网页模式1\n");
	}else if(settings.Web_Mode == 2){
		printf("网页模式2（需自备node.js）\n");
	}else{
		printf("本地模式\n");
	}
	printf("5.图像跟随程序变化\t\t当前为：");
	if(settings.Update_Pic_In_Time){
		printf("跟随\n");
	}else{
		printf("不跟随\n");
	}
	printf("\n\n\n【退出设置（不保存）[ESC]\t保存并退出设置[ENTER]】\n");
	
	//2.设置修改
	int key;
	bool tempSave = settings.Save_Temp_After_Quit;
	int tempMode = settings.Web_Mode;
	while(1){
		if (kbhit() != 0) {
			key = getch();
			switch (key) {
				case 49:	//1
					tempSave = !tempSave;
					gotoxy(40,12);
					if(tempSave){
						printf("保留    ");
					}else{
						printf("不保留  ");
					}
					break;
				case 50:	//2
				case 51:	//3
					gotoxy(51,14);
					printf("暂不支持修改！");
					break;
				case 52:	//4
					gotoxy(40,15);
					tempMode++;
					tempMode = tempMode % 3;
					if(tempMode){
						if(tempMode == 1){
							printf("网页模式1                \n");
						}else{
							printf("网页模式2（需自备node.js）\n");
						}
					}else{
						printf("本地模式                 \n");
					}
					break;
				case 53:	//5
					gotoxy(51,16);
					printf("暂不支持修改！");
					break;
				case 13:	//ENTER
					gotoxy(20,17);
					printf("已保存设置！");
					settings.Save_Temp_After_Quit = tempSave;
					settings.Web_Mode = (short)tempMode;
					if(settings.Web_Mode == 2){
						CreateBat();
					}
				case 27:	//ESC
					gotoxy(20,18);
					goto settingEnd;
			}
		}
	}
	settingEnd:
	setCursorVisible(true);
	return true;
}
bool DestroyProgram(){
	if(!settings.Save_Temp_After_Quit){
		gotoxy(0,5);
		printf("\t\t清除缓存文件....\n");
		remove(settings.Png_File_Name);
		remove(settings.Dot_File_Name);
		remove("./graphviz_lite1/navigation.js");
		remove("./graphviz_lite2/src/navigation.js");
		Sleep(1000);
	}
	if(isNodeAlive){
		printf("\t程序将停止所有node进程，按任意键确认退出\n\n");
		system("pause");
		system("taskkill /f /im node.exe");
	}
	return true;
}
bool PrintPageHead() {
	for (int i = 0; i < 20; ++i) {
		printf("===");
	}
	putchar('\n');
	return true;
}
bool ReturnGraphFile(Graph *g){
	FILE *basis_return = fopen("basis_return.txt","w");
	//异常校验
	if(basis_return == NULL){
		printf("\n错误：无法创建basis文件！（1308）\n");
		return false;
	}
	//1.还原node数组
	for (int i = 0; i < g->nodeNum; ++i) {
		if(g->nodes[i].isValid){
			fprintf(basis_return,"%d:%s:%.2lf,%.2lf\n",i, GbkToUtf8(g->nodes[i].data),g->nodes[i].pos_x,g->nodes[i].pos_x);
		}
	}
	//2.还原边，只看adj矩阵的上三角，看port矩阵的全部
	for (int i = 0; i < MaxNum; ++i) {
		for (int j = i + 1; j < MaxNum; ++j) {
			if(g->adjMatrix[i][j] != 0){
				fprintf(basis_return,"%d-%d-%d",i,j,g->adjMatrix[i][j]);
				if(g->adjGraph->ports[i][j] != 0 || g->adjGraph->ports[j][i] != 0)
					fprintf(basis_return, "-");
				if(g->adjGraph->ports[i][j] != 0)
					fprintf(basis_return,"%c",g->adjGraph->ports[i][j]);
				if(g->adjGraph->ports[j][i] != 0)
					fprintf(basis_return,",%c",g->adjGraph->ports[j][i]);
				fprintf(basis_return,"\n");
			}
		}
	}
	//3.OK
	fclose(basis_return);
	printf("\t\t  文件写入完毕\n");
	Sleep(1000);
	return true;
}