package main

import (
	"fmt"
	"math/rand"
	"time"
)

type Request struct {
	ID      int
	Payload []byte
}

type Response struct {
	ID int
}

func generateLargePayload() []byte {
	return make([]byte, 1024*1024) // 1MB的负载
}

func processRequest(req *Request) *Response {
	resp := &Response{
		ID: req.ID,
	}

	// 生成大内存负载
	largePayload := generateLargePayload()

	// 使用后立即释放
	for i := 0; i < 100; i++ {
		square := make([]byte, len(largePayload))
		copy(square, largePayload)
	}
	largePayload = nil

	return resp
}

func main() {
	rand.Seed(time.Now().UnixNano())
	start := time.Now()
	const numRequests = 10000

	for i := 0; i < numRequests; i++ {
		req := &Request{
			ID:      i,
			Payload: generateLargePayload(),
		}

		resp := processRequest(req)

		// 使用resp避免未使用的错误
		fmt.Printf("Response for request %d: %d\n", resp.ID, resp.ID)

		// 释放对象
		req, resp = nil, nil
	}

	elapsed := time.Since(start)
	fmt.Printf("Processed %d requests in %s\n", numRequests, elapsed)
}
