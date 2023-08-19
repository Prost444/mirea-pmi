using HorizonSideRobots
HSR=HorizonSideRobots
abstract type AbstractRobot end
HSR.move!(robot::AbstractRobot,side)=move!(get_robot(robot), side)
HSR.isborder(robot::AbstractRobot,side)=isborder(get_robot(robot),side)
HSR.putmarker!(robot::AbstractRobot)=putmarker!(get_robot(robot))
HSR.ismarker(robot::AbstractRobot)=ismarker(get_robot(robot))
HSR.temperature(robot::AbstractRobot)=temperature(get_robot(robot))

struct CoordRobot <: AbstractRobot 
	robot::Robot
	coords::Coordinates
end

function НоrizonSideRobots.move!(robot::CoordRobot, side)
	move!(robot.robot, side)
	move!(robot.coords, side)
end
	
get_robot(robot::CoordRobot) = robot.robot

get_coords(robot::CoordRobot) = get_coords(robot.coords)