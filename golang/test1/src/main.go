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
var buff string

// call main last
func main() {
	fmt.Printf("hello world\n")
	fmt.Printf("buff=%s\n", buff)
	n := 123
	println("n=", n)
}
