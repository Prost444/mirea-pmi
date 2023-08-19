function zakras!(robot)
    x, y = go11!(robot)
    side = Ost
    along_putmarker!(robot, side)
    while ! isborder(robot, Nord)
        move!(robot, Nord)
        side = HorizonSide((Int(side)+2)%4)
        along_putmarker!(robot, side)
    end
    go11!(robot)
    goback!(robot, x, y)
end

function go11!(robot)
    x0 = 0
    y0 = 0
    while ! isborder(robot, West)
        move!(robot, West)
        x0 += 1
    end
    while ! isborder(robot, Sud)
        move!(robot, Sud)
        y0 += 1
    end
    return x0, y0
end

function along_putmarker!(robot, side)
    putmarker!(robot)
    while ! isborder(robot, side)
        move!(robot, side)
        putmarker!(robot)
    end
end

function goback!(robot, x0, y0)
    for _ in 1:x0 move!(robot, Ost) end
    for _ in 1:y0 move!(robot, Nord) end
end

