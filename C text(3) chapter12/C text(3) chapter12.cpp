#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define _1203


#ifdef _1201
#define NAME_LEN 16

typedef enum { Term, Insert, Append, Delete, Print, Clear } Menu;

typedef struct __node {
	int no;
	char name[NAME_LEN];
	struct __node* next;
} Node;

void SetNode(Node* x, int no, const char* name, const Node* next) {
	x->no = no;
	x->next = (Node*)next;
	strcpy_s(x->name, name);
}

Node* AllocNode() {
	return (Node*)calloc(1, sizeof(Node));
}

void InsertNode(Node** top, int no, const char* name) {
	Node* ptr = *top;
	*top = AllocNode();
	SetNode(*top, no, name, ptr);
}

void AppendNode(Node** top, int no, const char* name) {
	if (*top == NULL) InsertNode(top, no, name);
	else {
		Node* ptr = *top;
		while (ptr->next != NULL) ptr = ptr->next;
		ptr->next = AllocNode();
		SetNode(ptr->next, no, name, NULL);
	}
}

void DeleteNode(Node** top) {
	if (*top != NULL) {
		Node* ptr = (*top)->next;
		free(*top);
		*top = ptr;
	}
}

void ClearList(Node** top) {
	while (*top != NULL) DeleteNode(top);
}

void PrintList(Node** top) {
	Node* ptr = *top;

	puts(" 【一覧表】 ");
	while (ptr != NULL) {
		printf("%5d %-16.16s\n", ptr->no, ptr->name);
		ptr = ptr->next;
	}
}

void InitList(Node** top) {
	*top = NULL;
}

void TermList(Node** top) {
	ClearList(top);
}

Node Read(const char* message) {
	Node temp{};

	printf("%sするデータを入力してください。\n", message);
	printf("番号 : "); scanf_s("%d", &temp.no);
	printf("名前 : "); scanf_s("%s", &temp.name, NAME_LEN);

	return temp;
}

Menu SelectMenu() {
	int ch = 0;

	do {
		printf("\n(1) 先頭にノードを挿入   (2) 末尾にノードを追加\n");
		printf("(3) 先頭のノードを削除   (4) 全ノードを表示\n");
		printf("(5) 全ノードを削除       (0)処  理  終  了 : ");
		scanf_s("%d", &ch);
	} while (ch < Term || ch > Clear);

	return (Menu)ch;
}

int main(void) {
	Menu menu{};
	Node* list = nullptr;

	InitList(&list);

	do {
		Node x{};
		switch (menu = SelectMenu()) {
		case Insert:
			x = Read("先頭に挿入");
			InsertNode(&list, x.no, x.name);
			break;
		case Append:
			x = Read("末尾に追加");
			AppendNode(&list, x.no, x.name);
			break;
		case Delete:
			DeleteNode(&list);
			break;
		case Print:
			PrintList(&list);
			break;
		case Clear:
			ClearList(&list);
			break;
		}
	} while (menu != Term);

	TermList(&list);

	return 0;
}
#endif

#ifdef _1202
#define Null -1
#define NAME_LEN 16

typedef enum { Term, Insert, Append, Delete, Print, Clear } Menu;

typedef int Index;

typedef struct {
	int no;
	char name[NAME_LEN];
	Index next;
	Index Dnext;
} Node;

typedef struct {
	Node* n;
	Index top;
	Index max;
	Index deleted;
} List;

#define Top (list->top)
#define Second (list->n[Top].next)
#define Next(x) (list->n[(x)].next)

void SetNode(Node* x, int no, const char* name, Index next) {
	x->no = no;
	x->next = next;
	strcpy_s(x->name, name);
}

int GetIndex(List* list) {
	if (list->deleted == Null) return ++(list->max);
	else {
		Index rec = list->deleted;
		list->deleted = list->n[rec].Dnext;
		return rec;
	}
}

void DeleteIndex(List* list, Index idx) {
	if (list->deleted == Null) {
		list->deleted = idx;
		list->n[idx].Dnext = Null;
	}
	else {
		Index ptr = list->deleted;
		list->deleted = idx;
		list->n[idx].Dnext = ptr;
	}
}

void InsertNode(List* list, int no, const char* name) {
	Index ptr = Top;
	Top = GetIndex(list);
	SetNode(&list->n[Top], no, name, ptr);
}

void AppendNode(List* list, int no, const char* name) {
	if (Top == Null) InsertNode(list, no, name);
	else {
		Index ptr = Top;
		while (Next(ptr) != Null) ptr = Next(ptr);
		Next(ptr) = GetIndex(list);
		SetNode(&list->n[Next(ptr)], no, name, Null);
	}
}

void DeleteNode(List* list) {
	if (Top != Null) {
		Index ptr = Second;
		DeleteIndex(list, Top);
		Top = ptr;
	}
}

void ClearList(List* list) {
	while (Top != Null) DeleteNode(list);
}

void PrintList(List* list) {
	Index ptr = Top;
	puts(" 【一覧表】 ");
	while (ptr != Null) {
		printf("%5d %-16.16s\n", list->n[ptr].no, list->n[ptr].name);
		ptr = Next(ptr);
	}
}

void InitList(List* list, int size) {
	list->n = (Node*)calloc(size, sizeof(Node));
	list->top = list->max = list->deleted = Null;
}

void TermList(List* list) {
	ClearList(list);
	free(list->n);
}

Node Read(const char* message) {
	Node temp{};

	printf("%sするデータを入力してください。\n", message);
	printf("番号 : "); scanf_s("%d", &temp.no);
	printf("名前 : "); scanf_s("%s", temp.name, NAME_LEN);

	return temp;
}

Menu SelectMenu() {
	int ch = 0;

	do {
		printf("\n(1) 先頭にノードを挿入   (2) 末尾にノードを追加\n");
		printf("(3) 先頭のノードを削除   (4) 全ノードを表示\n");
		printf("(5) 全ノードを削除       (0) 処  理  終  了 : ");
		scanf_s("%d", &ch);
	} while (ch <Term || ch > Clear);

	return (Menu)ch;
}

int main(void) {
	Menu menu{};
	List list{};

	InitList(&list, 64);

	do {
		Node x{};
		switch (menu = SelectMenu()) {
		case Insert:
			x = Read("先頭に挿入");
			InsertNode(&list, x.no, x.name);
			break;
		case Append:
			x = Read("末尾に追加");
			AppendNode(&list, x.no, x.name);
			break;
		case Delete:
			DeleteNode(&list);
			break;
		case Print:
			PrintList(&list);
			break;
		case Clear:
			ClearList(&list);
			break;
		}
	} while (menu != Term);

	TermList(&list);

	return 0;
}
#endif

#ifdef _1203
#define NAME_LEN 16
#define Null -1

typedef enum { Term, Insert, Append, Delete, Clear, Print, Print1, Print2 } Menu;

typedef int Index;

typedef struct {
	int no;
	Index ptr;
} key1;

typedef struct {
	char name[NAME_LEN];
	Index ptr;
} key2;

typedef struct {
	int no;
	char name[NAME_LEN];
	Index next;
	Index Dnext;
} Node;

typedef struct {
	Node* n;
	Index top;
	Index max;
	Index deleted;
	Index max2;
	key1* idx1;
	key2* idx2;
} List;

#define Top (list->top)
#define Second (list->n[Top].next)
#define Next(x) (list->n[(x)].next)

int Search1(List* list, int no, Index* flag) {
	int i = 0;
	int j = list->max2 - 1;

	if (j == -1) {
		*flag = 0;
		return 0;
	}

	do {
		int k = (i + j) / 2;
		int cmp = no - list->idx1[k].no;
		if (!cmp) {
			*flag = k;
			return 1;
		}
		else if (cmp > 0) {
			i = k + 1;
		}
		else {
			j = k - 1;
		}
	} while (i <= j);
	*flag = i;
	
	return 0;
}

int Search2(List* list, const char* name, Index* flag) {
	int i = 0;
	int j = list->max2 - 1;

	if (j == -1) {
		*flag = 0;
		return 0;
	}

	do {
		int k = (i + j) / 2;
		int cmp = strcmp(name, list->idx2[k].name);
		if (!cmp) {
			*flag = 1;
			return 1;
		}
		else if (cmp > 0) {
			i = k + 1;
		}
		else {
			j = k - 1;
		}
	} while (i <= j);
	*flag = i;

	return 0;
}

void Apnd(List* list, int no, const char* name, Index rec) {
	Index i = 0;
	Index idx = 0;

	Search1(list, no, &idx);
	for (i = list->max2; i > idx; i--) list->idx1[i] = list->idx1[i - 1];
	list->idx1[idx].no = list->n[rec].no;
	list->idx1[idx].ptr = rec;

	Search2(list, name, &idx);
	for (i = list->max2; i > idx; i--) list->idx2[i] = list->idx2[i - 1];
	strcpy_s(list->idx2[idx].name, list->n[rec].name);
	list->idx2[idx].ptr = rec;

	list->max2++;
}

void Delt(List* list, Index rec) {
	Index i = 0;

	for (i = 0; list->idx1[i].ptr != rec; i++);
	for (; i < list->max2; i++) list->idx1[i] = list->idx1[i + 1];
	for (i = 0; list->idx2[i].ptr != rec; i++);
	for (; i < list->max2; i++) list->idx2[i] = list->idx2[i + 1];

	list->max2--;
}

int GetIndex(List* list) {
	if (list->deleted == Null) return ++(list->max);
	else {
		Index rec = list->deleted;
		list->deleted = list->n[rec].Dnext;
		return rec;
	}
}

void DeleteIndex(List* list, Index idx) {
	if (list->deleted == Null) {
		list->deleted = idx;
		list->n[idx].Dnext = Null;
	}
	else {
		Index ptr = list->deleted;
		list->deleted = idx;
		list->n[idx].Dnext = ptr;
	}
}

void SetNode(Node* x, int no, const char* name, Index next) {
	x->no = no;
	x->next = next;
	strcpy_s(x->name, name);
}

void InsertNode(List* list, int no, const char* name) {
	Index ptr = Top;

	Top = GetIndex(list);
	SetNode(&list->n[Top], no, name, ptr);
	Apnd(list, no, name, Top);
}

void AppendNode(List* list, int no, const char* name) {
	if (Top == Null) InsertNode(list, no, name);
	else {
		Index ptr = Top;
		while (Next(ptr) != Null) ptr = Next(ptr);
		Next(ptr) = GetIndex(list);
		SetNode(&list->n[Next(ptr)], no, name, Null);
		Apnd(list, no, name, Next(ptr));
	}
}

void DeleteNode(List* list) {
	if (Top != Null) {
		Index ptr = Second;
		DeleteIndex(list, Top);
		Delt(list, Top);
		Top = ptr;
	}
}

void ClearList(List* list) {
	while (Top != Null) DeleteNode(list);
}

void PrintList(const List* list) {
	Index ptr = Top;

	puts(" 【一覧表】 ");
	while (ptr != Null) {
		printf("%5d %-16.16s\n", list->n[ptr].no, list->n[ptr].name);
		ptr = Next(ptr);
	}
}

void PrintList1(const List* list) {
	Index i = 0;

	puts(" 【一覧表/番号順】 ");
	for (i = 0; i < list->max2; i++) {
		Index j = list->idx1[i].ptr;
		printf("%5d %-16.16s\n", list->n[j].no, list->n[j].name);
	}
}

void PrintList2(const List* list) {
	Index i = 0;

	puts(" 【一覧表/名前順】 ");
	for (i = 0; i < list->max2; i++) {
		Index j = list->idx2[i].ptr;
		printf("%5d %-16.16s\n", list->n[j].no, list->n[j].name);
	}
}

void InitList(List* list, int size) {
	list->n = (Node*)calloc(size, sizeof(Node));
	list->idx1 = (key1*)calloc(size, sizeof(key1));
	list->idx2 = (key2*)calloc(size, sizeof(key2));
	
	list->max2 = 0;
	list->top = list->max = list->deleted = Null;
}

void TermList(List* list) {
	ClearList(list);

	free(list->n);
	free(list->idx1);
	free(list->idx2);
}

Node Read(const char* message) {
	Node temp{};

	printf("%sするデータを入力してください。\n", message);
	printf("番号 : "); scanf_s("%d", &temp.no);
	printf("名前 : "); scanf_s("%s", temp.name, NAME_LEN);

	return temp;
}

Menu SelectMenu() {
	int ch = 0;

	do {
		printf("\n(1) 先頭に要素を挿入   (2) 末尾に要素を追加\n");
		printf("(3) 先頭の要素を削除   (4) すべての要素を削除\n");
		printf("(5) 全要素を表示(リスト順)\n");
		printf("(6) 全要素を表示(番号順)\n");
		printf("(7) 全要素を表示(名前順)\n");
		printf("(0) 処   理   終   了   : ");
		scanf_s("%d", &ch);
	} while (ch < Term || ch > Print2);

	return (Menu)ch;
}

int main(void) {
	Menu menu{};
	List list{};

	InitList(&list, 64);

	do {
		Node x{};
		switch (menu = SelectMenu()) {
		case Insert:
			x = Read("先頭に挿入");
			InsertNode(&list, x.no, x.name);
			break;
		case Append:
			x = Read("末尾に追加");
			AppendNode(&list, x.no, x.name);
			break;
		case Delete:
			DeleteNode(&list);
			break;
		case Clear:
			ClearList(&list);
			break;
		case Print:
			PrintList(&list);
			break;
		case Print1:
			PrintList1(&list);
			break;
		case Print2:
			PrintList2(&list);
			break;
		}
	} while (menu != Term);

	TermList(&list);

	return 0;
}
#endif