#encoding=utf-8
#分词（去除停用词）
import jieba
import jieba.posseg
import jieba.analyse
import sys  
reload(sys)  
sys.setdefaultencoding('utf8')   

fin = open("data.txt","r");
fout = open("divData.txt","w");

stopwords={}.fromkeys([line.rstrip() for line in open('stopwords.txt')])
jieba.load_userdict("userdict.txt")
allText = ""
while True:
    line = fin.readline()
    if line:
        seg_list = jieba.cut(line)
        for seg in seg_list:
            if seg.encode('utf-8') not in stopwords:
                fout.write(seg)
                fout.write(' ')
        fout.write('\n')
    else:
        break
    
fin.close()
fout.close()