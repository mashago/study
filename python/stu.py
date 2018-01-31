#!/usr/bin/python36

import sys

def test0():
	print("test0")
	return 0

def test1():
	print("test1")
	return 0


test_funcs = [
	test0
,	test1
]

def main(input_list):
	print(input_list)

	test_case = len(test_funcs) - 1
	print("test_case=", test_case)
	
	try:
		if len(input_list) > 1:
			t = int(input_list[1])

			print("t=", t)
			if t >= 0 and t <= test_case:
				test_case = t
	except:
		pass

	func = test_funcs[test_case]
	func()


main(sys.argv)
