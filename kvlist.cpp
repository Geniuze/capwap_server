#include <iostream>
using namespace std;

#include "kvlist.h"
#include "dstring.h"
#include "log.h"

// key=value;key1=value1;   d-->; dd-->=
kvlist ParseString(string &str, string d, string dd)
{
    kvlist kv;

    vector<string> v = split(str, d);

    for (vector<string>::iterator it = v.begin();
         it != v.end(); it++)
    {
        string::size_type pos = 0;
        if ((pos = it->find(dd)) != string::npos)
        {
            string key = it->substr(0, pos);
            string value = it->substr(pos+dd.length());
            //dlog(LOG_DEBUG, "string=%s key=%s value=%s size=%d", it->c_str(), key.c_str(), value.c_str(), kv.max_size());
            SetValue(kv, key, value);
        }
    }
    return kv;
}
kvlist ParseString(const char *str, const char *d, const char *dd)
{
    string s = str;
    string sd = d;
    string sdd = dd;

    return ParseString(s, sd, sdd);
}

string GetValue(kvlist &kv, string key)
{
    kvlist::iterator it = kv.end();
    if ((it=kv.find(key)) != kv.end())
    {
        return it->second;
    }
    return "";
}
string GetValue(kvlist &kv, const char *key)
{
    string skey = key;
    return GetValue(kv, skey);
}
bool IsSet(kvlist &kv, string key)
{
    kvlist::iterator it = kv.end();
    if ((it = kv.find(key)) != kv.end())
    {
        return true;
    }
    return false;

}
int SetValue(kvlist &kv, string key, string value)
{
    kv[key] = value;
    return 0;
}
int SetValue(kvlist &kv, string key, uint32_t value)
{
    kv[key] = toString(value);
    return 0;
}
int SetValue(kvlist &kv, string key, bool value)
{
    kv[key] = toString(value);
    return 0;
}
int SetValue(kvlist &kv, vector<string> &keys, vector<string> &values)
{
    for (size_t i=0; i<keys.size(); i++)
    {
        SetValue(kv, keys[i], values[i]);
    }
    return 0;
}

int SetValue(kvlist &kv, const char *key, const char *value)
{
    string skey = key;
    string svalue = value;
    SetValue(kv, skey, svalue);
    return 0;
}

int SetValue(kvlist &kv, string key, const char *value)
{
    string svalue = value;
    SetValue(kv, key, svalue);
    return 0;
}

int Add(kvlist &kv, kvlist other)
{
    kvlist::iterator it= other.begin();
    for (; it != other.end(); it++)
    {
        SetValue(kv, it->first, it->second);
    }
    return 0;
}

string AssembleString(kvlist &kv, string d, string dd)
{
    string ret;
    kvlist::iterator  it = kv.begin();
    for (; it != kv.end(); it++)
    {
        ret.append(it->first + "=" + it->second + ";");
    }
    return ret;
}

int DumpKv(kvlist &kv)
{
    kvlist::iterator  it = kv.begin();
    string dumpstr;
    for (; it != kv.end(); it++)
    {
        dumpstr.append(it->first + "=" + it->second + ";");
    }

    dlog(LOG_DEBUG, "DUMP KVLIST : {%s}", dumpstr.c_str());

    return 0;
}
