package main

import (
    "fmt"
    "flag"
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
    fmt.Println("disconnect")
}

func main() {
    fmt.Println("hello simple server")

    var port int
    flag.IntVar(&port, "p", 0, "listen port")
    flag.Parse()

    if port == 0 {
        fmt.Println("error port", port)
        return
    }

    listener, err := net.Listen("tcp", fmt.Sprintf(":%d", port))
    if err != nil {
        fmt.Println("net listen error", err)
        return
    }
    defer listener.Close()

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
