class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> result;
        if (nums.empty() || nums.size() < k) return result;
        auto left = nums.begin();
        auto right = nums.begin() + k;
        while (right <= nums.end()) {
            auto it = max_element(left++, right++);
            result.push_back(*it);
        }
        return result;
    }
};

// solution3
class Solution {
public:
     vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        if (nums.empty() || k > nums.size() || k <= 0)
            return vector<int>();
         
         deque<int> dq; // 队列中存放的下标
         vector<int> res;
         int n = nums.size();
         // 先将前 size 个元素入队：这是第一个窗口
         // 举例：先入队2 3 4， 队列中的元素为：4(2,3小于4，被剔除)
         
         int i = 0;
         for (; i < k; i++) {
             //新入队元素大于队尾，剔除队尾
             while (!dq.empty() && nums[i] >= nums[dq.back()] )
                 dq.pop_back();
             
             dq.push_back(i);
         } // 第一个窗口元素经过入队操作后，在队列中只剩下最大元素 4
         
         res.push_back(nums[dq.front()]);
         
         // 对于下标i=size~n-1的元素，每个元素都是对应窗口的最后一个元素
         // 除第一个窗口，剩下窗口个数为 n-size
         for (int i = k; i < n; i++) {
             // 首先判断当前队头是否处在当前窗口内
             if (i-dq.front() >= k)
                 dq.pop_front();
             while( !dq.empty() && nums[i] >= nums[dq.back()] )
                 dq.pop_back();
             dq.push_back(i);
             
             // 队头为当前窗口最大值
             res.push_back(nums[dq.front()]);
         } //end for
         
         return res;
     }
};