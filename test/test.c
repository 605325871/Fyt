# include <stdio.h>
  
int mysum(int sum)
{
     return  sum?sum+mysum(sum-1):0;
}
int main()
{
       printf("程序准备执行\n");
      int result=mysum(100);
      printf("result = %d \n",result);
      printf("结束调用\n");
      return 0;
}
