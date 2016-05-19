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
	   char data[15];	//	��¼���� 
	   long freq;	//	��¼������ֵ�Ƶ�� 
	   char isUser[10];
} wordList[ MaxNumWords ];
long numWords;

struct RuleList {	//	��¼���򼯣��Գ�ָ�����ڷ�������ǶԳ�ָ�����ڷ������� 
	long length;
	long words[MaxRuleLength];
	long support;		//	֧�ֶȼ���	�Գ� 
	double interest;		//	��Ȥ����	�Գ� 
	double relative;		//	��ض�		�Գ� 
	double IS;			//	���Ҳ��	�Գ� 
	char hasUser[10];
}ruleList[ MaxNumRules ];
long numRules;
double minsup1, minsup2, minsup3;	//	��С֧�ֶ� 
double minconf;	//	��С���Ŷ� 

struct  InstanceList {	//	��¼�¼���
	long numInstWords; 
	char has[ MaxNumWords ]; 	//	��¼�¼����Ƿ���ĳ������ 
	long words[ MaxEachNumWords ];	//	�¼������б� 
}instanceList[ MaxNumInstances ];
long numInstances;

long output_count;

void readGate() {	//	��ȡ��ֵ
	/*
	 printf("��������С���Ŷ� minconf:");
	 scanf("%lf",&minconf);
	
	 printf("��������С֧�ֶ� minsup:");
	 scanf("%lf",&minsup);
*/
	 minconf = 0;
	 minsup1 = 0.02;
	 minsup2 = 0.01;
	 minsup3 = 0.005;
} 

void init() {	//	��ȡ�ִʺ͹ؼ������� ����ȡ�¼� 	 
	 
	 readGate();
	  
	 printf("��ȡ����...");
	 
	 divData = fopen("divData.txt","r");
	 wordWeight = fopen("wordWeight.txt","r");
	 output = fopen("edges.txt", "w");
	 
	 char word[15];
	 long i,n;
	 char flag[10]="yes";
	 memset(word,0,sizeof(word));
	 while(EOF!=fscanf(divData,"%s",word)) {
		if('\n'==word[0]) break;
		if(0==strcmp(word,"end")) {
			strcpy(flag,"yes");
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
			if(strcmp("yes",flag)==0) {
				strcpy(wordList[numWords].isUser,flag);
			}
			++numWords;
		}
		instanceList[numInstances].has[i]=1;	//	��¼�¼��еĵ��� 
		n=instanceList[numInstances].numInstWords++;
		instanceList[numInstances].words[n]=i;
	 	memset(word,0,sizeof(word));
		strcpy(flag,"no");
  	}
	printf("\n��ȡ��ɣ��� %ld ���¼��� %ld ������", numInstances, numWords);
	printf("\n��������1-�..."); 
	for(i=0;i<numWords;i++) 
		if( minsup1 <= (double)wordList[i].freq/numInstances ) {	//	���Ŷ���ֵ���
			ruleList[numRules].support=wordList[i].freq;
			ruleList[numRules].length=1;
			ruleList[numRules].words[0]=i;
			strcpy(ruleList[numRules].hasUser,wordList[i].isUser);
			++numRules;
		}
	printf("\n������ɣ��� %ld ��1-�\n",numRules);
	
	return;
}

void flush() {
	fclose( divData ); 
	fclose( wordWeight );
	fclose( output );
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

void outputRule(struct RuleList rule) {
	long i;
	/*
	for(i=0;i<rule.length;i++) {
		fprintf(output, "%s ",wordList[rule.words[i]].data);
	}
	fprintf(output, "%lf\n",(double)rule.support/numInstances);
	*/
	double sup = (double)rule.support/numInstances;
	double r1=sup/wordList[rule.words[0]].freq;
	double r2=sup/wordList[rule.words[1]].freq;
	double conf = (r1>r2?r1:r2)*numInstances; 
	fprintf(output, "%s %s %lf\n",wordList[rule.words[0]].data, wordList[rule.words[1]].data,conf);
//	fprintf(output, "<edge source=\"%s\" target=\"%s\" id=\"%s\"/>\n",wordList[rule.words[0]].data, wordList[rule.words[1]].data,rule.hasUser);
	output_count++;
}

void Aprori() {	//	���ɷ�� 
	long now;
	long numWord;
	struct RuleList tempRule;
	double tempSupport;
	for(now=0;now<numRules;now++) {
		if(ruleList[now].length<MaxRuleLength) {	//	û�ﵽ�������� 
			numWord=ruleList[now].words[ruleList[now].length - 1]+1;
		 	tempRule=ruleList[now];
			tempRule.length++;
			for( ; numWord<numWords; numWord++) 
				if(minsup1 <= (double)wordList[numWord].freq/numInstances) {
					tempRule.words[tempRule.length-1]=numWord;
					tempRule.support=getSupport(tempRule);	//	����֧�ֶ�
					
					//DEBUG
				//	outputRule(tempRule);
					 
					tempSupport=(double)tempRule.support/numInstances;	//	֧�ֶ� 
					if(minsup2<=tempSupport 
					|| ( minsup3<=tempSupport && (0==strcmp(wordList[numWord].isUser,"yes") || 0==strcmp(ruleList[now].hasUser,"yes")) )
					||  0<tempSupport && (0==strcmp(wordList[numWord].isUser,"yes") && 0==strcmp(ruleList[now].hasUser,"yes")) ) {	//	֧�ֶȼ��
				
						if(0==strcmp(wordList[numWord].isUser,"yes") || 0==strcmp(ruleList[now].hasUser,"yes"))
						{ strcpy(tempRule.hasUser,"yes"); } 
						if(0==strcmp(wordList[numWord].isUser,"yes") && 0==strcmp(ruleList[now].hasUser,"yes"))
						{ strcpy(tempRule.hasUser,"verygood"); } 
						ruleList[numRules]=tempRule;	//	�����ӽڵ� 
						++numRules;
					}
				}
		} else {	//	�ﵽ�������� 
			outputRule(ruleList[now]); 
		}
	} 
}

void RuleGen() {	// �������� 
	
}

int main() {
	init();
	Aprori(); 
	RuleGen(); 
	flush();
	printf("�����������Ϊ��%ld",output_count);
	system("pause");
	return 0;
}
