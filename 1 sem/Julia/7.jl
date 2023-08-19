function find_window!(robot)
    num_steps = 1
    side = Ost
    while isborder(robot, Nord)
        for _ in 1:num_steps 
            move!(robot, side)
            if !isborder(robot, Nord) return end
        end
        side = HorizonSide((Int(side)+2)%4)
        num_steps += 1
    end
end