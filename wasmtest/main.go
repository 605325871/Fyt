package main

import (
<<<<<<< HEAD
	"fmt"
	"math/rand"
	"time"

	"github.com/alibaba/higress/plugins/wasm-go/pkg/wrapper"
	"github.com/tetratelabs/proxy-wasm-go-sdk/proxywasm"
	"github.com/tetratelabs/proxy-wasm-go-sdk/proxywasm/types"
	"github.com/tidwall/gjson"
=======
	"github.com/tetratelabs/proxy-wasm-go-sdk/proxywasm"
	"github.com/tetratelabs/proxy-wasm-go-sdk/proxywasm/types"

	"github.com/alibaba/higress/plugins/wasm-go/pkg/wrapper"
>>>>>>> a2a1f6ae67f5cb1370dfd77872d80ac667b0104a
)

func main() {
	wrapper.SetCtx(
<<<<<<< HEAD
		// 插件名称
		"testForTinygo0.27-plugin",
		// 为解析插件配置，设置自定义函数
		wrapper.ParseConfigBy(parseConfig),
		// 为处理请求头，设置自定义函数
=======
		"hello-world",
>>>>>>> a2a1f6ae67f5cb1370dfd77872d80ac667b0104a
		wrapper.ProcessRequestHeadersBy(onHttpRequestHeaders),
	)
}

<<<<<<< HEAD
// 自定义插件配置

type MyConfig struct {
	start bool
}

func parseConfig(json gjson.Result, config *MyConfig, log wrapper.Log) error {
	// 解析出配置，更新到config中
	config.start = json.Get("start").Bool()
	return nil
}

func onHttpRequestHeaders(ctx wrapper.HttpContext, config MyConfig, log wrapper.Log) types.Action {

	requestUrl, err := proxywasm.GetHttpRequestHeader(":path")
	if err != nil {
		log.Warnf("get path failed: %v", err)
		return types.ActionContinue
	}
	if requestUrl == "/foo/start" {
		if config.start {
			Allocmemory()
			proxywasm.SendHttpResponse(200, nil, []byte("start end 0.81"), -1)
		} else {
			proxywasm.SendHttpResponse(409, nil, []byte("has not started 0.81"), -1)
		}
	} else {
		proxywasm.SendHttpResponse(200, nil, []byte("dont get start "+requestUrl), -1)
	}
	return types.ActionContinue

=======
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
>>>>>>> a2a1f6ae67f5cb1370dfd77872d80ac667b0104a
}

func Allocmemory() {
	rand.Seed(time.Now().UnixNano())
<<<<<<< HEAD
	const numRequests = 600
=======
	start := time.Now()
	const numRequests = 10000
>>>>>>> a2a1f6ae67f5cb1370dfd77872d80ac667b0104a

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

<<<<<<< HEAD
=======
	elapsed := time.Since(start)
	fmt.Printf("Processed %d requests in %s\n", numRequests, elapsed)
>>>>>>> a2a1f6ae67f5cb1370dfd77872d80ac667b0104a
}

type Request struct {
	ID      int
	Payload []byte
}

type Response struct {
	ID int
}

func generateLargePayload() []byte {
<<<<<<< HEAD
	return make([]byte, 1024*1024) //
=======
	return make([]byte, 1024*1024) // 1MB的负载
>>>>>>> a2a1f6ae67f5cb1370dfd77872d80ac667b0104a
}

func processRequest(req *Request) *Response {
	resp := &Response{
		ID: req.ID,
	}

	// 生成大内存负载
	largePayload := generateLargePayload()

	// 使用后立即释放
<<<<<<< HEAD
	for i := 0; i < 9; i++ {
=======
	for i := 0; i < 100; i++ {
>>>>>>> a2a1f6ae67f5cb1370dfd77872d80ac667b0104a
		square := make([]byte, len(largePayload))
		copy(square, largePayload)
	}
	largePayload = nil

	return resp
}
