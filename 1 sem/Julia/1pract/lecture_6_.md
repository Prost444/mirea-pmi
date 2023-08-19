# ЛЕКЦИЯ 6

## Иерархия типов
- абстрактные типы
- конкретные типы

Наследование методов, производные типы данных
В Julia наследование от конкретных типов невозможно, т.е. невозможно наследовать данные, можно наследовать только методы.

## Структуры - это пользовательские конкретные типы данных

Пользовательские типы данных, определяемые как структуры, являются **конкретными типами данных** (в противовес абстрактным типам данных, которые также могут определяться программистом, но которым не соответствует никакая структура, см. об этом ниже).

```julia
julia>  struct Student
            first_name::String
            second_name::String
            index::Int
        end

# Объекты (структуры) создаются с помощью специальных функций - констркутров типов. 
# Имя конструктора всегда соврадает с именем типа.
# Если используется конструктор по умолчанию, то у него должно быть столько же аргументов, стколько имеется полей (атрибутов) в структуре.

julia> stud = Student("Сергей", "Иванов", 1001) 

julia> stud.first_name |> println
Сергей

julia> stud.second_name |> println
Иванов

julia> stud.second_name = "Ivanov"
`ERROR: setfield!: immutable struct of type Student cannot be changed
```

Ошибка произошла потому, что по умолчанию структуры не изменяемые.

Следующая структура создается как изменяемая с использованием ключевого слова mutable.

```julia
using HorizonSideRobots

mutable struct Coordinates
    x::Int
    y::Int
end
Coordinates() = Coordinates(0,0)

function HorizonSideRobots.move!(coord::Coordinates, side::HorizonSide)
    if side==Nord
        coord.y += 1
    elseif side==Sud
        coord.y -= 1
    elseif side==Ost
        coord.x += 1
    else #if side==West
        coord.x -= 1
    end
end

get_coord(coord::Coordinates) = (coord.x, coord.y)
```

**Замечание.** В определении HorizonSideRobots.move!(coord::Coordinates, side::HorizonSide) используется квалифицированное имя с указанием имени пакета, из которого ранее была импортирована функция move!(::Robot, ::HorizonSide). Это требуется для того, чтобы новое определение не конфликтовало со старым (из пакета).

Сам по себе тип, Coordinates, не сявязан ни с каким роботом, и определенный здесь для этого типа метод функции move!, разумеется, никакого робота никуда не перемещает, а только лишь соответствующим образом изменяет значение координат.

Однако тип Coordinates будет полезен для проектирования новых типов, уже непосредственно связанных с роботом.

## Композиция типов

Следующая структура, представляющая новый тип данных CoordRobot, представляет собой композицию конкретного типа Robot и конкретного типа Coordinates.

```julia
using HorizonSideRobots

struct CoordRobot
    robot::Robot
    coord::Coordinates
end

CoordRobot(robot) = CoordRobot(robot, Coordinates()) 
```

Теперь остается только определить функции для типа CoordRobot, что бы обеспечить нужное поведение объектов типа CoordRobot.

```julia
function HorizonSideRobots.move!(robot::CoordRobot, side)
    move!(robot.robot, side)
    move!(robot.coord, side)
end
HorizonSideRobots.isborder(robot::CoordRobot, side) = isborder(robot.robot, side)
HorizonSideRobots.putmarker!(robot::CoordRobot) = putmarker!(robot.robot)
HorizonSideRobots.ismarker(robot::CoordRobot) = ismarker(robot.robot)
HorizonSideRobots.temperature(robot::CoordRobot) = temperature(robot.robot)

get_coord(robot::CoordRobot) = get_coord(robot.coord)
```

## Наследование

В рассмотренном примере композиции типов на база типов Robot и Coordinates был получен новый тип CoordRobot, который соединяет в себе черты обоих типов. Причем эти черты присутствуют как на уровне данных (тип CoordRobot содержит данные обоих типов), так и на уровне поведения объекта нового типа: объект типа CoordRobot умеет выполнять все теже действия, что и Robot, плюс, при перемещнении с помощью функции move! происходит отслеживание текущих координат робота.

В каком-то смысле можно говорить, что CoordRobot наследует от типв Robot и Cооrdinates их свойства (черты).
Однако наследование поведения мы добились за счет переопределения функций:

```julia
function HorizonSideRobots.move!(robot::CoordRobot, side)
    move!(robot.robot, side)
    move!(robot.coord, side)
end
HorizonSideRobots.isborder(robot::CoordRobot, side) = isborder(robot.robot, side)
HorizonSideRobots.putmarker!(robot::CoordRobot) = putmarker!(robot.robot)
HorizonSideRobots.ismarker(robot::CoordRobot) = ismarker(robot.robot)
HorizonSideRobots.temperature(robot::CoordRobot) = temperature(robot.robot)

get_coord(robot::CoordRobot) = get_coord(robot.coord)
```

Хотя такого рода переопределения функций и весьма короткие, но если их много, то это может быть проблемой.

Чтобы решить данную проблему в объектно-ориентированных языках программирования предусмотрен механизм наследования, при котором при определении нового типа достаточно только указать, от каких родительских типов он наследует, и тогда переопределения, подобные приведенным выше, не потребуются.

Однако в языке Julia такое наследование допускается только от абстрактных типов. Поскольку типы Robot и Coordinates являются конкретными, то наследовать от них не получится.

Тем не менее, можно было бы поступить следующим образом.

Сначала спроектируем абстрактный тип SimpleRobot, представляющий собой простую обертку типа Robot:

```julia
using HorizonSideRobots

abstract type SimpleRobot end

HorizonSideRobots.move!(robot::SimpleRobot, side) = move!(get_robot(robot), side)
HorizonSideRobots.isborder(robot::CoordRobot, side) = isborder(get_robot(robot), side)
HorizonSideRobots.putmarker!(robot::CoordRobot) = putmarker!(get_robot(robot))
HorizonSideRobots.ismarker(robot::CoordRobot) = ismarker(get_robot(robot))
HorizonSideRobots.temperature(robot::CoordRobot) = temperature(get_robot(robot))
```

Тут необходимо обратить внимание на использование функции get_robot(robot). Пока что эта функция не определена, но предполагается, что она возвращает ссылку на объект типа Robot. Но эта функция должна быть определена для каждого конкретного типа, производного от абстрактного типа SimpleRobot.

Например, тип CoordRobot теперь мог бы быть определен как производный тип от абстрактного типа SimpleRobot:

```julia
struct CoordRobot <: SimpleRobot # - это означает, что CoordRobot - наследует от SimpleRobot
    robot::Robot
    coord::Coordinates
end

CoordRobot(robot) = CoordRobot(robot, Coordinates()) 
```

Теперь остается определить функции для типа CoordRobot.

```julia
function HorizonSideRobots.move!(robot::CoordRobot, side)
    move!(robot.robot, side)
    move!(robot.coord, side)
end
get_coord(robot::CoordRobot) = get_coord(robot.coord)

get_robot(robot::CoordRobot) = robot.robot
```

**Замечание 1.** Может показаться, что введение абстрактного типа SimpleRobot ничего особенно не дало в плане уменьшения объема кода. В самом деле, те же самые определения функций, котрые прежде были сделаны непосредственно для типа CoordRobot, теперь сделаны для абстрактно типа SimpleRobot, после чего они уже наследуются типом CoordRobot.

В принципе это так, но зато теперь от SimpleRobot могут наследовать и какие-то другие типы, и при этом указаннные функции для них переопределять не потребуется.

**Замечание 2.** В языке Julia, в отличие от таких языков как Python или C++, множественное наследование не поддерживается. Т.е. у любого типа может быть не более одного родительского типа. Если же требуется наследовать от нескольких типов, то остается только воспользоваться композицией, так как это было в рассмотренном выше примере.

## Пример использования типа CoordRobot

ДАНО: робот - в юго-западном углу прямоугольного поля (без внутренних перегородок), на котором рассталено некоторое количество маркеров.

РЕЗУЛЬТАТ: получен массив (главная функция возвращает массив) с декартовыми координатами клеток с маркерами относительно юго-западного угла (координаты юго-западного угла принимаются за нулевые), и робот - в исходном положении.

**Решение.**

```julia
function get_coord_markers!(robot)
    coord_robot = CoordRobot(robot)
    side = Ost
    coord_markers = get_coord_markers!(coord_robot, side)
    while !isborder(robot, Nord)
        move!(robot, Nord)
        side = inverse(side)
        append!(coord_markers, get_coord_markers!(coord_robot, side))
    end
    return coord_markers
end

function get_coord_markers!(coord_robot, side)
    coord_markers = Vector{NTuple{2,Int}}[] # - пустой вектор типа Vector{NTuple{2,Int}}
    if ismarker(coord_robot)
        push!(coord_markers, get_coord(coord_robot))
    end
    while !isborder(robot, side)
        move!(robot, side)
        push!(coord_markers, get_coord(coord_robot))
    end
    return coord_markers
end
```

## Параметрические типы. Обобщенное программирование

Ранее в практике 5 была написана функция snake!, позволяющая перемещать робота по полю, до момента выпонения некотрого заданноно условия, или до того момента, когда все клетки поля будут пройдены.

```julia
"""
snake!(stop_condition::Function, robot, (move_side, next_row_side)::NTuple{2,HorizonSide} = (Nord, Ost))

-- выполняет движение зиейкой до выполнения условия останова stop_condition(cerrent_side), или не пока пройденны все ряды в направлении next_row_side (current_side - текущее направление робота при преремещениях вдоль очередного ряда)

stop_condition - функция, возвращающая логическое значение, и определяющяя условие останова при движении змейкой, c одним аргументом типа HorizonSide - имеется ввиду, что этот агрумет всегда имеет значение текущего направления перемещения 

"""
function snake!(stop_condition::Function, robot, (move_side, next_row_side)::NTuple{2,HorizonSide} = (Nord, Ost)) # - это обобщенная функция
    # Робот - в (inverse(next_row_side), inverse(move_side))-углу поля
    along!(stop_condition(move_side), robot, move_side)
    while !stop_condition(move_side) && try_move!(robot, next_row_side)
        move_side = inverse(move_side)
        along!(stop_condition(move_side), robot, move_side)
    end
end


"""
snake!(robot, (move_side, next_row_side)::NTuple{2,HorizonSide} = (Nord, Ost))

-- перемещает робта "змейкой" пока не будут пройденны все ряды в "поперечном" направлении next_row_side
"""
snake!(robot, (move_side, next_row_side)::NTuple{2,HorizonSide}=(Ost, Nord)) = snake!(side -> false, robot, (next_row_side, move_side))
```

С помощью этой функции можно было бы, например, найти маркер на поле, или - внутреннюю перегородку.
Для этого достаточно было просто передать в эту функцию высшего порядка через соответствуюший функциональный параметр требуемое условие останова.

### Пример 1

Пусть, например, робот находится в юго-западном углу поля и его требуется переместить в клетку с маркером. Тогда с помощью функции snake! решение выглядело бы так:

```julia
# robot - ссылка на объект типа Robot
snake!(_->ismarker(robot), robot) 
```

### Пример 2

Или, пусть при техже исходных условиях робота требуется переместить в клетку, с севера граничащую с имеющейся на поле внутренней прямоугольной перегородкой.

Тогда использование функции snake! для этой цели выглядело бы так:

```julia
snake!(_->isborder(robot, Nord), robot) 
```

**Замечание.** Здесь в записях условия останова на местеагрумента использован символ нижнего подчергивания, потому что в обоих случаях соответствующее условие не зависит от текущего направления перемещения (но тем не менее формально это условие требуется определить как функцию с аргументом).

**Важный вопрос** состоит в том, а можно ли использовать функцию snake! ещё и для решения каких-либо других задач, на сводящихся лишь к остановке робота в нужном месте. Оказывается, что - можно.

### Пример 3

Пусть требуется замаркировать все клетки поля, начиная с юго-западного угла.

Чтобы это можно было сделать с помощью функции snake! нужно, чтобы перед началом движени змейкой после каждого элементарного перемещения происходила постановка маркера. Для этого определим специальный тип робота, который и будет выполнять эти требуемые дейчтвия. 

Остаётся только решить вопрос, какое перемещение робота считать элементарным, то, которое - по команде move!, или то, которе по команде try_move!. Если на поле не предполагается наличие внутренних перегородок, то в равной степени подойдут оба варианта. Но если при перемещениях "змейкой" придется ещё обходить внутренние перегородки (см. ниже), то подойдет только второй вариант, т.е. функция try_move!. Функция же move! в дальнейшем будет использоваться также и для обходов внутренних перегородок.

```julia
using HorizonSideRobots

struct PutmarkerRobot <: SimpleRobot
    robot::Robot
end 

function try_move!(robot::PutmarkerRobot, side)
    result = try_move!(robot, side)
    putmarker(robot)
    return result
end
```

Новый тип данных  PutmarkerRobot здесь понадобился для того, чтобы можно было для этого типа дать определение нового, требуемого для решения задачи, метода функции try_move!.

После всех этих сделанных определений для решения задачи постановки маркеров во все клетки поля остаётся только сделать следующее.

```julia
# robot - ссылка на объект типа Robot

putmarker!(robot)
snake!(PutmarkerRobot(robot))
```

**Замечание.** Тут следует обрать внимание на то, что функция snake! имеет два метода, и второй из них не передполагает передачу условия останова. При его использовании, как в данном случае, происходит полный обход змейкой всего поля.

### Пример 4. Параметрические типы

Предположим теперь, что нужно решить задачу расстановки маркеров при наличии внутренних прямолинейных перегородок или перегородок прямоугольной формы.

При решении задачи 10 о подсчете числа прямоугольных перегородок на поле, нам уже приходилось использовать функцию, позволяющую перемещать робота в заданном направлении с обходом, при необходимости, прямоугольной или прямолинейной перегродки.

Вот эта функция.

```julia
function try_move!(robot, side)
    ortogonal_side = left(side)
    back_side = inverse(ortogonal_side)
    n=0
    while isborder(side)==true && isborder(robot, ortogonal_side)==false
        move!(robot, ortogonal_side)
        n += 1
    end
    if isborder(side)==true
        along!(robot, back_side, n)
        return false
    end
    move!(robot, side)
    while isborder(robot, back_side)
        move!(robot, side)
    end
    along!(robot, back_side, n)
    return true
end
```

Однако в связи с этой функцией имеется проблема, связанная с тем, что ранее тоже была определена функция с тем же именем и в точности с той же самой сигнатурой:

```julia
try_move!(robot, direct) = isborder(robot, direct) && begin move!(robot, direct); true end
```

(**сигнатурой функции** назавают набор типов её аргументов, указываемых при определении функции; в данном случае типы обоих параметраметров - Any).

Такое совпадение сигнатур двух разных функций означает, что новое определение полностью отменяет пережнее. А если же требуется иметь сразу два разных определения, то это в данной ситуации оказывается не возможным.

Чтобы выйти из данной конфликтной ситуации, новый метод функции try_move! нужно было определить для специально спроектированного нового типа аргумента (главное, чтобы этот тип отличался от Any).

Этот новый тип может быть просто оберткой имеющегося типа Robot (но при этом он будет иметь новое имя).

```julia
struct PassSimpleBorderRobot <: SimpleRobot
    robot::Robot
end
````

В действительности, такой тип будет не очень полезен, т.к. он базируется именно на типе Robot. Однако вместо типа Robot может понадобиться, например, тип PutmarkerRobot, или ещё какой-нибудь. Поэтому тип PassSimpleBorderRobot, позволяющий роботу обходить внутренние перегородки, стоит сделать параметрическим.

```julia
struct PassSimpleBorderRobot{TypeRobot} <: SimpleRobot
    robot::TypeRobot
end

get_robot(robot::PassSimpleBorderRobot) = get_robot(robot.robot) 
# для каждого производного от SimpleRobot типа должен быть определен метод get_robot

get_robot(robot::Robot) = robot # - это на случай, если TypeRobot == Robot
```

Теперь для этого параметрического типа можно определить новый метод функции try_move!

```julia
function try_move!(robot::PassSimpleBorderRobot, side)
    ortogonal_side = left(side)
    back_side = inverse(ortogonal_side)
    n=0
    while isborder(side)==true && isborder(robot, ortogonal_side)==false
        move!(robot, ortogonal_side)
        n += 1
    end
    if isborder(side)==true
        along!(robot, back_side, n)
        return false
    end
    move!(robot, side)
    while isborder(robot, back_side)
        move!(robot, side)
    end
    along!(robot, back_side, n)
    return true
end
```

Тело этой функции осталось без изменений, изменения касаются только её заголовка, где теперь указан тип аргумента robot.

И так, теперь после всех сделанных определений можно записать **окончательное решение** рассматриваемой задачи о расстановке маркеров в промежутках между внутренними перегородками.

```julia
# робот - в юго-западном углу
# robot - имеющаяся ссылка на рассматриваемый объект типа Robot

pass_simple_border_putmarker_robot = PassSimpleBorderRobot{PutmarkerRobot}(PutmarkerRobot(robot))
putmarker!(pass_simple_border_putmarker_robot)
snake!(pass_simple_border_putmarker_robot)
```

**Замечание.** Тип PassSimpleBorderRobot{TypeRobot} является параметрическим. При этом, если TypeRobot - это какой-либо фиксированный тип (предварительно определенный), то имеет место отношение:

```julia
PassSimpleBorderRobot{TypeRobot} <: PassSimpleBorderRobot
```

(тип PassSimpleBorderRobot{TypeRobot} - есть подип типа PassSimpleBorderRobot)

Поэтому, если в теле некоторой функции параметр TypeRobot явно не фигурирует, то тип её аргумента может быть определен просто как PassSimpleBorderRobot, что в рассмотренном примере с функцией try_move! и имеет место быть.

### Обобщенное программирование

На практическом занятии 2, при решении задачи о расстановки маркеров в форме косого креста (задача 4), было введено понятие об **обобщенном программировании**. Тогда было сказано, что обобщенное программирование сводится к проектированию так называемых обобщенных функций. Преимущество такого подхода к программированию состоит в том, что это позволяет существенно повы  сить возможности для повторного использования кода. 

Напоммним, что **обобщенные функции** - это такие функции, которые могут работать с аргументами разных типов, и в зависимости от фактических типов аргументов выполнять разные, соответствующие этим фактическим типам, действия, называются.

Теперь мы снова пришли к идее обобщенного программирования. Так функция snake! является еще одним примером обобщенной функции. В зависимости от того, какого типа будет ее аргумент robot, эта функция будет выполнять разные действия.

Если, например, аргумент robot, будет иметь тип Robot, то функция snake! просто будет перемещать робота змейкой. Если тип аргумента будет PutmarkerRobot, то по всей траектории перемещению будут поставлены маркеры. Если тип фкгумента robot будет PassSimpleBorderRobot{Robot}, то робот будет просто перемещаться этой функцие змейкой, но с обходом всех встречающихся на нути прямоугольных или прямолинейных перегородок. А если тип соответствующего аргумента будет PassSimpleBorderRobot{PutmarkerRobot}, то при движении змейкой (с обходом внутренних перегородок) будут ещё и ставиться маркеры.

Подобные применения обобщенной функции snake! можно продолжить неограниченно.

Другими примерами уже разработанных нами обобщенных функций являются along!, spiral!, shuttle!.

**Очень важное замечание.**

То, что функция along! является обобщенной, мы уже отмечали на практическом занятии 2 при решении задачи о расстановке маркеров в форме косого креста. Но тогда мы отмечали этот факт в связи с другим аргументом этой функции - аргументом side. Тогда нам понадобилось, чтобы аргумент side мог быть не только типа HorizonSide, но и типа NTuple{N,HorizonSide} (где параметр N=1,2,3,...).

Теперь же мы выяснили, что эта функция является также обобщенной и по аргументу robot. Это очень важное замечание, потому что далеко не в каждом языке программирования такое возможно. В языке Julia эта возможность обусловлена тем, что язык поодерживает так называемую **множественную диспетчеризацию** (multiple dispatch).

В языке Python, например, множественная диспетчерезация не поддерживается, и в этом языке функция (метод класса) может быть обобщенной только по одному первому своему агрументу.

Образно говоря, суть дела в том, что в классических объектноориентированных языках, таких как Python, каждый класс (тип) данных имеет своими методы (функциями), которыми он полностью "владеет", а в языках с множественной диспетчеризацией, таких как Julia, наоборот, функции имеют методы, которые "владеют" в общем случае сразу несколькими типами данных.

Суть обобщенного прогаммирования состоит в том, что решения самых разнообразных задач с применением обобщенных функции не требуют никаких изменений в коде самих этих функции. Именно это обеспечивает широкие возможности многократного повторного использования одого и того же кода в самых разных ситуациях.