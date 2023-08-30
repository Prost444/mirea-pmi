#1 Проверка на простоту числа, сложность алгоритма O(sqrt(n)). Оптимизируем шаг, ведь 
# каждое простое число большее 3 может быть представленно в виде 6k+1 или 6k+5
function is_prime(n::Int64)
    # проверка на случаи n < 2, где 2 - это наименьшее простое число
    if n < 2 return false end
    # проверка на простое число 2 и 3
    if n == 2 || n == 3 return true end
    # проверка на кратность 2 или 3
    if n % 2 == 0 || n % 3 == 0 return false end

    i = 5
    while i * i <= n
        if n % i == 0 || n % (i + 2) == 0
            return false
        end
        i += 6
    end
    # если прошли все проверки, то число n простое
    return true
end

#-----------------------------------------------------------------------------------------------------

#2 Решето Эратосфена
function eratosthenes_sieve(n::Int)
    # Создание списка заполненного True
    sieve = trues(n)
    # 0 и 1 не являются простыми числами, поэтому отмечаем их как False
    sieve[1] = false
    # Начинаем проверку с 2, первого простого числа
    for i in 2:isqrt(n)
        # Если i является простым числом, то отмечаем все числа, которые на него делятся как False
        if sieve[i]
            # j = i^2, i^2 + i, i^2 + 2i, i^2 + 3i, ..., n (с шагом i)
            for j in i^2:i:i*(n÷i)
                sieve[j] = false
            end
        end
    end
    # Возвращаем список простых чисел
    return filter(x -> sieve[x], 1:n)
end

# Пример использования
println(eratosthenes_sieve(50))

#-----------------------------------------------------------------------------------------------------

#3 Факторизация
function factorization(n::Int)
    # Для хранения простых множителей
    factors = Int[]
    # Для проверки делимости числа
    divisor = 2
    while n > 1
        # Если число делится на текущий делитель
        if n % divisor == 0
            # Добавляем делитель в массив множителей
            push!(factors, divisor)
            # Делим число на делитель, пока оно делится на него
            while n % divisor == 0
                n = n ÷ divisor
            end
        end
        # Иначе переходим к следующему делителю
        divisor += 1
        # Если проверяемый делитель становится больше, чем корень из исходного числа
        if divisor^2 > n
            # Добавляем оставшееся число как множитель и выходим из цикла
            if n > 1
                push!(factors, n)
            end
            break
        end
    end
    # Возвращаем массив найденных простых множителей
    return factors
end

# Пример использования
println(factorization(1234567890)) # [5, 7, 13, 29]

#-----------------------------------------------------------------------------------------------------

#4 Среднее квадратическое отклонение за один проход (формула Велета)
function std_deviation(data::Vector{Float64})
    n = length(data)
    sum = 0.0
    sq_sum = 0.0
    # Вычисление суммы и суммы квадратов элементов вектора
    for x in data
        sum += x
        sq_sum += x^2
    end
    # Вычисление среднего и среднего квадратического отклонения
    mean = sum / n
    return sqrt(sq_sum / n - mean^2), mean #лучше 2 аргумента
end

# Тестирование функции на случайном векторе данных
println(std_deviation(rand(100)))

#-----------------------------------------------------------------------------------------------------

#5 Взаимные преобразования различных способов представления деревьев
function trace(tree::Vector)
    if isempty(tree)
        return
    end
        
    println(tree[end]) # "обработка" корня
    
    for subtree in tree[1:end-1]
        trace(subtree)
    end
end

function convert!(intree::Vector, outtree::Dict{Int,Vector} = Dict{Int, Vector}())
    #println(outtree)
    if isempty(intree)
        return
    end
    list = []
    for subtree in intree[1:end-1]
        if isempty(subtree)
            push!(list, nothing)
            continue
        end 
        push!(list, subtree[end])
        convert!(subtree,outtree)
    end
    outtree[intree[end]]=list
    return outtree
end

struct Node
    index::Int
    childs::Vector      
end

function convert(intree::Dict{Int,Vector}, root::Union{Int,Nothing})::Union{Node,Nothing}
    if isnothing(root)
        return nothing
    end
    node = Node(root, [])
    for sub_root in intree[root]
        push!(node.childs, convert(intree, sub_root))
    end
    return node
end

function convert2(node::Union{Node, Nothing})::Array{Any}
    if node === nothing
        return []
    elseif isempty(node.childs)
        println("AAAAAAAAAAA")
        return node.index
    else
        childs = []
        for child in node.childs
            push!(childs, convert2(child))
        end
        push!(childs, node.index)
        return childs
    end
end




tree=Dict{Int, Vector}()
intree=[[[[],[],6], [], 2], [[[],[],4], [[],[],5], 3], 1]
println(convert(convert!(intree), 1))

#-----------------------------------------------------------------------------------------------------

#7
function tree_height(tree::Array)
    max_height = 0
    for child in tree[1:end-1]
        max_height = max(max_height, tree_height(child))
    end
    return isempty(tree) ? max_height + 1 : 0 # высота родительского узла на единицу больше, чем высота его наибольшего ребенка
end

function tree_height(tree::Dict, node=1)
    if !haskey(tree, node) # если узла нет в дереве, то он имеет нулевую высоту
        return 0
    end
    max_height = 0
    for child in tree[node]
        max_height = max(max_height, tree_height(tree, child))
    end
    return max_height + 1 # высота родительского узла на единицу больше, чем высота его наибольшего ребенка
end

function tree_size(tree::Dict, node = 1)
    if !haskey(tree, node) # если узла нет в дереве, то он не имеет потомков
        return 0
    end
    size = 1 # учитываем сам узел
    for child in tree[node]
        size += tree_size(tree, child) # рекурсивно считаем размер каждого потомка и суммируем
    end
    return size
end

function tree_size(tree::Array)
    size = Int(!isempty(tree))
    for child in tree[1:end-1]
        size += tree_size(child) # рекурсивно считаем размер каждого потомка и суммируем
    end
    return size
end

function tree_leaves(tree::Array)
    leaves = isempty(tree) ? 0 : Int(all(x -> isempty(x), tree[1:end-1]))
    for child in tree[1:end-1]
        leaves += tree_leaves(child) # рекурсивно считаем число листьев для каждого потомка и суммируем
    end 
    return leaves 
end


function tree_leaves(tree::Dict, node=1)
    if !haskey(tree, node) # если узла нет в дереве, то он не имеет потомков
        return 0
    end
    if isempty(tree[node]) || all(x -> x == nothing, tree[node]) # если у узла нет потомков, или его потомки не
                                                                 # содежат узлов, то он является листом
        return 1
    end
    leaves = 0
    for child in tree[node]
        leaves += tree_leaves(tree, child) # рекурсивно считаем число листьев для каждого потомка и суммируем
    end
    return leaves
end

function tree_max_degree(tree::Dict, node=1)
    if !haskey(tree, node) # если узла нет в дереве, то его валентность равна нулю
        return 0
    end
    max_degree = length(tree[node]) # количество потомков - валентность узла
    for child in tree[node] # рекурсивно считаем валентность каждого потомка
        max_degree = max(max_degree, tree_max_degree(tree, child))
    end
    return max_degree
end

function tree_max_degree(tree::Array)
    max_degree = isempty(tree) ? 0 : length(tree) - 1 # количество потомков - валентность узла
    for child in tree[1:end-1] # рекурсивно считаем валентность каждого потомка
        max_degree = max(max_degree, tree_max_degree(child))
    end
    return max_degree
end

function average_path_length(tree::Dict, node=1, path_length=0)
    if !haskey(tree, node) # если узла нет в дереве, то он не имеет потомков, и путь до него не влияет на среднюю длину пути
        return 0, 0
    end
    total_length = path_length
    total_leaves = 0
    for child in tree[node]
        child_length, child_leaves = average_path_length(tree, child, path_length + 1)
        total_length += child_length
        total_leaves += child_leaves
    end
    if total_leaves == 0 # если текущий узел - это лист, то добавляем его длину к общей сумме длин путей и увеличиваем счетчик листьев на 1
        return total_length, 1
    else # если текущий узел не является листом, то общая сумма длин путей и количество листьев остаются прежними
        return total_length, total_leaves
    end
end

function tree_average_path_length(tree::Dict, node=1, path_length=0)
    total_length, total_leaves = average_path_length(tree, node, path_length)
    return total_length / total_leaves
end

function average_path_length(tree::Array, path_length=0)
    total_length = path_length
    for child in tree[1:end-1]
        child_length = average_path_length(child, path_length + 1)
        total_length += child_length
    end
    return all(x -> isempty(x), tree[1:end-1]) ? 1 : total_length
end

function tree_average_path_length(tree::Array, path_length=0)
    total_length = average_path_length(tree, path_length)
    return total_length / tree_leaves(tree)
end

tree = convert!(intree)
println("Высота дерева равна ", tree_height(tree))
println("Число узлов дерева равно ", tree_size(tree))
println("Число листьев дерева равно ", tree_leaves(tree))
println("Валентность дерева равна ", tree_max_degree(tree))
println("Средняя длина пути дерева равна ", tree_average_path_length(intree))