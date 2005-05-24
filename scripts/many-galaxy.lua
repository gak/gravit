require("scripts/functions.lua")

function spawn()
	-- work out a number of galaxies based on spawnparticles
	galaxysize = randomint(10,1000)
	galaxies = math.floor(spawnparticles / galaxysize) + 1
	for i=0,galaxies-1 do
		makeball(randomrange(galaxies * 2000), randomrange(50), randomfloat(1, 1000), spawnparticles/galaxies*i, spawnparticles/(galaxies), randomfloat(0,1000), randomfloat(0,1000))
	end
end

