# include "strbuf.h"

//初始化 
void  StrBuf_Init(stb** sb, int alloc)
{
	*sb = BuyNode(alloc);

}
// 填充字符 
void StrBuf_attach(stb**sb, void* str,int len, int alloc)
{
	assert(*sb);
	stb* newnode = BuyNode(alloc);
	newnode->buf = (char*)str;
	newnode->len = len;
	newnode->alloc = alloc;
	if ((*sb)->buf == NULL)
	{
		(*sb) = newnode;
	}
	else
	{
		stb* cur = *sb;
		while (cur->next!=NULL)
		{
			cur = cur->next;
		}
		cur->next = newnode;
	}

}
//打印内容 长度 与 容量
void StrBuf_print(stb**sb)
{
	assert(*sb);	
	stb* cur = *sb;;
	while (cur!= NULL)
	{
		printf("%s\t\t", cur->buf);
		printf("此处已使用%d/%d\n", cur->len, cur->alloc);
		cur = cur->next;
	}

}
//释放内存 √
void StrBuf_Relese(stb** sb)
{
	assert(*sb);
	//单链表销毁函数

		stb* cur = *sb;
		while (cur)
		{
			stb* nextnode = cur->next;
			free(cur);
			cur = nextnode;
		}
		*sb= NULL;
	

}
// 交换两个strbuf
void StrBuf_Swap(stb**sb, stb** bs)
{

	char arr[1000]={0};
	strcpy(arr, (*sb)->buf);
	strcpy((*sb)->buf, (*bs)->buf);
	strcpy((*bs)->buf, arr);
}
//查找函数
stb* StrBuf_Find(stb*sb, char* x)
{
	stb* cur = sb;
	while (cur)
	{
		if (strcmp(cur->buf, x) == 0)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	return NULL;
}
//获取长度
char* StrBuf_Detach(stb* sb, int sz);
// 比较两个strbuf是否相同
int  StrBuf_cmp(const stb* first, const stb* second)
{
	return strcmp((stb*)first->buf, (stb*)second->buf);
}
// 清空sb
void StrBuf_reset(stb**sb)
{
	(*sb)->buf = 0;
	(*sb)->len = 0;
	(*sb)->alloc = 0;

}
// 申请新的sb
stb* BuyNode(int alloc)
{
	stb* newnode = (stb*)malloc(sizeof(stb));
	if (newnode == NULL)
	{
		perror("buynode");
		exit(-1);
	}
	else
	{
		newnode->alloc = alloc;
		newnode->buf = NULL;
		newnode->len = 0;
		newnode->next = NULL;
		return newnode;
	}
}

