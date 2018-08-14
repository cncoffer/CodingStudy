class Solution {
public:
    bool isPowerOfThree(int n) {
        while (n > 1)
        {
            if (n % 3 != 0) return false;
            n /= 3;
        }
        return n == 1;
    }
};


// solution2
class Solution {
public:
    bool isPowerOfThree(int n) {
        int l = static_cast<int>(log(n)/log(3));
        return n == 1 || pow(3, l) == n;
    }
};