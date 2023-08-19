function gcd_(a::T, b::T) where T
    # m, n = a, b
    # инвариант: НОД(a, b) = НОД(m, n)
    while b>0
        a, b = b, mod(a, b)
    end
    return a
end

function gcdx_(a::T, b::T) where T
    u, v = 1, 0
    u1, v1 = 0, 1
    # инвариант: НОД(a, b) = НОД(m, n) && a = um+vn && b = u1m + v1n
    while !iszero(b)
        k = div(a, b)
        a, b, u, v, u1, v1 = b, a-k*b, u1, v1, u-k*u1, v-k*v1
    end
    if a < 0
        a, u, v = -a, -u, -v
    end
    return a, b, u, v, u1, v1
end
