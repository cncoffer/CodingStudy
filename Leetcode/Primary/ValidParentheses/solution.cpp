class Solution {
public:
    bool isValid(string s) {
        if (s.empty()) return true;
        stack<char> stackP;
        stackP.push(s[0]);
        for (int i = 1; i < s.size(); ++i)
        {
            char c = s[i];
            if (c == '(' || c == '[' || c == '{')
                stackP.push(c);
            else
            {
                if (stackP.empty() || !isMatch(stackP.top(),c)
                    return false;
                stackP.pop();
            }
        }
        return stackP.empty();
    }
private:
    bool isMatch(char left, char right)
    {
        return (left == '(' && right == ')')
            || (left == '[' && right == ']')
            || (left == '{' && right == '}');
    }
};