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

function randomfloat(min,max)
	return math.random() * (max-min) + min
end

function randomint(min,max)
	return math.floor(randomfloat(min,max))
end

function distance(v1,v2)
	return math.sqrt((v1.x-v2.x)^2 + (v1.y-v2.y)^2 + (v1.z-v2.z)^2)
end

function randomrange(radius)
	bigrange = radius * math.pi
	origin = v(0,0,0)
	repeat
		pos = v(randomfloat(-bigrange,bigrange),randomfloat(-bigrange,bigrange),randomfloat(-bigrange,bigrange))
		d = distance(pos, origin)
	until d < radius
	return pos
end

function makeball(org, vel, radius, firstparticle, particles, massmin, massmax)

	for i=firstparticle,firstparticle+particles-1 do
		pos = org + randomrange(radius)
		mass = randomfloat(massmin, massmax)
		particle(i, pos, vel, mass)
	end
	
end
