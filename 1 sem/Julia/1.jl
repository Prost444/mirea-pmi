function cross!(robot)
    for side in (Nord, West, Sud, Ost)
        n = numsteps_putmarkers!(robot, side)
        along_numsteps!(robot, HorizonSide((Int(side)+2)%4), n)
    end
    putmarker!(robot)
end

function numsteps_putmarkers!(robot, side)
    num_steps = 0
    while ! isborder(robot, side)
        move!(robot, side)
        num_steps += 1
        putmarker!(robot)
    end
    return num_steps
end

along_numsteps!(robot, side, num_steps) = for _ in 1:num_steps move!(robot, side) end
