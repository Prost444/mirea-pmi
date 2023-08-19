function big_shahmat!(robot, n)
    x0, y0 = go11!(robot)
    x, y = 0, 0
    while ! (isborder(robot, Ost) && isborder(robot, Nord))
        if (x+y)%2 == 0
            if make_kletka!(robot, n)
                x = 0
                y += 1
            else
                x += 1
            end
        else
            if move_kletka!(robot, n)
                x = 0
                y += 1
            else
                x += 1
            end
        end
    end
    go11!(robot)
    gohome!(robot, x0, y0)
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

function gohome!(robot, x0, y0)
    for _ in 1:x0 move!(robot, Ost) end
    for _ in 1:y0 move!(robot, Nord) end
end

function make_kletka!(robot, n)
    for x in 1:n
        y1 = 0
        putmarker!(robot)
        for y in 1:n-1
            if ! isborder(robot, Nord)
                move!(robot, Nord)
                putmarker!(robot)
                y1 += 1
            end
        end
        along_numsteps!(robot, Sud, y1)   
        if ! isborder(robot, Ost)
            move!(robot, Ost)
        elseif isborder(robot, Nord)
            return true
        else
            gonextlayer!(robot, n)
            return true
        end
    end
    return false
end

function move_kletka!(robot, n)
    for x in 1:n
        if isborder(robot, Ost)
            gonextlayer!(robot, n)
            return true
        else
            move!(robot, Ost)
        end
    end
    return false
end

along_numsteps!(robot, side, num_steps) = for _ in 1:num_steps move!(robot, side) end

function gonextlayer!(robot, n)
    for _ in 1:n
        if isborder(robot, Nord)
            return
        end
        move!(robot, Nord)
    end
    while !isborder(robot, West) move!(robot, West) end
end
