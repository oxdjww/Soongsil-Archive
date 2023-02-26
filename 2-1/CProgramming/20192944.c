#include <stdio.h>
int main(void){
    int k;
    int i;
    printf("-128부터 127까지의 십진수 정수 하나를 입력하세요 : ");
    scanf("%d",&k);		//k로 입력 받기
    printf("%d의 이진수는 ",k);
    if(k>=0)	//양수일 경우
	    {
			for(i = 128 ; i > 0 ; i/=2)
			{
				printf("%d",k/i);	//i를 나눈 몫을 차례로 출력
				if(i==16)	//4자리 출력 후 띄어쓰기 한칸
					printf(" ");
				k %= i;		//몫을 빼고 남은 나머지를 k에 넣어줘서 for문 계속 돌림
			}
		}
    else if(k<0)	//음수일 경우
    {
		k = -k;		//부호를 바꾸고
        k -= 1;		//-1한 값을 k에 넣어준다
        for(i = 128 ; i > 0 ; i/=2)
        {		//부호바꾸고 -1한 8비트 2진수 k값은 초기 k값의 1의보수와 같다
            if(k/i)		//그래서 양수의 경우와 같이 i를 나눈 몫을 출력하는데
                printf("0");	//1일 경우엔 0 출력
            else	
                printf("1");	//0일 경우엔 1 출력
          
            
            if(i==16)	//4자리 출력 후 띄어쓰기 한칸
                printf(" ");
            k %= i;		//몫을 빼고 남은 나머지를 k에 넣어줘서 for문 계속 돌림
        }
    }
    printf("입니다.\n");
    return 0;
} 