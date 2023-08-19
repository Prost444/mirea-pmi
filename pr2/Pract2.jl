#1 Функция для быстрого возведения в степень
function power(base, exponent)
    if exponent == 0
        return 1
    elseif iseven(exponent)
        # Если степень четная, то используем рекурсию с делением пополам
        temp = power(base, exponent ÷ 2)
        return temp * temp
    else
        # Если степень нечетная, то уменьшаем степень на 1 и умножаем на base
        return base * power(base, exponent - 1)
    end
end

#2 Функция для нахождения n-го члена последовательности Фибоначчи
function fibonacci(n)
    if n <= 0
        return 0
    elseif n <= 2
        return 1
    end
    result = [1 1; 1 0]  # Изначально результат равен исходной матрице
    return power(result, n)[1, 1]
end

#println(fibonacci(7))

#3 Функция, вычисляющая приближенное значение логарифма.
function my_log(a, x, ε)
    # Проверяем, что основание логарифма больше 0 и не равно 1
    if a <= 0 || a == 1
        throw("Основание логарифма должно быть больше 0 и не равно 1")
    end
    # Проверяем, что x > 0
    if x <= 0
        throw("Аргумент логарифма должен быть больше 0")
    end
    flag = 1
    if a < 1      # Если основание < 1, делаем его > 1 и меняем знак ответа
        a = 1/a
        flag = -1
    end
    z = x
    t = 1
    y = 0
    # ИНВАРИАНТ:  x = z^t * a^y
    while z < 1/a || z > a || t > ε
        if z < 1/a
            z *= a
            y -= t
        elseif z > a
            z /= a
            y += t
        elseif t > ε  #писать это условие выше нельзя, так как возникает вероятность переполнения порядка
            t /= 2
            z *= z
        end
    end
    # Возвращаем искомое приближенное значение логарифма
    return y*flag
end
#println(my_log(0.1, 9, 1e-13))


#4 Функция, реализующая приближенное решение уравнения вида f(x)=0 методом деления отрезка пополам
function bisection_method(f, a, b, epsilon)
    # f - функция, уравнение f(x) = 0
    # a, b - начальные значения отрезка, на котором ищется решение
    # epsilon - требуемая точность
    # Проверка границ отрезка
    if f(a) == 0
        return a 
    end
    if f(b) == 0
        return b
    end
    # Проверка на то, что функция меняет знак на данном отрезке
    if f(a) * f(b) >= 0
        error("Функция не меняет знак на данном отрезке.")
    end
    
    # Итерационный процесс
    while (b - a) > epsilon
        c = (a + b) / 2 # находим середину отрезка
        f_c = f(c) # значение функции в середине отрезка
        if f_c == 0
            return c # нашли точное решение
        elseif f(a) * f_c < 0
            b = c # сужаем отрезок справа
        else
            a = c # сужаем отрезок слева
        end
    end
    return (a + b) / 2 # возвращаем приближенное решение
end

#5 Нахождение приближенного решения уравнения cos(x) = x методом деления отрезка пополам.
function f5(x) cos(x) - x end
#println(bisection_method(f5, 0, 2, 1e-6))

using ForwardDiff

#6 Обобщенная функция реализующая метод Ньютона приближенного решения уравнения вида f(x)=0
function newton_method(r::Function, x0, epsilon=1e-8, num_max::Int64 = 100)
    x = x0
    dx = -r(x)
    k = 0
    while abs(dx) >= epsilon && k <= num_max
        x += dx
        k += 1
        dx = -r(x)
    end
    k > num_max && @warn("Требуемая точность не достигнута")
    return x
end

#println(newton_method(x -> (cos(x) - x)/(-sin(x) - 1), 1, 1e-6, 100))



function horner(poly, x)
    n = length(poly)
    p = poly[n]
    dp = 0.0
    for i in (n-1):-1:1
        dp = dp*x + p
        p = p*x + poly[i]
    end
    return p/dp
end
#println(newton_method(x -> horner([161118720, 12730944, -5992784, 227532, 987, 1], x), -1000, 1e-6, 100))


Base.abs(x::Matrix{Float64}) = maximum(abs.(x))


function f(x)
    [x[1]^3 - x[2]^2 - 1; x[1]^2 - x[2]^2]
end

solve_system(x) = ForwardDiff.jacobian(f, x) \ f(x)

#println(newton_method(solve_system, [1.0, -1.0]))