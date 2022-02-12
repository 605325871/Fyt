# include "strbuf.h"

//��ʼ�� 
void  StrBuf_Init(stb* sb, int alloc)
{

}

// ��ʼ����
stb* stf_new(int alloc)
{
	stb* pstr = (stb*)malloc(sizeof(stb));
	int i = 0;
	if (pstr == NULL)
	{
		printf("��ʼ��ʧ��\n");
			exit(-1);
	}
	for (i = 0; i < alloc; i++)
	{
		*(pstr->buf + i) = '\0';
	}
	pstr->next = NULL;
	pstr->len = 0;
	pstr->alloc = alloc;
	return pstr;
}



// ����ַ� 
void StrBuf_attach(stb*sb, void* str,int len, int alloc)
{
	stb* newnode = stf_new(alloc);
	char* s = (char*)str;
	newnode->len = len;
	for (int i = 0; i < len&&*s!='\0'; i++)
	{
		*(newnode->buf + i) = *s;
		s++;
	}
	if (sb== NULL)
	{
		sb = newnode;
	}
	else
	{
		stb* cur = sb;
		while (cur->next != NULL)
		{
			cur = cur->next;
		}
		cur->next = newnode;
	}
}

//�������������ݱ���
void str_tocharattay(stb* sb, char* poubuf)
{
	*poubuf = '\0';
	while (sb != NULL)
	{
		strcat(poubuf, sb->buf);
		sb = sb->next;
	}
}

//��ӡ���� ���� �� ����
void StrBuf_print(stb* sb)
{
	int strlen = StrBuf_strlen(sb);
	char* output = (char*)malloc(sizeof(char) * (strlen + 1));
	str_tocharattay(sb, output);
	printf(" -----------------------\n");
	printf("\t%s\t\n",output);
	printf(" -------length=%d--------\n",strlen);
	printf(" -----------------------\n");
	free(output);
}
//�ͷ��ڴ� 
void StrBuf_Relese(stb* sb)
{
	stb* cur = NULL;
	while (sb!= NULL)
	{
		cur = sb->next;
		free(sb);
		sb = cur;
	}
}
char arr[100];
void StrBuf_Swap(stb*sb, stb* bs)
{
		sb = sb->next;
		bs = bs->next;
	strcpy(arr, sb->buf);
	strcpy(sb->buf, bs->buf);
	strcpy(bs->buf, arr);
}
//���Һ���
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
//��ȡ����
char* StrBuf_Detach(stb* sb, int sz);
// �Ƚ�����strbuf�Ƿ���ͬ
int  StrBuf_cmp(const stb* first, const stb* second)
{
	(stb*)first = first->next;
	(stb*)second = second->next;
	return strcmp((stb*)first->buf, (stb*)second->buf);
}


// ���sb
void StrBuf_reset(stb* sb)
{
	sb = sb->next;
	memset(sb->buf, 0, sizeof(sb->buf));

}
// ��鳤��
int StrBuf_strlen(stb* sb)
{
	int  length= 0;
	while (sb != NULL)
	{
		length += strlen(sb->buf);
		sb = sb->next;
	}
	return length;
}
// *׷��  ����
void StrBuf_add(stb* sb, const void* data, int len)
{
	int maxlen = sizeof(sb->buf)-1;
	int lastlen = 0;
	int strcount = 0;
	char* str1 = (char*)data;
	stb* plist = sb;
	while (plist->next != NULL)
	{
		plist = plist->next;
	}
	lastlen = (int)strlen(plist->buf);
	while (*str1 != '\0' && strcount < len)
	{
		if (lastlen < maxlen)
		{
			*(plist->buf + lastlen) = *str1;
			strcount++;
			lastlen++;
			str1++;
		}
		else
		{
			*(plist->buf + lastlen) = '\0';
			plist->next = stf_new(10);
			plist = plist->next;
			lastlen = 0;
		}

	}
	*(plist->buf + lastlen) = '\0';

}
// ׷�ӵ����ַ�
void StrBuf_addch(stb* sb, char c)
{
	char buf[2];
	buf[0] = c;
	buf[1] = '\0';
	StrBuf_add(sb, buf, 1);
}
 //׷���ַ���
void StrBuf_addstr(stb* sb, char* s)
{
	int len = strlen(s);
	StrBuf_add(sb, s, len);
}
//׷��һ��sb����һ��sb
void str_addbuf(stb* sb, const stb* sb2)
{
	sb = sb->next;
	sb2 = sb2->next;
	int s = strlen(sb2->buf);
	StrBuf_add(sb, sb2->buf, s);
}
