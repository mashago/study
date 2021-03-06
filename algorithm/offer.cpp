
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stack>
#include <stdexcept>
#include <exception>
#include <string>
#include <iostream>

template <typename T>
void print_list(T * list, int size)
{
	for (int i=0; i<size; i++) {
		std::cout << " " << list[i];
	}
	std::cout << std::endl;
}

int test0()
{
	return 0;
}

bool duplicate(int numbers[], int length, int *duplication)
{
	if (numbers == nullptr || length <= 0)
	{
		return false;
	}

	for (int i=0; i<length; ++i)
	{
		if (numbers[i] < 0 || numbers[i] > length - 1)
		{
			return false;
		}
	}

	// core: make i in pos numbers[i]
	for (int i=0; i<length; ++i)
	{
		while (numbers[i] != i)
		{
			if (numbers[i] == numbers[numbers[i]])
			{
				*duplication = numbers[i];
				return true;
			}
			// swap
			int temp = numbers[i];
			numbers[i] = numbers[temp];
			numbers[temp] = temp;
		}
	}
	return false;
}

int test1()
{
	int out = -1;
	bool ret = false;
	int numbers[] = {0, 1, 2, 3, 3};
	ret = duplicate(numbers, sizeof(numbers)/sizeof(int), &out);
	printf("ret=%d out=%d\n", ret, out);


	return 0;
}


int countRange(const int *numbers, int length, int start, int end);
int getDuplication(const int *numbers, int length)
{
	if (numbers == nullptr || length <= 0)
	{
		return -1;
	}
	int start = 1;
	int end = length - 1;

	// core change the range we check, not the input array
	while (end >= start)
	{
		int middle = ((end - start) >> 1) + start;
		int count = countRange(numbers, length, start, middle);
		if (end == start)
		{
			if (count > 1)
				return start;
			else
				break;
		}

		if (count > (middle - start + 1))
			end = middle;
		else
			start = middle + 1;
	}
	return -1;
}

int countRange(const int *numbers, int length, int start, int end)
{
	if (numbers == nullptr)
		return 0;
	
	int count = 0;
	for (int i = 0; i < length; ++i)
	{
		if (numbers[i] >= start && numbers[i] <= end)
			++count;
	}
	return count;
}

int test2() 
{
	int out = -1;
	int numbers[] = {1, 2, 3, 3};
	out = getDuplication(numbers, sizeof(numbers)/sizeof(int));
	printf("out=%d\n", out);
	return 0;
}

bool FindTwoDimensionalArray(int *matrix, int rows, int columns, int number)
{
	bool found = false;
	if (matrix != nullptr && rows > 0 && columns > 0)
	{
		int row = 0;
		int column = columns - 1;
		while (row < rows && column >= 0)
		{
			if (matrix[row * columns + column] == number)
			{
				found = true;
				break;
			}
			else if (matrix[row * columns + column] > number)
				--column;
			else
				++row;
		}
	}
	return found;
}

int test3() 
{
	bool ret = false;
	int matrix[4][4] = {{1, 2, 8, 9}, {2, 4, 9, 12}, {4, 7, 10, 13}, {6, 8, 11, 15}};
	ret = FindTwoDimensionalArray(*matrix, 4, 4, 7);
	printf("ret=%d\n", ret);

	return 0;
}

void ReplaceBlank(char string[], int length)
{
	if (string == nullptr || length <= 0)
		return;
	
	int originalLength = 0;
	int numberOfBlank = 0;
	int i = 0;
	while (string[i] != '\0')
	{
		++originalLength;
		if (string[i] == ' ')
			++numberOfBlank;
		++i;
	}

	int newLength = originalLength + numberOfBlank * 2;
	if (newLength > length)
		return;
	
	int indexOfOriginal = originalLength;
	int indexOfNew = newLength;
	while (indexOfOriginal >= 0 && indexOfNew > indexOfOriginal)
	{
		if (string[indexOfOriginal] == ' ')
		{
			string[indexOfNew--] = '0';
			string[indexOfNew--] = '2';
			string[indexOfNew--] = '%';
		}
		else
		{
			string[indexOfNew--] = string[indexOfOriginal];
		}
		--indexOfOriginal;
	}
}

int test4() 
{
	char string[100] = {0};
	strncpy(string, "we are happy", sizeof(string));
	ReplaceBlank(string, sizeof(string));
	printf("string=%s\n", string);

	return 0;
}

struct ListNode
{
	int m_nValue;
	ListNode *m_pNext;
};
void PrintList(ListNode **pHead)
{
	if (pHead == nullptr || *pHead == nullptr)
	{
		return;
	}

	while (*pHead)
	{
		printf("%d ", (*pHead)->m_nValue);
		pHead = &((*pHead)->m_pNext);
	}
	printf("\n");
}
void AddToTail(ListNode **pHead, int value)
{
	ListNode *pNew = new ListNode();
	pNew->m_nValue = value;
	pNew->m_pNext = nullptr;

	if (*pHead == nullptr)
	{
		*pHead = pNew;
	}
	else
	{
		ListNode *pNode = *pHead;
		while (pNode->m_pNext != nullptr)
			pNode = pNode->m_pNext;
		pNode->m_pNext = pNew;
	}
}
void RemoveNode(ListNode **pHead, int value)
{
	if (pHead == nullptr || *pHead == nullptr)
	{
		return;
	}

	ListNode *pToBeDeleted = nullptr;
	if ((*pHead)->m_nValue == value)
	{
		pToBeDeleted = *pHead;
		*pHead = (*pHead)->m_pNext;
	}
	else
	{
		ListNode *pNode = *pHead;
		while (pNode->m_pNext != nullptr && pNode->m_pNext->m_nValue != value)
			pNode = pNode->m_pNext;

		if (pNode->m_pNext != nullptr && pNode->m_pNext->m_nValue == value)
		{
			pToBeDeleted = pNode->m_pNext;
			pNode->m_pNext = pNode->m_pNext->m_pNext;
		}
	}

	if (pToBeDeleted != nullptr)
	{
		delete pToBeDeleted;
		pToBeDeleted = nullptr;
	}

}

void RemoveNode2(ListNode **pHead, int value)
{
	if (pHead == nullptr)
	{
		return;
	}
	for (ListNode **cur = pHead; *cur;)
	{
		ListNode *entry = *cur;
		if (entry->m_nValue == value)
		{
			*cur = entry->m_pNext;
			delete entry;
			return;
		}
		else
			cur = &entry->m_pNext;
	}
}

int test5() 
{
	{
	ListNode *list = nullptr;
	AddToTail(&list, 1);
	AddToTail(&list, 2);
	AddToTail(&list, 4);
	AddToTail(&list, 3);
	PrintList(&list);

	RemoveNode(&list, 4);
	PrintList(&list);

	RemoveNode(&list, 1);
	PrintList(&list);

	RemoveNode(&list, 3);
	PrintList(&list);
	}

	printf("\n");
	{
	ListNode *list = nullptr;
	AddToTail(&list, 1);
	AddToTail(&list, 2);
	AddToTail(&list, 4);
	AddToTail(&list, 3);
	PrintList(&list);

	RemoveNode2(&list, 4);
	PrintList(&list);

	RemoveNode2(&list, 1);
	PrintList(&list);

	RemoveNode2(&list, 3);
	PrintList(&list);

	RemoveNode2(&list, 3);
	PrintList(&list);

	RemoveNode2(nullptr, 3);

	}

	return 0;
}

void PrintListReversingly_Iteratively(ListNode *pHead)
{
	std::stack<ListNode *> nodes;
	ListNode *pNode = pHead;
	while (pNode != nullptr)
	{
		nodes.push(pNode);
		pNode = pNode->m_pNext;
	}

	while (!nodes.empty())
	{
		pNode = nodes.top();
		printf("%d\t", pNode->m_nValue);
		nodes.pop();
	}
	printf("\n");
}

void PrintListReversingly_Iteratively2(ListNode *pHead)
{
	ListNode *pNode = pHead;
	if (pNode != nullptr)
	{
		PrintListReversingly_Iteratively2(pNode->m_pNext);
		printf("%d\t", pNode->m_nValue);
	}
	printf("\n");
}

int test6() 
{
	ListNode *list = nullptr;
	AddToTail(&list, 1);
	AddToTail(&list, 2);
	AddToTail(&list, 4);
	AddToTail(&list, 3);
	PrintListReversingly_Iteratively(list);
	PrintListReversingly_Iteratively2(list);

	PrintListReversingly_Iteratively(nullptr);
	PrintListReversingly_Iteratively2(nullptr);

	return 0;
}

struct BinaryTreeNode
{
	int m_nValue;
	BinaryTreeNode *m_pLeft;
	BinaryTreeNode *m_pRight;
};

BinaryTreeNode *ConstructCore(int *startPreorder, int *endPreorder, int *startInorder, int *endInorder)
{
	// create root node
	int rootValue = startPreorder[0];
	BinaryTreeNode *root = new BinaryTreeNode();
	root->m_nValue = rootValue;
	root->m_pLeft = root->m_pRight = nullptr;

	// check if no child node
	if (startPreorder == endPreorder)
	{
		if (startInorder == endInorder && *startPreorder == *startInorder)
			return root;
		else
			throw std::runtime_error("Invalid input");
	}

	// get root in inorder
	int *rootInorder = startInorder;
	while (rootInorder <= endInorder && *rootInorder != rootValue)
		++rootInorder;
	
	if (rootInorder == endInorder && *rootInorder != rootValue)
		throw std::runtime_error("Invalid input");

	int leftLength = rootInorder - startInorder;
	int *leftPreorderEnd = startPreorder + leftLength;
	if (leftLength > 0)
	{
		// build left tree
		root->m_pLeft = ConstructCore(startPreorder+1, leftPreorderEnd, startInorder, rootInorder-1);
	}

	if (leftPreorderEnd < endPreorder)
	{
		// build right tree
		root->m_pRight = ConstructCore(leftPreorderEnd+1, endPreorder, rootInorder+1, endInorder);
	}

	return root;
}

BinaryTreeNode *Construct(int *preorder, int *inorder, int length)
{
	if (preorder == nullptr || inorder == nullptr || length <= 0)
		return nullptr;
	return ConstructCore(preorder, preorder + length - 1, inorder, inorder + length -1);
}

int test7() 
{
	int preorder[] = {1, 2, 4, 7, 3, 5, 6, 8};
	int inorder[] = {4, 7, 2, 1, 5, 3, 8, 6};
	Construct(preorder, inorder, sizeof(preorder)/sizeof(int));
	return 0;
}

struct BinaryTreeNode2
{
	int m_nValue;
	BinaryTreeNode2 *m_pLeft;
	BinaryTreeNode2 *m_pRight;
	BinaryTreeNode2 *m_pParent;
};

BinaryTreeNode2 *GetNext(BinaryTreeNode2 *pNode)
{
	if (pNode == nullptr)
		return nullptr;
	
	BinaryTreeNode2 *pNext = nullptr;
	if (pNode->m_pRight != nullptr)
	{
		// has right node, next is right node last left node
		BinaryTreeNode2 *pRight = pNode->m_pRight;
		while (pRight->m_pLeft)
		{
			pRight = pRight->m_pLeft;
		}
		pNext = pRight;
	}
	else if (pNode->m_pParent != nullptr)
	{
		/*
		if (pNode->m_pParent->m_pLeft == pNode)
		{
			// no right node, is parent left node, next is parent
			pNext = pNode->m_pParent;
		}
		else
		{
			// no right node, and is parent right node, should find forward parent which grandparent is left node
			while (pNode->m_pParent->m_pParent->m_pRight = pNode->m_pParent)
			{
				pNode = pNode->m_pParent;
			}
			pNext = pNode->m_pParent->m_pParent;
		}
		*/
		BinaryTreeNode2 *pCur = pNode;
		BinaryTreeNode2 *pParent = pNode->m_pParent;
		while (pParent != nullptr && pCur == pParent->m_pRight)
		{
			// if cur is right node, find forward until parent which child is no right
			pCur = pParent;
			pParent = pParent->m_pParent;
		}
		pNext = pParent;
	}
	return pNext;
}

int test8() 
{
	return 0;
}

template <typename T> class CQueue
{
public:
	CQueue(void)
	{
	}
	~CQueue(void)
	{
	}
	void appendTail(const T &node)
	{
		// always push into stack1
		stack1.push(node);
	}
	T deleteHead()
	{
		if (stack2.empty())
		{
			// stack2 is empty, pop stack1 and push into stack2
			while (!stack1.empty())
			{
				T &node = stack1.top();
				stack1.pop();
				stack2.push(node);
			}
		}

		if (stack2.empty())
		{
			throw std::runtime_error("queue empty");
		}

		// pop stack2
		T node = stack2.top();
		stack2.pop();
		return node;
	}
private:
	std::stack<T> stack1;
	std::stack<T> stack2;
};

int test9() 
{
	return 0;
}

long long Fibonacci(unsigned n)
{
	int result[2] = {0, 1};
	if (n < 2)
		return result[n];

	long long front = 0;
	long long back = 1;
	long long ret = 0;
	for (unsigned i=2; i<=n; ++i)
	{
		ret = front + back;
		front = back;
		back = ret;
	}
	return ret;
}

long long FlogJump(unsigned n)
{
	/*
	{
		f(1) = 1,
		f(2) = 2,
		f(n) = f(n-1) + f(n-2)
	}
	*/

	int result[3] = {0, 1, 2};
	if (n < 3)
		return result[n];
	
	long long front = 1;
	long long back = 2;
	long long ret = 0;
	for (unsigned i=3; i<=n; ++i)
	{
		ret = front + back;
		front = back;
		back = ret;
	}
	return ret;

}

long long RectangleCover(unsigned n)
{
	/*
	f(8) = f(7) + f(6)
	{
		f(1) = 1,
		f(2) = 2,
		f(n) = f(n-1) + f(n-2)
	}
	*/

	int result[3] = {0, 1, 2};
	if (n < 3)
		return result[n];
	
	long long front = 1;
	long long back = 2;
	long long ret = 0;
	for (unsigned i=3; i<=n; ++i)
	{
		ret = front + back;
		front = back;
		back = ret;
	}
	return ret;
}

int test10() 
{
	unsigned n = 2;
	printf("n=%d ret=%lld\n", n, Fibonacci(n));

	n = 3;
	printf("n=%d ret=%lld\n", n, Fibonacci(n));

	n = 4;
	printf("n=%d ret=%lld\n", n, Fibonacci(n));

	n = 5;
	printf("n=%d ret=%lld\n", n, Fibonacci(n));

	n = 100;
	printf("n=%d ret=%lld\n", n, Fibonacci(n));

	printf("----------\n");
	n = 3;
	printf("n=%d ret=%lld\n", n, FlogJump(n));
	n = 4;
	printf("n=%d ret=%lld\n", n, FlogJump(n));
	n = 5;
	printf("n=%d ret=%lld\n", n, FlogJump(n));
	return 0;
}

void quick_sort(int *list, int end, int start = 0);
void quick_sort(int *list, int end, int start)
{
	if (!list || end <= 0 || start < 0 || end < start)
	{
		return;
	}

	int i = start;
	int j = start+1;
	if (j >= end)
		return;

	int key = list[start];
	while (j < end)
	{
		if (list[j] >= key)
		{
			++j;
			continue;
		}

		int t = list[++i];
		list[i] = list[j];
		list[j] = t;
		j++;
	}
	int t = list[i];
	list[i] = key;
	list[start] = t;

	quick_sort(list, i, start);
	quick_sort(list, end, i+1);
};

int test11() 
{
	{
	int list[] = {1, 1, 2, 2, 3, 3, 1, 1, 2};
	quick_sort(list, sizeof(list)/sizeof(int));
	print_list(list, sizeof(list)/sizeof(int));
	}
	{
	int list[] = {2, 1, 2, 2, 3, 3, 1, 1, 2};
	quick_sort(list, sizeof(list)/sizeof(int));
	print_list(list, sizeof(list)/sizeof(int));
	}
	{
	int list[] = {2, 3, 3, 2, 3, 3, 1, 1, 2};
	quick_sort(list, sizeof(list)/sizeof(int));
	print_list(list, sizeof(list)/sizeof(int));
	}
	return 0;
}

int MinInSpin(int *numbers, int length)
{
	if (!numbers || length <= 0)
		throw std::runtime_error("input error");
	
	int index1 = 0;
	int index2 = length - 1;
	int indexMid = index1;

	// check head and tail
	while (numbers[index1] >= numbers[index2])
	{
		if (index2 - index1 == 1)
		{
			// only 2 num, tail is min
			indexMid = index2;
			break;
		}

		indexMid = (index2 + index1) / 2;

		if (numbers[index1] == numbers[index2] && numbers[index1] == numbers[indexMid])
		{
			// head, mid, tail same, loop all
			int result = numbers[index1];
			for (int i = index1+1; i <= index2; ++i)
			{
				if (numbers[i] > result)
				{
					result = numbers[i];
				}
			}
			return result;
		}

		if (numbers[indexMid] >= numbers[index1])
		{
			// mid >= head, min in [mid, tail]
			index1 = indexMid;
		}
		else if (numbers[indexMid] <= numbers[index2])
		{
			// mid <= head, min in [head, mid]
			index2 = indexMid;
		}
	}

	return numbers[indexMid];
}

int test12() 
{
	int numbers[] = {3, 4, 7, 1, 2};
	printf("%d\n", MinInSpin(numbers, sizeof(numbers)/sizeof(int)));
	return 0;
}

bool matrixHasPathCore(char *matrix, int rows, int cols, int row, int col, char *str, int &pathLength, bool *visited)
{
	if (str[pathLength] == '\0')
	{
		return true;
	}

	bool hasPath = false;
	if (row >= 0 && row < rows && col >= 0 && col < cols
		&& matrix[row * cols + col] == str[pathLength]
		&& !visited[row * cols + col])
	{
		// is in str
		++pathLength;
		visited[row * cols + col] = true;

		// check if around position is ok
		hasPath = matrixHasPathCore(matrix, rows, cols, row - 1, col, str, pathLength, visited)
				|| matrixHasPathCore(matrix, rows, cols, row + 1, col, str, pathLength, visited)
				|| matrixHasPathCore(matrix, rows, cols, row, col - 1, str, pathLength, visited)
				|| matrixHasPathCore(matrix, rows, cols, row, col + 1, str, pathLength, visited);

		if (!hasPath)
		{
			// wrong path, roll back
			--pathLength;
			visited[row * cols + col] = false;
		}
	}

	return hasPath;
}

bool matrixHasPath(char *matrix, int rows, int cols, char *str)
{
	if (!matrix || rows < 1 || cols < 1|| !str)
		return false;
	
	bool *visited = new bool[rows * cols];
	memset(visited, 0, rows * cols);

	int pathLength = 0;
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			// check every position
			if (matrixHasPathCore(matrix, rows, cols, row, col, str, pathLength, visited))
			{
				return true;
			}
		}
	}

	delete [] visited;
	return false;
}

int getDigitSum(int number)
{
	int sum = 0;
	while (number > 0)
	{
		sum += number % 10;
		number /= 10;
	}
	return sum;
}

int robotMoveCountCore(int threshold, int rows, int cols, int row, int col, bool *visited)
{
	int count = 0;
	bool is_ok = rows >= 0 && cols >= 0 && row < rows && col < cols
	&& getDigitSum(row) + getDigitSum(col) <= threshold && !visited[row * cols + col];

	if (is_ok)
	{
		count = 1 + robotMoveCountCore(threshold, rows, cols, row + 1, col, visited)
		+ robotMoveCountCore(threshold, rows, cols, row - 1, col, visited)
		+ robotMoveCountCore(threshold, rows, cols, row, col + 1, visited)
		+ robotMoveCountCore(threshold, rows, cols, row, col - 1, visited);
	}

	return count;
}

int robotMoveCount(int threshold, int rows, int cols)
{
	if (threshold < 0 || rows <= 0 || cols <= 0)
		return 0;
	
	bool *visited = new bool[rows * cols];
	memset(visited, 0, rows * cols);

	int count = robotMoveCountCore(threshold, rows, cols, 0, 0, visited);
	delete [] visited;

	return count;
}

int test13() 
{
	return 0;
}

int maxProductAfterCutting_solution1(int length)
{
	if (length < 2)
		return 0;
	if (length == 2)
		return 1;
	if (length == 3)
		return 2;
	
	int *products = new int[length + 1];
	products[0] = 0;
	products[1] = 1;
	products[2] = 1;
	products[3] = 2;
	
	// f(n) = max(f(i) * f(n-i))
	int max = 0;
	for (int i = 4; i <= length; ++i)
	{
		max = 0;
		// for (int j = 1; j <= i; ++j)
		for (int j = 1; j <= i/2; ++j)
		{
			int product = products[j] * products[i-j];
			if (product > max)
			{
				max = product;
			}
			products[i] = max;
		}
	}

	max = products[length];
	delete [] products;

	return max;
}

int maxProductAfterCutting_solution2(int length)
{
	if (length < 2)
		return 0;
	if (length == 2)
		return 1;
	if (length == 3)
		return 2;

	int timesOf3 = length / 3;
	if (length - timesOf3 * 3 == 1)
	{
		timesOf3 -= 1;
	}
	int timesOf2 = (length - timesOf3 * 3) / 2;

	return (int)(pow(3, timesOf3)) * (int)(pow(2, timesOf2));
}

int test14() 
{
	{
	int n = 5;
	printf("n=%d ret=%d\n", n, maxProductAfterCutting_solution1(n));
	}
	{
	int n = 6;
	printf("n=%d ret=%d\n", n, maxProductAfterCutting_solution1(n));
	}
	{
	int n = 7;
	printf("n=%d ret=%d\n", n, maxProductAfterCutting_solution1(n));
	}
	{
	int n = 8;
	printf("n=%d ret=%d\n", n, maxProductAfterCutting_solution1(n));
	}
	{
	int n = 9;
	printf("n=%d ret=%d\n", n, maxProductAfterCutting_solution1(n));
	}
	{
	int n = 10;
	printf("n=%d ret=%d\n", n, maxProductAfterCutting_solution1(n));
	}
	{
	int n = 10;
	printf("n=%d ret=%d\n", n, maxProductAfterCutting_solution2(n));
	}
	return 0;
}

int test_notyet() 
{
	// int ret;

	// ret = 0;
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
,	test13
,	test14
};

int main(int argc, char *argv[]) 
{
	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all"))
		{
			printf("run all case\n");
			for (int i=0; i<maxcase; i++)
			{
				printf("run case[%d]\n", i);
				ret = test_list[i]();
				if (ret != 0) 
				{
					printf("case[%d] ret=%d\n", i, ret);
					return 0;
				}
			}
			return 0;
		}
		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) 
		{
			testcase = maxcase - 1;
		}
	}

	printf("run case[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0) 
	{
		printf("case[%d] ret=%d\n", testcase, ret);
	}

	return 0;
}

