
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <stdexcept>
#include <exception>
#include <string>

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
			throw std::runtime_error("queue is empty");

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

