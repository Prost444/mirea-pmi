function cross!(robot)
    sides = [[Nord, West], [Nord, Ost], [Sud, Ost],
             [Sud, West],[Nord, West], [Nord, Ost]]
    for i in 1:4
        n = numsteps_putmarkers!(robot, sides[i])
        goback!(robot, sides[(i+2)], n)
    end
    putmarker!(robot)
end

function numsteps_putmarkers!(robot, sides)
    num_steps = 0
    first_side = sides[1]
    second_side = sides[2]
    while ! (isborder(robot, first_side) || isborder(robot, second_side))
        move!(robot, first_side)
        move!(robot, second_side)
        num_steps += 1
        putmarker!(robot)
    end
    return num_steps
end

function goback!(robot, sides, num_steps)
    for _ in 1:num_steps
        move!(robot, sides[1])
        move!(robot, sides[2])
    end
end

