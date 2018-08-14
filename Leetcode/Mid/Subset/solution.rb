# @param {Integer[]} nums
# @return {Integer[][]}
def recGenerate(nums:, rem:, ret:, left:0, tmpret:[])
    if rem <= 0
        ret << tmpret.clone
        return
    elsif left >= nums.size || rem > nums.size
        return
    elsif left + rem == nums.size
        tmp = tmpret.clone
        (left...nums.size).each {|n| tmp << nums[n]}
        ret << tmp
        return
    end
    (left...nums.size).each do |n|
        tmpret << nums[n]
        recGenerate(nums:nums, rem:(rem-1), ret:ret, left:(n+1), tmpret:tmpret)
        tmpret.pop
    end
end

def subsets(nums)
    ret = []
    (0...nums.size+1).each do |n|
        recGenerate(nums:nums, rem:n, ret:ret)
    end
    ret
end

nums = [1,2,3]
p nums
ret = subsets(nums)
p ret