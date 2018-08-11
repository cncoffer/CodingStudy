class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() <= 1) return 0;

        vector<int> ret(prices.size());
        ret[0] = 0;
        int maxprofit = 0;
        for (int i = 1; i < prices.size(); ++i)
        {
            ret[i] = max(0, ret[i-1] + prices[i] - prices[i-1]);
            maxprofit = max(maxprofit, ret[i]);
        }
        
        return maxprofit;
    }
};

class Solution2 {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() <=1)
            return 0;

        int minBuy = prices[0];
        int maxProfit = 0;
        for (int i = 1; i < prices.size(); ++i)
        {
            minBuy = min(minBuy, prices[i]);
            maxProfit = max(maxProfit, prices[i] - minBuy);
        }

        return maxProfit;
    }
};