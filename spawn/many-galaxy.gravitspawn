require("scripts/functions.lua")

function spawn()
	-- work out a number of galaxies based on spawnparticles
	local testing = 69
	local galaxysize = randomint(200,1000)
	local galaxies = math.floor(spawnparticles / galaxysize) + 1
	for i=0,galaxies-1 do
		local pos = randomrange(galaxies * 2000)
		local vel = randomrange(50)
		local rad = randomfloat(50, 1000)
		local massmin = randomfloat(10,1000)
		local massmax = randomfloat(10,1000)
		local particlestart = spawnparticles/galaxies*i
		local particlecount = spawnparticles/galaxies
		-- sometimes make a ball of particles, sometimes a spiral disc 
		if (randomint(0,1) == 0) then
			makeball(pos, vel, rad, massmin, massmax, particlestart, particlecount)
		else
			makespiral(pos, vel, rad, massmin, massmax, particlestart, particlecount)
		end
	end
end

