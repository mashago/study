package main

import (
    "time"
    "log"
    "net/http"
    "github.com/gorilla/websocket"
)
var upgrader = websocket.Upgrader{}

func Log(str string) {
    str_time := time.Now().Format("2006-01-02 15:04:05")
    log.Printf("[%s] %s\n", str_time, str)
}

func echo(w http.ResponseWriter, r *http.Request) {
    Log("new conn to echo");
    c, err := upgrader.Upgrade(w, r, nil)
    if err != nil {
        log.Println("upgrade:", err)
        return
    }
    defer c.Close()
    for {
        mt, message, err := c.ReadMessage() // block
        if err != nil {
            log.Println("read:", err)
            break
        }
        log.Printf("recv: %s", message)

        err = c.WriteMessage(mt, append([]byte("hello "),message[:]...))
        if err != nil {
            log.Println("write:", err)
            break
        }
    }
}

func main() {
    Log("hello websocket")
    http.HandleFunc("/echo", echo)
    log.Fatal(http.ListenAndServe("0.0.0.0:8080", nil))
}

