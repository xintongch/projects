import codecs
import jieba
import jieba.analyse
import sys
from wordcloud import WordCloud,STOPWORDS,ImageColorGenerator
import matplotlib.pyplot as plt
# from scipy.misc import imread
from imageio import imread
import os
from snownlp import SnowNLP
import numpy as np
import csv
import time
import json


class NovelTextProcessor:
    keywords=""
    names=[]
    stopwords=""
    wordcloud_text=[]
    background_color=''
    font_path=''
    random_state=1
    max_words=100
    chapterSeparate=[]
    text=""
    textLines=[]
    eadCharacterLines=[]
    leadCharacterSenti=[]
    def __init__(self,text,xingshi,stopwords,textlines):
        self.text=text
        self.xingshi=xingshi
        self.stopwords=stopwords
        self.background_color='white'
        self.font_path='STHeiti Light.ttc'
        self.random_state=42
        self.max_words=100
        self.textLines=textlines
        self.run()
    def run(self):
        self.keywords=jieba.analyse.extract_tags(self.text,60)
        self.character_names()
        self.wordcloud()
        self.chapters_separate()
        self.lead_c_lines()
        self.lead_c_sentiments()
    def get_keywords(self):
        return self.keywords
    def get_names(self):
        return self.names
    def get_sentiments(self):
        return self.leadCharacterSenti
    def character_names(self):
        count=0
        for keyword in self.keywords:
            for xing in self.xingshi:
                found=0
                xings=xing.strip()
                n=len(xings)
                for i in range(n):
                    if len(keyword)<n:
                        found=1
                        break
                    if xings[i]!=keyword[i]:
                        found=1
                if found==0:
                    count=count+1
                    if(count<10):
                        self.names.append(keyword)
        return self.names
    def wordcloud(self):
        # print("inside wordcloud")
        wordlist=[]
        word_generator=jieba.cut(self.text,cut_all=False)
        for word in word_generator:
            if word.strip() not in self.stopwords:
                wordlist.append(word)
        self.wordcloud_text=' '.join(wordlist)
        wc=WordCloud(background_color=self.background_color,
                        font_path=self.font_path,
                        max_words=self.max_words,
                        #mask=self.mask,
#                         max_font_size=self.max_font_size,
                        random_state=self.random_state)
        wc.generate(self.wordcloud_text)
        plt.imshow(wc)
        plt.axis('off')
        # print("create new wc.jpg")
        filename=os.getcwd()+'/wordcloud'+time.strftime("%Y%m%d-%H%M%S")+'.jpg'
        print(filename,end='')
        plt.savefig(filename)
    def chapters_separate(self):               
        # print("1. inside chapters seperate")
        chapterSeparate=[]
        chapter_tem=[]
        text=self.text
        lines=self.textLines
        for line in lines:
            # print("2. inside chapters seperate")
            line=line.strip()                   #把line前后的空格都去掉
            if len(chapter_tem)!=0:                   #如果这是temp已经有line了就把line加进去
                chapter_tem.append(line)     
            if '章 ' in line:                                #考虑到中文小说一般将“第一章”这样的title做为一行，所以章字后面会有空格 所以用章 来作为判断标准
                #chaptername.append(line)         #或者会有“第一章”+“（空格） ”+“（章的title）” 所以章后面也还是会有空格
                if len(chapter_tem)==0:              
                    chapter_tem.append(line)       #遇到的第一个章 将一行放入temp临时数组里 然后不管后面的，重新查找一下line
                    continue
                tem=[]                                     #作用于前面temp数组已经装满上一章并遇到第二个章句时，创建一个新临时数组
                tem.append(chapter_tem[-1])      #把temp里面装进去的第二个章句取出来放到tem数组了 下面再把这第一个章句在temp数组里删去
                del chapter_tem[-1]
                chapterSeparate.append(chapter_tem)      #把一整个temp数组加到chapter里，作为二维数组chapter里的某一维数组，这一维数组里包含了以章句开头的该章节的所有lines
                chapter_tem=tem                      #把tem数组赋给temp数组重新开始下一章的查找
            elif '回 ' in line:
                #chaptername.append(line)         #或者会有“第一章”+“（空格） ”+“（章的title）” 所以章后面也还是会有空格
                if len(chapter_tem)==0:              
                    chapter_tem.append(line)       #遇到的第一个章 将一行放入temp临时数组里 然后不管后面的，重新查找一下line
                    continue
                tem=[]                                     #作用于前面temp数组已经装满上一章并遇到第二个章句时，创建一个新临时数组
                tem.append(chapter_tem[-1])      #把temp里面装进去的第二个章句取出来放到tem数组了 下面再把这第一个章句在temp数组里删去
                del chapter_tem[-1]
                chapterSeparate.append(chapter_tem)      #把一整个temp数组加到chapter里，作为二维数组chapter里的某一维数组，这一维数组里包含了以章句开头的该章节的所有lines
                chapter_tem=tem  
        chapterSeparate.append(chapter_tem)
        self.chaptersSeparate=chapterSeparate
        # print("3. inside chapters seperate")
        # return chapterSeparate
    def lead_c_lines(self):   
        # print("inside lead c lines")                       #按章节找出主角所在的所有lines
        chapters=self.chaptersSeparate
        leadc_lines=[]                              
        names=self.names                 #所有角色姓名
        n=len(chapters)                     #n代表有多少章节
        #print("chapterlength=",n)
        for i in range(n):
            tem=[]
            for line in chapters[i]:
                if names[0] in line:             #names[0]就认为是主角名字了
                    tem.append(line)
            leadc_lines.append(tem)          #leadc_lines 将包含了第n章里的所有主角lines的tem数组加进去，声明自己是二维数组了
            
        self.leadCharacterLines=leadc_lines
        # return leadc_lines
    def lead_c_sentiments(self):   
        # print("inside lead c sentiments")            
        flag=0
        leadc_lines=self.leadCharacterLines
#         print("length of leadc_lines=",len(leadc_lines))     #打印出有主角在内的总章节数目
        allNames=self.names
        if(len(allNames)>1):
            leadC=allNames[0]                                            #leadc保存主角名字
            ll=len(allNames)
            leadc_sentiments=[{}for i in range(ll)]           #创建一个二维数组，每一维装一个其他角色和主角的情感数值变化
            # leadc_sentiments[0]["names"]='n'
            leadc_sentiments[0]["names"]=leadC
            # leadc_sentiments[ll+1]["names"]='m'
            # print("current ls=",leadc_sentiments)
            del allNames[0]                                               #再把主角名字删掉，留下其他角色名字在数组里 并在下面命名为others
            otherNames=allNames
            le=len(leadc_lines)
            indexes=[]
            indexes.append(0)
            # print("le=",le)
            if(le<=10):
                for i in range(0,le):
                    indexes.append(i)
            else:
                gap=le//10
                # print("gap=",gap)
                count=0
                for i in range(0,le):
                    # print("i=",i)
                    count=count+1
                    # print("count=",count)
                    if(count==gap):
                        indexes.append(i)
                        count=0
            # print("indexes=",indexes)
            for i in indexes:
                # leadc_sentiments[0].append(('第'+str(i+1)+'章',1))
                # leadc_sentiments[0]["第"+str(i+1)+"章"]=0
                leadc_sentiments[0]["第"+str(i+1)+"章"]=1
                # leadc_sentiments[ll]["第"+str(i+1)+"章"]=2
            for i in range(len(otherNames)):
                leadc_sentiments[i+1]["names"]=otherNames[i]
            temNames=[]
            for i in indexes:                           #从第一章开始循环
                if i==0:
                    temSentiFinal=[0.5 for i in range(len(otherNames))]             #对于第i章，创建一个数组，长度为其他角色数目，默认值为0.5（不开心也不生气）
                temSenti=[0 for i in range(len(otherNames))]                    #暂时的情感值是0，之后再一句一句加起来
                temTimes=[0 for i in range(len(otherNames))]
                for line in leadc_lines[i]:
                    n=0                                                               #一句话里出现其他角色和主角的数目，一开始是0
                    for name in otherNames:                                         #对于每一个line，遍历所有的其他角色名，看看有谁在里面
                        if name in line:                  
                            n+=1                                                   #出现其他角色的句子的数目加一
                            nn=otherNames.index(name)                          #获取出现的角色名字的指标，赋值给nn
                    if n==1:                                                       #如果n==1说明只有一个其他角色和主角在句子里   如果n不是1而是0或2等等，就不分析该句子
                        temNames.append(otherNames[nn])                     #把该角色放到tem名字数组里
                        temSenti[nn]+=SnowNLP(line).sentiments    #用情感分析得方法得到情感值然后赋值给暂时的情感数值数组里，该数组后面将积累数值并平均
                        temTimes[nn]+=1                                            #该角色的出现次数加一
                for k in range(len(otherNames)):                                
                    if temTimes[k]!=0:                                              #遍历其他角色的指标，如果在次数数组里该角色指标的值不为0，说明在该章节里，该角色出现了并被分析得情感值
                        temSentiFinal[k]=temSenti[k]/temTimes[k]                 #numbers数组里的默认值是0.5，但是可以重新赋值为新的情感值
                    leadc_sentiments[k+1]["第"+str(i+1)+"章"]=temSentiFinal[k]        #leadc——sentiment数组是二维数组，给对应的其他角色的一维数组里添加numbers的值，不论0.5还是新值，作为第i章的情感值，然后进行下一个循环，分析下一个章节
            self.leadCharacterSenti=leadc_sentiments
            # print("final ls=",leadc_sentiments)

    


def bottomLine(mySentiments):
    m=len(mySentiments)
    # print("m======================",m)
    mySentiments.append({})
    mySentiments[m]["names"]="n"
    keys=list(mySentiments[0].keys())
    for i in range(0,len(mySentiments[0])-1):
#         print(0,len(mySentiments[0])-1)
        # print('i=',i)
        mySentiments[m][keys[i+1]]=0





def main():
    # print("Test main function")
    # filename='xiangmi.txt'
    filename='temp.txt'
    xingshiFilename='baijiaxing.txt'
    stopwordFilename='stopwords.txt'
    stopword_text=codecs.open(stopwordFilename).read()
    textlines=codecs.open(filename).readlines()
    text=codecs.open(filename).read()
    xingshi=open(xingshiFilename).readlines()
    obj=NovelTextProcessor(text,xingshi,stopword_text,textlines)
    keywords=obj.get_keywords()
    names=obj.get_names()
    mySentiments=obj.get_sentiments()
    bottomLine(mySentiments)

    # print("check ()")
    # for i in mySentiments:
    #     print(i)
    # print("done check")

    tem=[]
    for i in range(0,int(len(mySentiments)/2)):
        tem=mySentiments[i]
        mySentiments[i]=mySentiments[len(mySentiments)-1-i]
        mySentiments[len(mySentiments)-1-i]=tem

    tem=[]
    length=len(mySentiments)
    # print("[0]")
    for i in range(length,0,-2):
        if((i-2)==0):
            break
        else:
            tem=mySentiments[i-2]
            # print('tem=',tem)
            tup={}
            nn=0
            for j in tem:
                
                # nn+=1
                if(j=='names'):
                    tup["names"]=tem[j]
                else:
                    # print("type of j[1]=",type(j[1]))
                    # print('0j=',tem[j])
                    # print('1j=',float(tem[j]))
                    # print('2j=',2-float(tem[j]))
                    tup[j]=2-float(tem[j])
                    # tup.append((j[0],2-j[1]))
            del mySentiments[i-2]
            mySentiments.append(tup)



    m=len(mySentiments)
    n=len(mySentiments[0])
    mySentiments.append({})
    mySentiments[m]["names"]="m"
    keys=list(mySentiments[0].keys())
    for i in range(0,len(mySentiments[0])-1):
            # print(0,len(mySentiments[0])-1)
            # print(i)
        mySentiments[m][keys[i+1]]=2






    # del mySentiments[0]
    # del mySentiments[1]
    # del mySentiments[2]
    # del mySentiments[3]
    # del mySentiments[4]
    print("|",end="")
    print(mySentiments,end="")
    # jsonObj=json.dumps(mySentiments)
    # with open(os.getcwd()+"/frontend/dist/frontend/temp.json","w") as f:
    #     f.write(jsonObj)

    # 写入数据
    # head=list(dict(mySentiments[0]))
    # with open(os.getcwd()+"/frontend/dist/frontend/temp.csv","w") as f:
    #     filenames=head
    #     writer=csv.DictWriter(f,fieldnames=filenames)
    #     writer.writeheader()
    #     for i in range(len(mySentiments)):
    #         writer.writerow(dict(mySentiments[i]))
    #     f.close()



    # print(keywords)
    # print("hello world with nodejs and python")
    sys.stdout.flush()

if __name__ == "__main__":
    main()


# print(names)

