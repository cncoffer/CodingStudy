class Solution {
public:
    int hammingWeight(uint32_t n) {
        int count = 0;
        while (n)
        {
            if (n % 2)
                ++count;
            n /= 2;
        }
        return count;
    }
};

 // solution2
class Solution {
public:
    int hammingWeight(uint32_t n) {
        n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
        n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
        n = (n & 0x0F0F0F0F) + ((n >> 4) & 0x0F0F0F0F);
        n = (n * (0x01010101) >> 24);
        return n;
    }
};