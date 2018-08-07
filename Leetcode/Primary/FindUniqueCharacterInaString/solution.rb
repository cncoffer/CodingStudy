# @param {String} s
# @return {Integer}
def first_uniq_char(s)
    h = Hash.new
    (0...s.size).each do |i|
        if (h[s[i]] == nil)
            h[s[i]] = i
        else
            h[s[i]] = -1
        end
    end

    ret = s.size
    h.each do |pair|
        if pair[1] != -1 && pair[1] < ret
            ret = pair[1]
        end
    end

    ret == s.size ? -1 : ret
end

# @param {String} s
# @return {Integer}
def first_uniq_char2(s)
    hash = {}
    s.each_char.each_with_index do |char, index|
      return index if !hash.key?(char) && s.rindex(char) == index
      hash[char] = index
    end
    -1
  end