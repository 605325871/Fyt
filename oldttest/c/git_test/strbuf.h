
# define _CRT_SECURE_NO_WARNINGS 1
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <string.h>

 # define lengh 256

typedef struct strbuf
{
    int len;
    int alloc;//容量
    char buf[lengh];
    struct strbuf* next;

}stb;
//初始化 √
void  StrBuf_Init(stb**sb, int alloc);
stb* stf_new(int alloc);
// 填充字符 √
void StrBuf_attach(stb* sb, void* str, int len, int alloc);
//释放内存 √
void StrBuf_Relese(stb* sb);
// 交换两个strbuf
void StrBuf_Swap(stb* sb, stb* bs);
//获取长度
char* StrBuf_Detach(stb* sb, int sz);
// 比较两个strbuf是否相同
int  StrBuf_cmp(const stb* first, const stb* second);
// 清空sb
void StrBuf_reset(stb* sb);
//打印内容 长度 与 容量
void StrBuf_print(stb* sb);
//查找函数
stb* StrBuf_Find(stb* sb, char* x);

//将缓存区的内容保存
void str_tocharattay(stb* sb, char* poubuf);




// 检查长度
int StrBuf_strlen(stb* sb);
// 扩大长度
void StrBuf_Grow(stb* sb, int extra);
// 追加  数据
void StrBuf_add(stb* sb, const void* data, int len);
// 追加单个字符
void StrBuf_addch(stb* sb, char c);
// 追加字符串
void StrBuf_addstr(stb* sb, char* s);
//追加一个sb到另一个sb
void str_addbuf(stb* sb, const stb* sb2);


//去除sb左端的所有空格 tab‘\t’
void strbuf_rtrim(stb* sb);
// 去掉右端的 
void strbuf_ltrim(stb* sb);
//  删除掉缓冲区从指定位置的字符
