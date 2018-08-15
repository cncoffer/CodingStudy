class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty()) return false;
        if (matrix[0].empty()) return false;
        return recSearch(matrix, target, 0, 0, matrix.size(), matrix[0].size());
    }
private:
    bool recSearchMatrix(vector<vector<int>> &matric, int target, int left, int top, int right, int bottom) {
        if (left > right || top > bottom) 
            return false;
        if (matric[left][top] > target || 
            matric[right][bottom] < target) 
            return false;
        if (left == right || top == bottom) 
            return recBinarySearch(matric, target, left, top, right, bottom);

        int CatercornerRight, CatercornerBottom;
        int CatercornerLength = min(right - left, bottom - top);
        CatercornerRight = left + CatercornerLength;
        CatercornerBottom = top + CatercornerRight;
        // search on catercorner
        for (int i = 1; i < CatercornerLength; ++i) {
            if (matric[left+i][top+i] > target)
                break;
        }
        // search in left bottom and right top rect
        bool ret = recSearchMatrix(matric, target, left, top+i, left+i-1, bottom) || recSearchMatrix(matric, target, left+i, top, right, top+i-1);
        return ret;
    }
    bool recBinarySearch(vector<vector<int>> &matric, int target, int left, int top, int right, int bottom) {
        if (left == right) {
            while (top < bottom) {
                int mid = static_cast<int>(((long)top + (long)bottom) / 2);
                if (matric[left][mid] == target) return true;
                else if (matric[left][mid] > target) bottom = mid-1;
                else top = mid+1;
            }
        }
        else if (top == bottom) {
            while (left < right) {
                int mid = static_cast<int>(((long)left + (long)right) / 2);
                if (matric[mid][top] == target) return true;
                else if (matric[mid][top] > target) right = mid-1;
                else left = mid+1;
            }
        }
        return false;
    }
};

// solution2
class Solution {
public:
    static bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.size()==0||matrix[0].size()==0)
            return false;
        int n=matrix.size();
        int m=matrix[0].size();

        int row=n-1;
        int col=0;
        while(row>=0&&col<=m-1){
            if(target==matrix[row][col]){
                return true;
            }else if(target<matrix[row][col]){
                row--;
            }else if(target>matrix[row][col]){
                col++;
            }
        }
        return false;
    }
};