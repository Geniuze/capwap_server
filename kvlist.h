#ifndef _KVLIST_H_
#define _KVLIST_H_

#include <map>
#include <vector>

typedef std::map<string,string> kvlist;

kvlist ParseString(string &str, string d=";", string dd = "=");
kvlist ParseString(const char *str, const char *d=";", const char *dd="=");
string GetValue(kvlist &kv, string key);
bool IsSet(kvlist &kv, string key);
int SetValue(kvlist &kv, string key, string value);
int SetValue(kvlist &kv, const char *key, const char *value);
int SetValue(kvlist &kv, string key, int value);
int SetValue(kvlist &kv, string key, bool value);
int SetValue(kvlist &kv, string key, const char *value);
int SetValue(kvlist &kv, vector<string> &keys, vector<string> &values);
int Add(kvlist &kv, kvlist other);
string AssembleString(kvlist &kv, string d = ";", string dd = "=");
int DumpKv(kvlist &kv);

#endif
