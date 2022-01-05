
# define _CRT_SECURE_NO_WARNINGS 1
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <string.h>

 # define lengh 256

typedef struct strbuf
{
    int len;
    int alloc;//����
    char buf[lengh];
    struct strbuf* next;

}stb;
//��ʼ�� ��
void  StrBuf_Init(stb**sb, int alloc);
stb* stf_new(int alloc);
// ����ַ� ��
void StrBuf_attach(stb* sb, void* str, int len, int alloc);
//�ͷ��ڴ� ��
void StrBuf_Relese(stb* sb);
// ��������strbuf
void StrBuf_Swap(stb* sb, stb* bs);
//��ȡ����
char* StrBuf_Detach(stb* sb, int sz);
// �Ƚ�����strbuf�Ƿ���ͬ
int  StrBuf_cmp(const stb* first, const stb* second);
// ���sb
void StrBuf_reset(stb* sb);
//��ӡ���� ���� �� ����
void StrBuf_print(stb* sb);
//���Һ���
stb* StrBuf_Find(stb* sb, char* x);

//�������������ݱ���
void str_tocharattay(stb* sb, char* poubuf);




// ��鳤��
int StrBuf_strlen(stb* sb);
// ���󳤶�
void StrBuf_Grow(stb* sb, int extra);
// ׷��  ����
void StrBuf_add(stb* sb, const void* data, int len);
// ׷�ӵ����ַ�
void StrBuf_addch(stb* sb, char c);
// ׷���ַ���
void StrBuf_addstr(stb* sb, char* s);
//׷��һ��sb����һ��sb
void str_addbuf(stb* sb, const stb* sb2);


//ȥ��sb��˵����пո� tab��\t��
void strbuf_rtrim(stb* sb);
// ȥ���Ҷ˵� 
void strbuf_ltrim(stb* sb);
//  ɾ������������ָ��λ�õ��ַ�
