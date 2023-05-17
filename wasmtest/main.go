package main

import (
	"fmt"
	"math/rand"
	"runtime"
	"time"
)

const payloadSize = 8192
const numRequests = 200

func main() {
	Allocmemory()
}

func Allocmemory() {
	rand.Seed(time.Now().UnixNano())

	// 记录内存分配信息
	var memStats runtime.MemStats
	runtime.ReadMemStats(&memStats)
	initialHeapInuse := memStats.HeapInuse
	initialTotalAlloc := memStats.TotalAlloc
	initialMallocs := memStats.Mallocs
	initialfrees := memStats.Frees

	// 记录时间统计信息
	for i := 0; i < numRequests; i++ {
		// 分配内存
		req := &Request{
			ID:      i + 1,
			Payload: generateLargePayload(payloadSize),
		}

		// 处理请求
		resp := processRequest(req)

		resp.ID = 1

		// 释放内存
		req.Payload = nil
		req = nil
		resp = nil

		// 输出内存分配信息
		runtime.ReadMemStats(&memStats)
		heapInuse := memStats.HeapInuse - initialHeapInuse
		totalAlloc := memStats.TotalAlloc - initialTotalAlloc
		mallocs := memStats.Mallocs - initialMallocs
		frees := memStats.Frees - initialfrees
		fmt.Printf("HeapInuse=%d TotalAlloc=%d Mallocs=%d Frees=%d Mallocs-Free=%d\n", heapInuse, totalAlloc, mallocs, frees, mallocs-frees)
	}

}

type Request struct {
	ID      int
	Payload []byte
}

type Response struct {
	ID int
}

func generateLargePayload(size int) []byte {
	return make([]byte, size)
}

func processRequest(req *Request) *Response {
	resp := &Response{
		ID: req.ID,
	}

	// 生成大内存负载
	largePayload := generateLargePayload(len(req.Payload))

	// 使用后立即释放
	for i := 0; i < 9; i++ {
		square := make([]byte, len(largePayload))
		copy(square, largePayload)
	}
	largePayload = nil

	return resp
}
