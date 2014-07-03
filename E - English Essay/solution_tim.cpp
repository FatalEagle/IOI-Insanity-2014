#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

const int MOD=1000000007;
unordered_map<string, int> id;
int nextid=1;
vector<pair<vector<pair<int, int>>, int>> adj[10003];
int dist[10003];
bool inf[10003];
vector<pair<int, pair<vector<pair<int, int>>, int>>> edges;

int get_id(string& s)
{
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
    pair<vector<pair<int, int>>, int> data;
    vector<string> v=split(s, ' ');
    bool has_symbol=false;
    data.second=0;
    unordered_map<int, int> freq;
    for(size_t i=0; i<v.size(); i++)
        if(v[i][0]=='<')
        {
            has_symbol=true;
            freq[get_id(v[i])]++;
        }
        else if(v[i][0]=='"')
            data.second+=v[i].length()-2;
        else
            data.second+=v[i].length();
    for(unordered_map<int, int>::iterator it=freq.begin(); it!=freq.end(); ++it)
        data.first.push_back(*it);
    if(!has_symbol)
        data.first.push_back(make_pair(0, 1));
    adj[idx].push_back(data);
}

int main()
{
    ios_base::sync_with_stdio(0);
    string s, t;
    memset(dist, 0x3f, sizeof dist);
    memset(inf, 1, sizeof inf);
    dist[get_id(s="<eol>")]=1;
    inf[get_id(s="<eol>")]=false;
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
    int src=get_id(s="<essay>");
    int dst=0;
    dist[dst]=0;
    inf[dst]=false;
    for(int i=1; i<nextid; i++)
        for(size_t j=0; j<adj[i].size(); j++)
            edges.push_back(make_pair(i, make_pair(adj[i][j].first, adj[i][j].second)));
#if 0
    printf("%d %d\n", nextid-1, edges.size());
    int suma=0;
    for(size_t i=0; i<edges.size(); i++)
        suma+=edges[i].second.first.size();
    printf("%d\n", suma);
    printf("ITER: %lld\n", ((long long)nextid)*suma);
#endif
    srand(0xdeadbeef);
    random_shuffle(edges.begin(), edges.end());
    for(int _=1; _<nextid; _++)
    {
        bool change=false;
        for(size_t i=0; i<edges.size(); i++)
        {
            vector<pair<int, int>>& u=edges[i].second.first;
            int v=edges[i].first;
            long long c=edges[i].second.second;
            for(size_t j=0; j<u.size(); j++)
            {
                int v=u[j].first;
                if(inf[v])
                    goto next;
                long long f=u[j].second;
                c+=f*dist[v];
            }
            c%=MOD;
            if(inf[v] || c<dist[v])
            {
                inf[v]=false;
                dist[v]=c;
                change=true;
            }
            next:;
        }
        if(!change)
            break;
    }
    if(dist[src]>=0x3f3f3f3f)
        cout<<"-1"<<endl;
    else
        cout<<dist[src]<<endl;
    return 0;
}