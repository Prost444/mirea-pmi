function shahmat(robot)
    x0, y0 = go11!(robot)
    s = x0+y0
    side = Ost
    x, y = 1, 1
    x, y = along_putmarker_shah!(robot, side, s, x, y)
    while ! isborder(robot, Nord)
        move!(robot, Nord)
        y += 1
        side = HorizonSide((Int(side)+2)%4)
        x, y = along_putmarker_shah!(robot, side, s, x, y)
    end
    go11!(robot)
    goback!(robot, x0, y0)
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

function along_putmarker_shah!(robot, side, s, x, y)
    if s%2 == (x+y)%2 putmarker!(robot) end
    while ! isborder(robot, side)
        move!(robot, side)
        if side == Ost x += 1
        else x -= 1 end
        if s%2 == (x+y)%2 putmarker!(robot) end
    end
    x, y
end

function goback!(robot, x0, y0)
    for _ in 1:x0 move!(robot, Ost) end
    for _ in 1:y0 move!(robot, Nord) end
end