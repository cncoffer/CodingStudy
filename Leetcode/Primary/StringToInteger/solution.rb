# @param {String} str
# @return {Integer}
def my_atoi(str)
    [[str.strip.to_i, 2147483647].min, -2147483648].max
end