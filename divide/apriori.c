#include<stdio.h>
#include<string.h>

#define MaxNumWords 5000
#define MaxEachNumWords 150
#define MaxNumInstances 300
#define MaxRuleLength 3
#define MaxNumRules 1000000 

FILE * divData;	//	分词结果
FILE * wordWeight;	//	提取关键词结果
FILE * output;	//	输出关联分析结果

struct WordList {	//	记录词 
	   char data[15];	//	记录单词 
	   long freq;	//	记录词语出现的频度 
} wordList[ MaxNumWords ];
long numWords;

struct RuleList {	//	记录规则集，对称用于分析项集，非对称用于分析规则 
	long length;
	long words[MaxRuleLength];
	long support;		//	支持度计数	对称 
	float confidence;	//	置信度 		非对称 
	float interest;		//	兴趣因子	对称 
	float relative;		//	相关度		对称 
	float IS;			//	余弦测度	对称 
}ruleList[ MaxNumRules ];
long numRules;
float minsup;	//	最小支持度 
float minconf;	//	最小置信度 

struct  InstanceList {	//	记录事件集
	char has[ MaxNumWords ]; 	//	记录事件中是否有某个单词 
	long words[ MaxEachNumWords ];	//	事件单词列表 
}instanceList[ MaxNumInstances ];
long numInstances;

void readGate() {	//	读取阈值

	 printf("请输入最小支持度 minconf:");
	 scanf("%f",&minconf);
	 
	 printf("请输入最小置信度 minsup:");
	 scanf("%f",&minsup);
} 

void init() {	//	读取分词和关键词数据 ，读取事件 	 
	 
	 readGate();
	  
	 printf("读取数据...");
	 
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
		instanceList[numInstances].has[i]=1;	//	记录事件中的单词 
	 	memset(word,0,sizeof(word));
  	}
	printf("\n读取完成，共 %ld 个事件， %ld 个单词", numInstances, numWords);
	printf("\n单词载入1-项集..."); 
	for(i=0;i<numWords;i++) 
		if(minsup <= (float)wordList[i].freq/numWords ) {	//	置信度阈值检测
			++numRules;
			ruleList[numRules].interest=wordList[i].freq;
			ruleList[numRules].confidence=wordList[i].freq;
		}
	printf("\n载入完成，共 %ld 个1-项集",numRules);
	
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
