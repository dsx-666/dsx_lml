#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"hash.h"
#include"heap.h"
#include"read_config.h"
void input_int(int* input);
void input_char(char* c);
//校园导游程序的菜单面板
void menu() {
	printf("-------------------------------------------------------------\n");
	printf("                      校园导游程序                           \n");
	printf("0：退出                                                      \n");
	printf("1：查询某个景点的基本信息                                    \n");
	printf("2：查询任意两个景点之间的最短路径                            \n");
	printf("3：查询任意两个景点的所有路径                                \n");
	printf("4：添加景点基本信息                                          \n");
	printf("5：删除某景点和其对应的路径信息                              \n");
	printf("6：更新某景点的基本信息                                      \n");
	printf("7：添加两个景点的路径信息                                    \n");
	printf("8：删除两个景点间的路径信息                                  \n");
	printf("9：更新两个对应景点的路径信息                                \n");
	printf("10：查看所有景点和其相关信息                                 \n");
	printf("11：显示所有路径信息                                         \n");
	printf("-------------------------------------------------------------\n");
	printf("请输入你的选择：");
}

//全局组件，各个方法都可能用到
HashTable* hash_str = NULL;
HashTable* hash_id = NULL;
Road* roads = NULL;

int get_max_location_id() {
	if (hash_id == NULL || hash_id->table == NULL) return 0;
	int max_id = 0;
	for (int i = 0; i < hash_id->size; i++) {
		HashNode* node = hash_id->table[i];
		while (node != NULL) {
			if (node->data != NULL && node->data->id > max_id) {
				max_id = node->data->id;
			}
			node = node->next;
		}
	}
	return max_id;
}

//初始化所有要用到的组件
int init_all_part()
{
	//从read_config.h读取哈希表初始大小
	int str_hash_size = 11;
	int id_hash_size = 11;
	read_config_int(FILE_NAME, "str_hash_size", &str_hash_size);
	read_config_int(FILE_NAME, "id_hash_size", &id_hash_size);
	//开始去初始化哈希表
	hash_str = initHashTable(0, str_hash_size);
	hash_id = initHashTable(1, id_hash_size);
	if (hash_str == NULL || hash_id == NULL) {
		printf("hash表初始化不成功\n");
		return -1;
	}



	//这里插个眼  Man, What Can I Say!!!
	int max_id = get_max_location_id();
	int id_num = (max_id == 0) ? 100 : max_id + 10;
	roads = (Road*)malloc(id_num*sizeof(Road));
	if (roads == NULL) {
		printf("邻接表初始化不成功\n");
		return -1;
	}
	for (int i = 0;i < id_num;i++) {
		roads[i].head == NULL;
		roads[i].link = NULL;
	}

	return 1;
}


void clearInputBuffer() {
	while (getchar() != '\n'); 
}

//查询某个景点的基本信息
void find_location()
{
	printf("====================== 查询景点基本信息 ======================\n");
	printf("1. 按ID查询  2. 按名称查询 0. 退出查询\n请选择：");
	int choice;
	location* loc = NULL;
	scanf("%d", &choice);
	//检验用户输入的选项是否正确
	while (true) {
		if (choice == 1 || choice == 2) {
			break;
		}
		else if (choice == 0) {
			return;
		}
		else {
			input_int(&choice);
		}
	}
	if (choice == 1) {
		printf("请输入景点id:");
		int id;
		scanf("%d", &id);
		loc = findLocation_by_id(hash_id, id);
		while (loc == NULL) {
			input_int(&id);
			loc = findLocation_by_id(hash_id, id);
		}
	}
	else {
		printf("请输入景点名称:");
		char name[NAME_MAX];
		scanf("%s", name);
		loc = findLocation_by_str(hash_str, name);
		if (loc == NULL) {
			input_char(name);
			loc = findLocation_by_str(hash_str, name);
		}
	}
	if (loc) {
		printf("===== 景点信息 =====\n");
		printf("ID：%d\n名称：%s\n简介：%s\n", loc->id, loc->name, loc->des);
	}
	else {
		printf("未找到该景点！\n");
	}

}

//添加景点基本信息
void add_location()
{
	printf("==================== 添加景点基本信息 ======================\n");
	location* loc = (location*)malloc(sizeof(location));
	if (loc == NULL) {
		printf("景点信息location初始化失败\n");
		return;
	}
	int id;
	char name[NAME_MAX], des[DES_MAX];
	printf("输入景点id:");
	scanf("%d", &id);
	printf("输入景点名称:");
	scanf("%s", name);
	printf("输入景点描述:");
	scanf("%s", des);
	clearInputBuffer();
	loc->id = id;
	strncpy(loc->name, name, NAME_MAX);
	strncpy(loc->des, des, DES_MAX);
	int res1 = insertLocation(hash_id, loc);
	int res2 = insertLocation(hash_str, loc);
	if (res1 == 1 && res2 == 1) {
		roads[loc->id].head = loc;
		printf("插入成功\n");
	}
	else if (res1 != 1 || res2 != 1) {
		printf("景点id已存在\n");
		free(loc);
	}
	else {
		printf("插入失败\n");
		free(loc);
	}
}

//更新某景点的基本信息
void update_location()
{
	printf("==================== 更新景点的基本信息 =====================\n");
	printf("请输入要修改的景点id:");
	int id;
	scanf("%d", &id);
	if (id == 0) {
		return;
	}
	//通过id查找看景点存不存在
	location* pre_loc = findLocation_by_id(hash_id, id);
	while (pre_loc == NULL) {
		printf("当前输入id不存在，如果不像继续输入，输入0即可退出该流程\n");
		input_int(&id);
		if (id == 0) {
			return;
		}
		pre_loc = findLocation_by_id(hash_id, id);
	}
	location new_loc;
	new_loc.id = id;
	char name[NAME_MAX], des[DES_MAX];
	printf("输入新名称（原：%s）：", pre_loc->name);
	scanf("%s", name);
	printf("输入新简介（原：%s）：", pre_loc->des);
	scanf("%s", des);
	strncpy(new_loc.name, name, NAME_MAX);
	strncpy(new_loc.des, des, DES_MAX);
	int res = updateLocation(hash_id, &new_loc);
	if (res == 1) {
		printf("更新成功\n");
	}
	else {
		printf("更新失败\n");
	}
}

//查看所有景点和其相关信息
void show_all_locations()
{
	showAllLocations(hash_id);
}


//添加两个景点的路径信息
int add_path(int startId, int endId, int length)
{
	if (findLocation_by_id(hash_id, startId) == NULL ||
		findLocation_by_id(hash_id, endId) == NULL){
			return -1;
		}
	if (startId == endId || length <= 0) {
		return -2;
	}
	Road_Link* link1 = (Road_Link*)malloc(sizeof(Road_Link));
	if (link1 == NULL) return -3;
	link1->id = startId;
	link1->length = length;
	link1->next = roads[endId].link;
	roads[endId].link = link1;
	Road_Link* link2 = (Road_Link*)malloc(sizeof(Road_Link));
	if (link2 == NULL) return -3;
	link2->id = endId;
	link2->length = length;
	link2->next = roads[startId].link;
	link2->next = roads[startId].link;
	roads[startId].link = link2;
	return 1;
}
void operate_add_path()
{
	printf("==================== 添加两个景点的路径信息 ====================\n");
	int startId, endId,length;
	printf("请输入起点Id:");
	scanf("%d", &startId);
	printf("请输入终点Id:");
	scanf("%d", &endId);
	printf("请输入路径长度:");
	scanf("%d", &length);
	int res=add_path(startId, endId, length);
	if (res == 1) {
		printf("添加成功\n");
	}
	else if (res == -1) {
		printf("输入的起点或者终点不存在\n");
	}
	else if (res == -2) {
		printf("参数有误（起点和终点的id一样或者长度为负数）\n");
	}
	else {
		printf("申请内存失败\n");
	}

}

//删除两个景点间的路径信息
int delete_path(int startId, int endId)
{
	if (startId == endId) {
		return -2;
	}
	if (findLocation_by_id(hash_id, startId) == NULL ||
		findLocation_by_id(hash_id, endId) == NULL) {
		return -1;
	}
	Road_Link* p = roads[startId].link, * pre = NULL;
	while (p)
	{
		if (p->id == endId) {
			if (pre) pre->next = p->next;
			else roads[startId].link = p->next;
			free(p);
			break;
		}
		pre = p;
		p = p->next;
	}
	p = roads[endId].link;
	while (p)
	{
		if (p->id == startId) {
			if (pre) pre->next = p->next;
			else roads[endId].link = p->next;
			free(p);
			break;
		}
		pre = p;
		p = p->next;
	}
	return 1;
 }
void operate_delete_path()
{
	printf("==================== 删除两个景点间的路径信息 ====================\n");
	int startId, endId;
	printf("请输入起点id:");
	scanf("%d", &startId);
	printf("请输入终点Id:");
	scanf("%d", &endId);
	int res = delete_path(startId, endId);
	if (res == 1) {
		printf("删除成功\n");
	}
	else if(res==-1) {
		printf("输入错误，无法从输入的Id找到对应景点\n");
	}
	else {
		printf("起点id和终点id不能相同\n");
	}
}

//更新两个对应景点的路径信息
void update_path()
{
	printf("==================== 更新两个对应景点的路径信息 ====================\n");
	int startId, endId,length;
	printf("请输入起点id:");
	scanf("%d", &startId);
	printf("请输入终点Id:");
	scanf("%d", &endId);
	printf("请输入新的路径长度:");
	scanf("%d", &length);
	int res1=delete_path(startId, endId);
	if(res1!=1) {
		printf("输入错误，无法从输入的Id找到对应景点\n");
		return;
	}
	int res2 = add_path(startId, endId, length);
	if (res2 == 1) {
		printf("添加成功\n");
	}
	while (res2 != 1) {
		if (res2 == -2) {
			printf("参数有误（起点和终点的id一样或者长度为负数）\n");
		}
		else {
			printf("申请内存失败\n");
		}
		printf("请重新输入:");
		scanf("%d", &length);
		res2=add_path(startId, endId, length);
		if (res2 == 1) {
			printf("添加成功\n");
		}
	}
	
}

//显示所有路径信息
void show_all_path()
{
	if (hash_id == NULL) {
		printf("hash表未初始化，无法查找到路径\n");
		return;
	}
	printf("==================== 显示所有路径信息 ====================\n");
	int has_location = 0;
	for (int i = 0;i < hash_id->size;i++) {
		HashNode* p = hash_id->table[i];
		while (p!=NULL) {
			has_location = 1;
			location* loc = p->data;
			if (loc == NULL) {
				p = p->next;
				continue;
			}
			int current_id = loc->id;
			Road_Link* path = roads[current_id].link;
			printf("景点【名称：%s（ID：%d）】的路径：\n", loc->name, current_id);
			if (path == NULL) {
				printf("该景点无任何路径\n");
			}
			else {
				while (path != NULL) {
					location* end_loc = findLocation_by_id(hash_id, path->id);
					if (end_loc == NULL) {
						printf("  → 未知景点（ID：%d），长度：%d米\n", path->id, path->length);
					}
					else {
						printf("→ 名称：%s(ID:%d),距离:%d m\n", end_loc->name, path->id, path->length);
					}
					path = path->next;
				}
			}
			printf("\n");
			p = p->next;
		}
	}
	if (!has_location) {
		printf("无任何景点\n");
	}
}


//查询任意两个景点的所有路径
int* path=NULL;           
int* visited=NULL;        
int path_length=0;     
int path_count=0;      
void dfs(int currentId, int endId, int currentLen,int max_id_plus)
{
	if (currentId < 0 || currentId > max_id_plus) {
		return;
	}
	if (currentId == endId) {
		path_count++;
		printf("路径 %d：", path_count);
		for (int i = 0;i < path_length;i++) {
			location* loc = findLocation_by_id(hash_id, path[i]);
			if (loc == NULL) {
				printf("未知景点(%d)", path[i]);
			}
			else {
				printf("%s(%d)", loc->name, loc->id);
			}
			if (i != path_length - 1) {
				printf("->");
			}
		}
		printf("  总长度：%d m\n", currentLen);
		
		return;
	}
	if (currentId > max_id_plus) return;
	visited[currentId] = 1;
	Road_Link* p = roads[currentId].link;
	while (p != NULL) {
		int next_id = p->id;
		if (!visited[next_id]) {
			path[path_length++] = next_id;
			dfs(next_id, endId, currentLen + p->length,max_id_plus);
			
			path_length--;
		}
		
		p = p->next;
	}
	visited[currentId]=0;
}
void query_two_all_paths()
{
	printf("================== 查询任意两个景点的所有路径 ==================\n");
	int startId, endId;
	printf("请输入起点id:");
	scanf("%d", &startId);
	printf("请输入终点Id:");
	scanf("%d", &endId);
	if (!findLocation_by_id(hash_id, startId) || !findLocation_by_id(hash_id, endId)) {
		printf("输入的id不正确，无法找到对应景点\n");
		clearInputBuffer();
		return;
	}
	if (startId == endId) {
		printf("起点和终点不能相等\n");
		clearInputBuffer();
		return;
	}
	 int max_id = 0;
	 for (int i = 0; i < hash_id->size; i++) {
	     HashNode* node = hash_id->table[i];
	     while (node) {
	         if (node->data->id > max_id) max_id = node->data->id;
	         node = node->next;
	     }
	 }
	 int max_id_plus = max_id + 10;
	 path = (int*)malloc(max_id_plus * sizeof(int));
	 visited = (int*)calloc(max_id_plus + 1,sizeof(int));
	if (path == NULL || visited == NULL) {
		printf("内存分配出问题\n");
		free(path);
		free(visited);
		return;
	}
	path_length = 0;
	path[path_length++] = startId;
	path_count = 0;
	printf("\n从 %s 到 %s 的所有路径：\n",
		findLocation_by_id(hash_id, startId)->name, findLocation_by_id(hash_id, endId)->name);
	dfs(startId, endId, 0,max_id_plus);
	if (path_count == 0) {
		printf("没有找到路径\n");
	}else{
		printf("共找到 %d 条路径\n", path_count);
	}
	free(path);
	free(visited);
	path = NULL;
	visited = NULL;
}

//删除某景点和其对应的路径信息
void delete_location_path()
{
	printf("================== 删除某景点和其对应的路径信息 ==================\n");
	printf("请输入要删除的景点ID:");
	int id;
	scanf("%d", &id);
	location* loc = findLocation_by_id(hash_id, id);
	if (loc == NULL) {
		printf("该景点ID不存在！\n");
		return;
	}
	for (int i = 0;i < hash_id->size;i++) {
		HashNode* p = hash_id->table[i];
		while (p != NULL) {
			int p_id = p->data->id;
			if (p_id != id) {
				delete_path(id, p_id);
			}
			p = p->next;
		}
	}
	int res1 = deleteLocation_by_id(hash_id, id);
	int res2 = deleteLocation_by_char(hash_str, loc->name);
	if (res1 && res2) {
		printf("景点及其关联路径删除成功！\n");
		free(loc);
	}
	else {
		printf("景点删除失败！\n");
	}
}

int shortest_len = INT_MAX;
int* shortest_path = NULL;
int shortest_path_len = 0;   //最短路径上面的节点数目
void dfs_shortest_path(int currentId, int endId, int currentLen, int* path, int path_len, int* visited, int max_id_plus)
{
	if (currentId == endId) {
		if (currentLen < shortest_len) {
			shortest_len = currentLen;
			shortest_path_len = path_len;
			mem
		}
	}
}

//如果整数输入错误，重新输入
void input_int(int* input)
{
	printf("输入错误，请重新输入:");
	scanf("%d", input);
}

void input_char(char* c)
{
	printf("输入错误，请重新输入:");
	scanf("%s", c);
}

//输入是否合规的检查方法
int judge_validity_input()
{
	int input;
	while (true) {
		if (scanf("%d", &input) == 1) {
			//输入合法，放行
			break;
		}
		else {
			//输入不合规，清除缓冲区
			while (getchar() != '\n');
			printf("输入不合规，请重新输入：");
		}
	}
	return input;
}

void enter_again() {
	printf("输入有误，重新输入");
}

//释放所有分配的内存
void free_all()
{
	if (roads != NULL&&hash_id!=NULL) {
		for (int i = 0;i < hash_id->size;i++) {
			HashNode* p1 = hash_id->table[i];
			while (p1 != NULL) {
				int delete_id = p1->data->id;
				if (delete_id >= 0) {
					Road_Link* p2 = roads[delete_id].link;
					while (p2 != NULL) {
						Road_Link* tmp = p2;
						p2 = p2->next;
						free(tmp);
					}
					roads[delete_id].link = NULL;
				}
				p1 = p1->next;
			}
		}
		free(roads);
		roads = NULL;
	}
	if (hash_id != NULL) {
		for (int i = 0;i < hash_id->size;i++) {
			HashNode* p = hash_id->table[i];
			while (p != NULL) {
				HashNode* tmp = p;
				p = p->next;
				free(tmp->data);
				free(tmp);
			}
		}
		free(hash_id->table);
		free(hash_id);
		hash_id = NULL;
	}
	if (hash_str != NULL) {
		for (int i = 0;i < hash_str->size;i++) {
			HashNode* p = hash_str->table[i];
			while (p != NULL) {
				HashNode* tmp = p;
				p = p->next;
				free(tmp);
			}
		}
		free(hash_str->table);
		free(hash_str);
		hash_str = NULL;
	}
}


int main()
{
	if (init_all_part() == -1) {
		printf("初始化失败\n");
		return -1;
	}
	while (true) {
		menu();
		int choice = judge_validity_input();
		while (true) {
			if (choice >= 0 && choice <= 12) {
				break;
			}
			else {
				//清除缓冲区数据
				while (getchar() != '\n');
				input_int(&choice);
			}
		}
		switch (choice) {
		case 1:
			find_location();
			break;
		case 2:
			break;
		case 3:
			query_two_all_paths();
			break;
		case 4:
			add_location();
			break;
		case 5:
			delete_location_path();
			break;
		case 6:
			update_location();
			break;
		case 7:
			operate_add_path();
			break;
		case 8:
			operate_delete_path();
			break;
		case 9:
			update_path();
			break;
		case 10:
			show_all_locations();
			break;
		case 11:
			show_all_path();
			break;
		case 12:
			break;
		case 0:
			printf("感谢您使用本校园导游系统！即将退出。\n");
			return 0;
		default:
			enter_again();
			break;
		}
	}
	

	return 0;
}