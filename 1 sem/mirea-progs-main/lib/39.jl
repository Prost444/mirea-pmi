include("arearobot.jl")
include("aroundrobot.jl")


arearobot = AreaRobot(Robot("39A.sit",animate=true))
aroundrobot = AroundRobot{AreaRobot}(arearobot)
around!(aroundrobot)
println(get_area(aroundrobot.robot))