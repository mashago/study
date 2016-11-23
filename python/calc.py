def calc1(numbers) : 
	a=0;
	b=1;
	c=4;

	for n in numbers:
		a=a+n*n;
	
	return a;


def calc2(numbers) : 
	a=0;
	for n in numbers: 
		a=a+n*n;
	
	return a;


tmp = calc1([1, 2]);
print tmp

tmp = calc2([1, 2]);
print tmp
