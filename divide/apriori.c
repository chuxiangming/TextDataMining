#include<stdio.h>
#include<string.h>

#define MaxNumWords 5000
#define MaxEachNumWords 150
#define MaxNumInstances 300
#define MaxRuleLength 3
#define MaxNumRules 1000000 

FILE * divData;	//	�ִʽ��
FILE * wordWeight;	//	��ȡ�ؼ��ʽ��
FILE * output;	//	��������������

struct WordList {	//	��¼�� 
	   char data[15];	//	��¼���� 
	   long freq;	//	��¼������ֵ�Ƶ�� 
} wordList[ MaxNumWords ];
long numWords;

struct RuleList {	//	��¼���򼯣��Գ����ڷ�������ǶԳ����ڷ������� 
	long length;
	long words[MaxRuleLength];
	long support;		//	֧�ֶȼ���	�Գ� 
	float confidence;	//	���Ŷ� 		�ǶԳ� 
	float interest;		//	��Ȥ����	�Գ� 
	float relative;		//	��ض�		�Գ� 
	float IS;			//	���Ҳ��	�Գ� 
}ruleList[ MaxNumRules ];
long numRules;
float minsup;	//	��С֧�ֶ� 
float minconf;	//	��С���Ŷ� 

struct  InstanceList {	//	��¼�¼���
	char has[ MaxNumWords ]; 	//	��¼�¼����Ƿ���ĳ������ 
	long words[ MaxEachNumWords ];	//	�¼������б� 
}instanceList[ MaxNumInstances ];
long numInstances;

void readGate() {	//	��ȡ��ֵ

	 printf("��������С֧�ֶ� minconf:");
	 scanf("%f",&minconf);
	 
	 printf("��������С���Ŷ� minsup:");
	 scanf("%f",&minsup);
} 

void init() {	//	��ȡ�ִʺ͹ؼ������� ����ȡ�¼� 	 
	 
	 readGate();
	  
	 printf("��ȡ����...");
	 
	 divData = fopen("divData.txt","r");
	 wordWeight = fopen("wordWeight.txt","r");
	 output = fopen("association.txt", "w");
	 
	 char word[15];
	 long i;
	 memset(word,0,sizeof(word));
	 while(EOF!=fscanf(divData,"%s",word)) {
		if('\n'==word[0]) break;
		if(strcmp(word,"end")) {
			++numInstances;
			continue;
		}
		numWords++;
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
		instanceList[numInstances].has[i]=1;	//	��¼�¼��еĵ��� 
	 	memset(word,0,sizeof(word));
  	}
	printf("\n��ȡ��ɣ��� %ld ���¼��� %ld ������", numInstances, numWords);
	printf("\n��������1-�..."); 
	for(i=0;i<numWords;i++) 
		if(minsup <= (float)wordList[i].freq/numWords ) {	//	���Ŷ���ֵ���
			++numRules;
			ruleList[numRules].interest=wordList[i].freq;
			ruleList[numRules].confidence=wordList[i].freq;
		}
	printf("\n������ɣ��� %ld ��1-�",numRules);
	
	return;
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
