struct Polynom{T<:Number}
    Coefs::Array{T}
    Degree::Int64
    Polynom{T}(degree::Int64) where {T} = new(Vector{T}(undef, degree+1), degree)
    Polynom{T}(coefs::Array{T}) where {T} = new(convert(Array{T}, coefs), length(coefs)-1)
    Polynom{T}(coefs::Vector{T}, degree::Int64) where {T} = new(coefs, degree)
    Polynom{T}(coefs::Vector{T}) where {T} = new(coefs, length(coefs)-1)
end

Base.oneunit(::Type{Polynom{T}}) where T = Polynom{T}([oneunit(T)])
Base.zero(::Type{Polynom{T}}) where T = Polynom{T}([zero(T)])

function get_degree(poly::Polynom{T}) where T
    for i in poly.Degree:-1:0
        if poly.Coefs[i] != Base.zero(T)
            return i
        end
    end
    return 0
end

get_coef(poly::Polynom{T}, n::Int64) where T = poly.Coefs[n+1]
set_coef(poly::Polynom{T},n::Integer,coef::T) where T = (poly.Coefs[n+1] = coef)

function display(poly::Polynom{T}) where T
    ar = poly.Coefs
    res = string(ar[1])*" "
    for i in 1:poly.Degree
        if ar[i+1] > 0
            res = res*"+ "*string(ar[i+1])*"x^"*string(i)*" "
        elseif  ar[i+1] < 0
            res = res*string(ar[i+1])*"x^"*string(i)*" "
        end
    end
    return res
end

function Base.:+(a::Polynom{T},b::Polynom{T})::Polynom{T} where T
    result = Polynom{T}(max(a.Degree,b.Degree))
	for i in 0:max(a.Degree,b.Degree)
        if i > a.Degree
            set_coef(result,i,get_coef(b,i))
        elseif i > b.Degree
            set_coef(result,i,get_coef(a,i))
        else
		    set_coef(result,i,get_coef(a,i) + get_coef(b,i))
        end
	end
	return result
end

function Base.:+(poly::Polynom{T},coef::T)::Polynom{T} where T
	result = Polynom{T}(poly.Degree)
	set_coef(result,1,get_coef(result,1) + coef)
	return result
end

function Base.:-(poly::Polynom{T},coef::T)::Polynom{T} where T
	result = Polynom{T}(poly.Degree)
	set_coef(result,1,get_coef(result,1) - coef)
	return result
end

function Base.:-(a::Polynom{T},b::Polynom{T})::Polynom{T} where T
    result = Polynom{T}(max(a.Degree,b.Degree))
	for i in 0:max(a.Degree,b.Degree)
        if i > a.Degree
            set_coef(result,i,-get_coef(b,i))
        elseif i > b.Degree
            set_coef(result,i,get_coef(a,i))
        else
		    set_coef(result,i,get_coef(a,i) - get_coef(b,i))
        end
	end
	return result
end

function Base.:*(poly::Polynom{T},mul::T)::Polynom{T} where T
	result = Polynom{T}(poly.Degree)
	for i in 0:poly.Degree
		set_coef(result,i,get_coef(poly,i) * mul)
	end
	return result
end

function Base.:*(a::Polynom{T},b::Polynom{T}) where T
    p1 = a.Coefs
    p2 = b.Coefs
    n = a.Degree
    m = b.Degree
    c = zeros(T, m+n+1)
    for i in 0:n, j in 0:m
        c[i+j+1] += p1[i+1] * p2[j+1]
    end
    return Polynom{T}(c, n+m)
end

function Base.:/(poly::Polynom{T},mul::T)::Polynom{T} where T
	result = Polynom{T}(poly.Power)
	for i in 0:poly.Power
		set_coef(result,i,get_coef(poly,i) / mul)
	end
	return result
end

function Base.:-(poly::Polynom{T})::Polynom{T} where T
    result = Polynom{T}(poly.Degree)
	for i in 0:poly.Degree
		set_coef(result,i,-get_coef(poly,i))
	end
	return result
end

function Base.mod(poly::Polynom{T},m::T)::Polynom{T} where T
	result = Polynom{T}(poly.Degree)
	for i in 0:poly.Power
		set_coef(result,i,mod(get_coef(poly,i),m))
	end
	return result
end

function Base.:^(poly::Polynom{T},power::Integer)::Polynom{T} where T
	result = oneunit(Polynom{T})
	for _ in 1:power
		result *= poly
	end
	return result
end

function Base.mod(a::Polynom{T},b::Polynom{T})::Polynom{T} where T
	quotient = zero(Polynom{T})
	n,m = get_degree(a),get_degree(b)
	while n >= m
		quotient += Polynom{T}([zero(T),oneunit(T)]) ^ (n - m) * (get_coef(a,n)/get_coef(b,m))
		a -= b * Polynom{T}([zero(T),oneunit(T)]) ^ (n - m) * (get_coef(a,n)/get_coef(b,m))
		n,m = get_degree(a),get_degree(b)
	end
	return a
end

function Base.div(a::Polynom{T},b::Polynom{T})::Polynom{T} where T
	quotient = zero(Polynom{T})
	n,m = get_power(a),get_power(b)
	while n >= m
		quotient += Polynom{T}([zero(T),oneunit(T)]) ^ (n - m) * (get_coefficient(a,n)/get_coefficient(b,m))
		a -= b * Polynom{T}([zero(T),oneunit(T)]) ^ (n - m) * (get_coefficient(a,n)/get_coefficient(b,m))
		n,m = get_power(a),get_power(b)
	end
	return quotient
end

a, b = Polynom{Int64}([3, -13, 19, -11, 2]), Polynom{Int64}([1, -3, 1])
println(display(a))
println(display(b))
println(display(a+b))
println(display(a-b))
println(display(a*b))
println(display(mod(a, b)))
