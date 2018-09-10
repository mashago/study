#!/usr/bin/python36

import sys

def test0():
	return 0

def test1():
	while True:
		mylist = ["a", "b", "c", "d"]
		mylist2 = mylist
		mylist[0] = "A"
		print mylist
		print mylist2
		print ""
		break

	while True:
		mylist = ["a", "b", "c", "d"]
		mylist2 = mylist[2:]
		mylist = mylist[:2]
		print mylist
		print mylist2
		print ""
		break
	
	while True:
		mylist = ["a", "b", "c", "d"]
		mylist2 = ["1", "2", "3"]
		mylist.extend(mylist2)
		print mylist
		print ""
		break

	return 0

test_funcs = [
	test0,
	test1,
]

def main(input_list):
	print input_list

	test_case = len(test_funcs) - 1

	try:
		if len(input_list) > 1:
			t = int(input_list[1])

			if t >= 0 and t <= test_case:
				test_case = t
	except:
		pass

	print "test_case=%s" % test_case
	test_funcs[test_case]()

main(sys.argv)
