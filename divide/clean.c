#include<stdio.h>
#include<string.h>

FILE *fin, *fout;

int main()
{
	freopen("rawdata.txt","r",stdin);
	freopen("data.txt","w",stdout);
	char c;
	while(scanf("%c",&c)!=EOF) {
		if(c<0||c=='\n')
			printf("%c",c);
	}
	return 0;
} 
