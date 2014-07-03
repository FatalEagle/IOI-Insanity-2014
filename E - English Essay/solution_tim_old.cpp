#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>

using namespace std;

const int MOD=1000000007;
unordered_map<string, int> id;
vector<pair<vector<int>, int>> link[10002];
int cache[10002];

int get_id(string& s)
{
    static int nextid=1;
    if(!id.count(s))
        return id[s]=nextid++;
    return id[s];
}

void lower(string& s)
{
    for(size_t i=0; i<s.length(); i++)
        s[i]=tolower(s[i]);
}

size_t skip(string& s, size_t pos)
{
    if(s[pos]=='[' || s[pos]=='{')
    {
        pos++;
        for(int depth=1; depth>0 && pos<s.length(); pos++)
            depth+=(s[pos]=='[' || s[pos]=='{')-(s[pos]==']' || s[pos]=='}');
    }
    return pos;
}

size_t skipq(string& s, size_t pos)
{
    if(s[pos]=='"')
    {
        for(pos++; pos<s.length() && s[pos]!='"'; pos++);
        pos++;
    }
    return pos;
}

void remove_braces(string& s)
{
    size_t pos=0;
    for(size_t i=0; ; i++)
    {
        i=skip(s, i);
        if(i>=s.length())
            break;
        s[pos++]=s[i];
    }
    s.erase(pos);
}

void strip(string& s, string delim=" \n")
{
    size_t first=s.find_first_not_of(delim);
    if(first==string::npos)
    {
        s.clear();
        return;
    }
    size_t last=s.find_last_not_of(delim);
    if(first>0 || last<s.length()-1)
        s=s.substr(first, last-first+1);
}

vector<string> split(string& s, char delim)
{
    vector<string> ret;
    size_t last=0;
    for(size_t i=0; i<s.length(); i++)
    {
        i=skipq(s, i);
        if(i>=s.length())
            break;
        if(s[i]==delim)
        {
            if(last==0 && i==0)
                ret.push_back("");
            else if(last<i)
            {
                if(s[last]==delim)
                    ret.push_back(s.substr(last+1, i-last-1));
                else
                    ret.push_back(s.substr(last, i-last));
                strip(ret.back());
            }
            last=i;
        }
    }
    if(last<s.length())
    {
        if(s[last]==delim)
            ret.push_back(s.substr(last+1, s.length()-last-1));
        else
            ret.push_back(s.substr(last, s.length()-last));
        strip(ret.back());
    }
    return ret;
}

void process(int idx, string& s)
{
    pair<vector<int>, int> data;
    vector<string> v=split(s, ' ');
    bool has_symbol=false;
    data.second=0;
    for(size_t i=0; i<v.size(); i++)
        if(v[i][0]=='<')
        {
            has_symbol=true;
            data.first.push_back(get_id(v[i]));
        }
        else if(v[i][0]=='"')
            data.second+=v[i].length()-2;
        else
            data.second+=v[i].length();
    if(!has_symbol)
        data.first.push_back(0);
    link[idx].push_back(data);
}

int solve(int cur)
{
    if(cache[cur]!=-1)
        return cache[cur];
    cache[cur]=-2;
    int ret=0x3f3f3f3f;
    for(size_t i=0; i<link[cur].size(); i++)
    {
        int sum=link[cur][i].second;
        for(size_t j=0; j<link[cur][i].first.size(); j++)
        {
            int val=solve(link[cur][i].first[j]);
            if(val==-2)
                goto next;
            sum=(sum+val)%MOD;
        }
        ret=min(ret, sum);
        next:;
    }
    if(ret!=0x3f3f3f3f)
        cache[cur]=ret;
    return cache[cur];
}

int main()
{
    ios_base::sync_with_stdio(0);
    string s, t;
    memset(cache, -1, sizeof cache);
    cache[0]=0;
    cache[get_id(s="<eol>")]=1;
    s.clear();
    vector<string> v;
    int cur=-1;
    while(getline(cin, t))
    {
        strip(t);
        lower(t);
        remove_braces(t);
        size_t pos=0;
        for(; pos+2<t.length(); pos++)
        {
            pos=skipq(t, pos);
            if(pos+2>=t.length())
                break;
            if(t[pos]==':' && t[pos+1]==':' && t[pos+2]=='=')
                break;
        }
        if(pos+2<t.length())
        {
            if(!s.empty())
            {
                v=split(s, '|');
                for(size_t i=0; i<v.size(); i++)
                    process(cur, v[i]);
            }
            s=t.substr(pos+3);
            strip(s);
            t=t.substr(0, pos);
            strip(t);
            cur=get_id(t);
        }
        else if(!t.empty())
        {
            s+=' ';
            s+=t;
        }
    }
    if(!s.empty())
    {
        v=split(s, '|');
        for(size_t i=0; i<v.size(); i++)
            process(cur, v[i]);
    }
    int ret=solve(get_id(s="<essay>"));
    if(ret<0)
        cout<<"-1"<<endl;
    else
        cout<<ret<<endl;
    return 0;
}
