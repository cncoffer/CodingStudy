class Solution {
public:
	typedef pair<int, char> Task; // remainder times, task
	int leastInterval(vector<char>& tasks, int n) {
		unordered_map<char, int> taskCount; // task, total times
		priority_queue<Task> taskList;
		deque<Task> waitTask;
		vector<string> debugList;
		for (char c : tasks) ++taskCount[c];
		for (auto it : taskCount)
			taskList.push(make_pair(it.second, it.first));
		int totalTime = 0;
		while (!taskList.empty() || !waitTask.empty()) {
			++totalTime;
			if (!waitTask.empty()) {
				Task cooldownTask = waitTask.front();
				waitTask.pop_front();
				if (cooldownTask.second != '\0') {
					taskList.push(cooldownTask);
				}
			}
			if (!taskList.empty()) {
				Task curTask = taskList.top();
				taskList.pop();
				curTask.first--;
				if (curTask.first) {
					while (waitTask.size() < n) {
						waitTask.push_back(make_pair('\0', 0));
					}
					waitTask.push_back(curTask);
				}
				string str;
				str = curTask.second;
				debugList.push_back(str);
			}
			else
				debugList.push_back("null");
		}
		if (!debugList.empty()) {
			for (string str : debugList)
				cout << str << "->";
			cout << "end" << endl;
		}

		return totalTime;
	}
};

// solution2
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
      vector<int> dp(26,0);
      int ret=0;
      for(auto a: tasks){
          dp[a-'A']++;
      }
     sort(dp.begin(),dp.end(),[](int c,int d){return c>d;});
     int i =0, mx = dp[0], len = tasks.size();
     while (i < 26 && dp[i] == mx) i++;
     return max(len, (mx - 1) * (n + 1) +  i);
    }
};