package main

import (
	"fmt"
	"runtime"
)

// call init func second
func init() {
	fmt.Printf("in init\n")
	buff = runtime.GOOS
}

// init var first
var buff string // only define, should define type

// call main last
func main() {
	fmt.Printf("hello world\n")
	fmt.Printf("buff=%s\n", buff)
	n1 := 111 // define and init
	var n2 = 222 // define and init
	println("n1=", n1, "n2=", n2)
}
