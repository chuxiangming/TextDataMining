#include<stdio.h>
#include<string.h>

FILE * divData;	//	分词结果
FILE * wordWeight;	//	提取关键词结果
FILE * output;	//	输出关联分析结果

struct wordlist {	//	记录词 
	   char data[15];
	   long freq;
} wordList[5000];
long numWords,totalWords;

void init() {
	 printf("Loading Data...");
	 
	 divData = fopen("divData.txt","r");
	 wordWeight = fopen("wordWeight.txt","r");
	 output = fopen("association.txt", "w");
	 
	 char word[15];
	 long i;
	 memset(word,0,sizeof(word));
	 while(EOF!=fscanf(divData,"%s",word)) {
		if('\n'==word[0]) break;
		totalWords++;
		for(i=0;i<numWords;i++) {
			if(strcmp(word,wordList[i].data)==0) {
				wordList[i].freq++;
				break;
			}
		}
		if(i==numWords) {
			++numWords;
			strcpy(wordList[numWords].data,word);
		}
	 	memset(word,0,sizeof(word));
  	}
	fclose( divData ); 
	divData = fopen("divData.txt","r");
}

void flush() {
	fclose( divData ); 
	fclose( wordWeight );
	fclose( output );
}

int main() {
	init();
	
	flush();
	return 0;
}
