/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
class Solution {
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        if (intervals.size() <= 1) return intervals;
        vector<Interval> result;
        sort(intervals.begin(), intervals.end(), 
            [](Interval a, Interval b){return a.start<b.start;});
        result.push_back(intervals[0]);
        for (int i = 1; i < intervals.size(); ++i) {
            Interval last = result.back();
            result.pop_back();
            if (last.end >= intervals[i].start) {
                Interval tmp(last.start, max(last.end, intervals[i].end));
                result.push_back(tmp);
            }
            else {
                result.push_back(last);
                result.push_back(intervals[i]);
            }
        }
        return result;
    }
};