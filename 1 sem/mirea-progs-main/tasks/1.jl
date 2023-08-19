include("c:\\Users\\elaza\\Desktop\\mirea-progs-main\\lib\\stdrobot.jl")
#	      Nord	
#
#	West    O	  Ost
#	
#	       Sud

#=
	ДАНО: Робот находится в произвольной клетке ограниченного прямоугольного поля без 
	внутренних перегородок и маркеров.
	РЕЗУЛЬТАТ: Робот — в исходном положении в центре прямого креста из маркеров, расставленных вплоть до внешней рамки.
=#

#отрисовать крест и вернуться на место
function makecross!(robot::Robot)
	for side in (HorizonSide(i) for i in 0:3)
		n = numsteps_putmarkers!(robot,side)
		along!(robot,inverse(side),n)#lib.jl
	end

	putmarker!(robot)
end

# Тест
robot = Robot("c:\\Users\\elaza\\Desktop\\mirea-progs-main\\tasks\\1.sit",animate=true)

makecross!(robot)