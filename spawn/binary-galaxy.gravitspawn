
require("scripts/functions.lua")

function spawn()

	local dist = randomfloat(1,3) * spawnparticles
	local vel = randomfloat(20,50)
	local mass1min = randomfloat(500,1000)
	local mass1max = randomfloat(500,1000)
	local mass2min = randomfloat(500,1000)
	local mass2max = randomfloat(500,1000)
	local ballsize1 = randomfloat(500,1000)
	local ballsize2 = randomfloat(500,1000)

	if (randomint(0,1) == 0) then
		makeball(v(0,dist,0), v(-vel,0,0), ballsize1, mass1min, mass1max, 0, spawnparticles/2)
	else
		makespiral(v(0,dist,0), v(-vel,0,0), ballsize1, mass1min, mass1max, 0, spawnparticles/2)
	end
	
	if (randomint(0,1) == 0) then
		makeball(v(0,-dist,0), v(vel,0,0), ballsize2, mass2min, mass2max, spawnparticles/2, spawnparticles/2)
	else
		makespiral(v(0,-dist,0), v(vel,0,0), ballsize2, mass2min, mass2max, spawnparticles/2, spawnparticles/2)
	end
	
end
