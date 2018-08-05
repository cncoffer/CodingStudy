# @param {Integer[]} nums
# @return {Integer}
def single_number(nums)
    result = nums[0]
    (1...nums.size).each do |index|
        result = result ^ nums[index]
    end
    return result
end