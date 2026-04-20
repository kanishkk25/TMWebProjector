#include<tmwp>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include<ctime>
using namespace std;
using namespace tmwp;

void dispatchTime(Request &request,Response &response)
{
time_t t=time(0);
char *now=ctime(&t);
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>The Clock</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>");
response.write(now);
response.write("</h1>");
response.write("<br>");
response.write("<a href='now'>Referesh</a><br>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}

void getCityView(Request &request,Response &response)
{
string cityCodeString=request.get("cityCode");
int cityCode=atoi(cityCodeString.c_str());
if(cityCode==1) request.forward("ujjain.html");
else if(cityCode==2) request.forward("indore.html");
else if(cityCode==3) request.forward("dewas.html");
else
{
request.forward("errorPage");
}
}

void createErrorPage(Request &request,Response &response)
{
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Error page</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1 style='color: green'>");
response.write("Some Error Message");
response.write("</h1>");
response.write("<br>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}
int main()
{
TMWebProjector server(7070);

server.onRequest("/now",dispatchTime);
server.onRequest("/getCity",getCityView);
server.onRequest("/errorPage",createErrorPage);
server.start();
return 0;
}