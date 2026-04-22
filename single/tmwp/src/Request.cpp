#include<tmwp>
#include<iostream>
#include<string.h>
using namespace tmwp;
using namespace std;

string Request::setKeyValue(string k,string v)
{
// add the v against k in the keyValues map
}
string Request::getValue(string k)
{
// find k in keyValues map,
// if iterator is at end return string("")
// else return whatever is the second part against the found pair
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