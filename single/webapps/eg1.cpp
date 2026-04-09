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
int main()
{
TMWebProjector server(7070);
server.onRequest("/now",dispatchTime);
server.start();
return 0;
}