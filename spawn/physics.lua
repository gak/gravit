-- vim:syntax=lua tabstop=4

-- ------------------------------------
-- physics-related functions
-- ------------------------------------

-- !!! needs loaded "constants.lua" and "functions.lua"


-- compute velocity for stable orbit 
function orbit_velocity(innermass, disttocenter)
	local result = 0.0

	if math.abs(disttocenter) > 0.1 then
		result= math.sqrt(gravit_g * math.abs(innermass) / math.abs(disttocenter))
	end

	if gravit_physics < PH_PROPER then
	        result=result*math.sqrt(math.abs(disttocenter))
	end

	return result
end


