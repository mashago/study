package main

import (
	"fmt"
	"runtime"
	"os"
	"strings"
	"strconv"
)

// call init func second
func init() {
	fmt.Printf("in init\n")
	buff = runtime.GOOS
}

func test0()(int) {
	return 0
}

func test1()(int) {
	return 0
}

type testcast_f func()(int)
var test_list = []testcast_f{
	test0
,	test1
}

// init var first
var buff string // only define, should define type

// call main last
func main() {

	fmt.Printf("hello golang\n")
	fmt.Println(os.Args)

	argc := len(os.Args)
	argv := os.Args
	maxcase := len(test_list)
	testcase := maxcase - 1

	if argc > 1 {
		if !strings.Compare(argv[1], "all") {
			fmt.Printf("run all case\n")
			for i:= 1; i < maxcase; i++ {
				ret := test_list[i]()
				if ret != 0 {
					fmt.Printf("cast[%d] ret=%d\n", i, ret)
					return 0
				}
			}
			return 0
		}
		testcase, err := strconv.Atoi(argv[1])
		if testcase < 0 || testcase >= maxcase {
			testcase = maxcase - 1
		}
	}

	fmt.Printf("run case[%d]\n", testcase)
	ret := test_list[testcase]()
	if ret != 0 {
		fmt.Printf("cast[%d] ret=%d\n", i, ret)
	}



	fmt.Printf("buff=%s\n", buff)
	n1 := 111 // define and init
	var n2 = 222 // define and init
	println("n1=", n1, "n2=", n2)
}
