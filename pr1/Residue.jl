include("gcd.jl")

struct Residue{T, M}
    a :: T
    Residue{T, M}(a) where {T, M} = new(mod(a, M))
end

display(a::Residue) = a.a

function inverse(a::Residue{T, M}) where {T, M}
    if gcdx_(a.a, M)[1] == 1
        return Residue{Int64, 5}(gcdx_(a.a, M)[3])
    else
        return 0
    end
end

Base. +(a::Residue{T, M}, b::Residue{T, M}) where {T, M} = Residue{T, M}(a.a+b.a)
Base. +(a::Residue{T, M}, b::Int64) where {T, M} = Residue{T, M}(a.a+b)

Base. -(a::Residue{T, M}, b::Residue{T, M}) where {T, M} = Residue{T, M}(a.a-b.a)
Base. -(b::Int64, a::Residue{T, M}) where {T, M} = Residue{T, M}(b-a.a)

Base.:-(a::Residue{T, M}) where {T, M} = Residue{T, M}(-(a.a))

Base. *(a::Residue{T, M}, b::Residue{T, M}) where {T, M} = Residue{T, M}(a.a*b.a)
Base. *(a::Residue{T, M}, b::Int64) where {T, M} = Residue{T, M}(a.a*b)

Base. ^(a::Residue{T, M}, k::Int64) where {T, M} = Residue{T, M}(a.a^k)

Base. div(a::Residue{T, M}, b::Residue{T, M}) where {T, M} = Residue{T, M}(div(a.a, b.a))

Base. >(a::Residue{T, M}, b::Int64) where {T, M} = a.a>b
Base. <(a::Residue{T, M}, b::Int64) where {T, M} = a.a<b


a = Residue{Int64, 5}(7)
b = Residue{Int64, 5}(9)

println(a, " ", b)
println(a+b)
println(a-b)
println(-a)
println(a*b)
println(a^2)
println(div(a, Residue{Int64, 5}(1)))
println(inverse(b))