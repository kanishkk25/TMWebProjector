#include<tmwp>
#include<iostream>
using namespace tmwp;
using namespace std;
string Request::get(string name)
{
return string("1");
}
void Request::forward(string forwardTo)
{
this->forwardTo=forwardTo;
}