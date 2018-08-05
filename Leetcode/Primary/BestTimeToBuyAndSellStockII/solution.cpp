class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() == 0)
            return 0;

        int total = 0;
        int buy_price = prices[0];

        for (auto sell_price : prices)
        {
            if (sell_price > buy_price)
                total += sell_price - buy_price;

            buy_price = sell_price;
        }

        return total;
    }
};