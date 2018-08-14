class Solution {
public:
    int countPrimes(int n) {
        int count = 0;
        for (int i = 2; i < n; ++i)
            if (isPrime(i))
                ++count;

        return count;
    }
private:
    bool isPrime(int n) {
        for (int i = 2; i < n / 2 + 1; ++i)
        {
            if (n % i == 0)
                return false;
        }
        return true;
    }
};

// solution2
class Solution {
public:
    int countPrimes(int n) {
        vector<bool> isPrime(n);
        fill(isPrime.begin(), isPrime.end(), true);

        for (int i = 2; i < n / 2 + 1; ++i) {
            for (int j = 2; i * j < n; ++j) {
                isPrime[i*j] = false;
            }
        }

        int count = 0;
        for (int i = 2; i < n; ++i)
            if (isPrime[i]==true)
                count++;

        return count;
    }
};

// solution3
class Solution {
public:
    int countPrimes(int n) {
        vector<bool> isPrime(n);
        fill(isPrime.begin(), isPrime.end(), true);

        for (int i = 2; i < n / 2 + 1; ++i) {
            if (isPrime[i])
                for (int j = 2; i * j < n; ++j) {
                    isPrime[i*j] = false;
                }
        }

        int count = 0;
        for (int i = 2; i < n; ++i)
            if (isPrime[i]==true)
                count++;

        return count;
    }
};