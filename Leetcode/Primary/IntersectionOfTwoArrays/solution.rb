# @param {Integer[]} nums1
# @param {Integer[]} nums2
# @return {Integer[]}
def intersect(nums1, nums2)
    ret = []
    h = Hash.new
    nums1.each do |num|
        h[num] = (h[num] == nil ? 1 : h[num] + 1)
    end
    nums2.each do |num|
        if (h[num] != nil && h[num] != 0)
            h[num] -= 1
            ret << num
        end
    end
    ret
end