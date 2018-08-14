class MinStack {
public:
	/** initialize your data structure here. */
	MinStack() {
		m_top = -1;
	}

	void push(int x) {
		m_min = m_top == -1 ? x : min(m_min, x);
		++m_top;
		if (m_top < m_stack.size())
			m_stack[m_top] = x;
		else
			m_stack.push_back(x);
	}

	void pop() {
		--m_top;
		if (m_top != -1)
		{
			auto it = min_element(m_stack.begin(), m_stack.begin() + m_top + 1);
			m_min = *it;
		}
	}

	int top() {
		return m_top == -1 ? 0 : m_stack[m_top];
	}

	int getMin() {
		return m_top == -1 ? 0 : m_min;
	}
private:
	vector<int> m_stack;
	int m_top;
	int m_min;
};

// solution2
class MinStack {
public:
    /** initialize your data structure here. */
    MinStack() {
        
    }
    
    void push(int x) {
        stElement.push(x);
        int minNum = x;
        if (!stMin.empty())
            minNum = min(x, stMin.top());
        stMin.push(minNum);
    }
    
    void pop() {
        stElement.pop();
        stMin.pop();
    }
    
    int top() {
        return stElement.top();
    }
    
    int getMin() {
        return stMin.top();
    }
private:
    stack<int> stElement;
    stack<int> stMin;
};