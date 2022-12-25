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
        wordlist=[]
        word_generator=jieba.cut(self.text,cut_all=False)
        for word in word_generator:
            if word.strip() not in self.stopwords:
                wordlist.append(word)
        self.wordcloud_text=' '.join(wordlist)
        wc=WordCloud(background_color=self.background_color,
                        font_path=self.font_path,
                        max_words=self.max_words,
                        random_state=self.random_state)
        wc.generate(self.wordcloud_text)
        plt.imshow(wc)
        plt.axis('off')
        filename=os.getcwd()+'/wordcloud'+time.strftime("%Y%m%d-%H%M%S")+'.jpg'
        print(filename,end='')
        plt.savefig(filename)
    def chapters_separate(self):               
        chapterSeparate=[]
        chapter_tem=[]
        text=self.text
        lines=self.textLines
        for line in lines:
            line=line.strip()                   
            if len(chapter_tem)!=0:                   
                chapter_tem.append(line)     
            if '章 ' in line:
                if len(chapter_tem)==0:              
                    chapter_tem.append(line)       
                    continue
                tem=[]                                    
                tem.append(chapter_tem[-1])      
                del chapter_tem[-1]
                chapterSeparate.append(chapter_tem)      
                chapter_tem=tem                     
            elif '回 ' in line:
                if len(chapter_tem)==0:              
                    chapter_tem.append(line)       
                    continue
                tem=[]                                    
                tem.append(chapter_tem[-1])      
                chapterSeparate.append(chapter_tem)      
                chapter_tem=tem  
            elif len(line)>3 and line[0]=='第' and line[len(line)-1]=='章':
                if len(chapter_tem)==0:              
                    chapter_tem.append(line)       
                    continue
                tem=[]                                     
                tem.append(chapter_tem[-1])      
                del chapter_tem[-1]
                chapterSeparate.append(chapter_tem)     
                chapter_tem=tem  
        chapterSeparate.append(chapter_tem)
        self.chaptersSeparate=chapterSeparate
    def lead_c_lines(self):   
        chapters=self.chaptersSeparate
        leadc_lines=[]                              
        names=self.names                
        n=len(chapters)                     
        for i in range(n):
            tem=[]
            for line in chapters[i]:
                if names[0] in line:
                    tem.append(line)
            leadc_lines.append(tem)         
            
        self.leadCharacterLines=leadc_lines
    def lead_c_sentiments(self):   
        flag=0
        leadc_lines=self.leadCharacterLines
        allNames=self.names
        if(len(allNames)>1):
            leadC=allNames[0]                                           
            ll=len(allNames)
            leadc_sentiments=[{}for i in range(ll)]          
            leadc_sentiments[0]["names"]=leadC
            del allNames[0]                                               
            otherNames=allNames
            le=len(leadc_lines)
            indexes=[]
            indexes.append(0)
            if(le<=10):
                for i in range(0,le):
                    indexes.append(i)
            else:
                gap=le//10
                count=0
                for i in range(0,le):
                    count=count+1
                    if(count==gap):
                        indexes.append(i)
                        count=0
            for i in indexes:
                leadc_sentiments[0]["第"+str(i+1)+"章"]=1
            for i in range(len(otherNames)):
                leadc_sentiments[i+1]["names"]=otherNames[i]
            temNames=[]
            for i in indexes:                           
                if i==0:
                    temSentiFinal=[0.5 for i in range(len(otherNames))]            
                temSenti=[0 for i in range(len(otherNames))]                    
                temTimes=[0 for i in range(len(otherNames))]
                for line in leadc_lines[i]:
                    n=0                                                             
                    for name in otherNames:                                        
                        if name in line:                  
                            n+=1                                                  
                            nn=otherNames.index(name)                         
                    if n==1:                                                      
                        temNames.append(otherNames[nn])                     
                        temSenti[nn]+=SnowNLP(line).sentiments   
                        temTimes[nn]+=1                                            
                for k in range(len(otherNames)):                                
                    if temTimes[k]!=0:                                             
                        temSentiFinal[k]=temSenti[k]/temTimes[k]                
                    leadc_sentiments[k+1]["第"+str(i+1)+"章"]=temSentiFinal[k]        
            self.leadCharacterSenti=leadc_sentiments
    


def bottomLine(mySentiments):
    m=len(mySentiments)
    mySentiments.append({})
    mySentiments[m]["names"]="n"
    keys=list(mySentiments[0].keys())
    for i in range(0,len(mySentiments[0])-1):
        mySentiments[m][keys[i+1]]=0





def main():
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

    tem=[]
    for i in range(0,int(len(mySentiments)/2)):
        tem=mySentiments[i]
        mySentiments[i]=mySentiments[len(mySentiments)-1-i]
        mySentiments[len(mySentiments)-1-i]=tem

    tem=[]
    length=len(mySentiments)
    for i in range(length,0,-2):
        if((i-2)==0):
            break
        else:
            tem=mySentiments[i-2]
            tup={}
            nn=0
            for j in tem:
                if(j=='names'):
                    tup["names"]=tem[j]
                else:
                    tup[j]=2-float(tem[j])
            del mySentiments[i-2]
            mySentiments.append(tup)



    m=len(mySentiments)
    n=len(mySentiments[0])
    mySentiments.append({})
    mySentiments[m]["names"]="m"
    keys=list(mySentiments[0].keys())
    for i in range(0,len(mySentiments[0])-1):
        mySentiments[m][keys[i+1]]=2


    print("|",end="")
    print(mySentiments,end="")
    sys.stdout.flush()

if __name__ == "__main__":
    main()




