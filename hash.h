#pragma once
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "graph.h"
#include "read_config.h"
#include <stdbool.h>
#define MAX_FZ 0.75
// 哈希表
typedef struct HashNode {
	location* data;   // 景点信息
    HashNode* next;  // 链表指针（解决冲突）
    Road_Link* link;
} HashNode;

typedef struct {
    HashNode** table;   // 桶数组
    int size;           // 表长
    int count;          // 元素个数
    int type;           // 判断否是字符串哈希(0)还是数组哈希(1)
} HashTable;

int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}

int nextPrime(int n) {
    while (!isPrime(n))
        n++;
    return n;
}

int str_hash_size = 0;
int id_hash_size = 0;
int a = read_config_int(FILE_NAME, "str_hash_size", &str_hash_size);
int b = read_config_int(FILE_NAME, "str_hash_size", &id_hash_size);
int hash_fun_id(int id, int key) {
    return id % key;
}

int hash_fun_str(const char* str, int key) {

    unsigned long long int hash_val = 0;
    const char* temp = str;
    while (*temp != '\0') {
        hash_val = (hash_val * 31 + *temp)%key; // 31用于减少冲突
        temp++; // 指针后移
    }
    return hash_val % key;
}

void rehash(HashTable* hash) {
    if (hash->count * 1.0 / hash->size > MAX_FZ) {
        int newSize = nextPrime(hash->size * 2);
        if (hash->type == 0) {
            change_config_int(FILE_NAME, "str_hash_size", newSize);
            str_hash_size = newSize;
        }
        else {
            change_config_int(FILE_NAME, "id_hash_size", newSize);
            id_hash_size = newSize;
        }
        HashNode** newTable = (HashNode**)malloc(sizeof(HashNode*) * newSize);
        if (newTable == NULL) {
            printf("哈希表自动扩容失败\n");
            return;
        }
        for (int i = 0; i < newSize; i++)
            newTable[i] = NULL;
        
        // 重新哈希
        for (int i = 0; i < hash->size; i++) {
            HashNode* p = hash->table[i];
            while (p) {
                HashNode* next = p->next;

                int index = 0;
                if (hash->type == 0) {
                    index = hash_fun_str(p->data->name, newSize);
                }
                else {
                    index = hash_fun_id(p->data->id, newSize);
                }


                p->next = newTable[index];
                newTable[index] = p;

                p = next;
            }
        }

        free(hash->table);
        hash->table = newTable;
        hash->size = newSize;
        printf("\n哈希表自动扩容完成，新容量：%d\n", newSize);
    }
    return;
}



HashTable* initHashTable(int type,int size) {
    HashTable* ans = (HashTable*)malloc(sizeof(HashTable));
    if (ans == NULL) {
        return NULL;
    }
    ans->count = 0;
    ans->type = type;
    ans->size = size;
    ans->table = (HashNode**)malloc(size * sizeof(HashNode*));
    if (ans->table == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        *(ans->table+i) = NULL;
    }
    return ans;
}

// 头插法
int insertLocation(HashTable* hash, location* loc) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    if (hash->type == 0) {
        index = hash_fun_str(loc->name, table_size);
    }
    else {
        index = hash_fun_id(loc->id, hash->size);
    }

    // 检查是否重复
    HashNode* p = hashTable[index];
    while (p) {
        if (p->data->id == loc->id || strcmp(p->data->name,loc->name) == 0)
            return 0;  // 已存在
        p = p->next;
    }

    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (node == NULL) {
        return -1;
    }
    node->data = loc;
    node->next = hashTable[index];
    node->link = NULL;
    hashTable[index] = node;

    hash->count++;
    rehash(hash);
    return 1;
}

location* findLocation_by_str(HashTable* hash,const char* name) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_str(name, table_size);
    
    HashNode* p = hashTable[index];

    while (p) {
        if (strcmp(p->data->name , name)==0)
            return p->data;
        p = p->next;
    }
    return NULL;
}
location* findLocation_by_id(HashTable* hash, int id) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_id(id, table_size);

    HashNode* p = hashTable[index];

    while (p) {
        if (p->data->id == id)
            return p->data;
        p = p->next;
    }
    return NULL;
}
int deleteLocation_by_id(HashTable* hash,int id) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_id(id, hash->size);
    HashNode* p = hashTable[index], * pre = NULL;

    while (p) {
        if (p->data->id == id) {
            if (pre)
                pre->next = p->next;
            else
                hashTable[index] = p->next;
            free(p);
            hash->count--;
            return 1;
        }
        pre = p;
        p = p->next;
    }
    return 0;
}

int deleteLocation_by_char(HashTable* hash, const char* name) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_str(name, hash->size);
    HashNode* p = hashTable[index], * pre = NULL;

    while (p) {
        if (strcmp(p->data->name,name)==0) {
            if (pre)
                pre->next = p->next;
            else
                hashTable[index] = p->next;
            free(p);
            hash->count--;
            return 1;
        }
        pre = p;
        p = p->next;
    }
    return 0;
}
int updateLocation(HashTable* table,location* loc) {
    struct location* p = findLocation_by_id(table,loc->id);
    if (!p) {
        struct location* p = findLocation_by_str(table, loc->name);
    }
    if (!p) return -1;

    strcpy(p->name, loc->name);
    strcpy(p->des, loc->des);
    return 1;
}

void showAllLocations(HashTable* hash) {
    printf("\n==================== 所有景点信息 ====================\n");
    for (int i = 0; i < hash->size; i++) {
        HashNode* p = hash->table[i];
        while (p) {
            printf("ID:%d\n名称:%s\n简介:%s\n\n",
                p->data->id, p->data->name, p->data->des);
            p = p->next;
        }
    }
    printf("======================================================\n");
}

int show_Locations_by_ID(HashTable* hash,int id) {
    
    int index = hash_fun_id(id,hash->size);
    HashNode* p = hash->table[index];
    while (p) {
        if (p->data->id == id) {
            printf("ID:%d\n名称:%s\n简介:%s\n\n",p->data->id, p->data->name, p->data->des);
            return 1;
        }
        p = p->next;
    }

    return 0;
}



