//#define _CRT_SECURE_NO_WARNINGS 1
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define NAME_MAX 100
//#define DES_MAX 500
//#define MAX_LOCATION 50
//int id=1;
//typedef struct location {
//	int id;  //景点id
//	char name[NAME_MAX];    //景点名称
//	char description[DES_MAX];   //景点描述
//}location;  //  景点结构体
//typedef struct Road_Link {
//	int id;  //对应景点id
//	int length;   //路径长度
//	Road_Link* next;  //指向下一个链表的指针
//}Road_Link;  //链表存放景点数据
//
//typedef struct Road {
//	location* head;  //链表头节点
//	Road_Link* link;  //路链表
//}Road;  //邻接表，表示头节点到其他景点的路径
//
//
////校园导游程序的菜单面板
//void menu() {
//	printf("-------------------------------------------------------------\n");
//	printf("                      校园导游程序                           \n");
//	printf("0：退出                                                      \n");
//	printf("1：查询某个景点信息                                          \n");
//	printf("2：查询任意两个景点之间最短路径                              \n");
//	printf("3：查询任意两个景点之间的所有可行路径                        \n");
//	printf("4：添加景点和其介绍                                          \n");
//	printf("5：删除某景点和其对应的道路信息                              \n");
//	printf("6：更新某景点信息                                            \n");
//	printf("7：添加两个景点和其对应的道路信息                            \n");
//	printf("8：删除两个景点间的道路信息                                  \n");
//	printf("9：更新两个景点间的道路信息                                  \n");
//	printf("10：显示所有景点和相关信息                                   \n");
//	printf("11：显示所有路径信息                                         \n");
//	printf("12：显示小组制作成员的信息                                   \n");
//	printf("-------------------------------------------------------------\n");
//	printf("请输入你的选择：");
//}
//
////初始化Road指针
//Road* initRoads() {
//	Road* roads = (Road*)malloc(sizeof(Road) * (MAX_LOCATION + 1));
//	if (roads == NULL) {
//		printf("动态分配Road指针失败！\n");
//		return NULL;
//	}
//	for (int i = 1; i <= MAX_LOCATION; i++) {
//		roads[i].head = NULL;  
//		roads[i].link = NULL;  
//	}
//	return roads;
//}
//
////添加景点基本信息
//int addLocation(Road* roads, int id, const char* name, const char* desc) {
//	if (roads == NULL || id < 1 || id > MAX_LOCATION || roads[id].head != NULL) {
//		printf("Road指针为空/景点ID无效/已存在！\n");
//		return -1;
//	}
//
//	location* loc = (location*)malloc(sizeof(location));
//	if (loc == NULL) {
//		printf("分配景点内存失败！\n");
//		return -1;
//	}
//
//	loc->id = id;
//
//	strncpy(loc->name, name, NAME_MAX - 1);
//	strncpy(loc->description, desc, DES_MAX - 1);
//	roads[id].head = loc;  
//
//	return 0;
//}
//
////添加两个景点路径信息
//int addPath(Road* roads, int startId, int endId, int length) {
//	if (roads == NULL || startId == endId || length <= 0 ||
//		startId < 1 || startId > MAX_LOCATION || endId < 1 || endId > MAX_LOCATION ||
//		roads[startId].head == NULL || roads[endId].head == NULL) {
//		printf("Road指针为空/路径参数无效/景点未添加！\n");
//		return -1;
//	}
//	Road_Link* link1 = (Road_Link*)malloc(sizeof(Road_Link));
//	if (link1 == NULL) {
//		printf("分配路径节点失败！\n");
//		return -1;
//	}
//	link1->id = endId;
//	link1->length = length;
//	link1->next = roads[startId].link;  
//	roads[startId].link = link1;
//	Road_Link* link2 = (Road_Link*)malloc(sizeof(Road_Link));
//	if (link2 == NULL) {
//		free(link1);
//		printf("分配路径节点失败！\n");
//		return -1;
//	}
//	link2->id = startId;
//	link2->length = length;
//	link2->next = roads[endId].link;
//	roads[endId].link = link2;
//
//	return 0;
//}
//
////用于DFS遍历获取任意两个景点的路径
//static int path[MAX_LOCATION];    
//static int pathLen[MAX_LOCATION]; 
//static int visited[MAX_LOCATION]; 
//static int pathCount = 0;         
//static int currentStep = 0;
////供queryTwoLocationPath调用
//static void dfsFindPaths(Road* roads, int curId, int endId) {
//	visited[curId] = 1;
//	path[currentStep] = curId;
//	currentStep++;
//
//	if (curId == endId) {
//		pathCount++;
//		int totalLen = 0;
//		printf("\n【路径%d】\n", pathCount);
//		printf("  路线：");
//		for (int i = 0; i < currentStep; i++) {
//			printf("%s", roads[path[i]].head->name);
//			if (i < currentStep - 1) {
//				printf(" → ");
//				totalLen += pathLen[i];
//			}
//		}
//		printf("\n  总长度：%d米\n", totalLen);
//		currentStep--;
//		visited[curId] = 0;
//		return;
//	}
//
//	
//	Road_Link* curLink = roads[curId].link;
//	while (curLink != NULL) {
//		int nextId = curLink->id;
//		if (!visited[nextId]) { 
//			pathLen[currentStep-1] = curLink->length; 
//			dfsFindPaths(roads, nextId, endId);     
//		}
//		curLink = curLink->next;
//	}
//
//
//	
//	currentStep--;
//	visited[curId] = 0;
//}
//
////查询任意两个景点的所有路径
//void queryTwoLocationPaths(Road* roads, int startId, int endId) {
//	if (roads == NULL) { printf("Road指针为空！\n"); return; }
//	if (startId < 1 || startId > MAX_LOCATION || endId < 1 || endId > MAX_LOCATION) {
//		printf("ID超出范围！\n"); return;
//	}
//	if (roads[startId].head == NULL || roads[endId].head == NULL) {
//		printf("起点/终点景点未添加！\n"); return;
//	}
//	if (startId == endId) {
//		printf("起点终点相同！\n"); return;
//	}
//
//	memset(path, 0, sizeof(path));
//	memset(pathLen, 0, sizeof(pathLen));
//	memset(visited, 0, sizeof(visited));
//	pathCount = 0;
//	currentStep = 0;
//
//	printf("===== 查询「%s」→「%s」的所有路径 =====\n",
//		roads[startId].head->name, roads[endId].head->name);
//	dfsFindPaths(roads, startId, endId);
//
//	if (pathCount == 0) {
//		printf("\n未找到路径\n");
//	}
//	else {
//		printf("\n共找到%d条路径\n", pathCount);
//	}
//}
//
//
////查询所有路径信息
//void showAllPaths(Road* roads) {
//	if (roads == NULL) {
//		printf("Road指针未初始化！\n");
//		return;
//	}
//
//	
//	int displayed[MAX_LOCATION + 1][MAX_LOCATION + 1] = { 0 };
//	int roadCount = 0;
//
//	printf("\n==================== 所有路径信息 ====================\n");
//
//	
//	for (int startId = 1; startId <= MAX_LOCATION; startId++) {
//		Road* curRoad = &roads[startId];  
//
//		
//		if (curRoad->head == NULL || curRoad->head->id != startId) {
//			continue;
//		}
//
//		
//		Road_Link* curLink = curRoad->link;
//		while (curLink != NULL) {
//			int endId = curLink->id;       
//			int len = curLink->length;     
//
//			
//			if (endId < 1 || endId > MAX_LOCATION || len <= 0 || displayed[startId][endId]) {
//				curLink = curLink->next;
//				continue;
//			}
//
//			
//			if (roads[endId].head == NULL || roads[endId].head->id != endId) {
//				curLink = curLink->next;
//				continue;
//			}
//
//			
//			roadCount++;
//			printf("【路径%d】\n", roadCount);
//			printf("  起点：%s（ID：%d）\n", curRoad->head->name, startId);
//			printf("  终点：%s（ID：%d）\n", roads[endId].head->name, endId);
//			printf("  长度：%d米\n\n", len);
//
//			
//			displayed[startId][endId] = 1;
//			displayed[endId][startId] = 1;
//
//			curLink = curLink->next;
//		}
//	}
//
//	
//	if (roadCount == 0) {
//		printf("暂无有效路径！\n");
//	}
//	else {
//		printf("=====================================================\n");
//		printf("共 %d 条有效路径\n", roadCount);
//	}
//}
//
//
////输入是否合规的检查方法
//int judge_validity_input()
//{
//	int input;
//	while (true) {
//		if (scanf("%d", &input) == 1) {
//			//输入合法，放行
//			break;
//		}
//		else {
//			//输入不合规，清除缓冲区
//			while (getchar() != '\n');
//			printf("输入不合规，请重新输入：");
//		}
//	}
//	return input;
//}
//
//
//void enter_again() {
//	printf("输入有误，重新输入");
//}
//
//
////释放内存
//void freeRoads(Road* roads) {
//	if (roads == NULL) return;
//
//	for (int i = 1; i <= MAX_LOCATION; i++) {
//
//		if (roads[i].head != NULL) {
//			free(roads[i].head);
//			roads[i].head = NULL;
//		}
//		Road_Link* cur = roads[i].link;
//		while (cur != NULL) {
//			Road_Link* temp = cur;
//			cur = cur->next;
//			free(temp);
//		}
//		roads[i].link = NULL;
//	}
//
//	free(roads);
//	roads = NULL;
//}
//
//int main() {
//	Road* roads=initRoads();
//	char name1[100] = "洞庭湖";
//	char name2[100] = "鄱阳湖";
//	char desc1[500] = "这是个很不一般的湖";
//	char desc2[500] = "这是也是一个很不一般的湖";
//	addLocation(roads, id++, name1, desc1);
//	addLocation(roads, id++, name2, desc2);
//	addPath(roads, 1, 2, 100);
//	showAllPaths(roads);
//	queryTwoLocationPaths(roads, 1, 2);
//	while (true) {
//		menu();
//		int choice = judge_validity_input();
//		while (true) {
//			if (choice >= 0 && choice <= 12) {
//				break;
//			}
//			else {
//				//清除缓冲区数据
//				while (getchar() != '\n');
//				printf("菜单栏无该选项，选项的合理范围是0-12，请重新输入：");
//				scanf("%d", &choice);
//			}
//		}
//		switch (choice) {
//		case 1:;
//		case 2:;
//		case 3:;
//		case 4:;
//		case 5:;
//		case 6:;
//		case 7:;
//		case 8:;
//		case 9:;
//		case 10:;
//		case 11:;
//		case 12:;
//		case 0:
//			printf("感谢您使用本校园导游系统！即将退出。\n");
//			return 0;
//		default:enter_again();
//		}
//	}
//	freeRoads(roads);
//	return 0;
//}