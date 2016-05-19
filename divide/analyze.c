#include<stdio.h>
#include<string.h>

#define MaxNumWords 5000
#define MaxEachNumWords 150
#define MaxNumInstances 300
#define MaxRuleLength 2
#define MaxNumRules 10000

#define TRUE 1
#define FALSE 0 

FILE * divData;	//	�ִʽ��
FILE * wordWeight;	//	��ȡ�ؼ��ʽ��
FILE * output;	//	��������������

struct WordList {	//	��¼�� 
	   char data[30];	//	��¼���� 
	   long freq;	//	��¼������ֵ�Ƶ�� 
} wordList[ MaxNumWords ];
long numWords;

struct RuleList {	//	��¼���򼯣��Գ�ָ�����ڷ�������ǶԳ�ָ�����ڷ������� 
	long length;
	long words[MaxRuleLength];
	long support;		//	֧�ֶȼ���	�Գ� 
	double interest;		//	��Ȥ����	�Գ� 
	double relative;		//	��ض�		�Գ� 
	double IS;			//	���Ҳ��	�Գ� 
}ruleList[ MaxNumRules ];
long numRules;
double minsup1, minsup2;	//	��С֧�ֶ� 
double minconf;	//	��С���Ŷ� 

struct  InstanceList {	//	��¼�¼���
	long numInstWords; 
	char has[ MaxNumWords ]; 	//	��¼�¼����Ƿ���ĳ������ 
	long words[ MaxEachNumWords ];	//	�¼������б� 
}instanceList[ MaxNumInstances ];
long numInstances;

void readGate() {	//	��ȡ��ֵ
	/*
	 printf("��������С���Ŷ� minconf:");
	 scanf("%lf",&minconf);
	 
	 printf("��������С֧�ֶ� minsup:");
	 scanf("%lf",&minsup);
	 */
	 minconf = 0;
	 minsup1 = 0.02;
	 minsup2 = 0;
} 


long getSupport(struct RuleList rule) {
	long i,j;
	char containsRule;
	long support=0;
	for(i=0; i<numInstances; i++) {
		containsRule=TRUE;
		for(j=0;j<rule.length;j++)
			if(FALSE==instanceList[i].has[rule.words[j]]) {	//	���������ǰ��������еĴ� 
				containsRule=FALSE;
				break; 
			}
		if(TRUE==containsRule) {
			++support;
		}
	}
	return support;
}


void init() {	//	��ȡ�ִʺ͹ؼ������� ����ȡ�¼� 	 
	 
	 readGate();
	  
	 printf("��ȡ����...");
	 
	 divData = fopen("divData.txt","r");
	 wordWeight = fopen("wordWeight.txt","r");
	 output = fopen("nodes.txt", "w");
	 
	 char word[15];
	 long i,n;
	 memset(word,0,sizeof(word));
	 while(EOF!=fscanf(divData,"%s",word)) {
		if('\n'==word[0]) break;
		if(0==strcmp(word,"end")) {
			++numInstances;
			continue;
		} 
		for(i=0;i<numWords;i++) {
			if(strcmp(word,wordList[i].data)==0) {
				wordList[i].freq++;
				break;
			}
		} 
		if(i==numWords) {
			strcpy(wordList[numWords].data,word);
			wordList[numWords].freq=1;
			++numWords;
		}
		instanceList[numInstances].has[i]=1;	//	��¼�¼��еĵ��� 
		n=instanceList[numInstances].numInstWords++;
		instanceList[numInstances].words[n]=i;
	 	memset(word,0,sizeof(word));
  	}
	printf("\n��ȡ��ɣ��� %ld ���¼��� %ld ������", numInstances, numWords);
	printf("\n��������1-�..."); 
	for(i=0;i<numWords;i++) {
		
		if( minsup1 <= (double)wordList[i].freq/numInstances ) {	//	֧�ֶ���ֵ���
			//fprintf(output, "<node id=\"%s\"/>\n", wordList[i].data);
			fprintf(output, "%s\t\n",wordList[i].data);
			ruleList[numRules].support=wordList[i].freq;
			ruleList[numRules].length=1;
			ruleList[numRules].words[0]=i;
			++numRules;
		}
	}
	printf("\n������ɣ��� %ld ��1-�\n",numRules);
	
	return;
}

void flush() {
	fclose( divData ); 
	fclose( wordWeight );
	fclose( output );
}

void outputRule(struct RuleList rule) {
	long i;
	for(i=0;i<rule.length;i++) {
		printf("%s %ld",wordList[rule.words[i]].data, getSupport(rule));
	}
	printf("%lf\n",(double)rule.support/numInstances);
}

int main() {
	init();
	flush();
	system("pause");
	return 0;
}
