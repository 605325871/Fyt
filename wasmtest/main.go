package main

import (
	"github.com/tetratelabs/proxy-wasm-go-sdk/proxywasm"
	"github.com/tetratelabs/proxy-wasm-go-sdk/proxywasm/types"

	"github.com/alibaba/higress/plugins/wasm-go/pkg/wrapper"
)

func main() {
	wrapper.SetCtx(
		"hello-world",
		wrapper.ProcessRequestHeadersBy(onHttpRequestHeaders),
	)
}

type HelloWorldConfig struct {
}

func onHttpRequestHeaders(ctx wrapper.HttpContext, config HelloWorldConfig, log wrapper.Log) types.Action {
	err := proxywasm.AddHttpRequestHeader("hello", "world")
	Allocmemory()
	if err != nil {
		log.Critical("failed to set request header")
	}
	proxywasm.SendHttpResponse(200, nil, []byte("hello world"), -1)
	return types.ActionContinue
}

func Allocmemory() {
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
