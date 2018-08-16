class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int size = nums.size();
        if (size <= 1) return size;

        vector<int> lengthCount(size, 0);
        lengthCount[0] = 1;
        for (int i = 1; i < size; ++i) {
            int maxLength = 1;
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i])
                    maxLength = max(maxLength, lengthCount[j] + 1);
            }
            lengthCount[i] = maxLength;
        }

        int maxLength = 1;
        for (auto length : lengthCount)
            maxLength = max(maxLength, length);

        return maxLength;
    }
};

// solution2
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {//O(nlogn)
       int d=nums.size();
        if(!d) return 0;
       int dp[d],l=0;
       dp[0]=nums[0];
       for(int i=1;i<d;i++)
       {
           int pos=upperbound(dp,nums[i],0,l);
           dp[pos]=nums[i];
           l=max(l,pos);
       }
        return l+1;
    }
    
private:
    int upperbound(int dp[],int x,int left,int right){
        if(x>dp[right]) return right+1;
        while(left<right){
            int mid=(left+right)/2;
            if(x<=dp[mid])
                right=mid;
            else
                left=mid+1;  
        }
        return left;
    }
};