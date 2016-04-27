#encoding=utf-8
#利用TF-IDF提取关键词（不去除停用词）
import jieba
import jieba.posseg
import jieba.analyse
import sys  
reload(sys)  
sys.setdefaultencoding('utf-8')   

fin = open("data.txt","r");
fout = open("wordWeight.txt","w");

stopwords={}.fromkeys([line.rstrip() for line in open('stopwords.txt')])

allText = ""
while True:
    line = fin.readline()
    if line:
        allText = allText + line
    else:
        break
    
fout.write("textrank:\n")
for word, weight in jieba.analyse.textrank(allText,50, withWeight=True):
    if word.encode('utf-8') not in stopwords:
        fout.write(word)
        fout.write('\t')
        fout.write(str(weight))
        fout.write('\n');
fout.write("TF-IDF:\n")
for word, weight in jieba.analyse.extract_tags(allText,50, withWeight=True):
    if word.encode('utf-8') not in stopwords:
        fout.write(word)
        fout.write('\t')
        fout.write(str(weight))
        fout.write('\n');
        
fin.close()
fout.close()