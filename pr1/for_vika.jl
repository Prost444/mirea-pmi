# Функция нахождения НОД(a,b)
function gcd(a::T,b::T) where T
    # Инвариант: НОД(a,b)=НОД(m,n)
    while b>0
        a,b = b, mod(a,b) # Вычисляем остаток от деления a на b
    end
    return a # Возвращаем НОД(a,b)
end

# Функция нахождения расширенного НОД(a,b)
function gcdx_(a::T,b::T) where T
    # gcd - greatest common divisor - нод
    u,v = 1,0
    u1,v1 = 0,1 # Вычисляем коэффициенты u и v для расширенного алгоритма Евклида
    while b>0
        k = div(a,b) # Вычисляем целую часть от деления a на b
        a,b = b, a-k*b # Обновляем значения a и b
        u,v = v, u-k*v # Обновляем значения коэффициентов u и v
        u1,v1 = v1, u1-k*v1 # Обновляем значения коэффициентов u1 и v1
    end
    return u,u1,a # Возвращаем коэффициенты u и u1 и НОД(a,b)
end

# Перегрузка функции gcdx_ для работы с целыми числами
function gcdx_(a::Integer, b::Integer)
    u,u1,a = gcdx_(a,b) # Вызываем базовую функцию gcdx_ для получения коэффициентов и НОД
    if a<0 # Если НОД отрицательный, меняем знаки всех значений
        return (-u,-u1,-a)
    else
        return (u,u1,a) # Возвращаем коэффициенты и НОД
    end
end



# Определение структуры кольца вычетов
struct Residue{T,M}
    a::T
    Residue{T,M} where {T,M} = new(mod(a,M)) # Создаем новый объект остатка с модулем M
end

# Функция получения значения остатка
get_value(residue::Residue) = residue.a
# Функция отображения остатка
display(residue::Residue) = display(residue.a)

# Перегрузка операторов для работы с кольцом вычетов
Base. +(a::Residue{T,M}, b::Residue{T,M}) where {T,M} = Residue{T,M}(a.a + b.a) # Сложение двух остатков
Base. -(a::Residue{T,M}, b::Residue{T,M}) where {T,M} = Residue{T,M}

# Поиск обратного элемента
function invmod_(a::Integer{T,M}) where {T,M}
    if gcdx_(a,M)[3]==1                       # проверяем, что НОД(a,M)=1
        if gcdx_(a,M)[1]<0                    # если u < 0, то a^-1 = M + u
            return M + gcdx_(a,M)[1]
        else 
            return gcdx_(a,M)[1]              # иначе a^-1 = u
        end
    else 
        return nothing                        # если НОД(a,M) != 1, обратного элемента не существует
    end
end 
 
a = Residue{Integer,5}(7)
b = Residue{Integer,5}(9)
 
println(a+b)                                 # Residue{Int64,5}(1)
println(a^2)                                 # Residue{Int64,5}(4)
println(display(a))                          # Residue{Int64,5}(2)
println(div(b,a))                            # Residue{Int64,5}(4)
 
 
 
 
# Реализация диофантового уравнения 
function diofant_solve(a::T, b::T, c::T) where T
    a,b = abs(a), abs(b)                      # берем модули a и b
    if (gcdx_(a,b)[3]==1) && (c%gcdx_(a,b)[3]==0)   # проверяем, что НОД(a,b)=1 и c кратно НОД(a,b)
         if gcdx_(a,b)[1]*c*a+gcdx_(a,b)[2]*c*b==c  # если ax+by=c имеет решение, то x=u*c, y=v*c
             return (gcdx_(a,b)[1]*c,gcdx_(a,b)[2]*c)
         else
             return (gcdx_(a,b)[2]*c,gcdx_(a,b)[1]*c)  # иначе x=v*c, y=u*c
         end
     else
         return nothing                             # если НОД(a,b) != 1 или c не кратно НОД(a,b), решения нет
     end
end
 
function gcdx_(a::T,b::T) where T         
    u,v = 1,0
    u1,v1 = 0,1     
    while b>0
        k = div(a,b)          
        a,b = b, a-k*b
        u,v = v, u-k*v
        u1,v1 = v1, u1-k*v1     
    end
    if a<0
        a,u,v = -a,-u,-v             # если a < 0, то меняем знак a и всех коэффициентов
    end
    return(u,u1,a)                   # возвращаем коэффициенты
end
 
 
print(diofant_solve(12,15,1))         # (2,-1)




# Реализация типа Polynom{T}
struct Polynom{T <: Number}
    Coefficient::Array{T}  # массив коэффициентов
    Power::Integer         # степень полинома
    Polynom{T}(power::Integer) where T = new(Vector{T}(undef,power + 1),power)
    Polynom{T}(coef::Array) where T = new(convert(Array{T},coef),length(coef) - 1)
end

# Определение единичного элемента для типа Polynom{T}
Base.oneunit(::Type{Polynom{T}}) where T = Polynom([oneunit(T)])

# Определение нулевого элемента для типа Polynom{T}
Base.zero(::Type{Polynom{T}}) where T = Polynom([zero(T)])

# Функция для получения максимальной степени полинома
function get_power(poly::Polynom{T}) where T 
    for i in poly.Power + 1:-1:0
        if get_coefficient(poly,i) != Base.zero(T)
            return i
        end
    end
    return 0
end

# Функция для получения коэффициента по заданной степени полинома
get_coefficient(poly::Polynom{T},n::Integer) where T = poly.Coefficient[n + 1]

# Функция для установки коэффициента по заданной степени полинома
set_coefficient(poly::Polynom{T},n::Integer,coef::T) where T = ( poly.Coefficient[n + 1] = coef )

# Функция вывода полинома на экран
function Base.display(poly::Polynom{T}) where T
    for i in poly.Power:-1:0
        if get_coefficient(poly,i) == Base.zero(T)
            continue  # пропустить нулевой коэффициент
        end
        print( get_coefficient(poly,i) )
        if i > 1
            print( " * t^")
            print( i )
            print( " + " )
        elseif i > 0
            print( " * t + ")
        end
    end
    println("")
end

# перегрузка оператора умножения для полинома на полином
function Base.:*(a::Polynom{T}, b::Polynom{T})::Polynom{T} where T
    result = Polynom{T}(a.Power + b.Power)
    for i in 0:a.Power
        for j in 0:b.Power
            set_coefficient(result, i+j, Base.zero(T))
        end
    end
    for i in 0:a.Power
        for j in 0:b.Power
            set_coefficient(result, i+j, get_coefficient(result, i+j) + get_coefficient(a, i) * get_coefficient(b, j))
        end
    end
    return result
end

# перегрузка оператора деления для полинома на скаляр
function Base.:/(poly::Polynom{T}, mul::T)::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    for i in 0:poly.Power
        set_coefficient(result, i, get_coefficient(poly, i) / mul)
    end
    return result
end

# перегрузка оператора сложения для полинома и скаляра
function Base.:+(poly::Polynom{T}, coef::T)::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    set_coefficient(result, 1, get_coefficient(result, 1) + coef)
    return result
end

# перегрузка оператора вычитания для полинома и скаляра
function Base.:-(poly::Polynom{T}, coef::T)::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    set_coefficient(result, 1, get_coefficient(result, 1) - coef)
    return result
end

# перегрузка оператора отрицания для полинома
function Base.:-(poly::Polynom{T})::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    for i in 0:poly.Power
        set_coefficient(result, i, -get_coefficient(poly, i))

# Функция умножения полинома на число
function Base.:*(poly::Polynom{T}, mul::T)::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    for i in 0:poly.Power
        set_coefficient(result, i, get_coefficient(poly, i) * mul)
    end
    return result
end

# Функция умножения полиномов
function Base.:*(a::Polynom{T}, b::Polynom{T})::Polynom{T} where T
    result = Polynom{T}(a.Power + b.Power)
    for i in 0:a.Power
        for j in 0:b.Power
            set_coefficient(result, i + j, Base.zero(T))
        end
    end
    for i in 0:a.Power
        for j in 0:b.Power
            set_coefficient(result, i + j, get_coefficient(result, i + j) + get_coefficient(a, i) * get_coefficient(b, j))
        end
    end
    return result
end

# Функция деления полинома на число
function Base.:/(poly::Polynom{T}, mul::T)::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    for i in 0:poly.Power
        set_coefficient(result, i, get_coefficient(poly, i) / mul)
    end
    return result
end

# Функция сложения полинома с числом
function Base.:+(poly::Polynom{T}, coef::T)::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    set_coefficient(result, 1, get_coefficient(result, 1) + coef)
    return result
end

# Функция вычитания числа из полинома
function Base.:-(poly::Polynom{T}, coef::T)::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    set_coefficient(result, 1, get_coefficient(result, 1) - coef)
    return result
end

# Функция изменения знака полинома
function Base.:-(poly::Polynom{T})::Polynom{T} where T
    result = Polynom{T}(poly.Power)
    for i in 0:poly.Power
        set_coefficient(result, i, -get_coefficient(poly, i))
    end
    return result
end

function Base.mod(poly::Polynom{T},m::T)::Polynom{T} where T
	result = Polynom{T}(poly.Power) # создаем многочлен результата с тем же степенями, что и у входного многочлена
	for i in 0:poly.Power
		set_coefficient(result,i,mod(get_coefficient(poly,i),m)) # вычисляем остаток от деления коэффициента i входного многочлена на m и записываем в многочлен результата
	end
	return result
end

function Base.mod(a::Polynom{T},b::Polynom{T})::Polynom{T} where T
	pa,pb = get_power(a),get_power(b) # получаем степени многочленов a и b
	while pb <= pa # пока степень многочлена b не превышает степени многочлена a
		ca,cb = get_coefficient(a,pa),get_coefficient(b,pb) # получаем коэффициенты при старших членах многочленов a и b
 
		a = a - ( b * ( ca / cb ) ) # вычитаем из a b, умноженный на отношение коэффициентов при старших членах
 
		pa,pb = get_power(a),get_power(b) # обновляем степени многочленов
	end
	return a # возвращаем остаток
end

println(display(Polynom{Int64}([1 4 6])+Polynom{Int64}([2 1 1])))