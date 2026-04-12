#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;

int extensionEquals(char *left,char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a>=65 && a<=90) a=a+32;
if(b>=65 && b<=90) b=b+32;
if(a!=b) break;
left++;
right++;
}
return *left==*right;
}

char * getMIMEType(char *resource)
{
char *mimeType;
int indexOfDot;
int length=strlen(resource);
if(resource==NULL) return NULL;
if(length<4) return NULL;
for(indexOfDot=length-1;indexOfDot>0 && resource[indexOfDot]!='.';indexOfDot--);
if(indexOfDot<=0) return NULL;
if(extensionEquals(resource+indexOfDot+1,(char *)"html"))
{
mimeType=(char *)malloc(sizeof(char)*9);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"text/html");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"text/css");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"text/javascript");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/png");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"ico"))
{
mimeType=(char *)malloc(sizeof(char)*13);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}

char isClientSideTechnologyResource(char *resource)
{
int i=0;
while(resource[i]!='\0' &&  resource[i]!='.')
{
i++;
}
if(resource[i]=='\0') return 'N';
return 'Y';
}

Request * parseRequest(const char *header)
{
Request *request;
char method[11];
char resource[100001];
int i,j,si;

if(header==NULL) return NULL;

// extract the Method part
for(i=0;header[i]!='\0' && header[i]!=' ';i++)
{
method[i]=header[i];
}
method[i]='\0';
i=i+2;

int dataCount=0;
char **data=NULL;

if(strcmp("GET",method)==0)
{
// extract the resource part
for(j=0;header[i]!='\0' && header[i]!=' ';i++,j++)
{
if(header[i]=='?') break;
resource[j]=header[i];
}
resource[j]='\0';
if(header[i]=='?')
{
si=i;
while(header[i]!='\0' && header[i]!=' ')
{
if(header[i]=='&') dataCount++;
i++;
}
dataCount++;

data=(char **)malloc(sizeof(char *)*dataCount);
int *pc=(int *)malloc(sizeof(int)*dataCount);
i=si;
j=0;
while(header[i]!=' ' && header[i]!='\0')
{
if(header[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si+1;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(data[j],header+i,howManyToPick);
data[j][howManyToPick]='\0';
i=pc[j]+1;
j++;
}
} // if for ? encounter ends
} // if for GET method ends

// create Request structure and populate it's members
request=new Request;
if(request==NULL) return NULL;
request->method=(char *)malloc(sizeof(char)*(strlen(method)+1));
if(request->method==NULL)
{
free(request);
return NULL;
}
strcpy(request->method,method);

request->dataCount=dataCount;
request->data=data;

if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnologyResource='Y';
request->mimeType=NULL;
}
else
{
request->resource=(char *)malloc(sizeof(char)*(strlen(resource)+1));
if(request->resource==NULL)
{
free(request->method);
free(request);
return NULL;
}
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientSideTechnologyResource(request->resource);
request->mimeType=getMIMEType(request->resource);
}
return request;
}

// Implementation of TMServer methods

TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
this->url=NULL;
this->ptrOnRequest=NULL;
}
TMWebProjector::~TMWebProjector()
{
if(this->url) delete [] this->url;
}

void TMWebProjector::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{
if(this->url) delete [] this->url;
this->url=NULL;
this->ptrOnRequest=NULL;
if(url==NULL || ptrOnRequest==NULL) return;
this->url=new char[strlen(url)+1];
strcpy(this->url,url);
this->ptrOnRequest=ptrOnRequest;
}

void TMWebProjector::start()
{
FILE *f;
int length;
char g;
int i,rc;
char responseBuffer[1024];
char requestBuffer[8192];
int bytesExtracted;
WORD ver;
WSADATA wsaData;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int successCode;
int len;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
char message[101];
if(successCode<0)
{
sprintf(message,"Unable to bind socket to port %d",this->portNumber);
printf("%s\n",message);
WSACleanup();
return;
}
listen(serverSocketDescriptor,10);
len=sizeof(clientSocketInformation);
while(1)
{
sprintf(message,"TMServer is ready to accept request on port %d",this->portNumber);
printf("%s\n",message);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to accept client connection");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
if(bytesExtracted<0)
{
// what to do is yet to decided
}else
if(bytesExtracted==0)
{
// what to do is yet to decided
}else
{
requestBuffer[bytesExtracted]='\0';
Request *request=parseRequest(requestBuffer);

while(1) // infinite loop to enable the forwarding feature
{
if(request->isClientSideTechnologyResource=='Y')
{
if(request->resource==NULL)
{
f=fopen("index.html","rb");
if(f!=NULL) printf("Sending index.html\n");
if(f==NULL)
{
f=fopen("index.htm","rb");
if(f!=NULL) printf("Sending index.html\n");
}
if(f==NULL)
{
printf("Sending 404 page\n");
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:163\nConnection: close\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource / not found</h2></body></html>");
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
closesocket(clientSocketDescriptor);
break;
}
else
{
fseek(f,0,2); // move the internal pointer to the end of file
length=ftell(f);
fseek(f,0,0); // move the internal pointer to the start of file
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i; //done done	
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
break;
}
}
else
{
f=fopen(request->resource,"rb");
if(f!=NULL) printf("Sending %s\n",request->resource);
printf("Not able to send %s\n",request->resource);
if(f==NULL)
{
// Something needs to be done over here
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
closesocket(clientSocketDescriptor);
break;
}
else
{
fseek(f,0,2); // move the internal pointer to the end of file
length=ftell(f);
fseek(f,0,0); // move the internal pointer to the start of file
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-1;
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
break;
}
}
}
else
{
//what to do in case of server side resource, is yet to be decided
if(this->url==NULL || this->ptrOnRequest==NULL)
{
printf("Seding 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
closesocket(clientSocketDescriptor);
break;
}else
{
int ii=0;
if(this->url[0]=='/') ii=1;
if(strcmp(this->url+ii,request->resource)==0)
{
Response response(clientSocketDescriptor);
this->ptrOnRequest(*request,response);

// new code
if(request->forwardTo.length()>0)
{
free(request->resource);
request->resource=(char *)malloc((sizeof(char)*request->forwardTo.length())+1);
strcpy(request->resource,request->forwardTo.c_str());
request->isClientSideTechnologyResource=isClientSideTechnologyResource(request->resource);
request->mimeType=getMIMEType(request->resource);
continue;
}
if(request->data!=NULL)
{
for(int k=0;k<request->dataCount;k++) free(request->data[k]);
free(request->data);
}
break;
}
else
{
printf("Seding 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection:close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
closesocket(clientSocketDescriptor);
break;
}
}
}
} // the infinite loop introduced because of the forwarding feature ends here
}
} // the infinite loop related to accept method ends here
closesocket(clientSocketDescriptor);
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}