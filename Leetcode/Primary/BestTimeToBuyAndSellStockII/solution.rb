# @param {Integer[]} prices
# @return {Integer}
def max_profit(prices)
    if (prices.size == 0)
        return 0
    end

    total = 0
    index = 1
    while index < prices.size
        total += [0, prices[index] - prices[index-1]].max
        index += 1
    end

    return total
end