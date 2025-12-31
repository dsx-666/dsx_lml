#pragma once
#define NAME_MAX 100
#define DES_MAX 1000
typedef struct location {
	int id;
	char name[NAME_MAX];
	char des[DES_MAX];
	
}location; 
typedef struct Road_Link {
    int id;  //对应景点id
    int length;   //路径长度
    Road_Link* next;  //指向下一个链表的指针
}Road_Link;  //链表存放景点数据

typedef struct Road {
    location* head;  //链表头节点
    Road_Link* link;  //路链表
}Road;  //邻接表，表示头节点到其他景点的路径




