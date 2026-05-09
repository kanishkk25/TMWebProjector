#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
int equalExtension(char *a,char *b)
{
char e,f;
while(*a && *b)
{
e=*a;
f=*b;
if(e>=65 && e<=90) e=e+32;
if(f>=65 && f<=90) f=f+32;
if(e!=f) break;
a++;
b++;
}
if(e!=f) return e-f;
return *a - *b;
}
int main(int c,char *v[])
{
if(c<2 || c>2)
{
printf("Invalid input\n");
printf("Usage : [tpl2c file_name.tpl]\n");
return 0;
}
char *fileName=v[1];
int i=0;
while(fileName[i]!='\0')
{
if(fileName[i]=='.') break;
i++;
}
if(fileName[i]=='\0')
{
printf("Invalid file format\n");
return 0;
}
if(equalExtension(fileName+i+1,(char *)"tpl")!=0)
{
printf("Invalid file extension\n");
return 0;
}
char file_name[51];
int j;
for(j=0;j<i;j++)
{
file_name[j]=fileName[j];
}
file_name[j]='\0';
printf("File name without extension : %s\n",file_name);

FILE *f;
FILE *newFile;
f=fopen(fileName,"rb");
if(f==NULL)
{
printf("File not found\n");
return 0;
}

int file_size;
fseek(f,0,SEEK_END);
file_size=ftell(f);
fseek(f,0,SEEK_SET);
if(file_size==0)
{
printf("File is of zero length\n");
fclose(f);
return 0;
}

string leftPart="response.write(\"";
string rightPart="\");\n";
string variableLeftPart="response.write(request.getValue(\"";
string variableRightPart="\"));\n";

string newFileName=fileName;
newFileName+=".cpp";

newFile=fopen(newFileName.c_str(),"wb");

char data[1001];

strcpy(data,"#include<stdio.h>\n");
strcat(data,"#incliude<stdlib.h>\n");
strcat(data,"#incliude<tmwp>\n");
strcat(data,"#incliude<ctime>\n");
strcat(data,"#incliude<iostream>\n");
strcat(data,"using namespace std;\n");

fwrite(data,strlen(data),1,newFile);

strcpy(data,"void get");
strcat(data,file_name);
strcat(data,"_TPL(Request &request,Response &response)\n");
strcat(data,"{\n");

fwrite(data,strlen(data),1,newFile);

int flag=0;
int toRead;
char m;
int k=0;
int s=0;
char variable[21];
char buffer[24];

i=0;
while(i<file_size)
{
toRead=file_size - i;
if(toRead>=24) toRead=24;
fread(buffer,toRead,1,f);
j=0;
while(j<toRead)
{
m=buffer[j];
if(m=='$')
{
flag=1;
}
else if(flag==1 && m=='{')
{
fwrite(leftPart.c_str(),leftPart.size(),1,newFile);
data[s]='\0';
fwrite(data,s,1,newFile);
fwrite(rightPart.c_str(),rightPart.size(),1,newFile);
s=0;
flag=2;
}
else if(flag==2 && m!='}')
{
variable[k]=m;
k++;
}
else if(flag==2 && m=='}')
{
fwrite(variableLeftPart.c_str(),variableLeftPart.size(),1,newFile);
variable[k]='\0';
fwrite(variable,k,1,newFile);
fwrite(variableRightPart.c_str(),variableRightPart.size(),1,newFile);
k=0;
flag=0;
}
else
{
//flag=0;
if(m=='\n')
{
fwrite(leftPart.c_str(),leftPart.size(),1,newFile);
data[s]='\0';
fwrite(data,s,1,newFile);
fwrite(rightPart.c_str(),rightPart.size(),1,newFile);
s=0;
}
else if(m=='\r')
{
// do nothing
}
else
{
data[s]=m;
s++;
}
}
j++;
}
i=i+toRead;
}

if(s>0)
{
fwrite(leftPart.c_str(),leftPart.size(),1,newFile);
data[s]='\0';
fwrite(data,s,1,newFile);
fwrite(rightPart.c_str(),rightPart.size(),1,newFile);
}

strcpy(data,"response.close();\n");
strcat(data,"}");
fwrite(data,strlen(data),1,newFile);

fclose(f);
fclose(newFile);
return 0;
}