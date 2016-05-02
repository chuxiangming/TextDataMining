#include<stdio.h>
#include<string.h>

#define MaxNumWords 5000
#define MaxEachNumWords 150
#define MaxNumInstances 300
#define MaxRuleLength 3
#define MaxNumRules 10000

#define TRUE 1
#define FALSE 0 

FILE * divData;	//	分词结果
FILE * wordWeight;	//	提取关键词结果
FILE * output;	//	输出关联分析结果

struct WordList {	//	记录词 
	   char data[15];	//	记录单词 
	   long freq;	//	记录词语出现的频度 
} wordList[ MaxNumWords ];
long numWords;

struct RuleList {	//	记录规则集，对称指标用于分析项集，非对称指标用于分析规则 
	long length;
	long words[MaxRuleLength];
	long support;		//	支持度计数	对称 
	double interest;		//	兴趣因子	对称 
	double relative;		//	相关度		对称 
	double IS;			//	余弦测度	对称 
}ruleList[ MaxNumRules ];
long numRules;
double minsup;	//	最小支持度 
double minconf;	//	最小置信度 

struct  InstanceList {	//	记录事件集
	long numInstWords; 
	char has[ MaxNumWords ]; 	//	记录事件中是否有某个单词 
	long words[ MaxEachNumWords ];	//	事件单词列表 
}instanceList[ MaxNumInstances ];
long numInstances;

void readGate() {	//	读取阈值
	/*
	 printf("请输入最小置信度 minconf:");
	 scanf("%lf",&minconf);
	 
	 printf("请输入最小支持度 minsup:");
	 scanf("%lf",&minsup);
	 */
	 minconf = 0;
	 minsup = 0.003;
} 

void init() {	//	读取分词和关键词数据 ，读取事件 	 
	 
	 readGate();
	  
	 printf("读取数据...");
	 
	 divData = fopen("divData.txt","r");
	 wordWeight = fopen("wordWeight.txt","r");
	 output = fopen("association.txt", "w");
	 
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
		instanceList[numInstances].has[i]=1;	//	记录事件中的单词 
		n=instanceList[numInstances].numInstWords++;
		instanceList[numInstances].words[n]=i;
	 	memset(word,0,sizeof(word));
  	}
	printf("\n读取完成，共 %ld 个事件， %ld 个单词", numInstances, numWords);
	printf("\n单词载入1-项集..."); 
	for(i=0;i<numWords;i++) 
		if( minsup <= (double)wordList[i].freq/numWords ) {	//	置信度阈值检测
			ruleList[numRules].support=wordList[i].freq;
			ruleList[numRules].length=1;
			ruleList[numRules].words[0]=i;
			++numRules;
		}
	printf("\n载入完成，共 %ld 个1-项集\n",numRules);
	
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
			if(FALSE==instanceList[i].has[rule.words[j]]) {	//	如果不含当前被检规则中的词 
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
	for(i=0;i<rule.length;i++) {
		printf("%s ",wordList[rule.words[i]].data);
	}
	printf("%lf\n",(double)rule.support/numWords);
}

void Aprori() {	//	生成繁项集 
	long now;
	long numWord;
	struct RuleList tempRule;
	double tempSupport;
	for(now=0;now<numRules;now++) {
		if(ruleList[now].length<MaxRuleLength) {	//	没达到最大规则宽度 
			numWord=ruleList[now].words[ruleList[now].length - 1]+1;
		 	tempRule=ruleList[now];
			tempRule.length++;
			for( ; numWord<numWords; numWord++) 
				if(minsup <= (double)wordList[numWord].freq/numWords) {
					tempRule.words[tempRule.length-1]=numWord;
					tempRule.support=getSupport(tempRule);	//	计算支持度
					
					//DEBUG
				//	outputRule(tempRule);
					 
					tempSupport=(double)tempRule.support/numWords;	//	支持度 
					if(minsup<=tempSupport) {	//	支持度检查
						ruleList[numRules]=tempRule;	//	增加子节点 
						++numRules;
					}
				}
		} else {	//	达到最大规则宽度 
			outputRule(ruleList[now]); 
		}
	} 
}

void RuleGen() {	// 产生规则 
	
}

int main() {
	init();
	Aprori(); 
	RuleGen(); 
	flush();
	system("pause");
	return 0;
}
