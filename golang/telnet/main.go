package main

import (
    "fmt"
    "net"
    "os"
)

var BUFF_SIZE int = 512

func HandleRead(conn net.Conn) {
    buff := make([]byte, BUFF_SIZE)
    for true {
        n, err := conn.Read(buff)
        if err != nil {
            fmt.Println("conn read error", err)
            os.Exit(0)
        }
        fmt.Print(string(buff[:n]))
    }
}

func main() {
    fmt.Println("hello telnet")

    argc := len(os.Args)
    if argc != 3 {
        fmt.Println("err input [ip] [port]")
        return
    }

    ip := os.Args[1]
    if ip == "0" {
        ip = "0.0.0.0"
    }

    conn, err := net.Dial("tcp", fmt.Sprintf("%s:%s", ip, os.Args[2]))
    if err != nil {
        fmt.Println("net dial error", err)
        return
    }
    defer conn.Close()

    go HandleRead(conn)
    buff := make([]byte, BUFF_SIZE)
    for true {
        n, err := os.Stdin.Read(buff)
        if err != nil {
            fmt.Println("stdin read error", err)
            return
        }

        n, err = conn.Write(buff[:n])
        if err != nil {
            fmt.Println("conn write error", err)
            return
        }
    }
}
