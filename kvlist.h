#ifndef _KVLIST_H_
#define _KVLIST_H_

#include <map>
typedef std::map<string,string> kvlist;

kvlist ParseString(string &str, string d=";", string dd = "=");
string GetValue(kvlist &kv, string key);
bool IsSet(kvlist &kv, string key);
int SetValue(kvlist &kv, string key, string value);
int SetValue(kvlist &kv, const char *key, const char *value);
int DumpKv(kvlist &kv);

#endif
