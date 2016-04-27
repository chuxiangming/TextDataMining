#encoding=utf-8
import sys
reload(sys)  
sys.setdefaultencoding('utf8')


import jieba
import jieba.posseg
import jieba.analyse

print('='*40)
print('1. �ִ�')
print('-'*40)

seg_list = jieba.cut("�����������廪��ѧ", cut_all=True)
print("Full Mode: " + "/ ".join(seg_list))  # ȫģʽ

seg_list = jieba.cut("�����������廪��ѧ", cut_all=False)
print("Default Mode: " + "/ ".join(seg_list))  # Ĭ��ģʽ

seg_list = jieba.cut("�����������׺��д���")
print(", ".join(seg_list))

seg_list = jieba.cut_for_search("С��˶ʿ��ҵ���й���ѧԺ�������������ձ�������ѧ����")  # ��������ģʽ
print(", ".join(seg_list))

print('='*40)
print('2. ����Զ���ʵ�/�����ʵ�')
print('-'*40)

print('/'.join(jieba.cut('����ŵ�post�н�����', HMM=False)))
#���/�ŵ�/post/�н�/����/��
print(jieba.suggest_freq(('��', '��'), True))
#494
print('/'.join(jieba.cut('����ŵ�post�н�����', HMM=False)))
#���/�ŵ�/post/��/��/����/��
print('/'.join(jieba.cut('��̨�С���ȷӦ�ò��ᱻ�п�', HMM=False)))
#��/̨/��/��/��ȷ/Ӧ��/����/��/�п�
print(jieba.suggest_freq('̨��', True))
#69
print('/'.join(jieba.cut('��̨�С���ȷӦ�ò��ᱻ�п�', HMM=False)))
#��/̨��/��/��ȷ/Ӧ��/����/��/�п�

print('='*40)
print('3. �ؼ�����ȡ')
print('-'*40)
print(' TF-IDF')
print('-'*40)

s = "���⣬��˾���ȫ���ӹ�˾����ŷ����ҵ���޹�˾����4.3��Ԫ��"+"���ʺ󣬼���ŷ����ҵע���ʱ���7000��Ԫ���ӵ�5��Ԫ������ŷ����ҵ��Ҫ��Ӫ��ΧΪ���ز��������ٻ����۵�ҵ��Ŀǰ�ڽ�����ŷ�ǳ�����ҵ�ۺ�����Ŀ��2013�꣬ʵ��Ӫҵ����0��Ԫ��ʵ�־�����-139.13��Ԫ��"
for x, w in jieba.analyse.extract_tags(s, withWeight=True):
    print('%s %s' % (x, w))

print('-'*40)
print(' TextRank')
print('-'*40)

for x, w in jieba.analyse.textrank(s, withWeight=True):
    print('%s %s' % (x, w))

print('='*40)
print('4. ���Ա�ע')
print('-'*40)

words = jieba.posseg.cut("�Ұ������찲��")
for word, flag in words:
    print('%s %s' % (word, flag))

print('='*40)
print('6. Tokenize: ���ش�����ԭ�ĵ���ֹλ��')
print('-'*40)
print(' Ĭ��ģʽ')
print('-'*40)

result = jieba.tokenize('���ͷ�װ��Ʒ���޹�˾')
for tk in result:
    print("word %s\t\t start: %d \t\t end:%d" % (tk[0],tk[1],tk[2]))

print('-'*40)
print(' ����ģʽ')
print('-'*40)

result = jieba.tokenize('���ͷ�װ��Ʒ���޹�˾', mode='search')
for tk in result:
    print("word %s\t\t start: %d \t\t end:%d" % (tk[0],tk[1],tk[2]))