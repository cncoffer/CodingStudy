class Solution {
public:
    double myPow(double x, int n) {
        if (x == 0) return 0;
        double ret = 1;
        auto multiply = [](double num, double x) {
                        return num*x;
                    };
        auto divide = [](double num, double x) {
                        return x==0 ? 0 : num/x;
                    };
        auto calc = n > 0 ? multiply : divide;
        for (int i = 0; i < abs(n); ++i) {
            ret = calc(ret, x);
        }
        return ret;
    }
};

class Solution {
public:
    double myPow(double x, int n) {
        if (x == 0 || n == 1) return x;
        if (n == 0) return 1;
        double ret;
        int flag = n;
        n = abs(n);
        if (n % 2 == 0) {
            ret = myPow(x, n/2);
            ret = ret*ret;
        }
        else {
            ret = myPow(x, (n-1)/2);
            ret = ret*ret*x;
        }
        return flag > 0 ? ret : (ret ? 1/ret : 0);
    }
};