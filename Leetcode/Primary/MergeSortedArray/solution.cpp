class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        // move nums1
        for (int i = m-1; i >= 0; --i)
            nums1[i+n] = nums1[i];

        // merge
        int i, j, index;
        i = n;
        index = j = 0;
        while (j < n && i < m + n)
        {
            nums1[index++] = nums1[i] < nums2[j] ? nums1[i++] : nums2[j++];
        }

        // copy last number
        while (i < m + n)
            nums1[index++] = nums1[i++];
        
        while (j < n)
            nums1[index++] = nums2[j++];
    }
};