function vadd(v1, v2)
	return v(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z)
end

function vmul(v1, f)
	return v(v1.x * f, v1.y * f, v1.z * f)
end

function v(_x, _y, _z)
	t = { x = _x, y = _y, z = _z };
	mt = { __add = vadd, __mul = vmul }
	setmetatable(t, mt)
	return t
end

local random_firstuse=1
function seed_random()
       local r_seed = os.clock() * 1000 + os.time() + math.random(0,32676)
       -- improved seeding on some platforms, by throwing away the high part of time,
       -- then reversing the digits so the least significant part makes the biggest change
       -- see http://lua-users.org/wiki/MathLibraryTutorial
       r_seed = tonumber(tostring( r_seed ):reverse():sub(1,8))
       math.randomseed( r_seed )
       random_firstuse=0
end

function randomfloat(min,max)
        if random_firstuse == 1 then
	   seed_random();
        end
	return math.random() * (max-min) + min
end

-- returns an integer between min and max inclusive
function randomint(min,max)
        if random_firstuse == 1 then
	   seed_random();
        end
	return math.random(min,max)
end

-- scalar size of a vector
function vlength(v1)
	return math.sqrt((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z))
end

-- scalar distance between two vectors
function distance(v1,v2)
	return math.sqrt((v1.x-v2.x)^2 + (v1.y-v2.y)^2 + (v1.z-v2.z)^2)
end

-- slow and buggy
-- function randomrange(radius)
--      ????
--	local bigrange = radius * math.pi
--	local origin = v(0,0,0)
--	local pos
--	local d
--	repeat
--		pos = v(randomfloat(-bigrange,bigrange),randomfloat(-bigrange,bigrange),randomfloat(-bigrange,bigrange))
--		d = distance(pos, origin)
--	until d < radius
--	return pos
-- end

function randomrange(radius)
	local pos
	local d2
	repeat
	        -- pick random position from cube(-1,1)
		pos = v(randomfloat(-1,1), randomfloat(-1,1), randomfloat(-1,1))
		-- d2 = square distance to v(0,0,0)
		d2 = pos.x*pos.x + pos.y*pos.y + pos.z*pos.z
                -- repeat util pos is inside a ball of radius 1
	until d2 < 1
	-- scale to target radius
	return pos * radius
end


function rotatevector(pos, theta, around)

	local result = v(0,0,0)

	-- fixme: Quaternion rotation only works properly when "around" has scalar size 1
	-- around = around * (1.0 / vlength(around))

	local costheta = math.cos(theta)
	local sintheta = math.sin(theta)

	result.x = result.x + (costheta + (1 - costheta) * around.x * around.x) * pos.x
	result.x = result.x + ((1 - costheta) * around.x * around.y - around.z * sintheta) * pos.y
	result.x = result.x + ((1 - costheta) * around.x * around.z + around.y * sintheta) * pos.z

	result.y = result.y + ((1 - costheta) * around.x * around.y + around.z * sintheta) * pos.x
	result.y = result.y + (costheta + (1 - costheta) * around.y * around.y) * pos.y
	result.y = result.y +((1 - costheta) * around.y * around.z - around.x * sintheta) * pos.z

	result.z = result.z + ((1 - costheta) * around.x * around.z - around.y * sintheta) * pos.x
	result.z = result.z + ((1 - costheta) * around.y * around.z + around.x * sintheta) * pos.y
	result.z = result.z + (costheta + (1 - costheta) * around.z * around.z) * pos.z

	return result

end

function makeball(org, vel, radius, massmin, massmax, firstparticle, particles)

	for i=firstparticle,firstparticle+particles-1 do
		local pos = org + randomrange(radius)
		local mass = randomfloat(massmin, massmax)
		particle(i, pos, vel, mass)
	end
	
end

function makespiral(galpos, galvel, galradius, massmin, massmax, firstparticle, particles)		

	local massrange = math.abs(massmin - massmax)
	local estmass = massrange / 2 * particles
	local speedbase = .0000001
	local galaxyrotation = randomrange(1)
    local galaxyangle = randomfloat(0, 2 * math.pi);
	local velocitychaos = randomfloat(0.00000001, 0.00001)
	local pos
	local vel
	local mass
	local radius
	local speed
	local angle

	for i=firstparticle,firstparticle+particles-1 do
        radius = randomfloat(0, galradius)
        speed = speedbase * radius * math.sqrt(estmass)
        angle = randomfloat(0,2*math.pi)
        pos = v(math.cos(angle)*radius, math.sin(angle)*radius, randomfloat(-1,1))
        vel = v(math.cos(angle+math.pi/2)*speed*radius, math.sin(angle+math.pi/2)*speed*radius, 0)
        mass = randomfloat(massmin,massmax)
		pos = rotatevector(pos, galaxyangle, galaxyrotation)
		vel = rotatevector(vel, galaxyangle, galaxyrotation)
        particle(i, galpos + pos, galvel + vel, mass)
	end

end

function makegalaxy(galpos, galvel, galradius, massmin, massmax, firstparticle, particles)

	if randomint(0, 2) == 0 then
		makeball(galpos, galvel, galradius, massmin, massmax, firstparticle, particles)		
	else
		makespiral(galpos, galvel, galradius, massmin, massmax, firstparticle, particles)		
	end

end

