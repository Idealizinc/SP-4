function CalculateBetweenDistanceSquared(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	--print(distanceSquare)
	return distanceSquare
end

function CalculateDistanceSquared(x1,y1,z1)
	local distanceSquare = (x1*x1) + (y1*y1) + (z1*z1)
	--print(distanceSquare)
	return distanceSquare
end

function CalculateDotProduct(x1,y1,z1,x2,y2,z2)
	local dot = (x1 * x2) + (y1 * y2) + (z1 * z2)
	--print(dot)
	return dot;
end