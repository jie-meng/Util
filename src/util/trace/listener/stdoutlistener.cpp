#include "stdoutlistener.hpp"

namespace util
{

using namespace std;

StdoutListener::StdoutListener() :
    InformationListener()
{
    //ctor
}

StdoutListener::~StdoutListener()
{
    //dtor
}

void StdoutListener::process(Information* pinfo)
{
    cout<<pinfo->getFormatedInformation()<<endl;
}

} // util
