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
//��ʼ�����
bool InitProgram(Graph **graph);
bool InitNode(Graph *graph, char *str);
bool InitEdge(int adjMatrix[][MaxNum], AdjGraph *adjGraph, char *str, int *total);
bool DestroyProgram();
//�ļ����
bool ReadGraphFile(Graph *g);	bool ReturnGraphFile(Graph *g);
bool ToPicFile(Graph *graph);
bool CreateBat();
//С����
bool isConnected(Graph *g, char *origin, char *dest);
int getSn(Graph *g, char *name);
bool PrintPageHead();
/*�龰����������*/
//��Ϣ���
void menu(Graph *g);
bool SelectMap(Graph *g);
bool AddBuildingInfo(Graph *g);
bool ReachableTest(Graph *g);
bool SelectBuilding(Graph *g);
bool ProgramSetting();
bool UpdateRoadInfo(Graph *g, char *origin);
bool UpdateBuildingInfo(Graph *g, char *origin);
//�����滮���
bool AddBuilding(Graph *g, char *name, bool isLocated, ...);	bool AddAdjNode(AdjGraph *adj, int srcSn, int resSn, int weight);
bool DelBuilding(Graph *g, char *name, bool isDelRoad);			bool DelAdjNodeInList(headNode *head, int aim);//ɾ�������Ҫ��
bool UpdateBuilding(Graph *g, char *name, bool isRename, bool isRelocated, ...);	bool UpdateAdjNodeInList(headNode *head, int aim, int weight);
bool AddRoad(Graph *g, char *origin, char *end, int length, bool isSetPorts, ...);
bool DelRoad(Graph *g, char *origin, char *end);
bool UpdateRoad(Graph *g, char *origin, char *end, int newLength, bool isSetPorts, ...);
//·�����
bool Navigate(Graph *g);
int FShortestPaths(Graph *g, int origin, int dest, int path[], int pathWeight[]);
bool isReachable(Graph *g, char *origin, char *dest);
/*һЩȫ�ֱ���*/
//��λ����
Link *emptySpot;
//һһ��Ӧ����
int basisSn[MaxNum];
//���м�סnodejs�Ƿ��
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
	//0.������ر���
	int code;
	//��ӡͷ
	PrintPageHead();
	printf("\n\t1.�鿴��ǰ��ͼ\t");
	printf("\t2.��ѯ������Ϣ\t\n");
	printf("\t3.�ɴ���У��\t");
	printf("\t4.����·�߹滮\t\n");
	printf("\t5.Ӧ������\t");
	printf("\t6.�˳�����\n\n");
	printf("\t\t������������룺");
	scanf_s("%c", &code);
	getchar();//���Ķ��໻�з���

	switch (code) {
		case '1':
			//�û���ѡ�������������˵�
			while(!SelectMap(g));
			printf("\n����������׼���ص���ҳ.");
			Sleep(2000);
			break;
		case '2':
			//ѡ�������������˵�
			while(!SelectBuilding(g));
			printf("\n����������׼���ص���ҳ.");
			Sleep(2000);
			break;
		case '3':
			//�ɴ���У�飬�������˵�
			while(!ReachableTest(g));
			printf("\n����������׼���ص���ҳ.");
			Sleep(2000);
			break;
		case '4':
			//�������������˵�
			while(!Navigate(g));
			printf("\n·�߹滮����~��������ص����˵�.\n");
			system("pause");
			break;
		case '5':
			//���ó��򣬷������˵�
			ProgramSetting();
			printf("\nӦ�����ý���.");
			Sleep(2000);
			break;
		case '6':
		case 'n':
		case 'N':
			ReturnGraphFile(g);
			//�˳�����
			DestroyProgram();
			printf("\t\t\t�ټ���");
			Sleep(1500);
			exit(0);
		default:
			printf("�޷�ʶ��Ĳ����룡");
			Sleep(2000);
	}
	system("cls");
}
bool MakeStatistic(Graph *g){
	//���������ҳ��ͷ������
	system("cls");
	PrintPageHead();
	printf("\n��ǰУ�ڹ��н���/��־�Եص㣺 %d ��\n", g->nodeNum);
	printf("\n��ǰУ�ڹ��е�·�� %d ��\n", g->edgeNum);
	return true;
}
bool SelectMap(Graph *g){
	//0.����������ļ�ͷ����ӡ�²˵�
	system("cls");
	PrintPageHead();
	printf("�鿴��ǰ��ͼ..\n\n");
	printf("1.�鿴��ǰ��ͼ��Ϣ\t");
	printf("2.����У԰����\n");
	printf("3.�鿴��ͼ����ͳ��\t");
	printf("4.������һ���˵�\n\n");
	printf("������������룺");
	//1.���岢��ʼ������
	int code;
	char startPicCmd[50];
	memset(startPicCmd, 0, sizeof(startPicCmd));
	strcat(startPicCmd, "start ");
	//2.��ȡ�û���Ϣ
	scanf_s("%c",&code);
	getchar();
	//3.�����û���Ϣ������
	switch (code) {
		case '1':	//�鿴��ͼ
			//�������ļ�����������ȷ���򿪷�ʽ
			ToPicFile(g);
			if(settings.Web_Mode){
				if(settings.Web_Mode == 1){	//��ҳ1-������ҳ
					strcat(startPicCmd, settings.Webpic1_File_Name);
				}else if(settings.Web_Mode == 2){	//��ҳ2-vue-cli��ҳ
					strcat(startPicCmd, settings.Webpic2_File_Name);
					if(!isNodeAlive){	//nodejsû���ſ�
						system("start nav.bat");
						isNodeAlive = true;
						printf("\n���Ժ�...��Ϊ�˸�node.js�㹻������ʱ�䣬�ȴ�8�룩");
						Sleep(8000);
					}
				}
			}else{	//���ر�����
				strcat(startPicCmd, settings.Png_File_Name);
			}
			//�򿪲��ĵ��ļ���������ҳ2���Ǿ�̬��
			system(startPicCmd);
			printf("\n\tͼƬ�Ѵ�~�����������..\n");
			system("pause");
			return false;
		case '2':	//���ӽ���
			while(!AddBuildingInfo(g));
			printf("\n����������׼��������һ��ҳ��..");
			Sleep(2000);
			return false;
		case '3':	//ͳ��
			MakeStatistic(g);
			printf("\nͳ�ƽ���~��������ص���һ��ҳ��..\n");
			system("pause");
			return false;
		case '4':
		case 'n':
		case 'N':
			break;
		default:
			printf("\n��Ч�Ĳ����룡\n");
			Sleep(2000);
			return false;
	}
	return true;
}
bool AddBuildingInfo(Graph *g){
	//0.���������ҳ��ͷ
	system("cls");
	PrintPageHead();
	printf("����У԰����...�����롰n��������һ���˵���\n");
	//1.������ز�������ʼ����Ԥ�����ռ27��
	char input[30];
	memset(input, 0, sizeof (input));
	char name[NameSize];
	memset(name, 0, sizeof (name));
	double x, y;
	//1.��ȡ�û�����
	printf("\n�������½�����Ϣ�������������Ϊ8�������ַ��Ҳ��ɺ����ַ��ţ������Ϊ��������֧��һλС�������ž�ΪӢ�ķ��ţ�\n��ʽ�����½�����:x����,y���꡿��ȱʡ��Ԫ��������������ķ���\n");
	scanf_s("%s", input);
	getchar();
	//2.�����û�����
	if(input[0] == 'n' || input[0] == 'N'){
		return true;
	}
	//2.1����input����
	char *str1 = strtok(input,":");
	char *str2 = strtok(NULL,":");
	bool located = false;
	//2.2�Ƿ���У�飺�ǿ�У�飬�ظ���У������һ��������
	if(str1 == NULL){
		printf("\n������������ʧЧ��(231)\n");
		Sleep(1000);
		return false;
	}else if(strlen(str1) > NameSize){
		printf("\n���󣺽������ƹ�����(235)\n");
		Sleep(1000);
		return false;
	}
	//2.3�����û�����
	if(str2 == NULL){		//���1����������������ָ������
		strcpy(name, str1);
	}else if(str2 != NULL){		//���2����������������������
		strcpy(name, str1);
		x = parseDouble(strtok(str2,","));
		y = parseDouble(strtok(NULL,","));
		located = true;
	}
	//3.�����ݽ���ҳ����ת
	if(AddBuilding(g, name, located, x, y)){
		printf("\n������ӳɹ���\n");
		Sleep(1500);
		ToPicFile(g);
	}
	return false;
}
bool ReachableTest(Graph *g){
	//0.���������ҳ��ͷ
	system("cls");
	PrintPageHead();
	printf("�ɴ���У��..\n");
	//1.��ȡ�û���Ϣ
	char *origin;
	char *dest;
	//1.1��ȡ��Ϣ	//1.2������һ��
	printf("��������ʼ�أ�");
	scanf_s("%s", origin);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	printf("������Ŀ�ĵأ�");
	scanf_s("%s", dest);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	//1.4�Ƿ���У��
	if(origin == NULL || dest == NULL){
		printf("\n������������ʧЧ��(279)\n");
		Sleep(1000);
		return false;
	}
	if(strlen(origin) > NameSize || strlen(dest) > NameSize){
		printf("\n���󣺽�����������(284)\n");
		Sleep(1000);
		return false;
	}
	if(getSn(g,origin) == -1){
		printf("\n������ʼ����/�ص㲻���ڣ�(289)\n");
		Sleep(1000);
		return false;
	}else if(getSn(g,dest) == -1){
		printf("\n�����յ㽨��/�ص㲻���ڣ�(293)\n");
		Sleep(1000);
		return false;
	}
	//2.�жϿɴ�		//3.����
	if(isReachable(g, origin, dest)){
		printf("�ɴ");
	}else{
		printf("���ɴ");
	}
	Sleep(2000);
	return false;
}
bool InitProgram(Graph **graph) {
	//0.�������ó�ʼ��
	//0.1���
	memset(settings.Basis_File_Name, 0, sizeof(settings.Basis_File_Name));
	memset(settings.Png_File_Name, 0, sizeof(settings.Png_File_Name));
	memset(settings.Dot_File_Name, 0, sizeof(settings.Dot_File_Name));
	memset(settings.Js_File_Name, 0, sizeof(settings.Js_File_Name));
	memset(settings.Webpic1_File_Name, 0, sizeof(settings.Webpic1_File_Name));
	memset(settings.Webpic2_File_Name, 0, sizeof(settings.Webpic2_File_Name));
	//0.2��ֵ
	strcpy(settings.Basis_File_Name, "basis.txt");	//Ĭ�϶�ȡbasis.txt
	settings.Save_Temp_After_Quit = true;	//Ĭ�ϲ��������
	settings.Update_Pic_In_Time = true;		//Ĭ���Զ�����ͼ
	strcpy(settings.Png_File_Name, "navigation.png");
	strcpy(settings.Webpic1_File_Name, "./graphviz_lite1/index.html");
	strcpy(settings.Webpic2_File_Name, "http://localhost:22334/");
	settings.Web_Mode = 0;	//Ĭ�ϴ򿪷�ʽΪ���ش�
	strcpy(settings.Dot_File_Name, "navigation.dot");
	strcpy(settings.Js_File_Name, "./graphviz_lite1");
	//1.��ʼ��ͼ
	(*graph) = malloc(sizeof(Graph));
	(*graph)->dependency = fopen(settings.Basis_File_Name, "r");
	(*graph)->edgeNum = 0;
	(*graph)->nodeNum = 0;
	memset((*graph)->adjMatrix, 0, sizeof((*graph)->adjMatrix));
	//2.��ʼ���������ֵ
	for (int i = 0; i < MaxNum; ++i) {
		memset((*graph)->nodes[i].data, 0, sizeof((*graph)->nodes[i].data));
		(*graph)->nodes[i].isValid = false;
		(*graph)->nodes[i].pos_x = -1.0;
		(*graph)->nodes[i].pos_y = -1.0;
		memset((*graph)->nodes[i].intro, 0, sizeof((*graph)->nodes[i].intro));
	}
	//3.��ʼ��ͼ���ڽӱ�ʽ�淨
	(*graph)->adjGraph = (AdjGraph *) malloc(sizeof(AdjGraph));
	for (int i = 0; i < MaxNum; ++i) {
		(*graph)->adjGraph->list[i].first = NULL;
	}
	memset((*graph)->adjGraph->ports, 0, sizeof((*graph)->adjGraph->ports));

	//4.��ʼ�����ж���
	emptySpot = InitQueue(&emptySpot);
	for (int i = 0; i < MaxNum; ++i) {
		EnQueue(emptySpot,i);
	}

	//5.����ڽӱ�
	ReadGraphFile(*graph);
	//6.����ͼ
	CreateAdjGraph((*graph)->adjGraph, (*graph)->adjMatrix, (*graph)->nodeNum, (*graph)->edgeNum);
	return true;
}
bool ToPicFile(Graph *graph) {
	//�п�����JS��Ҳ�п�����dot
	FILE *navFile = NULL;
	if(settings.Web_Mode){	//��ҳģʽ��JS����£���дһЩ
		if(settings.Web_Mode == 1){	//��ҳģʽ1�������ݲ�����get����
			navFile = fopen("./graphviz_lite1/navigation.js", "w");
		}else if(settings.Web_Mode == 2){//��ҳģʽ2ֱ��export�޶��෽��
			navFile = fopen("./graphviz_lite2/src/navigation.js", "w");
			fprintf(navFile, "export ");
		}
		fprintf(navFile, "var nav = ' ");
	}else{	//����ģʽ��DOT�����
		navFile = fopen("navigation.dot", "w");
	}

	if (navFile == NULL) {
		printf("error!");
		return false;
	}
	//�ļ�ͷ
	fprintf(navFile, "graph G{"
					 "layout=fdp;"
					 "node[shape=rect, fontname=KaiTi];"
					 "splines=curved;");
	//�����
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
	//����ߣ��ٽ����ֻ������������
	for (int i = 0; i < MaxNum; ++i) {
		for (int j = i + 1; j < MaxNum; ++j) {
			weight = graph->adjMatrix[i][j];
			if (weight > 0) {
				fprintf(navFile, "%d--%d[label=%d", i, j, weight);
				if ((tail = graph->adjGraph->ports[i][j]) != 0) {    //��������tailportsֵ
					fprintf(navFile, ",tailport=%c", tail);
				}
				if ((head = graph->adjGraph->ports[j][i]) != 0) {    //��������headportsֵ
					fprintf(navFile, ",headport=%c", head);
				}
				fprintf(navFile, "];");
			}
		}
	}
	fprintf(navFile, "}");
	if(settings.Web_Mode){
		fprintf(navFile, " ';\n");
		if(settings.Web_Mode == 1){	//��ҳģʽ1��дһ��get����
			fprintf(navFile,"\nfunction getNav(){return nav;}");
		}
	}
	fclose(navFile);
	//����dot�ļ�
	if(!settings.Web_Mode){	//����ģʽ��DOT����£�����ͼ
		system("dot -Tpng navigation.dot -o navigation.png");
	}
	return true;
}
bool ReadGraphFile(Graph *g) {
	//1.�������
	char tempStr[30];
	FILE *reader = g->dependency;	/*�ļ�����*/
	if (reader == NULL) {
		/*�޷����ļ�*/
		system("title Campus Navigation System");
		return false;
	}else{
		/*�������ļ�*/
		system("title Campus Navigation System - \"basis.txt\"");
	}
	//2.�ļ���ȡ
	while (!feof(reader)) {    /*��û�ж�ȡ���ļ�ĩ�ˣ�������ѭ�����й���*/
		fscanf(reader, "%s", tempStr);    /*��ȡһ�����ݴ���temp���ļ������Զ��ƶ�����һ��*/
		//3.��������ð��˵��Ϊ��㶨�壻�м���˵��Ϊ�߶���
		if(strchr(tempStr, ':') != NULL){
			InitNode(g, tempStr);
		}else if(strchr(tempStr, '-') != NULL){
			InitEdge(g->adjMatrix, g->adjGraph, tempStr, &g->edgeNum);
		}else{
			//�п���������
		}
	}
	fclose(reader);
	return true;
}
bool InitNode(Graph *graph, char *str) {
	//1.���������sn�ţ�������������λ�ã���ѡ����ָ��
	int sn;
	char *name = NULL;
	char *xy = NULL;
	double x = -1.0, y = -1.0;
	bool isLocated = false;
	//2.�ָ�����ַ�����sn�����֣�����
	sn = parseInt(strtok(str,":"));
	name = strtok(NULL,":");
	xy = strtok(NULL,":");
	if(xy != NULL){
		isLocated = true;
		x = parseDouble(strtok(xy,","));
		y = parseDouble(strtok(NULL,","));
	}
	//3.ת�����루���UTF8->�ڴ�GBK��
	name = Utf8ToGbk(name);

	//����������·��µ�sn�ţ����ں���Ҫ�ã����û�����basisSn��ͬ���������
	basisSn[seeFront(emptySpot)] = sn;
	//��ӽ���
	AddBuilding(graph,name,isLocated, x, y);

	return true;
}
bool InitEdge(int adjMatrix[][MaxNum], AdjGraph *adjGraph, char *str, int *edgeNum) {
	(*edgeNum)++;
	//1.�����������ʼ��
	int sn1, sn2, weight;
	char *tempPorts = NULL;
	//2.�ָ��ַ�����˫sn��Ȩֵ��ports
	sn1 = parseInt(strtok(str, "-"));
	sn2 = parseInt(strtok(NULL, "-"));
	weight = parseInt(strtok(NULL, "-"));
	tempPorts = strtok(NULL, "-");
	//3.���μӹ���˫sn��ȨֵУ�飬ports����
	if(sn1 == -1 || sn2 == -1 || weight == -1){
		//����
	}

	int portSize = 0;
	if(tempPorts != NULL){
		portSize = (int)strlen(tempPorts);
	}
	//4.������
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
	//0.������У��
	int dup = 0;
	//�ӿ�˵�����ٿ��õ�SN�����޷�����µĽ��
	if(isEmpty(emptySpot)){
		printf("\n���󣺵�ͼ�ռ䲻�㣬���ʧ�ܣ�(519)\n");
		Sleep(1000);
		return false;
	}else if( (dup = getSn(g, name)) != -1){
		printf("\n�����Ѵ���ͬ������/�ص㣬���ʧ�ܣ�(523)\n");
		Sleep(1000);
		return false;
	}
	//0.1���궨�塢��ֵ��У��
	double x = -1.0, y = -1.0;
	if(isLocated){
		//��ȡ�β��б�
		va_list vaList;
		//������ʼ��
		va_start(vaList,2);
		x = va_arg(vaList,double);
		y = va_arg(vaList,double);
		va_end(vaList);
		//����У��
		if(x < 0 || y < 0){
			printf("\n�������������ʽ�������ʧ�ܣ�(539)\n");
			Sleep(1000);
			return false;
		}
		if(x > 14 || y > 15){
			printf("\n�������곬��У԰��Χ�����ʧ�ܣ�(544)\n");
			Sleep(1000);
			return false;
		}
	}
	//1.�����±���г���
	int newNodeSn = DeQueue(emptySpot)->sn;
	//2.�����������Ϣ/λ����Ϣ
	strcpy(g->nodes[newNodeSn].data, name);
	g->nodes[newNodeSn].isValid = true;
	g->nodes[newNodeSn].pos_x = x;
	g->nodes[newNodeSn].pos_y = y;
	//3.���½����
	g->nodeNum++;

	return true;
}
int getSn(Graph *g, char *name){
	//�ң�û�ҵ�����Sn
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
	//ɾ��������1.����ڽӱ��е�������Ϣ��2.�ڽ�ͼ���н�������3.��������е���Ϣ��4.sn���
	int concernSn[MaxNum];
	int concern = 0;
	//1.�ҽ��sn
	int nodeSn = getSn(g, name);
	//û�ҵ����˳�
	if(nodeSn == -1){
		printf("\n���󣺽��������ڣ�(580)\n");
		Sleep(1000);
		return false;
	}
	//2.��������Ϣ
	//2.1�ڽӱ�
	//Ŀ��Ҫ����յ���
	headNode *aimRow = &g->adjGraph->list[nodeSn];
	//���Ŀ���Ƿ�Ϊ��
	ArcNode *first = g->adjGraph->list[nodeSn].first;
	while(first != NULL){
		//0.���¹�����Ϣ
		concernSn[concern++] = first->sn;
		//1.ɾ������
		DelAdjNodeInList(&g->adjGraph->list[first->sn], nodeSn);
		//2.ɾ�Լ�
		DelAdjNodeInList(aimRow, first->sn);
		//3.���¸���Ϊ��һ��
		first = g->adjGraph->list[nodeSn].first;
	}
	//2.2�ڽӾ����port�������������Ϣ��������·�������ิ��ճ��·�������������ѡ���Բ�·
	for (int i = 0; i < concern; ++i) {
		if(!isDelRoad){//��ɾ·�����¼���·�ߺ���ɾ��
			for (int j = 0; j < concern; ++j) {//����concern����
				//���ڲ����ڵ�������±ߣ�����port��·�������߲���
				if(!isConnected(g, g->nodes[concernSn[i]].data, g->nodes[concernSn[j]].data)){
					//����Ȩ������·����Ȩֵ���
					//����ports���������ڣ�����Ϊ0�ĺ���
					AddRoad(g, g->nodes[concernSn[i]].data, g->nodes[concernSn[j]].data, g->adjMatrix[nodeSn][concernSn[i]]+g->adjMatrix[nodeSn][concernSn[j]] ,true, g->adjGraph->ports[concernSn[i]][nodeSn], g->adjGraph->ports[concernSn[j]][nodeSn]);
				}
			}
		}
		g->adjMatrix[nodeSn][concernSn[i]] = 0;
		g->adjMatrix[concernSn[i]][nodeSn] = 0;
		g->adjGraph->ports[nodeSn][concernSn[i]] = 0;
		g->adjGraph->ports[concernSn[i]][nodeSn] = 0;
	}
	//2.3��յ���Ϣ
	memset(g->nodes[nodeSn].data, 0, sizeof(g->nodes[nodeSn].data));
	g->nodes[nodeSn].pos_x = -1.0;
	g->nodes[nodeSn].pos_y = -1.0;
	g->nodes[nodeSn].isValid = false;
	//3.��sn���ã����
	EnQueue(emptySpot,nodeSn);
	//4.�����ʾ��Ϣ
	printf("\n����/�ص�ɾ���ɹ���\n");
	return true;
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
bool DelAdjNodeInList(headNode *head, int aim){
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
bool UpdateAdjNodeInList(headNode *head, int aim, int weight){
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
bool UpdateBuilding(Graph *g, char *name, bool isRename, bool isRelocated, ...){
	char str[MaxIntro];
	memset(str, 0, sizeof(str));
	bool isIntro = false;
	//0.���ݹ淶��У��1
	if(strlen(name) > NameSize){
		printf("\n���󣺽������ƹ�����(694)\n");
		Sleep(1000);
		return false;
	}
	//2.���½���������/��λ�ü���
	int nodeSn = getSn(g, name);
	//2.1У��2
	if(nodeSn == -1){
		printf("\n����δ�ҵ�Ŀ�꽨����(702)\n");
		Sleep(1000);
		return false;
	}
	//3.�ļ��
	printf("�����뽨�������Ϣ����������25�����ģ��������ύ��n����\n");
	scanf_s("%s", str);
	getchar();
	if(!(str == NULL || str[0] == 'n' || str[0] == 'N')){
		if(strlen(str) < MaxIntro){
			strcpy(g->nodes[nodeSn].intro, str);
			isIntro = true;
		}else{
			printf("\n���󣺼�����ݹ�����(715)\n");
			Sleep(1000);
			return false;
		}
	}
	if(isRename || isRelocated || isIntro){
		//��ȡ�β��б�
		va_list vaList;
		//������ʼ��
		if(isRename && isRelocated){
			va_start(vaList,3);
		}else if(!isRename){
			va_start(vaList,2);
		} else {
			va_start(vaList,1);
		}
		//����
		char *newName;
		if(isRename){
			newName = va_arg(vaList, char*);
			if(strlen(newName) > NameSize){
				printf("\n���󣺽������ƹ�����(736)\n");
				Sleep(1000);
				return false;
			}
		}
		//��λ��
		double x, y;
		if(isRelocated){
			x = va_arg(vaList,double);
			y = va_arg(vaList,double);
			if(x > 14 || y > 15){
				printf("\n�������곬��У԰��Χ�����ʧ�ܣ�(747)\n");
				Sleep(1000);
			}
			if(x < 0 || y < 0){
				printf("\n�������������ʽ�������ʧ�ܣ�(751)\n");
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

		printf("\n�޸ĳɹ���\n");
		return true;
	}
	printf("\nδ�����޸�\n");
	return false;
}
bool AddRoad(Graph *g, char *origin, char *end, int length, bool isSetPorts, ...){
	//��·��1.����������±ꣻ2.�ڽӾ���˫���ֵ��2.1��·�߲��ɲ�����3.�����������㣻
	int sn1 = getSn(g,origin);
	int sn2 = getSn(g,end);
	if(sn1 == -1 && sn2 == -1){
		printf("\n����δ�ҵ�ָ������/�ص㣡(775)\n");
		Sleep(1000);
		return false;
	}
	//2.�����ڽӾ���
	g->adjMatrix[sn1][sn2] = length;
	g->adjMatrix[sn2][sn1] = length;
	//���ָ���˷���
	if(isSetPorts){
		//��ȡ�β��б�
		va_list vaList;
		//������ʼ��
		va_start(vaList,2);
		//���������
		char ch1, ch2;
		ch1 = (char)va_arg(vaList,int);
		ch2 = (char)va_arg(vaList,int);
		if(ch1 != 0)	//����0������
			g->adjGraph->ports[sn1][sn2] = ch1;
		if(ch2 != 0)
			g->adjGraph->ports[sn2][sn1] = ch2;
		va_end(vaList);
	}
	//3.����ӽ�㣬˫�򶼼�
	AddAdjNode(g->adjGraph,sn1,sn2,length);

	return true;
}
bool DelRoad(Graph *g, char *origin, char *end){
	//ɾ·��1.����������±ꣻ2.�ڽӾ���˫���ֵ��2.1û·�߲��ɲ�����3.����ɾ������㣻
	int sn1 = getSn(g, origin);
	int sn2 = getSn(g, end);
	if(sn1 == -1 && sn2 == -1){
		printf("\n����δ�ҵ�ָ������/�ص㣡(808)\n");
		Sleep(1000);
		return false;
	}
	//����˫��ɾֵ
	g->adjMatrix[sn1][sn2] = 0;
	g->adjMatrix[sn2][sn1] = 0;
	//����port˫��ɾֵ
	g->adjGraph->ports[sn1][sn2] = 0;
	g->adjGraph->ports[sn2][sn1] = 0;
	//ɾ���
	DelAdjNodeInList(&g->adjGraph->list[sn1],sn2);
	DelAdjNodeInList(&g->adjGraph->list[sn2],sn1);
	return true;
}
bool UpdateRoad(Graph *g, char *origin, char *end, int newLength, bool isSetPorts, ...){
	//����·��1.����������±ꣻ2.1��Ȩ���ڽӾ���˫���ֵ�����ĵ���ɾ����·��������·
	int sn1 = getSn(g, origin);
	int sn2 = getSn(g, end);
	if(sn1 == -1 && sn2 == -1){
		printf("\n����δ�ҵ�ָ������/�ص㣡(828)\n");
		Sleep(1000);
		return false;
	}
	//2.�����ڽӾ���
	g->adjMatrix[sn1][sn2] = newLength;
	g->adjMatrix[sn2][sn1] = newLength;
	//3.������
	UpdateAdjNodeInList(&g->adjGraph->list[sn1],sn2,newLength);
	UpdateAdjNodeInList(&g->adjGraph->list[sn2],sn1,newLength);
	//4.��ǿ���
	if(isSetPorts){
		//��ȡ�β��б�
		va_list vaList;
		//������ʼ��
		va_start(vaList,2);
		//���������
		g->adjGraph->ports[sn1][sn2] = va_arg(vaList,int);
		g->adjGraph->ports[sn2][sn1] = va_arg(vaList,int);
		va_end(vaList);
	}
	return true;
}
bool isReachable(Graph *g, char *origin, char *dest){
	//�������
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
	//0.����������ļ�ͷ
	system("cls");
	PrintPageHead();
	printf("����·�߹滮..�����롰n��������һ���˵���\n");
	PrintPageHead();
	//1.��ȡ�û���Ϣ
	char *origin;
	char *dest;
	//1.1��ȡ��Ϣ
	again:
	printf("��������ʼ�أ�");
	scanf_s("%s", origin);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	printf("������Ŀ�ĵأ�");
	scanf_s("%s", dest);
	getchar();
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	if(origin == NULL || dest == NULL){
		printf("\n������������ʧЧ��(896)\n");
		Sleep(1000);
		return false;
	}
	//1.2ת��sn
	int sn1 = getSn(g,origin);
	int sn2 = getSn(g, dest);
	//1.3�Ƿ�У��
	if(sn1 == -1 || sn2 == -1){
		if(sn1 == -1){
			printf("\n���󣺽��� %s �����ڣ�(906)\n", origin);
		}
		if(sn2 == -1){
			printf("\n���󣺽��� %s �����ڣ�(909)\n", dest);
		}
		Sleep(1000);
		return false;
	}
	//2.�õ���С·�������������յ㣩
	int path[MaxNum];
	int weight[MaxNum];
	int length = FShortestPaths(g, sn1, sn2, path, weight);
//	//3.�õ�ȫ��·��
//	int paths[MaxNum+1][MaxNum+1];
//	FindAllPaths(g->adjGraph, sn1, sn2, paths);
	//4.���·��
	int totalWeight = 0;
	for (int i = 0; i < length; i++) {
		printf("%s", g->nodes[path[i]].data);
		if(i != length-1){
			printf("<-%d->",weight[i+1]);
			totalWeight += weight[i+1];
		}
	}
	printf("\nȫ�̾���: %d\n", totalWeight);
	printf("������Ϊ����ѯ���Ĵ� %s �� %s �����·����ȫ�̾���\n", origin, dest);
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
	//0.��������ӡҳ��ͷ
	system("cls");
	PrintPageHead();
	printf("��ѯ������Ϣ..�����롰n��������һ���˵���\n");
	//1.��ȡ�û���Ϣ
	char origin[NameSize];
	char destination[NameSize];
	//1.0��ʼ������
	memset(origin, 0, sizeof (origin));
	memset(destination,0,sizeof (destination));
	//1.1��ȡ��Ϣ
	printf("���������ѯ��Ŀ�꽨�����ƣ�");
	scanf_s("%s",origin);
	getchar();//���Ķ��໻�з�
	if(origin[0] == 'n' || origin[0] == 'N'){
		return true;
	}
	//1.2ת��sn
	int sn1 = getSn(g, origin);
	//1.3�Ƿ���У��
	if(sn1 == -1){
		printf("û���ҵ�Ŀ�꽨��/�ص㡰%s����",origin);
		Sleep(1500);
		return false;
	}
	//2.���ڽӱ��д���չʾ
	system("cls");
	PrintPageHead();
	printf("����/�ص���Ϣ...\n");
	printf("�������ƣ�%s\n", origin);
	printf("SN��%d\n", sn1);
	if(g->nodes[sn1].pos_x == -1 || g->nodes[sn1].pos_y == -1){
		printf("λ�����꣺��δ����\n");
	}else{
		printf("λ�����꣺%.2lf,%.2lf\n",g->nodes[sn1].pos_x, g->nodes[sn1].pos_y);
	}
	printf("����/�ص���ܣ�%s\n", g->nodes[sn1].intro);
	printf("������������·���������\n");
	ArcNode *p = g->adjGraph->list[sn1].first;
	//2.1�����ڽ�����
	if(p == NULL){
		printf("\t���ޣ�");
	}
	while(p != NULL){
		printf("    %s<-%d->%s\n", origin, p->weight, g->nodes[p->sn].data);
		p = p->next;
	}
	putchar('\n');//�Ż��۸�
	//3.��һ������
	PrintPageHead();
	printf("1.�޸ĸý�����Ϣ\t");
	printf("2.�޸���ص�·��Ϣ\n");
	printf("3.ɾ���ý���/�ص�\t");
	printf("4.������һ��\n");
	printf("������������룺");
	int code;
	scanf_s("%c",&code);
	getchar();
	PrintPageHead();
	switch (code) {
		case '1':
			UpdateBuildingInfo(g, origin);
			ToPicFile(g);
			printf("������ɣ����ڷ�����һ��ҳ��..");
			Sleep(1500);
			break;
		case '2':
			UpdateRoadInfo(g, origin);
			ToPicFile(g);
			printf("������ɣ����ڷ�����һ��ҳ��..");
			Sleep(1500);
			break;
		case '3':
			printf("�Ƿ�ɾ����ص�·��Ϣ����Y/N��");
			char confirm;
			scanf_s("%c",&confirm);
			getchar();//���Ķ��໻��
			if(confirm == 'Y' || confirm == 'y'){
				DelBuilding(g, origin, true);
			}else if(confirm == 'N' || confirm == 'n'){
				DelBuilding(g, origin, false);
			} else{
				printf("\n��Ч�Ĳ����룡\n");
				Sleep(1000);
			}
			ToPicFile(g);
			break;
		case '4':
		case 'n':
		case 'N':
			printf("������һ��ҳ�棬���Ժ�...");
			Sleep(1500);
			return false;
		default:
			printf("��Ч�Ĳ����룡");
	}
	return true;
}
bool UpdateRoadInfo(Graph *g, char *origin){
	//1.������ݶ��弰��ʼ��
	char input[30];
	int portsSize = 0;
	char port1 = 0, port2 = 0;
	memset(input, 0, sizeof (input));
	//1.��ȡ�û�����
	printf("\n�������µ�·��Ϣ��"
		   "����ѡ��Ŀ�Ľ�����·�����Ƚ���Ϊ�����������ž�ΪӢ�ķ��ţ����ڷ���n/s/w/e��ΪСд��ĸ��\n"
		   "��ʽ���������������ɺ����ַ����ַ���-�³���-��·��㿪�ڷ���,��·�յ㿪�ڷ��򡿣�ȱʡ��Ԫ��������������ķ���\n"
		   "%s<->", origin);
	scanf_s("%s", input);
	getchar();
	//2.�����û�����
	if(input[0] == 'n' || input[0] == 'N'){
		return true;
	}
	int length;
	//2.1����input����
	char *str1 = strtok(input,"-");	//Ŀ�ĵ�
	char *str2 = strtok(NULL,"-");	//����
	char *str3 = strtok(NULL,"-");	//���ڷ���
	//2.2�Ƿ��Լ���1
	if(str1 == NULL){
		printf("\n������������ʧЧ��(1072)\n");
		Sleep(1000);
		return false;
	} else if(strlen(str1) > NameSize){
		printf("\n���󣺽������ƹ�����(1076)\n");
		Sleep(1000);
		return false;
	}
	//2.3�ı�ת��
	length = parseInt(str2);
	//2.3.1����port
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
	//2.4�Ƿ���У��
	if(getSn(g, str1) == -1){
		printf("\n���������Ŀ�꽨�������ڣ�(1100)\n");
		Sleep(1000);
		return false;
	}
	//3.�жϷ���
	if(isConnected(g, origin, str1)){
		if(length > 0){	//���1���ߴ��ڣ��޸�Ȩֵ
			UpdateRoad(g, origin, str1, length, portsSize, port1, port2);
		}else{	//���2���ߴ��ڣ�ɾ����
			DelRoad(g, origin, str1);
		}
	}else{	//���3���߲����ڣ����ӱ�
		AddRoad(g, origin, str1, length, portsSize, port1, port2);
	}
	return true;
}
bool UpdateBuildingInfo(Graph *g, char *origin){
	//0.������ʼ��
	char input[30];
	memset(input, 0, sizeof (input));
	char *newName;
	double x, y;
	//1.��ȡ�û����롾Ԥ���û����������25���֣�����\0����
	printf("\n�������½�����Ϣ�������������Ϊ8�������ַ��������Ϊ��������֧��һλС�������ž�ΪӢ�ķ��ţ�\n��ʽ���������֣����ɺ����ַ����ַ���:��x����,��y���꡿��ȱʡ��Ԫ��������������ķ��ţ����롰n�������޸ĸ���Ŀ\n");
	scanf_s("%s", input);
	getchar();
	//2.�����û�����
	//2.1����input����
	char *str1 = strtok(input,":");
	char *str2 = strtok(NULL,":");
	bool rename = false, relocated = false, remain = false;
	//2.2�Ƿ���У��1
	if(str1 == NULL){
		printf("\n������������ʧЧ��(1133)\n");
		Sleep(1000);
		return false;
	}
	//2.3��������
	if(str1[0] == 'n' || str1[0] == 'N'){	//���4�����޸ģ���ļ��
		remain = true;
		goto intro;
	}
	if(str2 == NULL && strchr(str1,',') != NULL){	//���1��������������ָֻ������
		x = parseDouble(strtok(str1,","));
		y = parseDouble(strtok(NULL,","));
		relocated = true;
	}else if(str2 == NULL){		//���2����������������ָ������
		newName = str1;
		rename = true;
	}else if(str2 != NULL && strchr(str1,',') != NULL ){		//���3����������������������
		newName = str1;
		rename = true;
		x = parseDouble(strtok(str2,","));
		y = parseDouble(strtok(NULL,","));
		relocated = true;
	}else{
		//2.2�Ƿ���У��
		printf("\n���������ʽ����(1153)\n");
		Sleep(1000);
		return false;
	}
	intro:
	//3.��������µĲ�ͬ�����б�
	if(rename && relocated){	//���1�����������ض�λ
		UpdateBuilding(g, origin, true, true, newName, x, y);
	}else if(rename && !relocated){	//���2��������
		UpdateBuilding(g, origin, true, false, newName);
	}else if(!rename && relocated){	//���3���ض�λ
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
	//����bat
	fprintf(batFile, "@echo off\nif \"%%1\" == \"h\" goto begin\nmshta vbscript:createobject(\"wscript.shell\").run(\"\"\"%%~nx0\"\"h\",0)(window.close)&&exit\n");
	fprintf(batFile, ":begin\n");
	//����bat
	fprintf(batFile, "cd ./graphviz_lite2\n");
	fprintf(batFile, "set startDir=%%cd%%\n");
	fprintf(batFile, "npm run serve\n");
	fclose(batFile);
	return true;
}
bool ProgramSetting(){
	setCursorVisible(false);
	//0.����ҳ��ͷ
	PrintPageHead();
	printf("\nCampus Navigation System У԰����ϵͳӦ������..\n");
	printf("���̰�����Ӧ���������ֿɶ����ý����޸�\n\n");
	//1.��ǰ����չʾ
	printf("1.�˳�������������ļ�\t��ǰΪ��");
	if(settings.Save_Temp_After_Quit){
		printf("����\n");
	}else{
		printf("������\n");
	}
	printf("2.���ɵı�������ͼ�ļ���\t��ǰΪ��%s\n", settings.Png_File_Name);
	printf("3.����ͼ�����ļ���ַ\t\t��ǰΪ��%s\n", settings.Basis_File_Name);
	printf("4.��ͼģʽ\t\t\t��ǰΪ��");
	if(settings.Web_Mode == 1) {
		printf("��ҳģʽ1\n");
	}else if(settings.Web_Mode == 2){
		printf("��ҳģʽ2�����Ա�node.js��\n");
	}else{
		printf("����ģʽ\n");
	}
	printf("5.ͼ��������仯\t\t��ǰΪ��");
	if(settings.Update_Pic_In_Time){
		printf("����\n");
	}else{
		printf("������\n");
	}
	printf("\n\n\n���˳����ã������棩[ESC]\t���沢�˳�����[ENTER]��\n");
	
	//2.�����޸�
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
						printf("����    ");
					}else{
						printf("������  ");
					}
					break;
				case 50:	//2
				case 51:	//3
					gotoxy(51,14);
					printf("�ݲ�֧���޸ģ�");
					break;
				case 52:	//4
					gotoxy(40,15);
					tempMode++;
					tempMode = tempMode % 3;
					if(tempMode){
						if(tempMode == 1){
							printf("��ҳģʽ1                \n");
						}else{
							printf("��ҳģʽ2�����Ա�node.js��\n");
						}
					}else{
						printf("����ģʽ                 \n");
					}
					break;
				case 53:	//5
					gotoxy(51,16);
					printf("�ݲ�֧���޸ģ�");
					break;
				case 13:	//ENTER
					gotoxy(20,17);
					printf("�ѱ������ã�");
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
		printf("\t\t��������ļ�....\n");
		remove(settings.Png_File_Name);
		remove(settings.Dot_File_Name);
		remove("./graphviz_lite1/navigation.js");
		remove("./graphviz_lite2/src/navigation.js");
		Sleep(1000);
	}
	if(isNodeAlive){
		printf("\t����ֹͣ����node���̣��������ȷ���˳�\n\n");
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
	//�쳣У��
	if(basis_return == NULL){
		printf("\n�����޷�����basis�ļ�����1308��\n");
		return false;
	}
	//1.��ԭnode����
	for (int i = 0; i < g->nodeNum; ++i) {
		if(g->nodes[i].isValid){
			fprintf(basis_return,"%d:%s:%.2lf,%.2lf\n",i, GbkToUtf8(g->nodes[i].data),g->nodes[i].pos_x,g->nodes[i].pos_x);
		}
	}
	//2.��ԭ�ߣ�ֻ��adj����������ǣ���port�����ȫ��
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
	printf("\t\t  �ļ�д�����\n");
	Sleep(1000);
	return true;
}