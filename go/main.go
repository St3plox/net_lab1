package main

import (
	"encoding/binary"
	"fmt"
	"net"
	"os"
)

const (
	address = "127.0.0.1"
	port    = 8080
)

func main() {

	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%d", address, port))
	if err != nil {
		fmt.Println("Connection failed:", err)
		os.Exit(1)
	}
	defer conn.Close()

	fmt.Println("Connected to server.")

	var num int
	fmt.Print("Введите целое число \n")
	fmt.Scan(&num)

	var action byte
	fmt.Print("Введите действие: \n 1 - извлечь корень \n 2 - возвести в квадрат \n")
	fmt.Scanf("%c", &action)

	buffer := make([]byte, 1+4)
	buffer[0] = action

	binary.BigEndian.PutUint32(buffer[1:], uint32(num))

	_, err = conn.Write(buffer)
	if err != nil {
		fmt.Println("Failed to send data:", err)
		os.Exit(1)
	}

	fmt.Printf("Sent action: %c and number: %d\n", action, num)

	_, err = conn.Read(buffer)
	if err != nil {
		if err.Error() == "EOF" {
			fmt.Println("Received EOF from server. Server might have closed the connection unexpectedly.")
		} else {
			fmt.Println("Failed to receive data:", err)
		}
		os.Exit(1)
	}

	receivedNumber := int(binary.BigEndian.Uint32(buffer))

	fmt.Printf("Received number: %d\n", receivedNumber)
}
