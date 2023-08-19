function main5!(robot)
    x, y = go11!(robot)
    insideframe_perim!(robot)
    vneshn_perim!(robot)
    gohome!(robot, x, y)
end

function insideframe_perim!(robot)
    for side in find_frame!(robot)
        putmarker!(robot)
        while isborder(robot, HorizonSide(side))
            move!(robot, HorizonSide((side+1)%4))
            putmarker!(robot)
        end
        putmarker!(robot)
        move!(robot, HorizonSide(side))
    end
end
    
function find_frame!(robot)
    side = Ost
    while !isborder(robot, Nord)
        while !isborder(robot, side)
            move!(robot, side)
            if isborder(robot, Nord)
                if side == Ost
                    while isborder(robot, Nord) move!(robot, Ost) end
                    move!(robot, West)
                end
                return (0, 3, 2, 1)
            end
        end
        move!(robot, Nord)
        side = HorizonSide((Int(side)+2)%4)
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

function vneshn_perim!(robot)
    go11!(robot)
    for side in (Ost, Nord, West, Sud)
        along_putmarkers!(robot, side)
    end
end

function along_putmarkers!(robot, side)
    while ! isborder(robot, side)
        move!(robot, side)
        putmarker!(robot)
    end
end

function gohome!(robot, x0, y0)
    for _ in 1:y0
        move!(robot, Nord)
    end
    y = 0
    for _ in 1:x0
        while isborder(robot, Ost)
            move!(robot, Sud)
            y += 1
        end
        move!(robot, Ost)
    end
    for _ in 1:y
        move!(robot, Nord)
    end
end