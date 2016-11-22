
#include <limits.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <bitset>
#include <vector>
#include <algorithm>
#include <list>

using std::cout;
using std::endl;

// utility
template <typename T>
void fill_list(T *list, int size, int range)
{
	for (int i=0; i<size; i++) {
		list[i] = std::rand() % range;
	}
}

template <typename T>
void fill_list(T *list, int size, int range, int negative)
{
	for (int i=0; i<size; i++) {
		list[i] = std::rand() % range * (negative?(std::rand()%2?-1:1):1);
	}
}

template <typename T>
void print_list(T * list, int size)
{
	for (int i=0; i<size; i++) {
		std::cout << " " << list[i];
	}
	std::cout << std::endl;
}

template <typename T>
bool is_sort(T * list, int size)
{
	for (int i=0; i<size-1; i++) {
		if (list[i] > list[i+1]) {
			return false;
		}
	}
	return true;
}

template <typename T>
void print_is_sort(T * list, int size)
{
	cout << "is_sort()=\t" << (is_sort(list, size)?"true":"false") << endl;
}

template <typename T>
bool is_nox(T * list, int x, int value)
{
	if (list[x-1] == value) {
		return true;
	}
	return false;
}

template <typename T>
void print_is_nox(T * list, int x, int value)
{
	cout << "is_nox()=" << (is_nox(list, x, value)?"true":"false") << endl;
}

template <typename T>
void print_list_binary(T * list, int size)
{
	if (list == NULL) { return; };
	for (int i=0; i<size; i++) {
		std::cout << std::bitset<8>(list[i]) << endl;
	}
}

// only can handle sorted array
template <typename T> int binary_search(T *a, const T &x, int size)
{
	int left = 0; int right = size - 1;
	while (left <= right) {
		int mid = (left + right) / 2;
		if (x == a[mid]) { return mid; };
		if (x < a[mid]) {
			right = mid - 1;	
		} else {
			left = mid + 1;
		}
		// cout << "left=" << left << " right=" << right << endl;
	}
	return -1;
}

// binary search in sorted list
int test0() 
{
	cout << "-------------binary search--------------" << endl;

	int ret;
	int find;

	int a1[3] = {1, 2, 4};
	find = 1;
	ret = binary_search(a1, find, 3);
	cout << "find 1 ret=" << ret << endl;

	int a2[5] = {2, 4, 6, 8, 10};
	find = 10;
	ret = binary_search(a2, find, 5);
	cout << "find 10 ret=" << ret << endl;

	int a3[5] = {2, 4, 6, 8, 10};
	find = 7;
	ret = binary_search(a3, find, 5);
	cout << "find 7 ret=" << ret << endl;

	return 0;
}

// sorted lista[left,mid] and lista[mid+1,right] merge into listb[left,right]
template <typename T>
void merge(int *lista, T *listb, int left, int mid, int right)
{
	int left_ptr = left;
	int right_ptr = mid + 1;
	int b_ptr = left;
	while ((left_ptr <= mid) && (right_ptr <= right)) {
		if (lista[left_ptr] < lista[right_ptr]) {
			listb[b_ptr] = lista[left_ptr];
			b_ptr++;
			left_ptr++;
		} else {
			listb[b_ptr] = lista[right_ptr];
			b_ptr++;
			right_ptr++;
		}
	}

	// copy last into listb
	if (left_ptr > mid) {
		for (;right_ptr<=right; right_ptr++) {
			listb[b_ptr] = lista[right_ptr];
			b_ptr++;
		}
	} else {
		for (;left_ptr<=mid; left_ptr++) {
			listb[b_ptr] = lista[left_ptr];
			b_ptr++;
		}
	}
	memcpy(lista+left, listb+left, (right-left+1)*sizeof(int));
}

template <typename T>
void merge_sort(T *lista, int size)
{
	T *listb = new T[size];

	int step = 1;
	while (step < size) {
		int index=0;
		// merge same size range
		for (index=0; index+2*step-1 < size; index+=2*step) {
			merge(lista, listb, index, index+step-1, index+2*step-1);
		}

		// two different size range at last, merge them
		if (index > 0 && (index+step < size)) {
			merge(lista, listb, index, index+step-1, size-1);
		}

		// merge final 2 range
		if (index == 0) {
			merge(lista, listb, 0, index+step-1, size-1);
		}
		step *= 2;
	}

}

// merge sort
int test1()
{

	cout << "-------------merge sort--------------" << endl;
	int lista[10] = {2, 3, 7, 9, 18, 1, 2, 5, 7, 10};
	int listb[10];
	merge(lista, listb, 0, 4, 9);
	std::cout << "lista: ";
	for (int i=0; i<10; i++) {
		std::cout << " " << lista[i];
	}
	std::cout << std::endl;

	int size = 2;
	int range = 30;
	int list1[size];
	fill_list(list1, size, range);
	std::cout << "before: list1: ";
	print_list(list1, size);

	merge_sort(list1, size);
	std::cout << "after: list1: ";
	print_list(list1, size);
	print_is_sort(list1, size);

	size = 3;
	int list2[size];
	fill_list(list2, size, range);
	std::cout << "before: list2: ";
	print_list(list2, size);

	merge_sort(list2, size);
	std::cout << "after: list2: ";
	print_list(list2, size);
	print_is_sort(list2, size);

	size = 4;
	int list4[size];
	fill_list(list4, size, range);
	std::cout << "before: list4: ";
	print_list(list4, size);

	merge_sort(list4, size);
	std::cout << "after: list4: ";
	print_list(list4, size);
	print_is_sort(list4, size);

	size = 5;
	int list5[size];
	fill_list(list5, size, range);
	std::cout << "before: list5: ";
	print_list(list5, size);

	merge_sort(list5, size);
	std::cout << "after: list5: ";
	print_list(list5, size);
	print_is_sort(list5, size);

	size = 10;
	int list10[size];
	fill_list(list10, size, range);
	std::cout << "before: list10: ";
	print_list(list10, size);

	merge_sort(list10, size);
	std::cout << "after: list10: ";
	print_list(list10, size);
	print_is_sort(list10, size);

	size = 11;
	int list11[size];
	fill_list(list11, size, range);
	std::cout << "before: list11: ";
	print_list(list11, size);

	merge_sort(list11, size);
	std::cout << "after: list11: ";
	print_list(list11, size);
	print_is_sort(list11, size);

	return 0;
}

// not good enough
template <typename T>
void quick_sort_old(T *list, int left, int end)
{
	int left_ptr = left;
	int right_ptr = end-1;

	if (left_ptr >= right_ptr) {
		return;
	}

	int key = list[left];

	// jump and dig
	while (left_ptr < right_ptr) {
		while (list[right_ptr] > key && left_ptr < right_ptr) {
			right_ptr--;
		}
		if (left_ptr >= right_ptr) {
			break;
		}
		list[left_ptr] = list[right_ptr];
		left_ptr++;

		while (list[left_ptr] <= key && left_ptr < right_ptr) {
			left_ptr++;
		}
		if (left_ptr >= right_ptr) {
			break;
		}
		list[right_ptr] = list[left_ptr];
		right_ptr--;
	}
	// here assert left_ptr = right_ptr
	list[left_ptr] = key;
	quick_sort_old(list, left, left_ptr);	
	quick_sort_old(list, left_ptr+1, end);	
}

// best quick sort
template <typename T>
void quick_sort(T *list, int left, int end)
{
	// first.
	// 3 4 2 6 4   (key is 3)
	// i j
	// second.
	// 3 4 2 6 4   (key is 3), need to swap A[i+1] and A[j], i++,j++
	// i   j
	// thrid.
	// 3 2 4 6 4   (key is 3)
	//   i   j
	int i=left;
	int j=i+1;
	if (j >= end) {
		return;
	}

	// choose first value as key
	int key = list[left];
	T tmp;
	while (j < end) {
		// compair all backward value
		if (list[j] > key) {
			j++;
			continue;
		}

		// if get a value less then key, swap it and list[i+1], and i,j ++
		i++;
		tmp = list[j];
		list[j] = list[i];
		list[i] = tmp;
		j++;
	}
	// swap list[i] and key:list[left]
	tmp = list[i];
	list[i] = list[left];
	list[left] = tmp;
	quick_sort(list, left, i);
	quick_sort(list, i+1, end);
}

int test2()
{
	cout << "-------------quick sort--------------" << endl;


	int size = 10;
	int range = 30;

	int list1[size];
	fill_list(list1, size, range);
	std::cout << "before: list1: ";
	print_list(list1, size);

	quick_sort_old(list1, 0, size);
	std::cout << "after: list1: ";
	print_list(list1, size);
	print_is_sort(list1, size);

	size = 10;
	int list2[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	std::cout << "before: list2: ";
	print_list(list2, size);

	quick_sort(list2, 0, size);
	std::cout << "after: list2: ";
	print_list(list2, size);

	size = 1;
	int list3[size];
	fill_list(list3, size, range);
	std::cout << "before: list3: ";
	print_list(list3, size);

	quick_sort(list3, 0, size);
	std::cout << "after: list3: ";
	print_list(list3, size);
	print_is_sort(list3, size);

	size = 2;
	int list4[size];
	fill_list(list4, size, range);
	std::cout << "before: list4: ";
	print_list(list4, size);

	quick_sort(list4, 0, size);
	std::cout << "after: list4: ";
	print_list(list4, size);
	print_is_sort(list4, size);

	size = 3;
	int list5[size];
	fill_list(list5, size, range);
	std::cout << "before: list5: ";
	print_list(list5, size);

	quick_sort(list5, 0, size);
	std::cout << "after: list5: ";
	print_list(list5, size);
	print_is_sort(list5, size);

	size = 10;
	int list6[size];
	fill_list(list6, size, range);
	std::cout << "before: list6: ";
	print_list(list6, size);

	quick_sort(list6, 0, size);
	std::cout << "after: list6: ";
	print_list(list6, size);
	print_is_sort(list6, size);

	return 0;
}

template <typename T>
void bubble_sort(T *list, int size)
{
	T tmp;
	for (int i=0; i<size-1; i++) {
		for (int j=0; j<size-1-i; j++) {
			if (list[j] > list[j+1]) {
				tmp = list[j+1];
				list[j+1] = list[j];
				list[j] = tmp;
			}
		}
	}
}

int test3()
{
	cout << "-------------bubble sort--------------" << endl;


	int size = 10;
	int range = 30;

	int list1[size];
	fill_list(list1, size, range);
	std::cout << "before: list1: ";
	print_list(list1, size);

	bubble_sort(list1, size);
	std::cout << "after: list1: ";
	print_list(list1, size);

	return 0;
}


// get small no.x in list, O(n), base on quick_sort
template <typename T>
int get_nox(T *list, int left, int end, int x)
{
	if (x > end || x < 1) {
		return -1;
	}
	int index = x - 1;

	int i = left;
	int j = left+1;
	int key = list[i];

	int tmp;

	while (j < end) {
		if (list[j] > key) {
			j++;
			continue;
		}
		i++;
		tmp = list[i];
		list[i] = list[j];
		list[j] = tmp;
		j++;
	}
	tmp = list[i];
	list[i] = list[left];
	list[left] = tmp;

	if (index == i) {
		return list[i];
	}
	if (index < i) {
		return get_nox(list, left, i, x);	
	} else {
		return get_nox(list, i+1, end, x);	
	}
}

int test4()
{
	cout << "-------------get no.x--------------" << endl;

	int ret;

	int size;
	int range;
	int x;

	size = 2;
	range = 30;
	x = std::rand() % size + 1;
	int list1[size];
	fill_list(list1, size, range);
	std::cout << "before: list1: ";
	print_list(list1, size);
	ret = get_nox(list1, 0, size, x);
	cout << "no." << x << " ret= " << ret << endl;
	quick_sort(list1, 0, size);
	std::cout << "after: list1: ";
	print_list(list1, size);
	print_is_nox(list1, x, ret);

	size = 3;
	range = 30;
	x = std::rand() % size + 1;
	int list2[size];
	fill_list(list2, size, range);
	std::cout << "before: list2: ";
	print_list(list2, size);
	ret = get_nox(list2, 0, size, x);
	cout << "no." << x << " ret= " << ret << endl;
	quick_sort(list2, 0, size);
	std::cout << "after: list2: ";
	print_list(list2, size);
	print_is_nox(list2, x, ret);

	size = 5;
	range = 30;
	x = std::rand() % size + 1;
	int list3[size];
	fill_list(list3, size, range);
	std::cout << "before: list3: ";
	print_list(list3, size);
	ret = get_nox(list3, 0, size, x);
	cout << "no." << x << " ret= " << ret << endl;
	quick_sort(list3, 0, size);
	std::cout << "after: list3: ";
	print_list(list3, size);
	print_is_nox(list3, x, ret);


	return 0;
}

/*
0000
0001
0011
0010
------- glass
0110
0111
0101
0100
*/
int * create_gray_list(int n)
{
	if (n < 0) {
		return NULL;
	}

	int size = 1<<n;
	int *list = new int[size];
	list[0] = 0;

	for (int times = 0; times < n; times++) {
		int step = 1 << times;
		for (int i=0; i<step; i++) {
			list[2*step-i-1] = list[i] + step;
		}
	}

	return list;
}

int test5()
{
	cout << "-------------gary code--------------" << endl;

	int n;

	int *list = NULL;

	n = 0;
	list = create_gray_list(n);
	cout << "list: n=" << n << endl;
	print_list_binary(list, 1<<n);
	delete []list;

	n = 1;
	list = create_gray_list(n);
	cout << "list: n=" << n << endl;
	print_list_binary(list, 1<<n);
	delete []list;

	n = 2;
	list = create_gray_list(n);
	cout << "list: n=" << n << endl;
	print_list_binary(list, 1<<n);
	delete []list;

	n = 3;
	list = create_gray_list(n);
	cout << "list: n=" << n << endl;
	print_list_binary(list, 1<<n);
	delete []list;

	n = 4;
	list = create_gray_list(n);
	cout << "list: n=" << n << endl;
	print_list_binary(list, 1<<n);
	delete []list;

	n = 5;
	list = create_gray_list(n);
	cout << "list: n=" << n << endl;
	print_list_binary(list, 1<<n);
	delete []list;

	return 0;
}

void insertion_sort(int * list, int size)
{
	if (size == 1) {
		return ;
	}

	/*
	 * 1 3 4 2 7 5 6
	 * 1 3-> 4-> 2 7 5 6   (key is 2)
	 *   |_______|
	 * 1 3 3 4 7 5 6       (cover j+1)
	 * 1 2 3 4 7 5 6	   (reset key final place)
	 *
	 */

	for (int i = 1; i < size; i++) {
		/*
		int key = i;
		for (int j = i - 1; j >= 0; j--) {
			if (list[key] >= list[j]) { break;};
			int tmp = list[j];
			list[j] = list[key];
			list[key] = tmp;
			key = j;
		}
		*/

		int key = list[i];
		int j = i-1;
		while (j >= 0 && list[j] > key) {
			list[j+1] = list[j];
			j--;
		}
		list[j+1] = key;
		
	}
}

int test6()
{
	cout << "-------------insertion sort--------------" << endl;

	int size = 1;
	int range = 30;

	int list1[size];
	fill_list(list1, size, range);
	std::cout << "before: list1: ";
	print_list(list1, size);

	insertion_sort(list1, size);
	std::cout << "after: list1: ";
	print_list(list1, size);
	print_is_sort(list1, size);

	size = 2;
	int list2[size];
	fill_list(list2, size, range);
	std::cout << "before: list2: ";
	print_list(list2, size);

	insertion_sort(list2, size);
	std::cout << "after: list2: ";
	print_list(list2, size);
	print_is_sort(list2, size);

	size = 3;
	int list3[size];
	fill_list(list3, size, range);
	std::cout << "before: list3: ";
	print_list(list3, size);

	insertion_sort(list3, size);
	std::cout << "after: list3: ";
	print_list(list3, size);
	print_is_sort(list3, size);

	size = 4;
	int list4[size];
	fill_list(list4, size, range);
	std::cout << "before: list4: ";
	print_list(list4, size);

	insertion_sort(list4, size);
	std::cout << "after: list4: ";
	print_list(list4, size);
	print_is_sort(list4, size);

	size = 10;
	int list5[size];
	fill_list(list5, size, range);
	std::cout << "before: list5: ";
	print_list(list5, size);

	insertion_sort(list5, size);
	std::cout << "after: list5: ";
	print_list(list5, size);
	print_is_sort(list5, size);
	return 0;
}

int max_child_list(int * list, int size)
{
	/*
	n is size
	index = [0,n];
	Start(x) is start from x max child list value
	M(0) is final answer
	M(0) = max{list[0], list[0]+Start(1), M(1)}
	M(1) = max{list[1], list[1]+Start(1+1), M(1+1)}
	M(n-1) = max{list[n-1], list[n]+Start(n), M(n)}
	M(n) = max{list[n], list[n]+Start(n+1), M(n+1)}

	Start(1) = max{list[1], list[1] + Start(1+1)}
	Start(n-2) = max{list[n-2], list[n-2] + Start(n-1)}
	Start(n-1) = max{list[n-1], list[n-1] + Start(n)}
	Start(n) = list[n];

	*/
	if (size == 1) {
		return list[0];
	}

	int s = 0; // s is Start(x)
	int max = list[size-1];

	for (int i=size-2; i>=0; i--) {
		// get Start(i+1)
		s = std::max(list[i+1], list[i+1] + s);	
		// get max in list[i], list[i]+Start(i+1)
		int t = std::max(list[i], list[i]+s);
		// param max is M(i+1)
		max = std::max(t, max);
	}

	return max;
}


int test7()
{
	cout << "-------------max child list--------------" << endl;

	int ret;
	int size;
	int range = 20;

	size = 1;
	int list1[size];
	fill_list(list1, size, range, 1);
	std::cout << "before: list1: ";
	print_list(list1, size);
	ret = max_child_list(list1, size);
	std::cout << "ret=" << ret << std::endl;


	size = 2;
	int list2[size];
	fill_list(list2, size, range, 1);
	std::cout << "before: list2: ";
	print_list(list2, size);
	ret = max_child_list(list2, size);
	std::cout << "ret=" << ret << std::endl;

	size = 3;
	int list3[size];
	fill_list(list3, size, range, 1);
	std::cout << "before: list3: ";
	print_list(list3, size);
	ret = max_child_list(list3, size);
	std::cout << "ret=" << ret << std::endl;

	size = 5;
	int list4[size];
	fill_list(list4, size, range, 1);
	std::cout << "before: list4: ";
	print_list(list4, size);
	ret = max_child_list(list4, size);
	std::cout << "ret=" << ret << std::endl;

	size = 10;
	int list5[size];
	fill_list(list5, size, range, 1);
	std::cout << "before: list5: ";
	print_list(list5, size);
	ret = max_child_list(list5, size);
	std::cout << "ret=" << ret << std::endl;



	return 0;
}

typedef struct node_t{
	int value;
	node_t * next;
} Node;

void print_node_list(Node *list)
{
	Node * n;
	n = list;
	while (n != NULL) {
		cout << " " << n->value;
		n = n->next;
	}
	cout << endl;
}

Node * reverse_linked_list(Node *list)
{
	if (list == NULL) {
		return NULL;
	}

	Node *current = list;
	Node *nt = current->next;
	Node *head = list;
	
	while (nt != NULL) {
		current->next = nt->next;
		nt->next = head;
		head = nt;
		nt = current->next;
	}
	return head;
}

int test8()
{
	cout << "-------------reverse linked list--------------" << endl;
	int size;

	size = 3;
	Node *list = new Node();
	list->value = 0;
	list->next = NULL;
	Node ** tail = &list->next;
	for (int i=1; i<size; i++) {
		Node * node = new Node();
		node->value = i;
		node->next = NULL;
		*tail = node;
		tail = &node->next;
	}

	cout << "before list:";
	print_node_list(list);

	list = reverse_linked_list(list);
	cout << "before list:";
	print_node_list(list);



	return 0;
}


// father base 1
void heap_adjust(int *list, int father, int size)
{
	// left child always (father*2)
	int lchild = father * 2;
	// right child always (father*2+1)
	int rchild = father * 2 + 1;
	int max = father;

	// no need to adjust not father leaf
	if (father > size/2) {
		return;
	}

	// compair father, left child, right child
	if (rchild <= size && list[rchild-1] > list[max-1]) {
		max = rchild;
	}
	if (lchild <= size && list[lchild-1] > list[max-1]) {
		max = lchild;
	}

	// swap bewteen max child and father
	if (max != father) {
		int tmp = list[father-1];
		list[father-1] = list[max-1];
		list[max-1] = tmp;

		// adjust from that child, make it still a heap
		heap_adjust(list, max, size);
	}

}

// max heap, root is bigest value
void heap_sort(int *list, int size)
{
	// index is base 1
	// start from last leaf father = size/2
	for (int i=size/2; i>=1; i--) {
		heap_adjust(list, i, size);
	}

	// now root is max value
	int tmp;
	for (int i=size; i>=1; i--) {
		// swap root and last leaf, now [lastleaf, end] is sort space, total size-1
		tmp = list[i-1];
		list[i-1] = list[0];
		list[0] = tmp;
		// adjust root, make it still a heap, total size-1
		heap_adjust(list, 1, i-1);
	}
}

int test9()
{
	cout << "-------------heap sort--------------" << endl;
	int size;
	int range = 20;

	size = 1;
	int list1[size];
	fill_list(list1, size, range, 0);
	std::cout << "before: list1: ";
	print_list(list1, size);
	heap_sort(list1, size);
	std::cout << "after: list1: ";
	print_list(list1, size);
	print_is_sort(list1, size);

	size = 2;
	int list2[size];
	fill_list(list2, size, range, 0);
	std::cout << "before: list2: ";
	print_list(list2, size);
	heap_sort(list2, size);
	std::cout << "after: list2: ";
	print_list(list2, size);
	print_is_sort(list2, size);

	size = 3;
	int list3[size];
	fill_list(list3, size, range, 0);
	std::cout << "before: list3: ";
	print_list(list3, size);
	heap_sort(list3, size);
	std::cout << "after: list3: ";
	print_list(list3, size);
	print_is_sort(list3, size);

	size = 10;
	int list4[size];
	fill_list(list4, size, range, 0);
	std::cout << "before: list4: ";
	print_list(list4, size);
	heap_sort(list4, size);
	std::cout << "after: list4: ";
	print_list(list4, size);
	print_is_sort(list4, size);

	return 0;
}

typedef struct _tnode {
	_tnode *p;
	_tnode *left;
	_tnode *right;
	int key;
	char value;
} TNode;

typedef struct _tree {
	TNode *root;
} Tree;

void tree_print_node(const TNode * node)
{
	if (node == NULL) {
		cout << "node == NULL" << endl;
	} else {
		cout << "node[" << node->key << "]=" << node->value << endl;
	}
}

TNode * tree_create_leaf(int key, char value)
{
	TNode *leaf = (TNode *)malloc(sizeof(TNode));
	bzero(leaf, sizeof(TNode));
	leaf->p = NULL;
	leaf->left = NULL;
	leaf->right = NULL;
	leaf->key = key;
	leaf->value = value;
	return leaf;
}

int tree_insert(Tree *tree, int key, char value)
{
	if (tree == NULL) {
		return -1;
	}
	if (tree->root == NULL) {
		tree->root = tree_create_leaf(key, value);
		return 0;
	}
	TNode *par;
	TNode *node = tree->root;
	while (node != NULL) {
		par = node;
		if (key == node->key) {
			// already in tree
			return -1;
		} else if (key < node->key) {
			node = node->left;
		} else {
			node = node->right;
		} 
	}
	node = tree_create_leaf(key, value);
	node->p = par;
	if (node->key < par->key) {
		par->left = node;
	} else {
		par->right = node;
	}
	return 0;
}

TNode * __tree_get(Tree *tree, int key)
{
	if (tree->root == NULL) {
		return NULL;
	}
	TNode *r;
	TNode *l = tree->root;
	while (l != NULL) {
		r = l;
		if (key < r->key) {
			l = r->left;
		} else if (key > r->key) {
			l = r->right;
		} else {
			// same key
			return r;
		}
	}
	// leaf not found
	return NULL;
}

int tree_update(Tree *tree, int key, char value)
{
	TNode * leaf = __tree_get(tree, key);
	if (leaf == NULL) {
		return -1;
	}
	leaf->value = value;
	return 0;
}

const TNode * tree_search(Tree *tree, int key)
{
	return __tree_get(tree, key);
}


TNode * tnode_minimum(TNode *plant)
{
	TNode *min = NULL;
	while (plant != NULL) {
		min = plant;
		plant = plant->left;
	}
	if (min != NULL) {
		tree_print_node(min);
	}
	return min;
}

void tree_delete(Tree *tree, int key)
{
	TNode *leaf = __tree_get(tree, key);
	if (leaf == NULL) {
		return;
	}

	/*
	 *  A
	 *  |
	 *  Z
	 */
	// no child, delete it
	if (leaf->left == NULL && leaf->right == NULL) {
		cout << "tree_delete: case 1" << endl;
		if (leaf->p == NULL) {
			tree->root = NULL;
		} else if (leaf->p->left == leaf) {
			leaf->p->left = NULL;
		} else {
			leaf->p->right = NULL;
		}
		free(leaf);
		return;
	}

	/*
	 *    A
	 *    |
	 *    Z
	 *   /
	 *  Y
	 */
	// only has left child
	if (leaf->left != NULL && leaf->right == NULL) {
		cout << "tree_delete: case 2" << endl;
		TNode *child = leaf->left;
		child->p = leaf->p;
		if (child->p == NULL) {
			tree->root = child;
		} else if (leaf->p->left == leaf) {
			leaf->p->left = child;
		} else {
			leaf->p->right = child;
		}
		free(leaf);
		return;
	}

	/*
	 *    A
	 *    |
	 *    Z
	 *     \
	 * 		Y 
	 */
	// only has right child
	if (leaf->left == NULL && leaf->right != NULL) {
		cout << "tree_delete: case 3" << endl;
		TNode *child = leaf->right;
		child->p = leaf->p;
		if (child->p == NULL) {
			tree->root = child;
		} else if (leaf->p->left == leaf) {
			leaf->p->left = child;
		} else {
			leaf->p->right = child;
		}
		free(leaf);
		return;
	}

	/*
	 *    A
	 *    |
	 *    Z
	 *   / \
	 *  X	Y 
	 *     / \
	 *    nil
	 */
	// has both child
	// right child minimum leaf, use for replace the delete leaf
	TNode *min = tnode_minimum(leaf->right);
	// min is leaf right child, min replace leaf
	if (min == leaf->right) {
		cout << "tree_delete: case 4" << endl;
		min->left = leaf->left;	
		if (min->left != NULL) {
			min->left->p = min;
		}
		min->p = leaf->p;
		if (min->p == NULL) {
			tree->root = min;
		} else if (leaf->p->left == leaf) {
			leaf->p->left = min;
		} else {
			leaf->p->right = min;
		}
		return;
	}

	/*
	 *    A
	 *    |
	 *    Z
	 *   / \
	 *  X	K 
	 *     / \
	 *    Y   ...
	 *   / \
	 *  nil ...
	 */
	cout << "tree_delete: case 5" << endl;
	// min is not leaf right child, is leaf right child minimum child (leaf->right->left->left...), and min has no left child, because it is minimum, and min is min->p->left
	// 1. bind min->p and min->right
	min->p->left = min->right;
	if (min->right != NULL) {
		min->right->p = min->p;
	}
	// 2. bind min and leaf->right 
	min->right = leaf->right;
	leaf->right->p = min;
	// 3. min replace leaf
	min->left = leaf->left;	
	if (min->left != NULL) {
		min->left->p = min;
	}
	min->p = leaf->p;
	if (min->p == NULL) {
		tree->root = min;
	} else if (leaf->p->left == leaf) {
		leaf->p->left = min;
	} else {
		leaf->p->right = min;
	}

}

void tnode_print(const TNode *node, int skip, int fskip, const char lr)
{
	if (node == NULL) {
		return;
	}
	for (int i=0; i<fskip; i++) {
		cout << " ";
	}

	if (fskip > 0) {
		cout << "╎";
	}
	for (int i=0; i<skip; i++) {
		cout << "─";
	}
	cout << " " << lr << " " << node->key << "[" << node->value << "]" << endl;
	fskip += skip + 2;
	tnode_print(node->left, skip, fskip, 'L');
	tnode_print(node->right, skip, fskip, 'R');
}

void tree_print(const Tree *tree)
{
	if (tree == NULL) {
		return;
	}
	if (tree->root == NULL) {
		return;
	}

	TNode * node = tree->root;
	cout << node->key << "[" << node->value << "]" << endl;
	tnode_print(node->left, 2, 1, 'L');
	tnode_print(node->right, 2, 1, 'R');
}

void tree_print2(const Tree *tree)
{
	if (tree == NULL) {
		return;
	}
	if (tree->root == NULL) {
		return;
	}
	std::vector<TNode*> vec;
	vec.push_back(tree->root);
	int cur = 0;
	int last = 0;
	while (vec.size() > cur) {
		last = vec.size();
		while (last > cur) {
			cout << vec[cur]->key << "[" << vec[cur]->value << "] ";
			if (vec[cur]->left != NULL) {
				vec.push_back(vec[cur]->left);
			}
			if (vec[cur]->right != NULL) {
				vec.push_back(vec[cur]->right);
			}
			cur ++;
		}
		cout << endl;
	}
}

int test10()
{
	cout << "-------------tree--------------" << endl;
	
	const int size = 7;
	int keys[size]    = { 3,   1,   5,   6,   4,   2,   8 };
	char values[size] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

	Tree t1;
	t1.root = NULL;
	for (int i=0; i<size; i++) {
		tree_insert(&t1, keys[i], values[i]);
	}
	tree_print(&t1);

	const TNode * node;
	int key;
	char value;

	cout << "--- tree search test:" << endl;
	key = 5;
	node = tree_search(&t1, key);
	tree_print_node(node);

	key = 2;
	node = tree_search(&t1, key);
	tree_print_node(node);

	key = 7;
	node = tree_search(&t1, key);
	tree_print_node(node);

	cout << "--- tree update test:" << endl;
	key = 2;
	value = 'H';
	tree_update(&t1, key, value);
	node = tree_search(&t1, key);
	tree_print_node(node);

	key = 7;
	value = 'I';
	tree_insert(&t1, key, value);
	node = tree_search(&t1, key);
	tree_print_node(node);

	key = 7;
	value = 'I';
	tree_insert(&t1, key, value);
	node = tree_search(&t1, key);
	tree_print_node(node);

	tree_print(&t1);

	cout << "--- tree delete test:" << endl;

	key = 8;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	key = 3;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	key = 4;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	key = 5;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	key = 6;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	key = 7;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	key = 2;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	key = 1;
	cout << "delete " << key << endl;
	tree_delete(&t1, key);
	tree_print(&t1);

	return 0;
}

enum RB_COLOR {RED, BLACK};

typedef struct _rbtnode {
	_rbtnode *p;
	_rbtnode *left;
	_rbtnode *right;
	int key;
	char value;
	int color;
} RBTNode;

typedef struct _rbtree {
	RBTNode *root;
} RBTree;

RBTNode * rbtree_create_node(int key, char value)
{
	RBTNode *leaf = (RBTNode *)malloc(sizeof(RBTNode));
	bzero(leaf, sizeof(RBTNode));
	leaf->p = NULL;
	leaf->left = NULL;
	leaf->right = NULL;
	leaf->key = key;
	leaf->value = value;
	leaf->color = RED;
	return leaf;
}

RBTNode * __rbtree_get(RBTree * tree, int key)
{
	if (tree == NULL) {
		return NULL;
	}
	if (tree->root == NULL) {
		return NULL;
	}

	RBTNode * node = tree->root;
	while (node != NULL) {
		if (node->key == key) {
			return node;
		} else if (node->key < key) {
			node = node->left;
		} else {
			node = node->right;
		}
	}
	return NULL;
}

// exchange between node and node->right, node->right will be node father
// therefore, old (node->right) be (node->p), old (node->right->left) be new (node->right)
void __rbtree_left_rotate(RBTree *tree, RBTNode *node)
{
	if (node == NULL) {
		return;
	}
	RBTNode *child = node->right;
	if (child == NULL) {
		return;
	}

	node->right = child->left;
	if (node->right != NULL) {
		node->right->p = node;
	}

	child->left = node;
	child->p = node->p;
	if (child->p != NULL) {
		if (child->p->left == node) {
			child->p->left = child;
		} else {
			child->p->right = child;
		}
	} else {
		tree->root = child;
	}
	node->p = child;
}

// exchange between node and node->left, node->left will be node father
// therefore, old (node->left) be (node->p), old (node->left->right) be new (node->left)
void __rbtree_right_rotate(RBTree *tree, RBTNode *node)
{
	if (node == NULL) {
		return;
	}
	RBTNode *child = node->left;
	if (child == NULL) {
		return;
	}

	node->left = child->right;
	if (node->left != NULL) {
		node->left->p = node;
	}

	child->right = node;
	child->p = node->p;
	if (child->p != NULL) {
		if (child->p->left == node) {
			child->p->left = child;
		} else {
			child->p->right = child;
		}
	} else {
		tree->root = child;
	}
	node->p = child;
}

void __rbtree_adjust(RBTree *tree, RBTNode * node)
{
	// how to remember:
	// father is left child of grand: <
	// 1. change color, jump to grand
	// 2. if node is right child of father, jump to father, left rotate (zigzag to straight)
	// 3. change father, grand color, grand right rotate

	// father is right child of grand: >
	// 1. change color, jump to grand
	// 2. if node is left child of father, jump to father, right rotate (zigzag to straight)
	// 3. change father, grand color, grand left rotate

	while (node->p != NULL && node->p->color == RED) {

		// first. node father is node grand father left child
		//
		//     G
		//    / \
		//   F   U
		//    \
		//     node
		//
		if (node->p == node->p->p->left) {
			// case 1. father and uncle are red, change color, use grand father for next step
			//     G(B)                  G(R)(node)
			//    /  \_                 /  \_
			//   F(R)  U(R)       -->  F(B)  U(B)
			//    \_                    \
			//     D(R)(node)            D(R)
			//
			RBTNode * uncle = node->p->p->right;
			if (uncle->color == RED) {
				cout << "left case 1" << endl;
				node->p->color = BLACK;
				uncle->color = BLACK;
				node->p->p->color = RED;
				node = node->p->p; // use node grand father
			}

			// case 2. node is father right child, change node to father, node left_rotate()
			//     G               G                               G
			//    /               /          node left_rotate     /
			//   C         -->   C(node)    ----------------->   D
			//    \_              \_                            /
			//     D(node)          D                          C(node)
			//
			else if (node == node->p->right) {
				cout << "left case 2" << endl;
				node = node->p;
				// for change node to node->right->left
				__rbtree_left_rotate(tree, node);
			}

			// case 3. node is father left child, change father and grand father color, grand father right_rotate()
			//     G(B)            G(R)                               D(B)
			//    /               /            G right_rotate        / \
			//   D(R)    ---->   D(B)          ----------------->   /   \
			//  /               /                                  /     \
			// C(node)         C(node)                            C(node) G(R)
			//
			else {
				cout << "left case 3" << endl;
				node->p->color = BLACK;
				node->p->p->color = RED;
				// for change node to node->left->right
				__rbtree_right_rotate(tree, node->p->p);
			}
		} 

		// second. node father is node grand father right child
		//
		//     G
		//    / \
		//   U   F
		//        \
		//        node
		//
		else if (node->p == node->p->p->right) {
			// case 1. father and uncle are red, change color, use grand father for next step
			RBTNode * uncle = node->p->p->left;
			if (uncle->color == RED) {
				cout << "right case 1" << endl;
				node->p->color = BLACK;
				uncle->color = BLACK;
				node->p->p->color = RED;
				node = node->p->p; // use node grand father

			// case 2. node is father left child, change node to father, node right_rotate()
			} else if (node == node->p->left) {
				cout << "right case 2" << endl;
				node = node->p;
				// for change node to node->left->right
				__rbtree_right_rotate(tree, node);

			// case 3. node is father right child, change father and grand father color, grand father left_rotate()
			} else {
				cout << "right case 3" << endl;
				node->p->color = BLACK;
				node->p->p->color = RED;
				// for change node to node->right->left
				__rbtree_left_rotate(tree, node->p->p);
			}

		}
		tree->root->color = BLACK;
	}
}

int rbtree_insert(RBTree *tree, int key, char value)
{
	if (tree == NULL) {
		return -1;
	}
	if (tree->root == NULL) {
		tree->root = rbtree_create_node(key, value);
		tree->root->color = BLACK; // root node always black
		return 0;
	}

	RBTNode *par;
	RBTNode *node = tree->root;
	while (node != NULL) {
		par = node;
		if (key == node->key) {
			// already in tree
			return -1;
		} else if (key < node->key) {
			node = node->left;	
		} else {
			node = node->right;	
		}
	}
	node = rbtree_create_node(key, value);
	node->p = par;
	if (node->key < par->key) {
		par->left = node;
	} else {
		par->right = node;
	}

	__rbtree_adjust(tree, node);
	
	return 0;
}

void rbtnode_print(const RBTNode *node, int skip, int fskip, const char lr)
{
	if (node == NULL) {
		return;
	}
	for (int i=0; i<fskip; i++) {
		cout << " ";
	}

	if (fskip > 0) {
		cout << "╎";
	}
	for (int i=0; i<skip; i++) {
		cout << "─";
	}
	cout << " " << lr << " " << node->key << "[" << node->value << "](" << node->color << ")" << endl;
	fskip += skip + 2;
	rbtnode_print(node->left, skip, fskip, 'L');
	rbtnode_print(node->right, skip, fskip, 'R');
}

void rbtree_print(const RBTree *tree)
{
	if (tree == NULL) {
		return;
	}
	if (tree->root == NULL) {
		return;
	}

	RBTNode * node = tree->root;
	cout << node->key << "[" << node->value << "](" << node->color << ")"<< endl;
	rbtnode_print(node->left, 2, 1, 'L');
	rbtnode_print(node->right, 2, 1, 'R');
}

int test11()
{
	cout << "-------------red-black tree--------------" << endl;
	
	const int size = 9;
	int keys[size]    = { 11,  2,   14,  1,   7,   15,  5,   8,   4};
	char values[size] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};

	RBTree t1;
	t1.root = NULL;
	for (int i=0; i<size; i++) {
		cout << "insert " << keys[i] << endl;
		rbtree_insert(&t1, keys[i], values[i]);
		rbtree_print(&t1);
		cout << endl;
	}
	return 0;
}

struct MaxStack
{
	std::list<int> container;
	int maxNum;
	void push(int num)
	{
		if (container.size() == 0)
		{
			maxNum = num;
			container.push_front(num);
		}
		else
		{
			if (maxNum > num)
			{
				// less then max, save negative num in container
				num = num - maxNum;
			}
			else
			{
				// more then max, update max, save postive in container
				int tmp = maxNum;
				maxNum = num;
				num = num - tmp;
			}
			container.push_front(num);
		}
	}

	int pop()
	{
		if (container.size() == 0)
		{
			return -INT_MAX;
		}

		int front = container.front();
		container.pop_front();
		if (front <= 0)
		{
			
			return front + maxNum;
		}

		// if front is positive, means the num is max, return max and update max
		int tmp = maxNum;
		maxNum = maxNum - front;
		return tmp;
	}

	int max()
	{
		return maxNum;
	}

	MaxStack() : maxNum(0) {};
};

int test12()
{
	cout << "------------- max in stack --------------" << endl;

	MaxStack s1;
	int input;

	input = 1;
	s1.push(input);
	printf("input=%d, max=%d\n", input, s1.max());

	input = 3;
	s1.push(input);
	printf("input=%d, max=%d\n", input, s1.max());

	input = 2;
	s1.push(input);
	printf("input=%d, max=%d\n", input, s1.max());

	input = 5;
	s1.push(input);
	printf("input=%d, max=%d\n", input, s1.max());

	input = 7;
	s1.push(input);
	printf("input=%d, max=%d\n", input, s1.max());

	input = 7;
	s1.push(input);
	printf("input=%d, max=%d\n", input, s1.max());

	printf("\n");

	int ret;

	ret = s1.pop();
	printf("ret=%d max=%d\n", ret, s1.max());

	ret = s1.pop();
	printf("ret=%d max=%d\n", ret, s1.max());

	ret = s1.pop();
	printf("ret=%d max=%d\n", ret, s1.max());

	ret = s1.pop();
	printf("ret=%d max=%d\n", ret, s1.max());

	ret = s1.pop();
	printf("ret=%d max=%d\n", ret, s1.max());
	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] =
{
	test0
,	test1
,	test2
,	test3
,	test4
,	test5
,	test6
,	test7
,	test8
,	test9
,	test10
,	test11
,	test12
};

int main(int argc, char *argv[])
{

	std::srand(time(NULL));

	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all")) {
			cout << "test:run_all_case" << endl;
			for (int i=0; i<maxcase; i++) {
				cout << endl << "test:run_case[" << i << "]" << endl;
				ret = test_list[i]();
				if (ret != 0) {
					cout << "test:case[" << i << "] " << ret << endl;
					return 0;
				}
			}
			return 0;
		}
		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) {
			testcase = maxcase - 1;
		}
	}

	cout << endl << "test:RUN case[" << testcase << "]" << endl;
	ret = test_list[testcase]();
	if (ret != 0) {
		cout << "test:case[" << testcase << "] " << ret << endl;
	}

	return 0;

}
