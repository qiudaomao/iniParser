/*
 * Create by fuzhuo at 11/12/2013
 */
#include "ConfigINI.h"

#include <iostream>
#include <sstream>
#include <string>
#include <map>

#define CONFIGINI_DEBUG 0
#define log printf
#define mlog(msg...) do{\
if(CONFIGINI_DEBUG) printf(msg);\
}while(0)

ConfigINI::ConfigINI(const char *fileNameWithPath, bool _autoCreate):
    data(NULL),
    fStream(NULL),
    autoSave(false),
    autoCreate(_autoCreate)
{
    strcpy(iniFileName, fileNameWithPath);
    //cout<<"DEBUG: load ini file, filePath="<<fileNameWithPath<<endl;
    loadConfigFile();
}

void ConfigINI::loadConfigFile()
{
    fstream fStream;
    string p = iniFileName;
    //log("p=%s", p.c_str());

    fStream.open(p,ios::in);
    //cout<<"data of file["<<iniFileName<<"]:"<<endl;
    if(!fStream){
        if(!autoCreate){
            //cout<<"BUG: file["<<iniFileName<<"] not exist"<<endl;
            log("load config, file [%s] not exist", iniFileName);
        }else{
            //cout<<"file["<<iniFileName<<"] not found, create a new file"<<endl;
            log("inifile [%s] not found, create a new file", iniFileName);
        }
        return;
    }else{
        //cout<<"file open OK"<<endl;
    }
    char line[4096];
    char ch;
    int i=0;
    string index;
    string str;
    bool isComment=false;
    while(!fStream.eof()){
        fStream.read(&ch, 1);

        ConfigINIEntry entry;
        if(ch=='#' && i==0) isComment = true;
        if(isComment==true && (ch=='\n' || ch=='\r')) {
            isComment=false;
            line[i++]='\0';
            i=0;
            //cout<<"COMMENT:"<<line<<endl;
            entry.isComment = true;
            entry.comment = line;
            datas.push_back(entry);
        }
        if(isComment==true) {
            line[i++]=ch;
            continue;
        }
        //zfu: all up for comment
        //log("read ch[%c]", ch);
        if(ch != '\n' || ch=='\r') line[i++]=ch;
        else{
            if(i==0) continue;
            line[i]='\0';
            str = string(line);
            //cout<<"read one line{"<<str<<"}"<<endl;
            mlog("read one line {%s}", str.c_str());
            if(line[0]=='['){
                index = str;
                //log("find a index{%s}", index.c_str());
            }else{
                entry.index = index.substr(1,index.length()-2);
                int fIndex = str.find_first_of('=');
                entry.name = str.substr(0,fIndex);
                entry.value = str.substr(fIndex+1, str.length()-fIndex-1);
                datas.push_back(entry);
                //cout<<"entry:\n  index@"<<entry.index<<"\n  name@"<<entry.name<<"\n  value@"<<entry.value<<"\n----------\n"<<endl;
                mlog("entry: index@[%s]\t name@[%s]\t value@[%s]", entry.index.c_str(), entry.name.c_str(), entry.value.c_str());
            }
            i=0;
        }
    }
    if(i!=0) {
        //cout<<"lastAdd"<<endl;
        ConfigINIEntry entry;
        entry.index = str;
        int fIndex = str.find_first_of('=');
        entry.name = str.substr(0,fIndex);
        entry.value = str.substr(fIndex+1, str.length()-fIndex-1);
        datas.push_back(entry);
        //cout<<"entry:\n  index@"<<entry.index<<"\n  name@"<<entry.name<<"\n  value@"<<entry.value<<"\n----------\n"<<endl;
        //log("last add: entry:\n index@%s\t name@%s\t value@%s", entry.index.c_str(), entry.name.c_str(), entry.value.c_str());
        mlog("last add entry: index@[%s]\t name@[%s]\t value@[%s]", entry.index.c_str(), entry.name.c_str(), entry.value.c_str());
    }
    fStream.close();
    //printAll();
}

ConfigINI::~ConfigINI()
{
    if(autoSave){
        cout<<"AUTO save Config file["<<iniFileName<<"]"<<endl;
        writeConfigFile();
    }
}

void ConfigINI::writeConfigFile(const char* fileName)
{
    autoSave=false;
    if(fileName==NULL) fileName = iniFileName;
    //string p = FileUtils::getInstance()->fullPathForFilename(iniFileName);
    //if(FileUtils::getInstance()->isFileExist(p)){
        //cout<<"file exist:"<<p<<endl;
    //}
    fstream fStream;
    fStream.open(fileName, ios_base::out | ios_base::trunc);
    //cout<<"start write File"<<endl;
    mlog("start write file[%s]", fileName);
    string index = string("");
    bool withComment = false;
    bool isStart=true;
    for(vector<ConfigINIEntry>::iterator it=datas.begin(); it!= datas.end(); it++){
        ConfigINIEntry entry = *it;
        if(entry.isComment) {
            withComment=true;
            if(isStart) fStream<<entry.comment.c_str()<<endl;
            else fStream<<endl<<entry.comment.c_str()<<endl;
            isStart=false;
            
            //cout<<entry.comment<<endl;
            mlog("write comment:%s", entry.comment.c_str());
            continue;
        }
        if(strcmp(index.c_str(), entry.index.c_str()) != 0){
            index = entry.index;
            if(withComment || isStart) {
                fStream<<'['<<entry.index<<']'<<endl;
                withComment=false; isStart=false;
                //cout<<'['<<entry.index<<']'<<endl;
                mlog("write index[%s]", entry.index.c_str());
            }
            else{
                fStream<<endl<<'['<<entry.index<<']'<<endl;
                //cout<<endl<<'['<<entry.index<<']'<<endl;
                mlog("write index [%s]", entry.index.c_str());
            }
        }
        if (strlen(entry.name.c_str())==0 || strlen(entry.value.c_str())==0) {
            mlog("skip invalid entry");
            continue;
        }
        fStream<<entry.name<<'='<<entry.value<<endl;
        //cout<<entry.name<<"="<<entry.value<<endl;
        mlog("write :%s=%s", entry.name.c_str(), entry.value.c_str());
    }
    fStream<<endl;
    fStream.close();
    //cout<<"write ConfigFile["<<fileName<<"] end"<<endl;
    mlog("write configfile[%s] end", fileName);
}

void ConfigINI::setStringValueWithIndex(const char *index, const char* name, const char* value)
{
    autoSave = true;
    ConfigINIEntry entry;
    entry.index = index;
    entry.name = name;
    entry.value = value;
    if(datas.size() == 0) {/*cout<<"data is NULL, push and return"<<endl; */
        datas.push_back(entry);
        return;
    }
    //cout<<"begin find"<<endl;
    vector<ConfigINIEntry>::iterator it=datas.begin();
    //cout<<"entry size="<<datas.size()<<endl;
    bool findIndex=false;
    bool findName=false;
    vector<ConfigINIEntry>::iterator itInsertPos;
    for(it=datas.begin(); it!=datas.end(); it++){
        //cout<<"entry index: "<<it->index<<endl;
        if(findIndex==false){
            if(strcmp(it->index.c_str(), index) == 0){
                findIndex=true;
            }
        }
        if(findIndex==true){
            //cout<<"entry name: "<<it->name<<endl;
            if(strcmp(it->index.c_str(), index) != 0){
                //cout<<"only find index"<<endl;
                break;
            }else{
                itInsertPos = it;
            }
            if(strcmp(it->name.c_str(), name)==0){
                //cout<<"find Index and name!"<<endl;
                findName=true;
                itInsertPos = it;
                break;
            }
            continue;
        }
        itInsertPos=it;
    }
    if(findIndex && findName){
        //cout<<"find index and name update"<<endl;
        itInsertPos->value = string(value);
        return;
    }

    //if(findIndex) cout<<"only found index , insert after:"<<itInsertPos->name<<endl;
    //else cout<<"not found, insert end, after:"<<itInsertPos->name<<endl;
    datas.insert(++itInsertPos, 1, entry);
}

/***********getter*************/
bool ConfigINI::getBoolValue(const char* index, const char *name)
{
    const char *str = getStringValue(index, name);
    if(str == NULL) {log("notfound for [%s]-[%s]", index, name); return false;}
    if(strcmp(str,"true") == 0) return true;
    else return false;
}

int ConfigINI::getIntValue(const char *index, const char* name)
{
    const char *str = getStringValue(index, name);
    if(!str){
        return -1;
    }else{
        return atoi(str);
    }
}

float ConfigINI::getFloatValue(const char* index, const char *name)
{
    const char *str = getStringValue(index, name);
    if(str == NULL) {cout<<"notfound"<<endl; return -1.0;}
    return atof(str);
}

const char* ConfigINI::getStringValue(const char* index, const char *name)
{
    mlog("find index[%s]-name[%s]", index, name);
    for(unsigned int i=0; i<datas.size(); i++){
        if(strcmp(datas[i].index.c_str(), index) == 0){
            mlog("find index[%s]", datas[i].index.c_str());
            for(;i<datas.size();i++){
                if(strcmp(datas[i].name.c_str(), name)==0)
                    return datas[i].value.c_str();
            }
        }
    }
    cout<<"DEBUG: ["<<index<<"] of--["<<name<<"] not found"<<endl;
    return NULL;
}

/***********setter*************/
void ConfigINI::setBoolValue(const char* index, const char *name, bool value)
{
    if(value) sprintf(str, "true");
    else sprintf(str, "false");
    setStringValueWithIndex(index,name,str);
}

void ConfigINI::setIntValue(const char* index, const char *name, int value)
{
    sprintf(str, "%d", value);
    //cout<<"str:"<<str<<endl;
    setStringValueWithIndex(index,name,str);
}

void ConfigINI::setFloatValue(const char* index, const char *name, float value)
{
    sprintf(str, "%f", value);
    setStringValueWithIndex(index,name,str);
}

void ConfigINI::setStringValue(const char *index, const char* name, const char* value)
{
    setStringValueWithIndex(index,name,value);
}

/*------------------------------------ for DEBUG ---------------------------------------*/
void ConfigINI::printAll()
{
    for(vector<ConfigINIEntry>::iterator it=datas.begin(); it!= datas.end(); it++){
        ConfigINIEntry entry = *it;
        //cout<<"------------------------"<<endl;
        log("--------print All Entry of file[%s]------------", iniFileName);
        if(entry.isComment) {cout<<entry.comment<<endl;continue;}
        //cout<<"  index:"<<entry.index<<endl;
        //cout<<"  name :"<<entry.name<<endl;
        //cout<<"  value:"<<entry.value<<endl;
        log("  index:%s", entry.index.c_str());
        log("  name:%s", entry.name.c_str());
        log("  value:%s", entry.value.c_str());
    }
}

