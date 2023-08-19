function find_marker!(robot)
    num_steps = 0
    while !ismarker(robot)
        move!(robot, West)
        move!(robot, Sud)
        num_steps += 2
        for side in (Ost, Nord, West, Sud)
            for _ in 1:num_steps
                if ismarker(robot) return end
                move!(robot, side)
                if ismarker(robot) return end
            end
        end
    end
end