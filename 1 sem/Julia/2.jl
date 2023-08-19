function perim!(robot)
    x, y = go11!(robot)
    for side in (Ost, Nord, West, Sud)
        along_putmarkers!(robot, side)
    end
    gohome!(robot, x, y)
end

function along_putmarkers!(robot, side)
    while ! isborder(robot, side)
        move!(robot, side)
        putmarker!(robot)
    end
end

function go11!(robot)
    x0 = 0
    y0 = 0
    while !(isborder(robot, West) && isborder(robot,Sud))
        while ! isborder(robot, Sud)
            move!(robot, Sud)
            y0 += 1
        end
        while ! isborder(robot, West)
            move!(robot, West)
            x0 += 1
        end
    end
    return x0, y0
end 

function gohome!(robot, x0, y0)
    for _ in 1:x0 move!(robot, Nord) end
    for _ in 1:y0 move!(robot, Ost) end
end
