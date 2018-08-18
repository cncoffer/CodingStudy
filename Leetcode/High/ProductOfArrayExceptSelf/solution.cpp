class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int total = 1;
        vector<int> result;
        int zoreCount = 0;
        for (int num : nums) {
            if (!num) ++zoreCount;
            else total *= num;
        }
        for (int num : nums) {
            int rel;
            if (zoreCount > 1)
                rel = 0;
            else if (zoreCount == 1) {
                if (num == 0)
                    rel = total;
                else
                    rel = 0;
            }
            else
                rel = total / num;
            result.push_back(rel);
        }
        return result;
    }
};

// solution2
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int size = nums.size();
        vector<int> result(size, 1);
        vector<int> left(size, 1);
        vector<int> right(size, 1);
        for (int i = 1; i < size; ++i) {
            left[i] = left[i-1] * nums[i-1];
        }
        for (int i = size-2; i >= 0; --i) {
            right[i] = right[i+1] * nums[i+1];
        }
        for (int i = 0; i < size; ++i)
            result[i] = left[i] * right[i];
        return result;
    }
};

// solution3
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int> output(nums.size(),1);

        for(int i=1;i<nums.size();i++){
            output[i]=output[i-1]*nums[i-1];
        }
        
        int r=1;
        for(int i=nums.size()-1;i>=0;i--){
            output[i]=output[i]*r;
            r*=nums[i];
        }
        
        return output;
    }
};