
require("scripts/functions.lua")

function spawn()

	curveness1 = randomfloat(0, 0.1)
	curveness2 = randomfloat(0, 0.1)

	curvechangelimit = 0.0001
	
	if (randomint(0,4) == 0) then
		curvechange1 = randomfloat(-curvechangelimit,curvechangelimit)
	else
		curvechange1 = 0
	end
	
	if (randomint(0,4) == 0) then
		curvechange2 = randomfloat(-curvechangelimit,curvechangelimit)
	else
		curvechange2 = 0
	end
	
	speed = randomfloat(0,5)
	if (randomint(0,1) == 0) then
		massincrement = randomfloat(-0.01,0.01)
	else
		massincrement = 0
	end
		
	massrandom = randomfloat(0,100)
	distance = randomfloat(0.1, 5)

	pos = v(0,0,0)
	vel = v(0,0,0)
	ang1 = 0
	ang2 = 0

	for i=0,spawnparticles-1 do
	
		pos.x = pos.x + math.cos(ang1) * distance
		pos.y = pos.y + math.sin(ang1) * distance
		pos.y = pos.y + math.cos(ang2) * distance
		pos.z = pos.z + math.sin(ang2) * distance
		
		vel.x = math.cos(ang1) * speed
		vel.y = math.sin(ang1) * speed
		vel.y = math.cos(ang2) * speed
		vel.z = math.sin(ang2) * speed

		ang1 = ang1 + curveness1
		ang2 = ang2 + curveness2

		curveness1 = curveness1 + curvechange1
		curveness2 = curveness2 + curvechange2
		
		particle(i, pos, vel, randomfloat(0,massrandom) + massincrement * i)
	
	end

end

