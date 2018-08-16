class Solution {
public:
	int coinChange(vector<int>& coins, int amount) {
		if (coins.empty()) return -1;
		result.clear();
		for (int i = 0; i < coins.size(); ++i)
			recGenerate(coins, i, amount, 0);
		if (result.empty()) return -1;
		int minCount = INT_MAX;
		for (auto count : result)
			minCount = min(minCount, count);

		return minCount;
	}
private:
	vector<int> result;
	void recGenerate(vector<int>& coins, int useCoin, int amountRem, int count) {
		if (amountRem < 0)
			return;
		if (amountRem == 0) {
			result.push_back(count);
			return;
		}
		if (useCoin == 0) {
			if (amountRem % coins[useCoin] == 0)
				result.push_back(count + amountRem / coins[useCoin]);
			return;
		}
		int max_use = amountRem / coins[useCoin];
		for (int i = 0; i <= max_use; i++) {
			recGenerate(coins, useCoin - 1, amountRem - i*coins[useCoin], count + i);
		}
	}
};

// solution2
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        if (coins.empty()) return -1;
        sort(coins.begin(), coins.end(), 
            [](int a, int b){return a>b;});
        result.clear();
		for (int i = 0; i < coins.size(); ++i)
			recGenerate(coins, i, amount, 0);
		if (result.empty()) return -1;
        return result[0];
    }
private:
    vector<int> result;
	void recGenerate(vector<int>& coins, int useCoin, int amountRem, int count) {
		if (amountRem < 0 || !result.empty())
			return;
		if (amountRem == 0) {
			result.push_back(count);
			return;
		}
		if (useCoin == coins.size()-1) {
			if (amountRem % coins[useCoin] == 0)
				result.push_back(count + amountRem / coins[useCoin]);
			return;
		}
		int max_use = amountRem / coins[useCoin];
		for (int i = max_use; i >= 0; --i) {
			recGenerate(coins, useCoin + 1, amountRem - i*coins[useCoin], count + i);
		}
	}
};

// solution3
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount+1,amount+1);
        dp[0]=0;
        int n=coins.size();
        if(n==0||amount==0) return 0;
        sort(coins.begin(),coins.end());
        for(int i=1;i<=amount;i++){
            for(int j=0;j<coins.size()&&coins[j]<=i;j++){
                dp[i]=min(dp[i],dp[i-coins[j]]+1);
            }
        }
        if(dp[amount]==amount+1) return -1;
        else return dp[amount];
    }
};