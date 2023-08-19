using HorizonSideRobots

#повернуть налево
left(side::HorizonSide) = HorizonSide((Int(side) + 1 ) % 4)

#повернуть направо
right(side::HorizonSide) = Int(side) < 1 ? HorizonSide(3) : HorizonSide(Int(side) - 1)

mutable struct AroundRobot{TypeRobot}
    robot::TypeRobot
    edge_side::HorizonSide
    coordx::Int
    coordy::Int
    function AroundRobot{TypeRobot}(robot::TypeRobot,side::HorizonSide=West,x::Int=0,y::Int=0) where TypeRobot
		new(robot,side,x,y)
    end
end

get_robot(robot::AroundRobot) = robot.robot
get_edge_side(robot::AroundRobot) = robot.edge_side
get_forward_side(robot::AroundRobot) = right(robot.edge_side)
coordx(robot::AroundRobot) = robot.coordx
coordy(robot::AroundRobot) = robot.coordy


function HorizonSideRobots.move!(robot::AroundRobot{AreaRobot}, side::HorizonSide)
    move!(robot.robot, side)
	if side == Nord robot.coordy += 1
	elseif side == Sud robot.coordy -= 1
	elseif side == West robot.coordx -= 1
	else robot.coordx += 1 end
end

function forward!(edge::AroundRobot)
    if isborder(edge.robot,edge.edge_side) && !isborder(edge.robot,get_forward_side(edge)) #если мы в положении ⬤➝
        move!(edge, get_forward_side(edge))    
                                                #                 ¯¯¯¯¯¯¯¯
                                                #                      🠕
        if !isborder(edge.robot,edge.edge_side) #если мы в положении   ⬤|¯¯¯
            move!(edge, get_edge_side(edge))    #                   ¯¯¯¯¯
            edge.edge_side = left(edge.edge_side)
                                                     #                      🠕
            if !isborder(edge.robot,edge.edge_side)  #если мы в положении   ⬤|
                move!(edge, get_edge_side(edge))     #                   ¯¯¯¯¯¯¯¯¯¯
                edge.edge_side = left(edge.edge_side)
            end
        end
    else   #если мы уперлись в угол
        check!(edge.robot, get_forward_side(edge))
        edge.edge_side = right(edge.edge_side)
    end
end

function around!(edge::AroundRobot)
    forward!(edge)
    while edge.coordx !=0  || edge.coordy !=0
		forward!(edge)
	end
end