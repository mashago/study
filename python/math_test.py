import math

def move(x, y, step, angle=0):
	nx = x + step * math.cos(angle)
	ny = y - step * math.sin(angle)
	return nx, ny

x, y = move(50, 50, 50) #, math.pi / 6)
print x, y



def add_end(x=None):
	if x is None:
		print "x is None"
	else:
		print "x is not None"

	if x == None:
		print "x is None"
	else:
		print "x is not None"
	print "----------"

add_end();
add_end(10);
