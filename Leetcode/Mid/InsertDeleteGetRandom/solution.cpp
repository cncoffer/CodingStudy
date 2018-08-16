class RandomizedSet {
public:
    /** Initialize your data structure here. */
    RandomizedSet() {
        m_set.clear();
    }
    
    /** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
    bool insert(int val) {
        if (m_set.find(val) != m_set.end())
            return false;

        m_set.insert(val);
        return true;
    }
    
    /** Removes a value from the set. Returns true if the set contained the specified element. */
    bool remove(int val) {
        auto it = m_set.find(val);
        if (it == m_set.end())
            return false;
        
        m_set.erase(it);
        return true;
    }
    
    /** Get a random element from the set. */
    int getRandom() {
        uniform_int_distribution<int> di(0, m_set.size()-1);
        int select = dr(dre);
        auto it = m_set.begin();
        for (int i = 0; i < select; ++i) it++;
        return *it;
    }
private:
    default_random_engine dre;
    unordered_set<int> m_set;
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet obj = new RandomizedSet();
 * bool param_1 = obj.insert(val);
 * bool param_2 = obj.remove(val);
 * int param_3 = obj.getRandom();
 */