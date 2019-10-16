package main

import (
    "time"
    "log"
    "os"
    "github.com/gorilla/websocket"
)

func Log(str string) {
    str_time := time.Now().Format("2006-01-02 15:04:05")
    log.Printf("[%s] %s\n", str_time, str)
}

func main() {
    Log("hello websocket client")
    url := "ws://localhost:8080/echo"

	c, _, err := websocket.DefaultDialer.Dial(url, nil)
	if err != nil {
		log.Fatal("dial:", err)
	}
	defer c.Close()

    go func() {
		var buffer [512]byte
		for {
			n, err := os.Stdin.Read(buffer[:])
			if err != nil {
			log.Println("read error:", err)
				return
			}

			err = c.WriteMessage(websocket.TextMessage, buffer[:n])
			if err != nil {
				log.Println("write:", err)
				return
			}
	    }
    }()

    for {
        _, message, err := c.ReadMessage()
        if err != nil {
            log.Println("read:", err)
            return
        }
        log.Printf("recv: %s", message)
    }
}
