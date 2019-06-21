package main

import (
    "fmt"
    "net"
)

func HandleConn(conn net.Conn) {
    defer conn.Close()
    var buff [512]byte
    for true {
        n, err := conn.Read(buff[:])
        if err != nil {
            fmt.Println("conn read error", err)
            return
        }
        fmt.Println(n, string(buff[:n]))

        n, err = conn.Write(buff[:n])
        if err != nil {
            fmt.Println("conn write error", err)
            return
        }
    }
}

func main() {
    fmt.Println("hello simple server")

    listener, err := net.Listen("tcp", ":7710")
    defer listener.Close()
    if err != nil {
        fmt.Println("net listen error", err)
        return
    }

    for true {
        conn, err := listener.Accept()
        if err != nil {
            fmt.Println("accept error", err)
            break
        }
        fmt.Println("new conn")
        go HandleConn(conn)
    }
}
