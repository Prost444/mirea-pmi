function super_perim1!(robot)
    x, y = go11!(robot)
    for side in (Ost, Nord, West, Sud)
        along_putmarkers!(robot, side)
    end
    gohome!(robot, x, y)
end

function super_perim2!(robot)
    x0, y0 = go11!(robot)
    x, y = 0, 0
    for side in (Ost, Nord, West, Sud)
        while ! isborder(robot, side)
            move!(robot, side)
            if side == Ost x += 1
            elseif side == West x -= 1
            elseif side == Nord y += 1
            elseif side == Sud y -= 1 end
            if x==x0 || y==y0 putmarker!(robot) end
        end
    end
    gohome!(robot, x0, y0)
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
    for _ in 1:y0
        move!(robot, Nord)
    end
    y = 0
    x = 0
    while x != x0
        if isborder(robot, Ost)
            while isborder(robot, Ost)
                move!(robot, Sud)
                y += 1
            end
            move!(robot, Ost)
            x += 1    
            while isborder(robot, Nord)
                move!(robot, Ost)
                x += 1
            end
            for _ in 1:y
                move!(robot, Nord)
            end
        else
            move!(robot, Ost)
            x += 1
        end
    end
end

function along_putmarkers!(robot, side)
    while ! isborder(robot, side)
        move!(robot, side)
        putmarker!(robot)
    end
end
