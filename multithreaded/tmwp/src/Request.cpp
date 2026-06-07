#include<tmwp>
#include<iostream>
#include<string.h>
using namespace tmwp;
using namespace std;

void Request::setKeyValue(string k,string v)
{
keyValues.insert(pair<string,string>(k,v));
}
string Request::getValue(string k)
{
map<string,string>::iterator iter;
iter=keyValues.find(k);
if(iter==keyValues.end()) return string("");
return iter->second;
}
string Request::get(string name)
{
string val;
int i,e;
for(i=0;i<this->dataCount;i++)
{
for(e=0;this->data[i][e]!='\0' && this->data[i][e]!='=';e++);
if(this->data[i][e]!='=') continue;
if(strncmp(this->data[i],name.c_str(),e)==0)
{
break;
}
}
if(i==this->dataCount) val="";
else val=string(this->data[i]+(e+1));
return val;
}
void Request::forward(string forwardTo)
{
this->forwardTo=forwardTo;
}