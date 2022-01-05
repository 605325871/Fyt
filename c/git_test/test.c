# include "strbuf.h"

void test()
{
	stb* sb = stf_new(10);
	stb* bs = stf_new(10);
	StrBuf_attach(sb, "xi", 2, 10);
	StrBuf_print(sb);

	StrBuf_add(sb, "youlinuxnb", 10);
	StrBuf_print(sb);

	StrBuf_attach(bs, "hello", 5, 10);

	StrBuf_addch(bs, 'x');
	StrBuf_print(bs);
	StrBuf_addstr(bs, "man");
	StrBuf_print(bs);
	printf("\n\n");

 	StrBuf_Swap(bs, sb);
	StrBuf_print(bs);
	int i = StrBuf_cmp(sb, bs);
	printf("\n%d \n", i);
	if (i == 0)
	{
		printf("һ��\n");
	}
	else
	{
		printf("��һ��\n");
	}
	str_addbuf(sb, bs);
	StrBuf_print(sb);


	StrBuf_reset(sb);


	StrBuf_Relese(sb);
}



int main()
{
	test();
		
}