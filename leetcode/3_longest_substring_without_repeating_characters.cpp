/*
Given a string, find the length of the longest substring without repeating characters. For example, the longest substring without repeating letters for "abcabcbb" is "abc", which the length is 3. For "bbbbb" the longest substring is "b", with the length of 1.

Tags: Hash Table, Two Pointers, String
*/

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int max = (s.size()==0 ? 0 : 1);
        
        unordered_map<char, int> m;
        int i=0;
        int j=1;
        while (i<s.size() && j<s.size()) {
            m[s[i]] = i;
            if (s[i] == s[j] || (m[s[j]] > 0 && m[s[j]] > i)) {
                cout << "i=" << i << " j=" << j << endl;
                if (s[i] != s[j]) {
                    m[s[i]] = -1;
                }
                i = m[s[j]] + 1;
                m[s[j]] = j;
                j++;
                continue;
            }
            
            m[s[j]] = j;
            j++;
            if (j-i > max) {
                max = j-i;
            }
        }
        return max;
    }
};

