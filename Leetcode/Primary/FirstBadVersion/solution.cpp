// Forward declaration of isBadVersion API.
bool isBadVersion(int version);

class Solution {
public:
    int firstBadVersion(int n) {
        return getFirstBadVer(0, n);

    }
private:
    int getFirstBadVer(int left, int right) {
        if (left == right)
            return left;

        int mid = static_cast<int>(left / 2.0 + right / 2.0);
        if (isBadVersion(mid))
            return getFirstBadVer(left, mid);
        else
            return getFirstBadVer(mid+1, right);
    }
};

class Solution2 {
public:
    int firstBadVersion(int n) {
        int left, right, mid;
        left = 0;
        right = n;
        while (left < right)
        {
            mid = static_cast<int>(left / 2.0 + right / 2.0);
            if (isBadVersion(mid))
                right = mid;
            else
                left = mid + 1;
        }

        return right;
    }
};