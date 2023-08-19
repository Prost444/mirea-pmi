# AbstractRobot
using HorizonSideRobots

HSR = HorizonSideRobots

abstract type AbstractRobot end

HSR.move!(robot::AbstractRobot, side) = move!(get_robot(robot), side)
HSR.isborder(robot::AbstractRobot, side) = isborder(get_robot(robot), side)
HSR.putmarker!(robot::AbstractRobot) = putmarker!(get_robot(robot))
HSR.ismarker(robot::AbstractRobot) = ismarker(get_robot(robot))
HSR.temperature(robot::AbstractRobot) = temperature(get_robot(robot))

# CountTurnsRobot

mutable struct CountTurnsRobot <: AbstractRobot
    robot::Robot
    num_rotates::Int64

    CountTurnsRobot(robot::Robot) = begin
        new(robot, 0)
    end
end

get_robot(robot::CountTurnsRobot) = robot.robot
get_num_rotates(robot::CountTurnsRobot) = robot.num_rotates

function check_rotates!(robot::CountTurnsRobot, side::HorizonSide)
    if !isborder(robot.robot, side) || isborder(robot.robot, right(side))
        if !isborder(robot.robot, side)
            robot.num_rotates -= 1
        elseif isborder(robot.robot, right(side))
            robot.num_rotates += 1
        end
    end
end