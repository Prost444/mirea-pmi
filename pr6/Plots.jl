using LinearAlgebra

Vector2D{T <: Real} = NamedTuple{(:x, :y), Tuple{T,T}}

Base. +(a::Vector2D{T},b::Vector2D{T}) where T = Vector2D{T}(Tuple(a) .+ Tuple(b))
Base. -(a::Vector2D{T}, b::Vector2D{T}) where T = Vector2D{T}(Tuple(a) .- Tuple(b))
Base. *(α::T, a::Vector2D{T}) where T = Vector2D{T}(α.*Tuple(a))

# norm(a) - длина вектора, эта функция опредедена в LinearAlgebra
LinearAlgebra.norm(a::Vector2D) = norm(Tuple(a))

# dot(a,b)=|a||b|cos(a,b) - скалярное произведение, эта функция определена в LinearAlgebra
LinearAlgebra.dot(a::Vector2D{T}, b::Vector2D{T}) where T = dot(Tuple(a), Tuple(b))

Base. cos(a::Vector2D{T}, b::Vector2D{T}) where T = dot(a,b)/norm(a)/norm(b)

# xdot(a,b)=|a||b|sin(a,b) - косое произведение
xdot(a::Vector2D{T}, b::Vector2D{T}) where T = a.x*b.y-a.y*b.x

Base.sin(a::Vector2D{T}, b::Vector2D{T}) where T = xdot(a,b)/norm(a)/norm(b)
Base.angle(a::Vector2D{T}, b::Vector2D{T}) where T = atan(sin(a,b),cos(a,b))
Base.sign(a::Vector2D{T}, b::Vector2D{T}) where T = sign(sin(a,b))

Segment2D{T <: Real} = NamedTuple{(:A, :B), NTuple{2,Vector2D{T}}}

function lims!(x1,y1,x2,y2)
	stored_lims[1] = min(x1-1,stored_lims[1])
	stored_lims[2] = min(y1-1,stored_lims[2])
	stored_lims[3] = max(x2+1,stored_lims[3])
	stored_lims[4] = max(y2+1,stored_lims[4])

	xlims!(stored_lims[1], stored_lims[3])
	ylims!(stored_lims[2], stored_lims[4])
end

lims!(x,y) = lims!(x,y,x,y)

function draw(vertices::AbstractArray{Vector2D{T}}) where T
	vertices = copy(vertices)
	push!(vertices,first(vertices))

	x = [v.x for v in vertices]
	y = [v.y for v in vertices]

	plot(x, y, color=:blue, legend=false)

	lims!( minimum(x) , minimum(y) , maximum(x) , maximum(y) )
end

function draw(point::Vector2D{T}) where T
	scatter!([point.x,point.x], [point.y,point.y], color=:red, markersize=5, legend=false)

	lims!( point.x , point.y )
end

function clear()
	fill!(stored_lims,0)

	xlims!(0,1)
	ylims!(0,1)

	plot!()
end

function grekhom(points::AbstractArray{Vector2D{T}})::AbstractArray{Vector2D{T}} where T

    function next!(convex_shell2::AbstractVector{Int64}, points2::AbstractVector{Vector2D{T}}, ort_base::Vector2D{T})::Int64 where T
        cos_max = typemin(T)
        i_base = convex_shell2[end]
        resize!(convex_shell2, length(convex_shell2) + 1)
        for i in eachindex(points2)
            if points2[i] == points2[i_base] # тут не обязательно, что i == i_base
                continue
            end
            ort_i = points2[i] - points2[i_base] # - не нулевой вектор, задающий направление на очередную точку
            cos_i = cos(ort_base, ort_i)
            if cos_i > cos_max
                cos_max = cos_i
                convex_shell2[end] = i
            elseif cos_i == cos_max && dot(ort_i, ort_i) > dot(ort_base, ort_base) # на луче, содержащем сторону выпуклого многоугольника, может оказаться более двух точек заданного множества (надо выбрать самую дальнюю из них)
                convex_shell2[end] = i
            end
        end
        return convex_shell2[end]
    end

    @assert length(points) > 1
    ydata = [points[i].y for i in firstindex(points):lastindex(points)]
    i_start = findmin(ydata)
    convex_shell = [i_start[2]]
    ort_base = (x=oneunit(T), y=zero(T))

    while next!(convex_shell, points, ort_base) != i_start[2]
        ort_base = points[convex_shell[end]] - points[convex_shell[end-1]]
    end

	pop!(convex_shell)

    return points[convex_shell]
end