class Solution {
public:
    vector<string> fizzBuzz(int n) {
        const string Fizz = "Fizz";
        const string Buzz = "Buzz";

        vector<string> ret;
        for (int i = 1; i <= n; ++i)
        {
            string str;
            if (i % 3 == 0 && i % 5 == 0)
                str = Fizz + Buzz;
            else if (i % 3 == 0)
                str = Fizz;
            else if (i % 5 == 0)
                str = Buzz;
            else
                str = to_string(i);
            ret.push_back(str);
        }
        return ret;
    }
};