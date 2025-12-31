#pragma once
#include<stdlib.h>
#include "graph.h"
typedef struct {
	int value;
	location* data;
}Value;

typedef struct {
	Value** arr;
	int size;
}Value_Array;
// 初始化Value_Array，未成功返回NULL

Value_Array* init_value_array() {
	Value_Array* ans = (Value_Array*)malloc(sizeof(Value_Array));
	if (ans == NULL) {
		return NULL;
	}
	ans->arr = NULL;
	ans->size = 0;
	return ans;
}

// 判断是否为空 1代表空 -1代表非空
int empty_value_array(Value_Array* a) {
	if (a == NULL||a->size == 0) {
		return 1;
	}
	else {
		return -1;
	}
}
// 取对应index的array数据 不成功返回NULL
Value* show_array(Value_Array* a, int index) {
	if (a == NULL || a->size <= index) {
		return NULL;
	}
	else {
		return a->arr[index];
	}

}
// 放入value进Value_Array尾 成功返回1 不成功返回-1
int push_value(Value_Array* a, Value* data) {
	if (a == NULL || data == NULL) {
		return -1;
	}
	Value** temp = (Value**)realloc(a->arr, (a->size + 1) * sizeof(Value*));
	if (temp == NULL) {
		return -1;
	}
	a->arr = temp;
	a->arr[a->size] = data;
	a->size++;
	return 1;
}
// 交换array中对应i和j的索引
void swap_array(Value_Array* a, int i, int j) {
	if (a == NULL || i < 0 || j < 0 || i >= a->size || j >= a->size) {
		return;
	}
	Value temp = *show_array(a, i);
	*show_array(a, i) = *show_array(a, j);
	*show_array(a, j) = temp;
}
// 将Value_Array尾元素赋值到首元素并且删除尾元素 如果成功将temp复制首元素 不成功返回-1
int pop_value(Value_Array* a,Value* ans) {
	if (a == NULL || ans == NULL || empty_value_array(a) == 1) {
		return -1;
	}else {
		*ans = *show_array(a, 0);
		swap_array(a, 0, a->size - 1);
		Value** temp = (Value**)realloc(a->arr, (a->size - 1) * sizeof(Value*));
		a->size--;
		if (temp == NULL) {
			return 1;
		}
		a->arr = temp;
		return 1;
	}
}

// 上浮小根堆
void up_heap(Value_Array* a, int size) {
	if (size == 0 || a == NULL || size >= a->size) {
		return;
	}
	else {
		int cur_child = size;
		while (cur_child > 0) {
			int parent = (cur_child - 1) / 2; // 父节点下标公式不变
			// 小根堆规则：父节点值 > 子节点值  交换，继续上浮
			if (show_array(a, parent)->value > show_array(a, cur_child)->value) {
				swap_array(a, parent, cur_child);
				cur_child = parent; // 子节点上移到父节点位置，继续循环
			}
			else {
				break; // 父节点更小，满足堆规则，结束上浮
			}
		}
	}

}
// 下沉小根堆
void down_heap(Value_Array* a, int parent) {
	if ((parent + 1) * 2 >= a->size || a == NULL || parent < 0) {
		return;
	}
	else {
		int size = a->size;
		while (1) {
			int left = 2 * parent + 1;  // 左孩子下标
			int right = 2 * parent + 2; // 右孩子下标
			int min_idx = parent;       // 记录最小值的下标

			// 找左右孩子中更小的那个
			if (left < size && show_array(a, left)->value < show_array(a, min_idx)->value) {
				min_idx = left;
			}
			if (right < size && show_array(a, right)->value < show_array(a, min_idx)->value) {
				min_idx = right;
			}

			// 最小值就是父节点  满足堆规则，结束下沉
			if (min_idx == parent) {
				break;
			}
			// 否则交换，父节点下沉到最小值孩子的位置，继续循环
			swap_array(a, parent, min_idx);
			parent = min_idx;
		}
	}

}
// 堆插入 插入成功返回1，否则返回-1
int insert_heap(Value_Array* a,Value* data) {
	if (push_value(a, data) == -1) {
		return -1;
	}
	up_heap(a, a->size-1);
	return 1;
}
// 堆删除并且提出最小数据 删除成功返回1，否则返回-1
int erase_heap(Value_Array* a, Value* data) {
	if (pop_value(a, data) == -1) {
		return -1;
	}
	else {
		down_heap(a, 0);
		return 1;
	}

}
void free_Value_Array(Value_Array* a) {
	if (a == NULL) return;
	if (a->arr != NULL) {
		free(a->arr);
		a->arr = NULL;
	}
	free(a);
	a = NULL;
}