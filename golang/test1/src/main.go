package main

import (
	"fmt"
	"runtime"
	"os"
	"strings"
	"strconv"
)

// first, init var
var buff string // only define, should define type

// second, call init func
func init() {
	fmt.Printf("in init\n")
	buff = runtime.GOOS
}

type testcast_f func()(int)

func test0()(int) {

	fmt.Printf("buff=%s\n", buff)
	n1 := 111 // define and init
	var n2 = 222 // define and init
	println("n1=", n1, "n2=", n2)

	return 0
}

func test1()(int) {
	var b1 bool = true
	var b2 bool = false
	fmt.Printf("b1=%t b2=%t\n", b1, b2)

	var i1 int = 111
	var i2 int32 = 222
	var i3 int64 = 333
	fmt.Printf("i1=%d i2=%d i3=%d\n", i1, i2, i3)

	var f1 float32 = 3.14
	var f2 float64 = 3.1415926123456
	fmt.Printf("f1=%f f1=%g f2=%f f2=%g\n", f1, f1, f2, f2)

	return 0
}

func test2()(int) {
	return 0
}

func test3()(int) {
	return 0
}

// is ok
var test4 testcast_f = func()(int) {
	return 0
}

// is ok
var test5 func()(int) = func()(int) {
	return 0
}

var test_list = []testcast_f{
// var test_list = []func()(int){ -- is ok
test0,
test1,
test2,
test3,
test4,
test5,
}

// last, call main
func main() {

	fmt.Printf("hello golang\n")
	fmt.Printf("os.Args=%s\n", os.Args)

	argc := len(os.Args)
	argv := os.Args
	maxcase := len(test_list)
	testcase := maxcase - 1
	var err error

	if argc > 1 {
		if 0 == strings.Compare(argv[1], "all") {
			for i:= 0; i < maxcase; i++ {
				fmt.Printf("\nrun case[%d]\n", i)
				ret := test_list[i]()
				if ret != 0 {
					fmt.Printf("cast[%d] ret=%d\n", i, ret)
					return
				}
			}
			return
		}
		testcase, err = strconv.Atoi(argv[1])
		if err != nil || testcase < 0 || testcase >= maxcase {
			testcase = maxcase - 1
		}
	}

	fmt.Printf("run case[%d]\n", testcase)
	ret := test_list[testcase]()
	if ret != 0 {
		fmt.Printf("cast[%d] ret=%d\n", testcase, ret)
	}
}
