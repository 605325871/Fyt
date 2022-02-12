# include "strbuf.h"

void test()
{
	stb *sb=NULL;
	StrBuf_Init(&sb, 10);
	StrBuf_attach(&sb, "xi", 2, 10);
	StrBuf_attach(&sb, "you", 3, 10);
	StrBuf_attach(&sb, "linux", 5, 10);

	StrBuf_attach(&sb, "niu", 3, 10);

	StrBuf_attach(&sb, "bi", 2, 10);
	StrBuf_print(&sb);
	
	stb* p1 = StrBuf_Find(sb, "xi");
	stb* p2 = StrBuf_Find(sb, "you");

	if (StrBuf_cmp(p1, p2) == 0)
	{
		printf("一样");
	}
	else
	{
		printf("不一样");
	}
	

	StrBuf_Relese(&sb);
}



int main()
{
	test();
		
}
