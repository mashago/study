
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
}
#include <utility>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

int test0()
{
	return 0;
}

// 16ms, hashmap, 62.34%
vector<int> twoSum(vector<int> &nums, int target)
{
	vector<int> result; // base 0
	unordered_map<int, int> index_map; // base 0
	for (int i = 0; i < (int)nums.size(); i++)
	{
		auto iter = index_map.find(target - nums[i]);
		if (iter != index_map.end())
		{
			result.push_back(iter->second);
			result.push_back(i);
			return result;
		}
		else
		{
			index_map[nums[i]] = i;
		}
	}
	return result;
}

bool compair_pair_asc(pair<int, int> p, pair<int, int> q)
{
	return p.first < q.first;
}

// 16ms, two pointer, 62.34%
vector<int> twoSum2(vector<int> &nums, int target)
{
	vector<int> result; // base 0
	vector<pair<int, int>> pos_vec;
	for (int i = 0; i < (int)nums.size(); i++)
	{
		pos_vec.push_back(make_pair(nums[i], i));
	}

	sort(pos_vec.begin(), pos_vec.end(), compair_pair_asc);

	int i = 0;
	int j = (int)pos_vec.size() - 1;
	while (i < j)
	{
		int sum = pos_vec[i].first + pos_vec[j].first;
		if (sum == target)
		{
			result.push_back(pos_vec[i].second);
			result.push_back(pos_vec[j].second);
			return result;
		}
		else if (sum < target)
		{
			i++;
		}
		else
		{
			j--;
		}
	}
	return result;
}

int test1()
{
	printf("Two Sum\n");
	int target;
	vector<int> nums;
	vector<int> result;

	printf("twoSum()\n");

	nums = {1, 5, 9, 2, 6, 10};
	target = 7;
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		printf(" %d", nums[i]);
	}
	printf("}\n");
	result = twoSum(nums, target);
	printf("target=%d result.size=%lu\n", target, result.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		printf("[%lu]=%d num=%d\n", i, result[i], nums[result[i]]);
	}
	printf("\n");

	nums = {1, 5, 9, 2, 6, 10};
	target = 11;
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		printf(" %d", nums[i]);
	}
	printf("}\n");
	result = twoSum(nums, target);
	printf("target=%d result.size=%lu\n", target, result.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		printf("[%lu]=%d num=%d\n", i, result[i], nums[result[i]]);
	}
	printf("\n");

	printf("twoSum2()\n");

	nums = {1, 5, 9, 2, 6, 10};
	target = 7;
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		printf(" %d", nums[i]);
	}
	printf("}\n");
	result = twoSum2(nums, target);
	printf("target=%d result.size=%lu\n", target, result.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		printf("[%lu]=%d num=%d\n", i, result[i], nums[result[i]]);
	}
	printf("\n");

	nums = {1, 5, 9, 2, 6, 10};
	target = 11;
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		printf(" %d", nums[i]);
	}
	printf("}\n");
	result = twoSum2(nums, target);
	printf("target=%d result.size=%lu\n", target, result.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		printf("[%lu]=%d num=%d\n", i, result[i], nums[result[i]]);
	}
	printf("\n");

	nums = {3, 2, 4};
	target = 6;
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		printf(" %d", nums[i]);
	}
	printf("}\n");
	result = twoSum2(nums, target);
	printf("target=%d result.size=%lu\n", target, result.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		printf("[%lu]=%d num=%d\n", i, result[i], nums[result[i]]);
	}
	printf("\n");

	return 0;
}

struct ListNode
{
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL) {}
};

// 36ms, 69,72%
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
{
	ListNode *result = NULL;
	ListNode *head = NULL;

	while (l1 != NULL || l2 != NULL)
	{
		if (result == NULL)
		{
			result = new ListNode(0);
			head = result;
		}
		else 
		{
			if (head->next == NULL)
			{
				head->next = new ListNode(0);
			}
			head = head->next;
		}

		if (l1 != NULL)
		{
			head->val += l1->val;
			l1 = l1->next;
		}
		if (l2 != NULL)
		{
			head->val += l2->val;
			l2 = l2->next;
		}
		if (head->val >= 10)
		{
			head->val -= 10;
			head->next = new ListNode(1);
		}
	}
	return result;
}

int test2()
{
	printf("Add Two Numbers\n");
	ListNode *result = NULL;
	ListNode *ptr = NULL;

	ListNode *l1 = new ListNode(1);
	ptr = l1;
	ptr->next = new ListNode(5);
	ptr = ptr->next;

	ptr = l1;
	printf("l1=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	ListNode *l2 = new ListNode(2);
	ptr = l2;
	ptr->next = new ListNode(6);
	ptr = ptr->next;
	ptr->next = new ListNode(3);
	ptr = ptr->next;

	ptr = l2;
	printf("l2=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	result = addTwoNumbers(l1, l2);
	printf("result=");
	while (result != NULL)
	{
		printf("[%d]", result->val);
		result = result->next;
	}
	printf("\n");

	return 0;
}



// 52ms, 37.77%
int lengthOfLongestSubstring(string s)
{
	int max_len = 0;
	int head_pos = 0;
	unordered_map<char, int> pos_map;
	for (int i = 0; i < (int)s.size(); i++)
	{
		auto iter = pos_map.find(s[i]);
		if (iter != pos_map.end() && iter->second >= head_pos)
		{
			head_pos = iter->second + 1;
		}
		else
		{
			if (max_len < i - head_pos + 1)
			{
				max_len = i - head_pos + 1;
			}
		}
		pos_map[s[i]] = i;
	}
	return max_len;
}

// 16ms, 61.25%
int lengthOfLongestSubstring2(string s)
{
	int max_len = 0;
	int head_pos = 0;
	int pos_array[255];
	memset(pos_array, -1, sizeof(pos_array));
	for (int i = 0; i < (int)s.size(); i++)
	{
		int c = (int)s[i];
		int pos = pos_array[c];
		if (pos != -1 && pos >= head_pos)
		{
			head_pos = pos + 1;
		}
		else
		{
			if (max_len < i - head_pos + 1)
			{
				max_len = i - head_pos + 1;
			}
		}
		pos_array[c] = i;
	}
	return max_len;
}

int test3()
{
	printf("Longest Substring Without Repeating Characters\n");
	string s;
	int len;

	s = "abcdefg";
	len = lengthOfLongestSubstring(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "abcabc";
	len = lengthOfLongestSubstring(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "abcabcbb";
	len = lengthOfLongestSubstring(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "aab";
	len = lengthOfLongestSubstring(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "tmmzuxt";
	len = lengthOfLongestSubstring(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	printf("\n");

	s = "abcdefg";
	len = lengthOfLongestSubstring2(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "abcabc";
	len = lengthOfLongestSubstring2(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "abcabcbb";
	len = lengthOfLongestSubstring2(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "aab";
	len = lengthOfLongestSubstring2(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	s = "tmmzuxt";
	len = lengthOfLongestSubstring2(s);
	printf("len=%d s=[%s]\n", len, s.c_str());

	return 0;
}


int findNumByPos(vector<int>& nums1, vector<int>& nums2, int k)
{
	int m = nums1.size();
	int n = nums2.size();
	if (m > n)
	{
		return findNumByPos(nums2, nums1, k); // nums1 should be shorter
	}
	if (m == 0)
	{
		return nums2[k-1];
	}
	if (k == 1)
	{
		return min(nums1[0], nums2[0]);
	}

	int ia = min(k/2, m), ib = k - ia;
	if (nums1[ia-1] < nums2[ib-1])
	{
		nums1.erase(nums1.begin(), nums1.begin()+ia);
		return findNumByPos(nums1, nums2, k - ia);
	}
	else if (nums1[ia-1] > nums2[ib-1])
	{
		nums2.erase(nums2.begin(), nums2.begin()+ib);
		return findNumByPos(nums1, nums2, k - ib);
	}
	return nums1[ia-1];
}

// 44ms, 47.99%
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)
{
	vector<int> l1 = nums1;
	vector<int> l2 = nums2;
	int total = l1.size() + l2.size();
	if (total & 0x1)
	{
		return findNumByPos(l1, l2, total/2+1);
	}
	else
	{
		return (findNumByPos(nums1, nums2, total/2) + findNumByPos(l1, l2, total/2+1)) / 2.0;
	}
}


int findNumByPos2(vector<int>& nums1, vector<int>& nums2, int pos1, int pos2, int k)
{
	// real size
	int m = nums1.size() - pos1;
	int n = nums2.size() - pos2;

	if (m > n)
	{
		return findNumByPos2(nums2, nums1, pos2, pos1, k); // nums1 should be shorter
	}
	if (m == 0)
	{
		return nums2[pos2 + k - 1];
	}
	if (k == 1)
	{
		return min(nums1[pos1], nums2[pos2]);
	}

	// relative index
	int ra = min(k/2, m);
	int rb = k - ra;

	// real index
	int ia = ra + pos1;
	int ib = rb + pos2;

	// printf("ra=%d rb=%d ia=%d ib=%d\n", ra, rb, ia, ib);
	// printf("nums1[ia-1]=%d nums2[ib-1]=%d\n", nums1[ia-1], nums2[ib-1]);

	if (nums1[ia-1] < nums2[ib-1])
	{
		return findNumByPos2(nums1, nums2, ia, pos2, k - ra);
	}
	else if (nums1[ia-1] > nums2[ib-1])
	{
		return findNumByPos2(nums1, nums2, pos1, ib, k - rb);
	}
	return nums1[ia-1];
}

// 44ms
double findMedianSortedArrays2(vector<int>& nums1, vector<int>& nums2)
{
	int total = nums1.size() + nums2.size();
	if (total & 0x1)
	{
		return findNumByPos2(nums1, nums2, 0, 0, total/2+1);
	}
	else
	{
		return (findNumByPos2(nums1, nums2, 0, 0, total/2) + findNumByPos2(nums1, nums2, 0, 0, total/2+1)) / 2.0;
	}
}

int test4()
{
	printf("Median of Two Sorted Arrays\n");
	vector<int> nums1;
	vector<int> nums2;
	double ret;

	nums1 = {1, 3, 5, 6, 9};
	nums2 = {2, 4, 6, 8};
	ret = findMedianSortedArrays(nums1, nums2);
	printf("ret = %lf\n", ret);

	nums1 = {1, 3, 5, 6, 9};
	nums2 = {2, 4, 6, 8, 10};
	ret = findMedianSortedArrays(nums1, nums2);
	printf("ret = %lf\n", ret);

	nums1 = {1, 3, 5, 6, 9};
	nums2 = {2, 4, 6, 8};
	ret = findMedianSortedArrays2(nums1, nums2);
	printf("ret = %lf\n", ret);

	nums1 = {1, 3, 5, 6, 9};
	nums2 = {2, 4, 6, 8, 10};
	ret = findMedianSortedArrays2(nums1, nums2);
	printf("ret = %lf\n", ret);


	return 0;
}

// 68ms, 57.22%
string longestPalindrome(string s)
{
	string result = "";
	int total = s.size();
	// odd
	for (int i = 0; i < total; i++)
	{
		int front = i;
		int back = i;
		while (front - 1 >= 0 && back + 1 < total && s[front-1] == s[back+1])
		{
			front -= 1;
			back += 1;
		}
		if (back - front + 1 > (int)result.size())
		{
			result = s.substr(front, back-front+1);
		}
	}

	// even
	for (int i = 0; i < total - 1; i++)
	{
		if (s[i] != s[i+1])
		{
			continue;
		}
		int front = i;
		int back = i+1;
		while (front - 1 >= 0 && back + 1 < total && s[front-1] == s[back+1])
		{
			front -= 1;
			back += 1;
		}
		if (back - front + 1 > (int)result.size())
		{
			result = s.substr(front, back-front+1);
		}
	}

	return result;
}

// 36ms, 69.95%
string longestPalindrome2(string s)
{
	string result = "";
	int total = s.size();

	// odd left
	for (int i = total / 2; i > 0; i--)
	{
		int front = i;
		int back = i;
		while (front - 1 >= 0 && back + 1 < total && s[front-1] == s[back+1])
		{
			front -= 1;
			back += 1;
		}
		if (back - front + 1 > (int)result.size())
		{
			result = s.substr(front, back-front+1);
		}
		if (back + 1 - result.size() < 0)
		{
			break;
		}
	}

	// odd right
	for (int i = total / 2; i < total; i++)
	{
		int front = i;
		int back = i;
		while (front - 1 >= 0 && back + 1 < total && s[front-1] == s[back+1])
		{
			front -= 1;
			back += 1;
		}
		if (back - front + 1 > (int)result.size())
		{
			result = s.substr(front, back-front+1);
		}
		if (front + 1 + (int)result.size() > total)
		{
			break;
		}
	}

	
	// event left
	for (int i = total / 2; i > 0; i--)
	{
		if (s[i-1] != s[i])
		{
			continue;
		}
		int front = i-1;
		int back = i;
		while (front - 1 >= 0 && back + 1 < total && s[front-1] == s[back+1])
		{
			front -= 1;
			back += 1;
		}
		if (back - front + 1 > (int)result.size())
		{
			result = s.substr(front, back-front+1);
		}
		if (back + 1 - result.size() < 0)
		{
			break;
		}
	}

	// event right
	for (int i = total / 2; i < total - 1; i++)
	{
		if (s[i] != s[i+1])
		{
			continue;
		}
		int front = i;
		int back = i+1;
		while (front - 1 >= 0 && back + 1 < total && s[front-1] == s[back+1])
		{
			front -= 1;
			back += 1;
		}
		if (back - front + 1 > (int)result.size())
		{
			result = s.substr(front, back-front+1);
		}
		if (front + 1 + (int)result.size() > total)
		{
			break;
		}
	}

	return result;
}

int test5()
{
	printf("Longest Palindromic Substring\n");

	string input;
	string result;

	input = "a";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "ab";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "aa";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "aaa";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "aba";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "abba";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "acbba";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "tembwxtvddsttolegryohdlhxhycymqybzfzcbkzdwcekzfapmpfyeivfoeeqoqdhylziqpnyzuzeeutpqpalbddlliuehvkhqevgjdkskvphidcjmpcmetzwqkzcnxjcjywhfzplntbkuddmbcovearburjqyirbladcrhfkfdfgsmyhdsfmmxmslwkymkgaguilxghmfgaldcogtfnbqakctqtqakupwrxkmbjpmzqngwldmaugzizgwmediyzxevspxdwruyzrmnhchtxlgtb";
	result = longestPalindrome(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());


	printf("\n");

	input = "a";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "ab";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "aa";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "aaa";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "aba";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "abba";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "acbba";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "tembwxtvddsttolegryohdlhxhycymqybzfzcbkzdwcekzfapmpfyeivfoeeqoqdhylziqpnyzuzeeutpqpalbddlliuehvkhqevgjdkskvphidcjmpcmetzwqkzcnxjcjywhfzplntbkuddmbcovearburjqyirbladcrhfkfdfgsmyhdsfmmxmslwkymkgaguilxghmfgaldcogtfnbqakctqtqakupwrxkmbjpmzqngwldmaugzizgwmediyzxevspxdwruyzrmnhchtxlgtb";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "abb";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	input = "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffgggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg";
	result = longestPalindrome2(input);
	printf("input=[%s] result=[%s]\n", input.c_str(), result.c_str());

	return 0;
}





// 16ms, 76.68%
string convert(string s, int numRows)
{
	if (numRows == 1)
	{
		return s;
	}
	string result = "";
	result.reserve(s.size());
	int total = s.size();
	int loop = total / numRows + (total % numRows == 0 ? 0 : 1);
	int step = 2 * numRows - 2;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < loop; j++)
		{
			int p = j * step + i;
			if (p >= total)
			{
				continue;
			}
			result += s[p];
			if (i == 0 || i == numRows - 1)
			{
				continue;
			}
			p = j * step + step - i;
			if (p >= total)
			{
				continue;
			}
			result += s[p];
		}
	}
	return result;
}


int test6()
{
	printf("ZigZag Conversion\n");
	string s;
	string result;
	int numRows;

	/*
	 * A   E
	 * B D F H
	 * C   G
	 *
	 * r = 3
	 * pos(B) + pos(D) = 2 * r - 2
	 */

	s = "A";
	numRows = 1;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "AB";
	numRows = 1;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "1";
	numRows = 2;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "12";
	numRows = 2;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "121212";
	numRows = 2;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "1212121";
	numRows = 2;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "123212321232";
	numRows = 3;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "12321232123212";
	numRows = 3;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	s = "123432123432123";
	numRows = 4;
	result = convert(s, numRows);
	printf("numRows=%d s=[%s] result=[%s]\n", numRows, s.c_str(), result.c_str());
	printf("\n");

	return 0;
}




// 8ms, 50.08%
int reverse(int x)
{
	int flag = (x < 0 ? -1 : 1);
	long lx = (long)x * flag;

	long result = 0;

	while (lx)
	{
		result = result * 10 + lx % 10;
		lx /= 10;
	}
	if (result > INT_MAX || result * flag < INT_MIN)
	{
		return 0;
	}

	return (int)result * flag;
}

int test7()
{
	printf("Reverse Integer\n");

	int x;
	int ret;

	long y = -2147483648;
	y *= -1;
	printf("y=%ld\n", y);

	x = 0;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = 1;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = -1;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = 100;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = 2001;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = -2001;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = 1234567899;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = INT_MAX;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);

	x = INT_MIN;
	ret = reverse(x);
	printf("x=%d ret=%d\n", x, ret);


	return 0;
}



// 12ms, 15.61%
int myAtoi(string str)
{
	long result = 0;
	int pos = 0;
	int flag = 1;

	// skip space
	for (int i = 0; i < (int)str.size(); i++)
	{
		if (str[i] != ' ')
		{
			break;
		}
		pos++;
	}
	
	// skip flag
	for (int i = pos; i < (int)str.size(); i++)
	{
		if (str[i] == '+' || str[i] == '-')
		{
			flag = (str[i] == '+' ? 1 : -1); 
			pos++;
			break;
		}
	}

	bool has_num = false;
	for (int i = pos; i < (int)str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			if (has_num == false)
			{
				// illegal num
				return 0;
			}
			break;
		}
		has_num = true;
		result *= 10;
		result += str[i] - '0';

		if (result * flag > INT_MAX)
		{
			return INT_MAX;
		}
		if (result * flag < INT_MIN)
		{
			return INT_MIN;
		}
	}

	return (int)result * flag;
}

int test8()
{
	printf("String to Integer\n");

	string str = "";
	int result = 0;

	str = "";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "+";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "-";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "++";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "--";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = " 1";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = " 100";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "100";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "100 ";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "+100";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "-100";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "-2147483648";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "2147483648";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	str = "9223372036854775809";
	result = myAtoi(str);
	printf("str=[%s] result=%d\n", str.c_str(), result);

	return 0;
}



// 72ms, 83.61%
bool isPalindrome(int x)
{
	
	if (x < 0)
	{
		return false;
	}
	if (x == 0)
	{
		return true;
	}

	int t1 = x;
	int t2 = 0;
	while (t1)
	{
		t2 = t2 * 10 + t1 % 10;
		t1 /= 10;
	}

	return t2 == x;
}

int test9()
{
	printf("Palindrome Number\n");

	int x = 0;
	bool result = true;

	x = 100;
	result = isPalindrome(x);
	printf("x=%d result=%d\n", x, result);

	x = -1001;
	result = isPalindrome(x);
	printf("x=%d result=%d\n", x, result);

	x = 101;
	result = isPalindrome(x);
	printf("x=%d result=%d\n", x, result);


	return 0;
}

// 24ms, 43.68%
bool isMatchPtr(const char *s, const char *p)
{
	if (*p == '\0')
	{
		return *s == '\0';
	}

	if (*(p+1) != '*')
	{
		if (*s == *p || (*p == '.' && *s != '\0'))
		{
			return isMatchPtr(s+1, p+1);
		}
		return false;
	}
	else
	{
		while (*s == *p || (*p == '.' && *s != '\0'))
		{
			if (isMatchPtr(s, p+2)) // dynamic programming
			{
				return true;
			}
			s++; 
		}
		return isMatchPtr(s, p+2);
	}

	return false;
}

bool isMatch(string s, string p)
{
	return isMatchPtr(s.c_str(), p.c_str());
}

int test10()
{
	printf("Regular Expression Matching\n");

	string s = "";
	string p = "";
	bool m = false;

	s = "aa";
	p = "a";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);

	s = "aa";
	p = "aa";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);

	s = "aaa";
	p = "aa";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);

	s = "aa";
	p = "a*";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);

	s = "aa";
	p = ".*";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);

	s = "ab";
	p = ".*";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);

	s = "aab";
	p = "c*a*b";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);

	s = "ab";
	p = ".*c";
	m = isMatch(s, p);
	printf("s=[%s] p=[%s] m=%d\n", s.c_str(), p.c_str(), m);



	return 0;
}





int maxArea(vector<int>& height)
{
	int result = 0;
	int l = 0;
	int r = height.size() - 1;
	while (l != r)
	{
		int t = min(height[l],height[r]) * (r - l);
		if (t > result)
		{
			result = t;
		}
		if (height[l] > height[r])
		{
			r --;
		}
		else
		{
			l ++;
		}
	}
	return result;
}

int test11()
{
	printf("Container With Most Water\n");


	vector<int> height;
	int result;

	height.push_back(1);
	height.push_back(3);
	height.push_back(2);
	height.push_back(4);
	height.push_back(3);
	result = maxArea(height);
	printf("result=%d\n", result);

	return 0;
}


// 48ms, 15.18%
string intToRoman(int num)
{
	string roman[4][10] = 
	{ 
		{"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"}, 
		{"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"}, 
		{"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"}, 
		{"", "M", "MM", "MMM"} 
	};

	int pos = 0;
	string result = "";

	while (num)
	{
		result = roman[pos][num % 10] + result;
		pos++;
		num /= 10;
	}

	return result;
}


int test12()
{
	printf("Integer to Roman\n");

	int num = 0;
	string result = "";

	num = 5;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	num = 10;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	num = 15;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	num = 55;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	num = 100;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	num = 150;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	num = 159;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	num = 2345;
	result = intToRoman(num);
	printf("num=%d result=[%s]\n", num, result.c_str());

	return 0;
}



// 68ms, 29.92%
int romanToInt(string s)
{
	unordered_map<char, int> h;
	h['I'] = 1; h['V'] = 5; h['X'] = 10; h['L'] = 50; h['C'] = 100; h['D'] = 500; h['M'] = 1000;

	int result = h[s[0]];
	for (int i = 1; i < (int)s.size(); i++)
	{
		if (h[s[i]] <= h[s[i-1]])
		{
			result += h[s[i]];
		}
		else
		{
			result += h[s[i]] - 2 * h[s[i-1]];
		}
	}
	
	return result;
}

int test13()
{
	printf("Roman to Integer\n");

	int result = 0;
	string s = "";

	s = "V";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "IV";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "VI";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "X";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "XV";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "LV";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "C";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "CL";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "CLIX";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	s = "MMCCCXLV";
	result = romanToInt(s);
	printf("s=[%s] result=%d\n", s.c_str(), result);

	return 0;
}


// 8ms, 7.58%
string longestCommonPrefix(vector<string>& strs)
{
	if (strs.size() == 0)
	{
		return "";
	}
	string result = strs[0];
	for (int i = 1; i < (int)strs.size(); i++)
	{
		int size = min(result.size(), strs[i].size());
		bool flag = true;
		for (int j = 0; j < size; j++)
		{
			if (result[j] != strs[i][j])
			{
				flag = false;
				result = result.substr(0, j);
				break;
			}
		}
		if (flag)
		{
			result = result.substr(0, size);
		}
	}
	return result;
}

// 4ms, 53.44
string longestCommonPrefix2(vector<string>& strs)
{
	if (strs.size() == 0)
	{
		return "";
	}
	string result = "";
	bool flag = true;
	for (int i = 0; i < (int)strs[0].size() && flag; i++)
	{
		int j = 1;
		for (; j < (int)strs.size() && flag; j++)
		{
			if (strs[0][i] != strs[j][i])
			{
				flag = false;
			}
		}
		if (flag)
		{
			result += strs[0][i];
		}
	}
	return result;
}

int test14()
{
	printf("Longest Common Prefix\n");

	string result = "";
	vector<string> strs;

	strs.clear();
	strs.push_back("abc");
	strs.push_back("abdd");
	strs.push_back("ab");
	result = longestCommonPrefix(strs);
	printf("result=[%s]\n", result.c_str());

	strs.clear();
	strs.push_back("aa");
	strs.push_back("a");
	result = longestCommonPrefix(strs);
	printf("result=[%s]\n", result.c_str());


	strs.clear();
	strs.push_back("abc");
	strs.push_back("abdd");
	strs.push_back("ab");
	result = longestCommonPrefix2(strs);
	printf("result=[%s]\n", result.c_str());

	strs.clear();
	strs.push_back("aa");
	strs.push_back("a");
	result = longestCommonPrefix2(strs);
	printf("result=[%s]\n", result.c_str());

	strs.clear();
	strs.push_back("aca");
	strs.push_back("cba");
	result = longestCommonPrefix2(strs);
	printf("result=[%s]\n", result.c_str());

	return 0;
}


// 52ms, 80.31%
vector<vector<int>> threeSum(vector<int>& nums)
{
	vector<vector<int>> result;

	sort(nums.begin(), nums.end());
	for (int i = 0; i < (int)nums.size()-2 && nums[i] <= 0; i++)
	{
		int j = i+1;
		int k = nums.size() - 1;
		while (j < k)
		{
			int sum = nums[i] + nums[j] + nums[k];
			if (sum == 0)
			{
				vector<int> d = {nums[i], nums[j], nums[k]};
				result.push_back(d);
				j++;
				k--;
				while (nums[j] == nums[j-1] && j < k)
				{
					j++;
				}
				while (nums[k] == nums[k+1] && j < k)
				{
					k--;
				}
			}
			else if (sum < 0)
			{
				j++;
			}
			else // sum > 0
			{
				k--;
			}
		}
		while (nums[i] == nums[i+1])
		{
			i++;
		}
	}

	return result;
}

int test15()
{
	printf("3Sum\n");

	vector<int> nums;
	vector<vector<int>> result;

	nums = {-1, -1, 5, 5, -9, -9, 2, 2, -6, -6, 10, 10, -5, -5};
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = threeSum(nums);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");


	nums = {-4,-2,1,-5,-4,-4,4,-2,0,4,0,-2,3,1,-5,0};
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = threeSum(nums);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");

	return 0;
}


// 12ms, 56.71%
int threeSumClosest(vector<int>& nums, int target)
{
	int offset = INT_MAX;
	sort(nums.begin(), nums.end());
	for (int i = 0; i < (int)nums.size()-2; i++)
	{
		int j = i+1;
		int k = nums.size() - 1;
		while (j < k)
		{
			int sum = nums[i] + nums[j] + nums[k];
			if (sum == target)
			{
				return target;
			}
			else if (sum < target)
			{
				if (abs(sum - target) < abs(offset))
				{
					offset = sum - target;
				}
				j++;
				while (nums[j] == nums[j-1] && j < k)
				{
					j++;
				}
			}
			else // sum > target
			{
				if (abs(sum - target) < abs(offset))
				{
					offset = sum - target;
				}
				k--;
				while (nums[k] == nums[k+1] && j < k)
				{
					k--;
				}
			}
		}
		while (nums[i] == nums[i+1])
		{
			i++;
		}
	}

	return target+offset;
}

int test16()
{
	printf("3Sum Closest\n");
	
	int result;
	int target;
	vector<int> nums;

	target = 10;
	nums = {-1, -1, 5, 5, -9, -9, 2, 2, -6, -6, 10, 10, -5, -5};
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = threeSumClosest(nums, target);
	printf("target=%d result=%d\n", target, result);

	target = 0;
	nums = {0,1,2};
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = threeSumClosest(nums, target);
	printf("target=%d result=%d\n", target, result);

	target = 100;
	nums = {1,1,1,0};
	printf("nums={");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = threeSumClosest(nums, target);
	printf("target=%d result=%d\n", target, result);

	return 0;
}


const string num_words[10] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

// 0ms, 17.18%
vector<string> letterCombinations(string digits)
{
	vector<string> result;

	if (digits.size() == 0)
	{
		return result;
	}
	
	string word = num_words[digits[0] - '0'];
	vector<string> last = letterCombinations(digits.substr(1));
	if (last.size() == 0)
	{
		last.push_back("");
	}

	for (int i = 0; i < (int)word.size(); i++)
	{
		for (int j = 0; j < (int)last.size(); j++)
		{
			result.push_back(string(1, word[i]) + last[j]);
		}
	}

	return result;
}

int test17()
{
	printf("Letter Combinations of a Phone Number\n");

	string digits;
	vector<string> result;

	digits = "2";
	result = letterCombinations(digits);
	printf("digits=%s result={", digits.c_str());
	for (int i = 0; i < (int)result.size(); i++)
	{
		if (i == 0)
		{
			printf("[%s]", result[i].c_str());
		}
		else
		{
			printf(", [%s]", result[i].c_str());
		}
	}
	printf("}\n\n");

	digits = "23";
	result = letterCombinations(digits);
	printf("digits=%s result={", digits.c_str());
	for (int i = 0; i < (int)result.size(); i++)
	{
		if (i == 0)
		{
			printf("[%s]", result[i].c_str());
		}
		else
		{
			printf(", [%s]", result[i].c_str());
		}
	}
	printf("}\n\n");


	return 0;
}




void _threeSum(vector<int>& nums, int pos, int target, int first, vector<vector<int>>& result)
{
	for (int i = pos; i < (int)nums.size()-2; i++)
	{
		int j = i+1;
		int k = nums.size() - 1;
		while (j < k)
		{
			int sum = nums[i] + nums[j] + nums[k];
			if (sum == target)
			{
				vector<int> d = {first, nums[i], nums[j], nums[k]};
				result.push_back(d);
				j++;
				k--;
				while (nums[j] == nums[j-1] && j < k)
				{
					j++;
				}
				while (nums[k] == nums[k+1] && j < k)
				{
					k--;
				}
			}
			else if (sum < target)
			{
				j++;
			}
			else // sum > target
			{
				k--;
			}
		}
		while (nums[i] == nums[i+1])
		{
			i++;
		}
	}

}

// 82ms, 54.49%
vector<vector<int>> fourSum(vector<int>& nums, int target)
{
	vector<vector<int>> result;
	sort(nums.begin(), nums.end());
	for (int i = 0; i < (int)nums.size()-3; i++)
	{
		_threeSum(nums, i+1, target - nums[i], nums[i], result);
		while (nums[i] == nums[i+1] && i < (int)nums.size())
		{
			i++;
		}
	}
	return result;
}

// 100ms, 40.50%
vector<vector<int>> fourSum2(vector<int>& nums, int target)
{
	vector<vector<int>> result;
	sort(nums.begin(), nums.end());

	for (int i = 0; i < (int)nums.size()-3; i++)
	{
		for (int p = i+1; p < (int)nums.size()-2; p++)
		{
			int j = p+1;
			int k = nums.size() - 1;
			while (j < k)
			{
				int sum = nums[i] + nums[p] + nums[j] + nums[k];
				if (sum == target)
				{
					vector<int> d = {nums[i], nums[p], nums[j], nums[k]};
					result.push_back(d);
					j++;
					k--;
					while (nums[j] == nums[j-1] && j < k)
					{
						j++;
					}
					while (nums[k] == nums[k+1] && j < k)
					{
						k--;
					}
				}
				else if (sum < target)
				{
					j++;
				}
				else // sum > target
				{
					k--;
				}
			}
			while (nums[p] == nums[p+1])
			{
				p++;
			}
		}
		while (nums[i] == nums[i+1] && i < (int)nums.size())
		{
			i++;
		}
	}
	return result;
}

int test18()
{
	printf("4Sum\n");
	int target;
	vector<int> nums;
	vector<vector<int>> result;

	target = 0;
	nums = {-1, -1, 5, 5, -9, -9, 0, 0, 2, 2, -6, -6, 10, 10, -5, -5};
	printf("target=%d nums={", target);
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = fourSum(nums, target);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");

	target = 0;
	nums = {1,0,-1,0,-2,2};
	printf("target=%d nums={", target);
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = fourSum(nums, target);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");

	target = -11;
	nums = {1,-2,-5,-4,-3,3,3,5};
	printf("target=%d nums={", target);
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = fourSum(nums, target);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");


	printf("fourSum2()\n");

	target = 0;
	nums = {-1, -1, 5, 5, -9, -9, 0, 0, 2, 2, -6, -6, 10, 10, -5, -5};
	printf("target=%d nums={", target);
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = fourSum2(nums, target);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");

	target = 0;
	nums = {1,0,-1,0,-2,2};
	printf("target=%d nums={", target);
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = fourSum2(nums, target);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");

	target = -11;
	nums = {1,-2,-5,-4,-3,3,3,5};
	printf("target=%d nums={", target);
	for (int i = 0; i < (int)nums.size(); i++)
	{
		if (i == 0)
		{
			printf("%d", nums[i]);
		}
		else
		{
			printf(", %d", nums[i]);
		}
	}
	printf("}\n");
	result = fourSum2(nums, target);
	printf("result.size=%lu\n", result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("result[%d]={", i);
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			if (j == 0)
			{
				printf("%d", result[i][j]);
			}
			else
			{
				printf(", %d", result[i][j]);
			}
		}
		printf("}\n");
	}
	printf("\n");

	return 0;
}


// 4ms, 37,53%
ListNode* removeNthFromEnd(ListNode* head, int n)
{
	ListNode *result = head;

	while (n > 1)
	{
		head = head->next;
		n--;
	}

	ListNode *target = result;
	ListNode *last = NULL;
	while (head->next != NULL)
	{
		head = head->next;
		last = target;
		target = target->next;
	}

	if (last == NULL)
	{
		result = result->next;
	}
	else if (target->next == NULL)
	{
		last->next = NULL;
	}
	else
	{
		last->next = last->next->next;
	}

	return result;
}

int test19()
{
	printf("Remove Nth Node From End of List\n");

	int n;
	ListNode *result;
	ListNode *head;
	ListNode *ptr;

	n = 1;
	head = new ListNode(2);
	ptr = head;

	ptr = head;
	printf("head=");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");
	result = removeNthFromEnd(head, n);
	printf("result=");
	while (result != NULL)
	{
		printf("[%d]", result->val);
		result = result->next;
	}
	printf("\n");

	n = 1;
	head = new ListNode(2);
	ptr = head;
	ptr->next = new ListNode(6);
	ptr = ptr->next;
	ptr->next = new ListNode(3);
	ptr = ptr->next;

	ptr = head;
	printf("head=");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");
	result = removeNthFromEnd(head, n);
	printf("result=");
	while (result != NULL)
	{
		printf("[%d]", result->val);
		result = result->next;
	}
	printf("\n");

	n = 2;
	head = new ListNode(2);
	ptr = head;
	ptr->next = new ListNode(6);
	ptr = ptr->next;
	ptr->next = new ListNode(3);
	ptr = ptr->next;

	ptr = head;
	printf("head=");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");
	result = removeNthFromEnd(head, n);
	printf("result=");
	while (result != NULL)
	{
		printf("[%d]", result->val);
		result = result->next;
	}
	printf("\n");

	n = 3;
	head = new ListNode(2);
	ptr = head;
	ptr->next = new ListNode(6);
	ptr = ptr->next;
	ptr->next = new ListNode(3);
	ptr = ptr->next;

	ptr = head;
	printf("head=");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");
	result = removeNthFromEnd(head, n);
	printf("result=");
	while (result != NULL)
	{
		printf("[%d]", result->val);
		result = result->next;
	}
	printf("\n");

	return 0;
}


// 0ms
bool isValid(string s)
{
	stack<char> my_stack;
	for (int i = 0; i < (int)s.size(); i++)
	{
		if (s[i] == '(' || s[i] == '{' || s[i] == '[')
		{
			my_stack.push(s[i]);
		}
		else
		{
			if (my_stack.empty())
			{
				return false;
			}
			char t = my_stack.top();
			if ((t == '(' && s[i] != ')')
			|| (t == '[' && s[i] != ']')
			|| (t == '{' && s[i] != '}'))
			{
				return false;
			}
			my_stack.pop();
		}
	}
	return my_stack.empty();
}

int test20()
{
	printf("Valid Parentheses\n");

	string s;
	bool result;

	s = "()";
	result = isValid(s);
	printf("s=%s result=%d\n", s.c_str(), result);

	s = "[]";
	result = isValid(s);
	printf("s=%s result=%d\n", s.c_str(), result);

	s = "()[]";
	result = isValid(s);
	printf("s=%s result=%d\n", s.c_str(), result);

	s = "([])";
	result = isValid(s);
	printf("s=%s result=%d\n", s.c_str(), result);

	s = "([]{})";
	result = isValid(s);
	printf("s=%s result=%d\n", s.c_str(), result);

	s = "(([]{})";
	result = isValid(s);
	printf("s=%s result=%d\n", s.c_str(), result);



	return 0;
}





// 12ms, 9.37%
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
{
	if (l1 == NULL)
	{
		return l2;
	}

	if (l2 == NULL)
	{
		return l1;
	}

	ListNode *result = NULL;
	ListNode *tail = NULL;
	while (l1 != NULL && l2 != NULL)
	{
		if (l1->val <= l2->val)
		{
			if (result == NULL)
			{
				result = l1;
				tail = result;
			}
			else
			{
				tail->next = l1;
				tail = tail->next;
			}
			l1 = l1->next;
		}
		else if (l1->val > l2->val)
		{
			if (result == NULL)
			{
				result = l2;
				tail = result;
			}
			else
			{
				tail->next = l2;
				tail = tail->next;
			}
			l2 = l2->next;
		}
	}
	if (l1 == NULL && l2 == NULL)
	{
		return result;
	}

	while (l1 != NULL)
	{
		tail->next = l1;
		tail = tail->next;
		l1 = l1->next;
	}

	while (l2 != NULL)
	{
		tail->next = l2;
		tail = tail->next;
		l2 = l2->next;
	}
		
	return result;
}

int test21()
{
	printf("Merge Two Sorted Lists\n");
	ListNode *result = NULL;
	ListNode *ptr = NULL;

	ListNode *l1 = new ListNode(1);
	ptr = l1;
	ptr->next = new ListNode(5);
	ptr = ptr->next;

	ptr = l1;
	printf("l1=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	ListNode *l2 = new ListNode(2);
	ptr = l2;
	ptr->next = new ListNode(3);
	ptr = ptr->next;
	ptr->next = new ListNode(6);
	ptr = ptr->next;

	ptr = l2;
	printf("l2=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	result = mergeTwoLists(l1, l2);
	printf("result=");
	while (result != NULL)
	{
		printf("[%d]", result->val);
		result = result->next;
	}
	printf("\n");

	return 0;
}


void _generateParenthesis(int left, int right, string s, vector<string> &result)
{
	// left always >= right
	if (right == 0)
	{
		result.push_back(s);
		return;
	}

	if (left)
	{
		_generateParenthesis(left-1, right, s+"(", result);
	}

	if (right && right > left)
	{
		_generateParenthesis(left, right-1, s+")", result);
	}

}

// 4ms, 12.82%
vector<string> generateParenthesis(int n)
{
	vector<string> result;
	_generateParenthesis(n, n, "", result);

	return result;
}

int test22()
{
	printf("Generate Parentheses\n");

	int n;
	vector<string> result;

	n = 0;
	result = generateParenthesis(n);
	printf("n=%d result.size()=%lu", n, result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("%s\n", result[i].c_str());
	}
	printf("\n");

	n = 1;
	result = generateParenthesis(n);
	printf("n=%d result.size()=%lu\n", n, result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("%s\n", result[i].c_str());
	}
	printf("\n");

	n = 2;
	result = generateParenthesis(n);
	printf("n=%d result.size()=%lu\n", n, result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("%s\n", result[i].c_str());
	}
	printf("\n");

	n = 3;
	result = generateParenthesis(n);
	printf("n=%d result.size()=%lu\n", n, result.size());
	for (int i = 0; i < (int)result.size(); i++)
	{
		printf("%s\n", result[i].c_str());
	}
	printf("\n");

	return 0;
}


struct cmp
{
	bool operator()(pair<ListNode*, int> a, pair<ListNode*, int> b)
	{
		return a.first->val > b.first->val;
	}
};

// 52ms, 24.81%
ListNode* mergeKLists(vector<ListNode*>& lists)
{
	std::priority_queue<int, vector<pair<ListNode*, int>>, cmp> heap;
	// push every list first node into heap
	for (int i = 0; i < (int)lists.size(); i++)
	{
		if (lists[i] == NULL)
		{
			continue;
		}
		heap.push(make_pair(lists[i], i));
		lists[i] = lists[i]->next;
	}

	ListNode *result = new ListNode(0);
	ListNode *ptr = result;

	while (heap.size())
	{
		pair<ListNode *, int> node_pair = heap.top();
		heap.pop();

		if (node_pair.first->next != NULL)
		{
			heap.push(make_pair(node_pair.first->next, node_pair.second));
			lists[node_pair.second] = node_pair.first->next;
		}

		ptr->next = node_pair.first;
		ptr = ptr->next;
	}

	return result->next;
}

// 32ms, 80.34%
ListNode* mergeKLists2(vector<ListNode*>& lists)
{
	int n = lists.size();
	if (n == 0)
	{
		return NULL;
	}

	while (n > 1)
	{
		int k = (n + 1) / 2;
		for (int i = 0; i < n / 2; i++)
		{
			lists[i] = mergeTwoLists(lists[i], lists[i+k]);
		}
		n = k;
	}

	return lists[0];
}

int test23()
{
	printf("Generate Parentheses\n");

	vector<ListNode *> lists;
	ListNode *ptr;
	ListNode *l1;
	ListNode *l2;
	ListNode *l3;

	printf("mergeKLists\n");

	l1 = new ListNode(1);
	ptr = l1;
	ptr->next = new ListNode(5);
	ptr = ptr->next;
	lists.push_back(l1);

	ptr = l1;
	printf("l1=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	l2 = new ListNode(2);
	ptr = l2;
	ptr->next = new ListNode(3);
	ptr = ptr->next;
	ptr->next = new ListNode(6);
	ptr = ptr->next;
	lists.push_back(l2);

	ptr = l2;
	printf("l2=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	l3 = new ListNode(1);
	ptr = l3;
	ptr->next = new ListNode(4);
	ptr = ptr->next;
	ptr->next = new ListNode(9);
	ptr = ptr->next;
	lists.push_back(l3);

	ptr = l3;
	printf("l3=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	
	ptr = mergeKLists(lists);
	printf("result=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	////// mergeKLists2
	
	printf("mergeKLists2\n");

	lists.clear();

	l1 = new ListNode(1);
	ptr = l1;
	ptr->next = new ListNode(5);
	ptr = ptr->next;
	lists.push_back(l1);

	ptr = l1;
	printf("l1=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	l2 = new ListNode(2);
	ptr = l2;
	ptr->next = new ListNode(3);
	ptr = ptr->next;
	ptr->next = new ListNode(6);
	ptr = ptr->next;
	lists.push_back(l2);

	ptr = l2;
	printf("l2=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	l3 = new ListNode(1);
	ptr = l3;
	ptr->next = new ListNode(4);
	ptr = ptr->next;
	ptr->next = new ListNode(9);
	ptr = ptr->next;
	lists.push_back(l3);

	ptr = l3;
	printf("l3=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	
	ptr = mergeKLists2(lists);
	printf("result=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	return 0;
}



// 4ms, 3.86%
ListNode* swapPairs(ListNode* head)
{
	ListNode *result = NULL;
	ListNode *ptr = NULL;
	ListNode *last_ptr = NULL;

	if (head == NULL)
	{
		return NULL;
	}

	if (head->next == NULL)
	{
		return head;
	}
	result = head->next;

	while (head && head->next)
	{
		if (head->next->next)
		{
			ptr = head->next->next;
		}
		else
		{
			ptr = NULL;
		}
		if (last_ptr != NULL)
		{
			last_ptr->next = head->next;
		}
		head->next->next = head;
		head->next = ptr;
		last_ptr = head;
		head = ptr;
	}
	return result;
}

int test24()
{
	printf("Swap Nodes in Pairs\n");

	ListNode *ptr = NULL;
	ListNode *result = NULL;

	ListNode *l1 = new ListNode(1);
	ptr = l1;
	ptr->next = new ListNode(5);
	ptr = ptr->next;
	ptr->next = new ListNode(7);
	ptr = ptr->next;

	ptr = l1;
	printf("l1=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	result = swapPairs(l1);
	ptr = result;
	printf("result=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	l1 = new ListNode(1);
	ptr = l1;
	ptr->next = new ListNode(2);

	ptr = l1;
	printf("l1=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	result = swapPairs(l1);
	ptr = result;
	printf("result=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	l1 = new ListNode(1);
	ptr = l1;
	ptr->next = new ListNode(2);
	ptr = ptr->next;
	ptr->next = new ListNode(3);
	ptr = ptr->next;
	ptr->next = new ListNode(4);

	ptr = l1;
	printf("l1=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");

	result = swapPairs(l1);
	ptr = result;
	printf("result=    ");
	while (ptr != NULL)
	{
		printf("[%d]", ptr->val);
		ptr = ptr->next;
	}
	printf("\n");


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
,	test15
,	test16
,	test17
,	test18
,	test19
,	test20
,	test21
,	test22
,	test23
,	test24
};

int main(int argc, char **argv)
{
	printf("hello test\n");

	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1)
	{
		if (!strcmp(argv[1], "all"))
		{
			printf("\n---- run all test ----\n");
			for (int i = 0; i < maxcase; i++)
			{
				printf("\n---- run test%d ----\n", i);
				if ((ret = test_list[i]()) != 0)
				{
					printf("**** test%d error %d****\n", i, ret);
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

	printf("\n---- run test%d ----\n", testcase);
	if ((ret = test_list[testcase]()) != 0)
	{
		printf("**** test%d error %d****\n", testcase, ret);
	}

	return 0;
}
