package main

import (
    "fmt"
    "net"
    "os"
)

var BUFF_SIZE int = 5

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

    conn, err := net.Dial("tcp", "127.0.0.1:7710")
    defer conn.Close()
    if err != nil {
        fmt.Println("net dial error", err)
        return
    }

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
