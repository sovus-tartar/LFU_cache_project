// C++ program for LFU cache implementation
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
// Generic function to swap two pairs
void swap(pair< int , int >& a, pair< int , int >& b)
{
pair< int , int > temp = a;
a = b;
b = temp;
}
// Returns the index of the parent node
inline int parent( int i)
{
return (i - 1) / 2;
}
// Returns the index of the left child node
inline int left( int i)
{
return 2 * i + 1;
}
// Returns the index of the right child node
inline int right( int i)
{
return 2 * i + 2;
}
// Self made heap tp Rearranges
// the nodes in order to maintain the heap property

int rem = 0;
int ins = 0;

void heapify(vector<pair< int , int > >& v,
unordered_map< int , int >& m, int i, int n)
{
int l = left(i), r = right(i), minim;
if (l < n)
minim = ((v[i].second < v[l].second) ? i : l);
else
minim = i;
if (r < n)
minim = ((v[minim].second < v[r].second) ? minim : r);
if (minim != i) {
m[v[minim].first] = i;
m[v[i].first] = minim;
swap(v[minim], v[i]);
heapify(v, m, minim, n);
}
}
// Function to Increment the frequency
// of a node and rearranges the heap
void increment(vector<pair< int , int > >& v,
unordered_map< int , int >& m, int i, int n)
{
++v[i].second;
heapify(v, m, i, n);
}
// Function to Insert a new node in the heap
void insert(vector<pair< int , int > >& v,
unordered_map< int , int >& m, int value, int & n, FILE *fw)
{
if (n == v.size()) {
m.erase(v[0].first);
    rem++;
//fprintf(fw, "Cache block %d removed. \n", v[0].first);
//cout << "Cache block " << v[0].first << " removed. " ;
v[0] = v[--n];
heapify(v, m, 0, n);
}
v[n++] = make_pair(value, 1);
m.insert(make_pair(value, n - 1));
int i = n - 1;
// Insert a node in the heap by swapping elements
while (i && v[parent(i)].second > v[i].second) {
m[v[i].first] = parent(i);
m[v[parent(i)].first] = i;
swap(v[i], v[parent(i)]);
i = parent(i);
}
    ins++;
//fprintf(fw, "Cache block %d inserted. \n", value);
//cout << "Cache block " << value << " inserted. " ;
}
// Function to refer to the block value in the cache
void refer(vector<pair< int , int > >& cache, unordered_map< int ,
int >& indices, int value, int & cache_size, FILE *fw)
{
if (indices.find(value) == indices.end())
insert(cache, indices, value, cache_size, fw);
else
increment(cache, indices, indices[value], cache_size);
}
// Driver Code
int main()
{
    int tries, m, n, succesб num_of_test;
    printf("Enter name of test file\n");
    char namefr[100], namefw[100];
    scanf("%s", namefr);
    FILE *fr = fopen(namefr, "r");
    printf("Enter name of res file\n");
    scanf("%s", namefw);
    FILE *fw = fopen(namefw, "w");
    fscanf(fr, "%d", &num_of_test);
    while (fscanf(fr, "%d", &m) == 1)
    {
        int cache_max_size = m, cache_size = 0;
        rem = 0;
        ins = 0;
        vector<pair< int , int > > cache(cache_max_size);
        unordered_map< int , int > indices;
        fscanf(fr, "%d", &tries);
        for (int i = 0; i < tries; i++)
        {
            fscanf(fr, "%d", &n);
            refer(cache, indices, n, cache_size, fw);
        }
        succes = tries - ins;
        fprintf(fw, "%0.2f\n", ((float) succes * 100) / tries);
    }
}
