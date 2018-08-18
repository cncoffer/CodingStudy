class Solution {
public:
    int divide(int dividend, int divisor) {
        if (!divisor) return INT_MAX;
        if (!dividend) return 0;
        int flag = 1;
        if ((dividend > 0 && divisor < 0) ||
            (dividend < 0 && divisor > 0))
            flag = -1;
        dividend = abs(dividend);
        divisor = abs(divisor);
        int count = 0;
        while ((dividend -= divisor) >= 0)  {
            ++count;
            if (count < 0) // overflow
                return INT_MAX;
        }
        return flag > 0 ? count : -count;
    }
};

// solution2
class Solution {
public:
    int divide(int dividend, int divisor) {
        long long m = abs((long long)dividend), n = abs((long long)divisor), res = 0;
        if (m < n) return 0;    
        while (m >= n) {
            long long t = n, p = 1;
            while (m > (t << 1)) {
                t <<= 1;
                p <<= 1;
            }
            res += p;
            m -= t;
        }
        if ((dividend < 0) ^ (divisor < 0)) res = -res;
        return res > INT_MAX ? INT_MAX : res;
    }
};