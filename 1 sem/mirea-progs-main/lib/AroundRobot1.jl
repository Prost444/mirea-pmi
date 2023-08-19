include("Функции.jl")
include("typerobot.jl")
include("CountTurnsRobot.jl")

using HorizonSideRobots

# AroundRobot

mutable struct AroundRobot{TypeRobot}
    robot::TypeRobot
    main_side::HorizonSide
    coord_x::Int64
    coord_y::Int64

    function AroundRobot{TypeRobot}(robot::TypeRobot, main_side::HorizonSide=Ost, coord_x::Int64=0, coord_y::Int64=0) where TypeRobot
        new(robot, main_side, coord_x, coord_y)
    end

end

get_robot(robot::AroundRobot) = robot.robot
get_main_side(robot::AroundRobot) = robot.main_side
get_guide_side(robot::AroundRobot) = right(robot.main_side)
get_opposite_side(robot::AroundRobot) = left(robot.main_side)
coord_x(robot::AroundRobot) = robot.coord_x
coord_y(robot::AroundRobot) = robot.coord_y

function HorizonSideRobots.move!(robot::AroundRobot{CountTurnsRobot}, side::HorizonSide)
    move!(robot.robot, side)
	if side == Nord
        robot.coord_y += 1
	elseif side == Sud
        robot.coord_y -= 1
    elseif side == Ost
        robot.coord_x += 1
	elseif side == West
        robot.coord_x -= 1
    end
end

function along_the_border!(bypass::AroundRobot)
	# Проход робота, если нет преград на его направлении, а также нет впадин
	if isborder(bypass.robot, bypass.main_side) && !isborder(bypass.robot, get_guide_side(bypass))
        move!(bypass, get_guide_side(bypass))

	# Проход робота, если есть впадина или преграда
	elseif !isborder(bypass.robot, bypass.main_side) || isborder(bypass.robot, get_guide_side(bypass))

		# Проход робота, если есть впадина
		if !isborder(bypass.robot, bypass.main_side)
            check_rotates!(bypass.robot, bypass.main_side)

			move!(bypass, bypass.main_side)
            bypass.main_side = get_opposite_side(bypass)

		# Проход робота, если перед роботом преграда на его основном направлении
		elseif isborder(bypass.robot, get_guide_side(bypass))
            check_rotates!(bypass.robot, bypass.main_side)
			
			bypass.main_side = right(bypass.main_side)			
		end
	end
end

function around_the_maze!(bypass::AroundRobot)
    along_the_border!(bypass)
    while true
        along_the_border!(bypass)
        if bypass.coord_x == 0 && bypass.coord_y == 0
            break
        end
    end
end