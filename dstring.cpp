#include <iostream>
using namespace std;

#include <sstream>

#include "dstring.h"
#include "log.h"

string toString(uint32_t value)
{
    stringstream ss;
    ss << dec << value;

    return ss.str();
}
string toString(int value)
{
    stringstream ss;
    ss << dec << value;

    return ss.str();
}
string toString(time_t value)
{
    stringstream ss;
    ss << dec << value;

    return ss.str();
}

string toString(char *value)
{
    stringstream ss;
    ss << value;

    return ss.str();
}

string toString(uint8_t value)
{
    stringstream ss;
    ss << (uint32_t)value;

    return ss.str();
}

string toString(uint16_t value)
{
    stringstream ss;
    ss << (uint32_t)value;

    return ss.str();
}

uint32_t toInt(string value)
{
    stringstream ss;
    uint32_t ret = 0;

    ss << value;
    ss >> ret;

    return ret;
}

uint64_t toInt64(string value)
{
    stringstream ss;
    uint64_t ret= 0;

    ss << value;
    ss >> ret;
    return ret;
}

vector<string> split(string &str, string d)
{
    vector<string> v;
    string::size_type pos = 0;
    string::size_type lpos = 0;

    while ((pos = str.find(d, lpos)) != string::npos)
    {
        v.push_back(str.substr(lpos, pos-lpos));
        lpos = pos+d.length();
        pos += d.length();
    }

    if (lpos < str.length())
    {
        v.push_back(str.substr(lpos));
    }
    return v;
}

int format_macaddr(string &mac)
{
    for (int i=0; i<mac.length(); i++)
    {
        if ((mac[i] >= 'a') && mac[i] <= 'z')
            mac[i] -= ('a' - 'A');
        if (mac[i] == ':')
            mac[i] = '-';
    }
    return 0;
}
