class Solution { // 超时
public:
    void rotate(vector<int>& nums, int k) {
        if (nums.empty())
            return;

        k %= nums.size();
        for (int i = 0; i < k; ++i)
            rotateRight(nums);
    }
private:
    void rotateRight(vector<int>& nums) {
        int lastElement = nums[nums.size()-1];
        for (auto it = nums.end() - 1; it != nums.begin(); --it)
            *it = *(it-1);

        nums[0] = lastElement;
    }
};

class Solution3 {
public:
    void rotate(vector<int>& nums, int k) {
        int size = nums.size();
        if (size == 0 || size == 1)
            return;
        
        k = k % size;
        std::reverse(nums.begin(), nums.begin()+size-k);
        std::reverse(nums.begin()+size-k, nums.end());
        std::reverse(nums.begin(), nums.end());
    }
};

class Solution4 {
public:
    void rotate(vector<int>& nums, int k) {
        int size = nums.size();
        if (size == 0 || size == 1)
            return;
            
        k = k % size;
        for (int i = 0; i < size - k; ++i)
        {
            nums.push_back(nums[0]);
            nums.erase(nums.begin());
        }
    }
};