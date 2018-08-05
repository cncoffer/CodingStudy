# @param {Integer[]} nums
# @return {Integer}
def remove_duplicates(nums)
    if nums.size == 0
        return 0
    end
    fast_index = 1;
    slow_index = 0;
    while fast_index < nums.size
        if (nums[fast_index] != nums[slow_index])
            slow_index += 1
            nums[slow_index] = nums[fast_index]
        end
        fast_index += 1
    end

    return slow_index + 1
end