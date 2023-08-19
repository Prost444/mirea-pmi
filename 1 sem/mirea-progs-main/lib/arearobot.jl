using HorizonSideRobots
HSR=HorizonSideRobots
abstract type AbstractRobot end
HSR.move!(robot::AbstractRobot,side)=move!(get_robot(robot), side)
HSR.isborder(robot::AbstractRobot,side)=isborder(get_robot(robot),side)
HSR.putmarker!(robot::AbstractRobot)=putmarker!(get_robot(robot))
HSR.ismarker(robot::AbstractRobot)=ismarker(get_robot(robot))
HSR.temperature(robot::AbstractRobot)=temperature(get_robot(robot))

mutable struct AreaRobot <: AbstractRobot 
	robot::Robot
	area::Int64
	coordy::Int64
	AreaRobot(robot::Robot) = begin
		new(robot,0,0)
	end
end

get_robot(robot::AreaRobot) = robot.robot
coordy(robot::AreaRobot) = robot.coordy
get_area(robot::AreaRobot) = robot.area

function HorizonSideRobots.move!(robot::AreaRobot, side::HorizonSide)
	check!(robot::AreaRobot, side::HorizonSide)
	move!(robot.robot, side)
	if side == Nord robot.coordy += 1
	elseif side == Sud robot.coordy -= 1 end
end

function check!(robot::AreaRobot, side::HorizonSide)
	if isborder(robot.robot, left(side)) && left(side)==Sud
		robot.area += robot.coordy
	end
	if isborder(robot.robot, left(side)) && left(side)==Nord
		robot.area -= robot.coordy+1
	end
end 

#повернуть налево
left(side::HorizonSide) = HorizonSide((Int(side) + 1) % 4)