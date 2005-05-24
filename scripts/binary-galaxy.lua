
require("scripts/functions.lua")

function spawn()

	dist = randomfloat(1,3) * spawnparticles
	vel = randomfloat(20,50)
	mass1min = randomfloat(500,1000)
	mass1max = randomfloat(500,1000)
	mass2min = randomfloat(500,1000)
	mass2max = randomfloat(500,1000)
	ballsize1 = randomfloat(500,1000)
	ballsize2 = randomfloat(500,1000)
	
	makeball(v(0,dist,0), v(-vel,0,0), ballsize1, 0, spawnparticles/2, mass1min, mass1max)
	makeball(v(0,-dist,0), v(vel,0,0), ballsize2, spawnparticles/2, spawnparticles/2, mass2min, mass2max)
	
end
