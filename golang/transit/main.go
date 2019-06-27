package main

import (
    "fmt"
    "flag"
    "net"
    "sync"
)

func Transit(in net.Conn, out net.Conn, wg *sync.WaitGroup) {
    defer wg.Done()
    defer in.Close()
    defer out.Close()

    var buff [512]byte
    for true {
        n, err := in.Read(buff[:])
        if err != nil {
            fmt.Println("in read error", err)
            return
        }
        // fmt.Println(n, string(buff[:n]))

        n, err = out.Write(buff[:n])
        if err != nil {
            fmt.Println("out write error", err)
            return
        }
    }
}

func HandleConn(ct net.Conn, host string) {
    defer ct.Close()

    st, err := net.Dial("tcp", host)
    if err != nil {
        fmt.Println("net dial error", err)
        return
    }
    defer st.Close()

    var wg sync.WaitGroup
    wg.Add(2)
    go Transit(ct, st, &wg)
    go Transit(st, ct, &wg)
    wg.Wait()

    fmt.Println("conn die")
}

func main() {
    fmt.Println("hello transit")

    var port int
    var host string
    flag.IntVar(&port, "p", 0, "listen port")
    flag.StringVar(&host, "h", "", "target host")
    flag.Parse()

    fmt.Printf("port=%d host=%s\n", port, host)
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
        go HandleConn(conn, host)
    }

}
