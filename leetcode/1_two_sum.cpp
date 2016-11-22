/*
Given an array of integers, find two numbers such that they add up to a specific target number.

The function twoSum should return indices of the two numbers such that they add up to the target, where index1 must be less than index2. Please note that your returned answers (both index1 and index2) are not zero-based.

You may assume that each input would have exactly one solution.

Input: numbers={2, 7, 11, 15}, target=9
Output: index1=1, index2=2

Tags: Array, Hash Table
*/

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> m;
        vector<int> r;
        for (int i=0; i<nums.size(); i++) {
            int num = nums[i];
            
            if (m[num] != 0) {
                int index1, index2;
                
                if (m[num] > i+1) {
                    index1 = i+1;
                    index2 = m[num];
                } else {
                    index1 = m[num];
                    index2 = i+1;
                }
                r.push_back(index1);
                r.push_back(index2);
                return r;
            }
            
            m[target-num] = i+1;
        }
        return r;
    }
};
