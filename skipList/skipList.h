#ifndef _SKIP_LIST_H_
#define _SKIP_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SKIPLIST_P 0.5
#define MAX_LEVEL 4
#define SortKey int
#define SortVal int


// �ṹģ��
/*  ���� ������
	---------------
	|      | next |
	| key  |------|
	| val  | next |
	|      |------|
	|      | next |
	---------------
*/

struct ListNode {
	SortKey key;			// �����key
	SortVal val;			// �����value
	ListNode* next[1]; // ����ڵ��ж��ٲ㣬 �ͻ��ж��ٸ�nextָ��
};

struct SkipList {
	int level;
	ListNode * head;
};

int randLv() {
	int level = 1;
	while (rand() % 2)
		level++;
	return (MAX_LEVEL > level) ? level : MAX_LEVEL;
}

ListNode* newNode(int lv, SortKey key, SortVal val){
	int sz = sizeof(ListNode) + lv * sizeof(ListNode*);
	ListNode* node= (ListNode*)malloc(sz);
	memset(node, 0, sz);
	node->key = key;
	node->val = val;

	return node;
}

SkipList* createList()
{
	SkipList* list = (SkipList* )malloc(sizeof(list));
	list->level = 0;
	list->head = (ListNode * )malloc(sizeof(ListNode) + ((MAX_LEVEL - 1) * sizeof(ListNode *)));
	memset(list->head, 0, sizeof(ListNode) + ((MAX_LEVEL - 1) * sizeof(ListNode*)));
	srand(time(0));
	return list;
}

static ListNode* update[MAX_LEVEL];
bool insertNode(SkipList * list, SortKey key, SortVal val){
	memset(update, 0, MAX_LEVEL * sizeof(ListNode*));

	ListNode* p = NULL;
	ListNode* head = list->head;
	int lv = list->level - 1;

	for (; lv >= 0; lv --){
		//printf("head->next[lv] = %p\n", &head->next[lv]);
		while ((p = head->next[lv]) && p->key < key)
			head = p;
		update[lv] = head;
	}

	if (p && p->key == key){
		p->val = val;
		return true;
	}

	// ���lv
	int newLv = randLv();
	if (newLv > list->level){
		for (int i = list->level; i < newLv; i++){// �ҵ���Ҫ���µĽڵ�
			update[i] = list->head;
		}
		list->level = newLv;
	}

	// new �ڵ�
	ListNode* node = newNode(newLv, key, val);
	if (!node){
		return false;
	}
	
	// ��Ҫ���µĽڵ㶼����
	for (int i = newLv - 1; i >= 0; --i){
		node->next[i] = update[i]->next[i];
		update[i]->next[i] = node;
		//printf("update[i]->next[i] = %p  %p  %p\n", &update[i]->next[i], node, &list->head->next[i]);
	}

	printf("\n");
	return true;
}

bool erase(SkipList * list, SortKey key){
	memset(update, 0, MAX_LEVEL * sizeof(ListNode*));
	ListNode* h = list->head;
	ListNode* p = NULL;
	int lv = list->level - 1;
	
	// ���ϼ����¼���
	for (; lv >= 0; lv--){
		while ((p = h->next[lv]) && p->key < key)
			h = p;
		update[lv] = h;
	}

	if (!p && (p && p->key != key)){ // û���ҵ��� ������key �����
		return false;
	}

	// ɾ��Ԫ��
	lv = list->level - 1;
	for (; lv >= 0; lv --){
		if (update[lv]->next[lv] == p){
			update[lv]->next[lv] = p->next[lv];
			if (!list->head->next[lv]) {
				list->level--;
			}
		}
	}

	free(p);
	p = NULL;
	return true;
}

// ���ϲ����²��ӡ
void PrintList(SkipList * list){
	ListNode* h = list->head;
	ListNode* p = NULL;
	for (int i = list->level - 1; i >= 0; i --){
		p = h->next[i];
		printf("level %d\n", i);
		while (p){
			printf("key = %d val = %d \t", p->key, p->val  );
			p = p->next[i];
		}
		printf("\n");
	}
	printf("\n");
}




//
//
//template<typename sortKey, typename sortValue>
//struct ListNode {
//	ListNode(): m_key(0), m_val(0), next(NULL) {
//	}
//
//	ListNode(int lv, sortKey key, sortValue val) : m_key(key), m_val(val) {
//		next = new ListNode<sortKey, sortValue>[lv];	// ������new��ʵ�е��˷ѣ� ��Ϊֻ�õ��� next ָ�룬 key�� value��û���õ�
//	}
//
//	~ListNode() {
//		delete []next;
//	}
//
//	sortKey m_key;			// �����key
//	sortValue m_val;			// �����value
//	ListNode<sortKey, sortValue> * next; // ����ڵ��ж��ٲ㣬 �ͻ��ж��ٸ�nextָ��
//};
//
//template<typename sortKey, typename sortValue>
//class SkipList {
//public:
//	SkipList(int maxLv) : m_maxLv(maxLv) {
//		srand(time(0));
//		m_head = new ListNode<sortKey, sortValue>(maxLv, 0, 0);
//	}
//
//
//	~SkipList() {
//		delete m_head;
//	}
//
//	// ����ڵ�
//	// 1���ҳ���Ҫ���µĽڵ�, ���ϲ�������
//	bool insertNode(sortKey key, sortValue val) {
//		// ���ҳ���Щ������Ҫ���µģ� ������
//		ListNode<sortKey, sortValue> *update = new ListNode<sortKey, sortValue>[m_maxLv];
//		auto head = m_head;
//		ListNode<sortKey, sortValue>* p = NULL;
//		int i = m_level;
//
//		for (; i > 0; i--){
//			if (head->next[i].m_key < key) {
//				p = head->next[i].next;
//				head = p;
//			}
//			update[i].next = p;
//		}
//
//		if (p && p.m_key == key){
//			delete[] update;
//			return true;
//		}
//		
//
//		
//		delete[] update;
//		return true;
//	}
//		
//	
//private:
//	int randLv() {
//		int level = 1;
//		while ((rand() & 0xFFFF) < (SKIPLIST_P * 0xFFFF))
//			level += 1;
//		return (level < m_maxLv) ? level : m_maxLv;
//	}
//
//
//private:
//	int m_maxLv; // ����ж��ٲ�
//	int m_level; // ��ǰ�����ж��ٲ�
//	ListNode<sortKey, sortValue> * m_head;
//};
//
//












#endif
