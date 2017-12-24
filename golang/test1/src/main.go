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
	var i4 = 444
	fmt.Printf("i1=%d i2=%d i3=%d i4=%d\n", i1, i2, i3, i4)

	var f1 float32 = 3.14
	var f2 float64 = 3.1415926123456
	fmt.Printf("f1=%f f1=%g f2=%f f2=%g\n", f1, f1, f2, f2)

	return 0
}

func test2()(int) {
	// array
	var a1 [4]int = [4]int{1, 2, 3, 4}
	fmt.Printf("a1=%v len=%d\n", a1, len(a1))
	var a2 [4]int = [4]int{2, 3, 4}
	fmt.Printf("a2=%v len=%d\n", a2, len(a2))
	a3 := [...]int{2, 3, 4} // auto size len
	fmt.Printf("a3=%v len=%d\n", a3, len(a3))
	var a4 = [4]int{2, 3, 4}
	fmt.Printf("a4=%v len=%d\n", a4, len(a4))
	fmt.Printf("a4[1]=%d\n", a4[1])

	fmt.Println()

	// slice
	var s1 = []int{1, 2, 3, 4}
	fmt.Printf("s1=%v len=%d\n", s1, len(s1))
	s2 := []int{1, 2, 3, 4}
	fmt.Printf("s2=%v len=%d\n", s2, len(s2))
	var s3 = []int{}
	fmt.Printf("s3=%v len=%d\n", s3, len(s3))
	s3 = append(s3, 111)
	fmt.Printf("s3=%v len=%d\n", s3, len(s3))
	s3 = make([]int, 10) // 0 * 10
	fmt.Printf("s3=%v len=%d\n", s3, len(s3))

	// map

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

// var test_list = []testcast_f{ // as slice
// var test_list = []func()(int){ // is ok
var test_list = [...]testcast_f{ // as array
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
			// for i, fnc := range test_list[:] {
			for i, fnc := range test_list {
				fmt.Printf("\nrun case[%d]\n", i)
				ret := fnc()
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
