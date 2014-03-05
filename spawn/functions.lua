function vadd(v1, v2)
	return v(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z)
end

function vsub(v1, v2)
	return v(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z)
end

function vmul(v1, f)
	return v(v1.x * f, v1.y * f, v1.z * f)
end

function v(_x, _y, _z)
	t = { x = _x, y = _y, z = _z };
	mt = { __add = vadd,  __sub = vsub, __mul = vmul }
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

-- compute cross product of two vectors
-- returns a vector that is perpendicular to both input vectors
function vecproduct(vect1,vect2)
	local vect3=v(0,0,0)

	vect3.x = vect1.y * vect2.z - vect1.z * vect2.y
	vect3.y = vect1.z * vect2.x - vect1.x * vect2.z
	vect3.z = vect1.x * vect2.y - vect1.y * vect2.x
	return vect3
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

-- randomly pick a point inside a ball
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

-- randomly pick a point on the surface of a ball
function randomshell(radius)
    local pos
    local d2
    local rscale

    -- pick random position inside unit sphere (radius = 1)
    repeat
        pos = v(randomfloat(-1,1), randomfloat(-1,1), randomfloat(-1,1))
	d2 = pos.x*pos.x + pos.y*pos.y + pos.z*pos.z
    until (d2 > 0) and (d2 <= 1)

    -- scale position vector to target radius (so the point is always on the surface of the sphere)
    rscale = radius / math.sqrt(d2)
    return pos * rscale
end



function rotatevector(pos, theta, around)

	local result = v(0,0,0)

	--  Quaternion rotation only works properly when "around" has scalar size 1
	around = around * (1.0 / vlength(around))

	local costheta = math.cos(theta)
	local sintheta = math.sin(theta)
	local tcostheta = 1 - costheta

	result.x = result.x + (tcostheta * around.x * around.x + costheta) * pos.x
	result.x = result.x + (tcostheta * around.x * around.y - around.z * sintheta) * pos.y
	result.x = result.x + (tcostheta * around.x * around.z + around.y * sintheta) * pos.z

	result.y = result.y + (tcostheta * around.x * around.y + around.z * sintheta) * pos.x
	result.y = result.y + (tcostheta * around.y * around.y + costheta) * pos.y
	result.y = result.y + (tcostheta * around.y * around.z - around.x * sintheta) * pos.z

	result.z = result.z + (tcostheta * around.x * around.z - around.y * sintheta) * pos.x
	result.z = result.z + (tcostheta * around.y * around.z + around.x * sintheta) * pos.y
	result.z = result.z + (tcostheta * around.z * around.z + costheta) * pos.z

	return result

end

-- returns a random vector that is orthogonal to the input vector
function randomortho(vector, radius)
	local vect1=v(0,0,0)
	local vect2=v(0,0,0)
	local vectresult=v(0,0,0)
	local a=randomfloat(-1, 1)
	local b=randomfloat(-1, 1)


	-- step 0 : special handling for zero size vector
	if (math.abs(vector.x) + math.abs(vector.y) + math.abs(vector.z) < 0.000001)  then
		return v(0,radius,0)
	end

	-- step one : choose one orthogonal vector
	-- see http://www.wer-weiss-was.de/theme50/article3103419.html
	if (math.abs(vector.z)>0.0001) then
	   vect1 = v(0, vector.z, -vector.y)
	else
	   vect1 = v(vector.y, -vector.x,0)
	end

	-- step two : compute vect2 othogonal to vector and vect2
	-- (using cross product), so 
	-- (vect1, vect2, vector are othogonal now)
	vect2 = vecproduct(vector,vect1)

	--step 3: normalize vectors (optional..)
	vect1=vect1 * (1/distance(vect1, v(0,0,0)))
	vect2=vect2 * (1/distance(vect2, v(0,0,0)))

	--step 4: vector3 = a*vect1 + b*vect2
	vectresult.x= a*vect1.x + b*vect2.x
	vectresult.y= a*vect1.y + b*vect2.y
	vectresult.z= a*vect1.z + b*vect2.z

	--step 4: normalize and scale
	vectresult = vectresult * (radius / distance(vectresult, v(0,0,0)))
	return vectresult
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

