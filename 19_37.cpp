// #pragma GCC diagnostic error "-std=c++11"
#include <vector>
#include <cassert> 
#include <sys/stat.h> 
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <pthread.h>
//#include <ctime>
//clock_t start,endss;
// #define Input "/data/test_data.txt"
// #define Output "/projects/student/result.txt"
#define Input "/root/sbss/38252/test_data.txt"
#define Output "/root/sbss/result.txt"

#define N 4200000
#define BuffSize (67108864) 

using namespace std;
vector<string> map;

vector<int> map_record[N][2];
vector<int> inverse_record[N][2]; 
int key_list2[N];
int key_list_check[3][N];

vector<vector<pair< pair<int, int>, int> > > visitA[N], visitB[N];

struct yxy{
  int start;
  int end;
  vector<vector<int > >* result;
};


namespace out{
        char buff[BuffSize], *ptr = NULL, *pend = NULL;
        FILE *stream = NULL;
        int itemsize, maxbytes; 

        inline void init(const char *path, const char *mode="wb", const int element_size=1){
            assert(stream == NULL && (stream = fopen(path, mode)) != NULL);
            itemsize = element_size;
            maxbytes = (BuffSize / element_size) * element_size; 
            ptr = buff;
            pend = buff + maxbytes;
        }

        inline void init(const std::string &path, const char *mode="wb", const int element_size=1){
            init(path.c_str(), mode, element_size);
        }

        inline void flush(){
            fwrite(buff, itemsize, (ptr - buff) / itemsize, stream);
            ptr = buff; 
            fflush(stream);
        }

        inline void write(const char &c){
            if (ptr >= pend) flush();
            *ptr++ = c;
        }

        inline void write(const char *s){
            for(; *s; ++s) write(*s); 
        }

        inline void write(const std::string &s){
            write(s.c_str());
        }

        inline int close(){
            if (ptr > buff) flush();
            int ret = fclose(stream);
            ptr = pend = NULL;
            stream = NULL;
            return ret;
        }
    }

void DeepSearch1(int p_start, int tmp){
    visitA[p_start].resize(3);
	int index1, index2;
	float m1, m2, m3;
        
    for(int i=0;i<map_record[p_start][0].size();i++){
		index1 = map_record[p_start][0][i];
		
		if (index1<=tmp )  continue;
		m1 = (float)map_record[p_start][1][i];
		visitA[p_start][0].push_back(make_pair(make_pair(index1,0), map_record[p_start][1][i])); 

		for(int j=0;j<map_record[index1][0].size();j++){
		index2 = map_record[index1][0][j];
		m2     = (float)map_record[index1][1][j];
		if(index2<=tmp || index2==p_start || m2/m1 <0.2 || m2/m1 > 3 )  continue;
		visitA[p_start][1].push_back(make_pair(make_pair(index2,index1),  map_record[index1][1][j]));
		
		for(int k=0;k<map_record[index2][0].size();k++){
			m3 = (float)map_record[index2][1][k];
			if(map_record[index2][0][k] <= tmp || map_record[index2][0][k] == p_start || m3/m2 <0.2 || m3/m2 > 3) continue;
			visitA[p_start][2].push_back(make_pair(make_pair(map_record[index2][0][k],index2), map_record[index2][1][k]));   
		}
		}
    }
    sort(visitA[p_start][0].begin(),visitA[p_start][0].end());
    sort(visitA[p_start][1].begin(),visitA[p_start][1].end());
    sort(visitA[p_start][2].begin(),visitA[p_start][2].end());
        
    for (int i=0;i<visitA[p_start][1].size();i++)
		visitA[p_start][1][i].first.second = std::lower_bound(visitA[p_start][0].begin(), visitA[p_start][0].end(), visitA[p_start][1][i].first.second,
			[](pair<pair<int, int>,int> x, int y) { return x.first.first < y; }) - visitA[p_start][0].begin();  
		
    for (int i=0;i<visitA[p_start][2].size();i++)
		visitA[p_start][2][i].first.second = std::lower_bound(visitA[p_start][1].begin(), visitA[p_start][1].end(), visitA[p_start][2][i].first.second,
			[](pair<pair<int, int>,int> x, int y) { return x.first.first < y; }) -visitA[p_start][1].begin();

}
void DeepSearch2(int p_start){
    visitB[p_start].resize(3);
    int index1, index2;
	float m1,m2,m3;
    for(int i=0;i<inverse_record[p_start][0].size();i++){
		index1 = inverse_record[p_start][0][i];
		if (index1<=p_start) continue;
		m1 = (float)inverse_record[p_start][1][i];
		visitB[p_start][0].push_back(make_pair(make_pair(index1,0), inverse_record[p_start][1][i])); 
		
		for(int j=0;j<inverse_record[index1][0].size();j++){
			index2 = inverse_record[index1][0][j];
			m2 	   = (float)inverse_record[index1][1][j];
			if(index2<=p_start || m1/m2 < 0.2 || m1/m2 >3 ) continue;
			
			visitB[p_start][1].push_back(make_pair(make_pair(index2,index1), inverse_record[index1][1][j]));
			for(int k=0;k<inverse_record[index2][0].size();k++){
				m3 = (float)inverse_record[index2][1][k];
				if( inverse_record[index2][0][k] <= p_start  || m2/m3 <0.2 || m2/m3 >3 ) continue;
				visitB[p_start][2].push_back(make_pair(make_pair(inverse_record[index2][0][k],index2), inverse_record[index2][1][k]));   
			}
		}
    }
    sort(visitB[p_start][0].begin(),visitB[p_start][0].end());
    sort(visitB[p_start][1].begin(),visitB[p_start][1].end());
    sort(visitB[p_start][2].begin(),visitB[p_start][2].end());
        
    for (int i=0;i<visitB[p_start][1].size();i++)
		visitB[p_start][1][i].first.second = std::lower_bound(visitB[p_start][0].begin(), visitB[p_start][0].end(), visitB[p_start][1][i].first.second,
			[](pair<pair<int, int>,int> x, int y) { return x.first.first < y; }) - visitB[p_start][0].begin();  
		
    for (int i=0;i<visitB[p_start][2].size();i++)
		visitB[p_start][2][i].first.second = std::lower_bound(visitB[p_start][1].begin(), visitB[p_start][1].end(), visitB[p_start][2][i].first.second,
			[](pair<pair<int, int>,int> x, int y) { return x.first.first < y; }) -visitB[p_start][1].begin();

}

vector<vector<pair<int,int> > >  find_path1( int x, int cnt, int p, vector< vector<pair<pair<int, int>, int> > >* list, int xx) {
	
	vector<vector<pair<int,int> > > multi_path;  
	vector<pair<int,int> > path;
	path.resize(cnt+1);
	int* index;
	int* index1;

  for (int i=p;i<list[x][cnt].size() && list[x][cnt][i].first.first == list[x][cnt][p].first.first;i++)
  {       

	if ((i!=p && list[x][cnt][i].first.second == list[x][cnt][i-1].first.second) || list[x][cnt][i].first.first == xx) continue;
	
	
	if (cnt<=0){
		path[0] = make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second );    
		multi_path.push_back(path);
		continue;
	}
	index = &list[x][cnt][i].first.second;
	
	for (int j=*index; j<list[x][cnt-1].size() && list[x][cnt-1][j].first.first== list[x][cnt-1][*index].first.first; j++){
		if((j!=*index && list[x][cnt-1][j].first.second == list[x][cnt-1][j-1].first.second)||list[x][cnt-1][j].first.first == xx || list[x][cnt-1][j].first.first==list[x][cnt][i].first.first ) continue;
		if(cnt<=1) {
		path[0] = make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second);
		path[1] = make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second);
		multi_path.push_back(path);
		continue;
		}
		index1 = &list[x][cnt-1][j].first.second;
		for (int k=*index1;k<list[x][cnt-2].size() && list[x][cnt-2][k].first.first == list[x][cnt-2][*index1].first.first; k++ ){
			if((k!=*index1 && list[x][cnt-2][k].first.second == list[x][cnt-2][k-1].first.second)||list[x][cnt-2][k].first.first == xx || list[x][cnt-2][k].first.first == list[x][cnt][i].first.first || list[x][cnt-2][k].first.first == list[x][cnt-1][j].first.first ) continue;
			path[0] = make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second);
			path[1] = make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second);
			path[2] = make_pair(list[x][cnt-2][k].first.first,list[x][cnt-2][k].second);
			multi_path.push_back(path);
		} //for k 
	}//for j
  }//for i
  
  return multi_path;
	
}
vector<vector<pair<int,int> > >  find_path0( int x, int cnt, int p, vector< vector<pair<pair<int, int>, int> > >* list, int xx) {
  
  vector<vector<pair<int,int> > > multi_path;  
  vector<pair<int,int> > path;
  path.resize(cnt+1);
  int* index;
  int* index1;
  for (int i=p;i<list[x][cnt].size() && list[x][cnt][i].first.first == list[x][cnt][p].first.first;i++)
  {    
	if ((i!=p && list[x][cnt][i].first.second == list[x][cnt][i-1].first.second) || list[x][cnt][i].first.first <= xx ) continue;
	if (cnt<=0){
	path[0] = make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second);    
	multi_path.push_back(path);
	continue;
	}
	
	index = &list[x][cnt][i].first.second;
	
	for (int j=*index; j<list[x][cnt-1].size() && list[x][cnt-1][j].first.first== list[x][cnt-1][*index].first.first; j++){
		if((j!=*index && list[x][cnt-1][j].first.second == list[x][cnt-1][j-1].first.second)||list[x][cnt-1][j].first.first<= xx /*||list[x][cnt-1][j].first.first == list[x][cnt][i].first.first*/)  continue;
		if(cnt<=1) {
		path[0] = make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second);
		path[1] = make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second);
		multi_path.push_back(path);
		continue;
		}
		index1 = &list[x][cnt-1][j].first.second;
		for (int k=*index1;k<list[x][cnt-2].size() && list[x][cnt-2][k].first.first == list[x][cnt-2][*index1].first.first; k++ ){
			if((k!=*index1 && list[x][cnt-2][k].first.second == list[x][cnt-2][k-1].first.second)||list[x][cnt-2][k].first.first<=xx /*|| list[x][cnt-2][k].first.first == list[x][cnt-1][j].first.first ||list[x][cnt-2][k].first.first == list[x][cnt][i].first.first*/) continue;
			path[0] = make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second);
			path[1] = make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second);
			path[2] = make_pair(list[x][cnt-2][k].first.first,list[x][cnt-2][k].second);
			multi_path.push_back(path);
		} //for k 
	}//for j
  }//for i
  
  return multi_path;
	
}

void find_circuit(int x, int y,  int lx,int ly, int money, vector<vector<int> > * result) {
	int p1=0, p2=0, ss=lx+ly+3, f1, f2;
	float X,Y; 
	vector<vector<pair<int,int> > >   routB, routA;
	bool least;
	vector<int>  path[2];
      
	while(p1<visitB[x][lx].size() && p2 < visitA[y][ly].size()){
	f1 = visitB[x][lx][p1].first.first;
	f2 = visitA[y][ly][p2].first.first;
	
	if( !(f1>x && f2>x)){
		if (f1<=f2) p1++;
		if (f1>=f2) p2++;
		continue;
	}
	
	if (f1==f2) {
		routA.clear();
		routB.clear();
		routB = find_path1(x,lx,p1,visitB,y);
		routA = find_path0(y,ly,p2,visitA,x);

		for(int m=0;m<routB.size();m++){
			for(int n=0;n<routA.size();n++){
				least=1;

				for (int i=0;i<routB[m].size();i++)  for (int j=1;j<routA[n].size();j++)  if (routB[m][i].first==routA[n][j].first)  least=0;			

				if(!least)  continue;

				path[0].resize(ss);
				path[1].resize(ss);

				path[0][0] = x; 			 path[1][0] = routB[m][routB[m].size()-1].second;
				path[0][1] = y;             path[1][1] = money;
				for (int i=routA[n].size()-1;i>0;i--)     	{path[0][1-i+routA[n].size()] =  routA[n][i].first; 	path[1][1-i+routA[n].size()] = routA[n][i].second;}
				for (int i=0;i<routB[m].size()-1;i++)       {path[0][routA[n].size()+1+i] =  routB[m][i].first;   path[1][2+i+routA[n].size()] = routB[m][i].second;}

				path[0][routA[n].size()+routB[m].size()] = routB[m][routB[m].size()-1].first;
				path[1][1+routA[n].size()] = routA[n][0].second;
				
				if(!least) continue;
					
				for (int i=1;i<ss;i++) {
					Y = (float)path[1][i];
					X = (float)path[1][i-1];
					if(   Y/X < 0.2 ||  Y/X > 3 ){
						least = 0;
						break;
					}
				}
				
				if(least){
					Y = (float)path[1][0];
					X = (float)path[1][ss-1];
					if(   Y/X < 0.2 ||  Y/X> 3 ) least = 0;
				}
				if(least)  result[ss-3].push_back(path[0]);  
			}// for n
		}// for m
		
		while(p1<visitB[x][lx].size() && visitB[x][lx][p1].first.first == f1 ) p1++;
		while(p2<visitA[y][ly].size() && visitA[y][ly][p2].first.first == f2 ) p2++;
	}
	if (f1<f2) p1++;
	if (f1>f2) p2++;
	}
	}//function

void* D4(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);

 for(int i = L;i<*length;i++)  if((!i || key_list_check[0][i]!=key_list_check[0][i-1])&& *std::max_element(map_record[key_list_check[0][i]][0].begin(),map_record[key_list_check[0][i]][0].end()) > key_list_check[0][i] )    DeepSearch2(key_list_check[0][i]);
 pthread_exit(0); 
 return NULL;
  
}
void* D3(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);
 for(int i = L;i<*length;i++)     if((!i || key_list2[i]!=key_list2[i-1]) && inverse_record[key_list2[i]][0][0]<key_list2[i])    DeepSearch1(key_list2[i],inverse_record[key_list2[i]][0][0]);
 pthread_exit(0); 
 return NULL;
  
}
void* D2(void* args)
  {
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);

 for(int i = 0;i<L;i++)  if((!i || key_list_check[0][i]!=key_list_check[0][i-1]) && *std::max_element(map_record[key_list_check[0][i]][0].begin(),map_record[key_list_check[0][i]][0].end()) > key_list_check[0][i] )     DeepSearch2(key_list_check[0][i]);
 pthread_exit(0); 
 return NULL;
  
}
void* D1(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);
 for(int i = 0;i<L;i++)    if((!i || key_list2[i]!=key_list2[i-1]) && inverse_record[key_list2[i]][0][0]<key_list2[i])    DeepSearch1(key_list2[i],inverse_record[key_list2[i]][0][0]);
 pthread_exit(0); 
 return NULL;
}

void* DD1(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());

	 pthread_exit(0);  
 return NULL;
}
void* DD2(void* args)
{
 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());

	 pthread_exit(0);  
 return NULL;
}
void* DD3(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());

	 pthread_exit(0);  
 return NULL;
}
void* DD4(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());
	 pthread_exit(0);  
 return NULL;
}

void* DD5(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());
	 pthread_exit(0);  
 return NULL;
}
/*
void* DD6(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());
	 pthread_exit(0);  
 return NULL;
}
void* DD7(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());
	 pthread_exit(0);  
 return NULL;
}
void* DD8(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
	 vector<vector<int > >* result = part->result;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i], result);
			find_circuit(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i], result);
		}
	 
	} 
      sort(result[0].begin(),result[0].end());
      sort(result[1].begin(),result[1].end());
      sort(result[2].begin(),result[2].end());
      sort(result[3].begin(),result[3].end());
      sort(result[4].begin(),result[4].end());
	 pthread_exit(0);  
 return NULL;
}*/
int main(int argc, char **argv) {
  	vector<pair<int, pair<int, string> > > all;
  	vector<pair<pair<int,int>,int > > graph;
	vector<int> key_list1;
  	vector<vector<int > >  result_1[5],result_2[5],result_3[5],result_4[5],result_5[5]/*,result_6[5],result_7[5],result_8[5]*/;

   	freopen(Input, "r", stdin);
	//    	freopen(Output, "w", stdout);
		int x, y, z;
		char *cx = (char*)malloc(32);
		char *cy = (char*)malloc(32);
		string kx, ky;
		while (~scanf(" %[^,],%[^,],%d", cx, cy, &z)) {
			kx = cx;
			ky = cy;
			x = atoi(cx);
			y = atoi(cy);
			graph.push_back(make_pair(make_pair(0,0),z));
			all.push_back(make_pair(x, make_pair(graph.size() * 2 - 2, kx)));		
			all.push_back(make_pair(y, make_pair(graph.size() * 2 - 1, ky)));	
		}
	//    	for(auto i:all) cout<<i.second.second<<",";
			// start = clock();
			sort(all.begin(), all.end());
			for (int i=0; i<all.size();i++) {
				if (!i || all[i].first != all[i - 1].first)  map.push_back(all[i].second.second);	  
				int index = all[i].second.first / 2, left = all[i].second.first % 2;
				(left? graph[index].first.second : graph[index].first.first) = map.size() - 1;
			}
		
			int F = graph.size();
			vector<int> outs;
			sort(graph.begin(),graph.end(),[](pair<pair<int, int>,int> x, pair<pair<int, int>,int> y) { return x.first.second < y.first.second; });
			for(int i=0;i<F;i++)   if(!i || graph[i].first.second != graph[i-1].first.second) outs.push_back(graph[i].first.second);  //outs is a set of ouput nodes
			sort(graph.begin(),graph.end());
		

			int length = 0;
			int sub_length=0;
			for(int i=0;i<F;i++){
				int index1 = lower_bound(outs.begin()+sub_length,outs.end(),graph[i].first.first)-outs.begin();
				if(outs[index1]== graph[i].first.first){
					sub_length = index1;
					if(!i || graph[i].first.first!= graph[i-1].first.first) key_list1.push_back(graph[i].first.first);
					key_list_check[0][length] = graph[i].first.first;
					key_list_check[1][length] = graph[i].first.second;
					key_list_check[2][length] = graph[i].second;
					length++; 
				}
			}  

			sub_length=0;
			for(int i=0;i<length;i++){

				int index = lower_bound(key_list1.begin() ,key_list1.end(),key_list_check[1][i])-key_list1.begin();
				if (key_list1[index] == key_list_check[1][i]) {
					map_record[key_list_check[0][i]][0].push_back(key_list_check[1][i]); 
					map_record[key_list_check[0][i]][1].push_back(key_list_check[2][i]); 
				
					inverse_record[key_list_check[1][i]][0].push_back(key_list_check[0][i]);
					inverse_record[key_list_check[1][i]][1].push_back(key_list_check[2][i]);
					
					key_list_check[0][sub_length] = key_list_check[0][i];
					key_list_check[1][sub_length] = key_list_check[1][i];
					key_list_check[2][sub_length] = key_list_check[2][i];

					key_list2[sub_length]   =  key_list_check[1][i];

					sub_length++;
				}
			}
			sort(key_list2,key_list2+sub_length);

			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 
			
			pthread_t t1,t2,t3,t4, t5/*,t6,t7,t8*/;
			// clock_t start_1 = clock();

			pthread_create(&t1,&attr, D1, (void*)&sub_length);
			pthread_create(&t3,&attr, D3, (void*)&sub_length);
			pthread_create(&t2,&attr, D2, (void*)&sub_length);
			pthread_create(&t4,&attr, D4, (void*)&sub_length);
			
			pthread_join(t1, NULL);
			pthread_join(t3, NULL);
			pthread_join(t2, NULL);
			pthread_join(t4, NULL);
			//  clock_t  start_2 = clock();
			//  clock_t start_3 = clock();

			int line[4];
			struct yxy part1,part2,part3,part4,part5/*,part6,part7,part8*/;
			line[0] = (int)(0.2*sub_length);  
			line[1] = (int)(0.4*sub_length);   
			line[2] = (int)(0.6*sub_length); 
		       line[3] = (int)(0.8*sub_length); 
		//        line[4] = (int)(0.625*sub_length);   
		//        line[5] = (int)(0.75*sub_length); 
		//        line[6] = (int)(0.875*sub_length); 
		//        
			part1.start = 0; 			part1.end = line[0];		part1.result = result_1;
			part2.start = line[0];		part2.end = line[1];		part2.result = result_2;
			part3.start = line[1];		part3.end = line[2];		part3.result = result_3;
			part4.start = line[2];		part4.end = line[3];		part4.result = result_4;
		       part5.start = line[3];		part5.end = sub_length;		part5.result = result_5;
		//        part6.start = line[4];		part6.end = line[5];		part6.result = result_6;
		//        part7.start = line[5];		part7.end = line[6];		part7.result = result_7;
		//        part8.start = line[6];		part8.end = sub_length;		part8.result = result_8;


			pthread_create(&t1,&attr, DD1, (void*)&part1);

			pthread_create(&t2,&attr, DD2, (void*)&part2);

			pthread_create(&t3,&attr, DD3, (void*)&part3);
		// 
			pthread_create(&t4,&attr, DD4, (void*)&part4);     
			
		       pthread_create(&t5,&attr, DD5, (void*)&part5);  
		//        
		//        pthread_create(&t6,&attr, DD6, (void*)&part6);  
		// 
		//        pthread_create(&t7,&attr, DD7, (void*)&part7);  
		// 
		//        pthread_create(&t8,&attr, DD8, (void*)&part8);  

			pthread_join(t1, NULL);
			pthread_join(t2, NULL);
			pthread_join(t3, NULL);
			pthread_join(t4, NULL);
		      pthread_join(t5, NULL);
		//       pthread_join(t6, NULL);
		//       pthread_join(t7, NULL);
		//       pthread_join(t8, NULL); 
		//       clock_t start_4 = clock();

	int sum=0;

		// endss = clock();
	for(int i=0;i<5;i++)  sum  = sum + result_1[i].size()+ result_2[i].size()+ result_3[i].size()+ result_4[i].size()+ result_5[i].size() /* +result_6[i].size()+ result_7[i].size() + result_8[i].size() */;
	out::init(Output, "w");
	out::write(to_string(sum)+"\n");
		//   cout<<(double)(endss-start)/CLOCKS_PER_SEC<<endl;
		//start = clock();

		// length = 3
		for(int j=0;j<result_1[0].size();j++){
			out::write(map[result_1[0][j][0]]+","+map[result_1[0][j][1]]+","+map[result_1[0][j][2]]+"\n");
			// printf("%s,%s,%s\n", result_1[0][j][0],result_1[0][j][1],result_1[0][j][2]);
	// 		cout<<map[result_1[0][j][0]].c_str()<<","<<map[result_1[0][j][1]].c_str()<<","<<map[result_1[0][j][2]].c_str()<<endl;
		}   //for result_1
		for(int j=0;j<result_2[0].size();j++){
		out::write(map[result_2[0][j][0]]+","+map[result_2[0][j][1]]+","+map[result_2[0][j][2]]+"\n");
		}   //for result_2
		for(int j=0;j<result_3[0].size();j++){
		out::write(map[result_3[0][j][0]]+","+map[result_3[0][j][1]]+","+map[result_3[0][j][2]]+"\n");
		}   //for result_3
		for(int j=0;j<result_4[0].size();j++){
		out::write(map[result_4[0][j][0]]+","+map[result_4[0][j][1]]+","+map[result_4[0][j][2]]+"\n");
		}   //for result_4
		for(int j=0;j<result_5[0].size();j++){
			 out::write(map[result_5[0][j][0]]+","+map[result_5[0][j][1]]+","+map[result_5[0][j][2]]+"\n");
			}   //for result_4
		// 	for(int j=0;j<result_6[0].size();j++){
		// 	 out::write(map[result_6[0][j][0]]+","+map[result_6[0][j][1]]+","+map[result_6[0][j][2]]+"\n");
		// 	}   //for result_4
		// 	for(int j=0;j<result_7[0].size();j++){
		// 	 out::write(map[result_7[0][j][0]]+","+map[result_7[0][j][1]]+","+map[result_7[0][j][2]]+"\n");
		// 	}   //for result_4
		// 	for(int j=0;j<result_8[0].size();j++){
		// 	 out::write(map[result_8[0][j][0]]+","+map[result_8[0][j][1]]+","+map[result_8[0][j][2]]+"\n");
		// 	}   //for result_4
			
			// length = 4
			for(int j=0;j<result_1[1].size();j++){
			out::write(map[result_1[1][j][0]]+","+map[result_1[1][j][1]]+","+map[result_1[1][j][2]]+","+map[result_1[1][j][3]]+"\n");
			}   //for result_1
			for(int j=0;j<result_2[1].size();j++){
			out::write(map[result_2[1][j][0]]+","+map[result_2[1][j][1]]+","+map[result_2[1][j][2]]+","+map[result_2[1][j][3]]+"\n");
			}   //for result_2
			for(int j=0;j<result_3[1].size();j++){
			out::write(map[result_3[1][j][0]]+","+map[result_3[1][j][1]]+","+map[result_3[1][j][2]]+","+map[result_3[1][j][3]]+"\n");
			}   //for result_3
			for(int j=0;j<result_4[1].size();j++){
			out::write(map[result_4[1][j][0]]+","+map[result_4[1][j][1]]+","+map[result_4[1][j][2]]+","+map[result_4[1][j][3]]+"\n");
			}   //for result_4
			for(int j=0;j<result_5[1].size();j++){
			 out::write(map[result_5[1][j][0]]+","+map[result_5[1][j][1]]+","+map[result_5[1][j][2]]+","+map[result_5[1][j][3]]+"\n");
			}   //for result_4
		// 	for(int j=0;j<result_6[1].size();j++){
		// 	 out::write(map[result_6[1][j][0]]+","+map[result_6[1][j][1]]+","+map[result_6[1][j][2]]+","+map[result_6[1][j][3]]+"\n");
		// 	}   //for result_6
		// 	for(int j=0;j<result_7[1].size();j++){
		// 	 out::write(map[result_7[1][j][0]]+","+map[result_7[1][j][1]]+","+map[result_7[1][j][2]]+","+map[result_7[1][j][3]]+"\n");
		// 	}   //for result_7
		// 	for(int j=0;j<result_8[1].size();j++){
		// 	 out::write(map[result_8[1][j][0]]+","+map[result_8[1][j][1]]+","+map[result_8[1][j][2]]+","+map[result_8[1][j][3]]+"\n");
		// 	}   //for result_7

			// length = 5
			for(int j=0;j<result_1[2].size();j++){
			out::write(map[result_1[2][j][0]]+","+map[result_1[2][j][1]]+","+map[result_1[2][j][2]]+","+map[result_1[2][j][3]]+","+map[result_1[2][j][4]]+"\n");
			}   //for result_1
			for(int j=0;j<result_2[2].size();j++){
			out::write(map[result_2[2][j][0]]+","+map[result_2[2][j][1]]+","+map[result_2[2][j][2]]+","+map[result_2[2][j][3]]+","+map[result_2[2][j][4]]+"\n");
			}   //for result_2
			for(int j=0;j<result_3[2].size();j++){
			out::write(map[result_3[2][j][0]]+","+map[result_3[2][j][1]]+","+map[result_3[2][j][2]]+","+map[result_3[2][j][3]]+","+map[result_3[2][j][4]]+"\n");
			}   //for result_3
			for(int j=0;j<result_4[2].size();j++){
			out::write(map[result_4[2][j][0]]+","+map[result_4[2][j][1]]+","+map[result_4[2][j][2]]+","+map[result_4[2][j][3]]+","+map[result_4[2][j][4]]+"\n");
			}   //for result_4
			for(int j=0;j<result_5[2].size();j++){
			  out::write(map[result_5[2][j][0]]+","+map[result_5[2][j][1]]+","+map[result_5[2][j][2]]+","+map[result_5[2][j][3]]+","+map[result_5[2][j][4]]+"\n");
			}   //for result_4
		// 	for(int j=0;j<result_6[2].size();j++){
		// 	 out::write(map[result_6[2][j][0]]+","+map[result_6[2][j][1]]+","+map[result_6[2][j][2]]+","+map[result_6[2][j][3]]+","+map[result_6[2][j][4]]+"\n");
		// 	}   //for result_6
		// 	for(int j=0;j<result_7[2].size();j++){
		// 	  out::write(map[result_7[2][j][0]]+","+map[result_7[2][j][1]]+","+map[result_7[2][j][2]]+","+map[result_7[2][j][3]]+","+map[result_7[2][j][4]]+"\n");
		// 	}   //for result_7
		// 	for(int j=0;j<result_8[2].size();j++){
		// 	  out::write(map[result_8[2][j][0]]+","+map[result_8[2][j][1]]+","+map[result_8[2][j][2]]+","+map[result_8[2][j][3]]+","+map[result_8[2][j][4]]+"\n");
		// 	}   //for result_7

			// length = 6
			for(int j=0;j<result_1[3].size();j++){
			out::write(map[result_1[3][j][0]]+","+map[result_1[3][j][1]]+","+map[result_1[3][j][2]]+","+map[result_1[3][j][3]]+","+map[result_1[3][j][4]]+","+map[result_1[3][j][5]]+"\n");
			}   //for result_1
			for(int j=0;j<result_2[3].size();j++){
			out::write(map[result_2[3][j][0]]+","+map[result_2[3][j][1]]+","+map[result_2[3][j][2]]+","+map[result_2[3][j][3]]+","+map[result_2[3][j][4]]+","+map[result_2[3][j][5]]+"\n");
			}   //for result_2
			for(int j=0;j<result_3[3].size();j++){
			out::write(map[result_3[3][j][0]]+","+map[result_3[3][j][1]]+","+map[result_3[3][j][2]]+","+map[result_3[3][j][3]]+","+map[result_3[3][j][4]]+","+map[result_3[3][j][5]]+"\n");
			}   //for result_3
			for(int j=0;j<result_4[3].size();j++){
			out::write(map[result_4[3][j][0]]+","+map[result_4[3][j][1]]+","+map[result_4[3][j][2]]+","+map[result_4[3][j][3]]+","+map[result_4[3][j][4]]+","+map[result_4[3][j][5]]+"\n");
			}   //for result_4
			for(int j=0;j<result_5[3].size();j++){
			  out::write(map[result_5[3][j][0]]+","+map[result_5[3][j][1]]+","+map[result_5[3][j][2]]+","+map[result_5[3][j][3]]+","+map[result_5[3][j][4]]+","+map[result_5[3][j][5]]+"\n");
			}   //for result_4
		// 	for(int j=0;j<result_6[3].size();j++){
		// 	  out::write(map[result_6[3][j][0]]+","+map[result_6[3][j][1]]+","+map[result_6[3][j][2]]+","+map[result_6[3][j][3]]+","+map[result_6[3][j][4]]+","+map[result_6[3][j][5]]+"\n");
		// 	}   //for result_6
		// 	for(int j=0;j<result_7[3].size();j++){
		// 	  out::write(map[result_7[3][j][0]]+","+map[result_7[3][j][1]]+","+map[result_7[3][j][2]]+","+map[result_7[3][j][3]]+","+map[result_7[3][j][4]]+","+map[result_7[3][j][5]]+"\n");
		// 	}   //for result_7
		// 	for(int j=0;j<result_8[3].size();j++){
		// 	  out::write(map[result_8[3][j][0]]+","+map[result_8[3][j][1]]+","+map[result_8[3][j][2]]+","+map[result_8[3][j][3]]+","+map[result_8[3][j][4]]+","+map[result_8[3][j][5]]+"\n");
		// 	}   //for result_7

			// length = 7
			for(int j=0;j<result_1[4].size();j++){
			out::write(map[result_1[4][j][0]]+","+map[result_1[4][j][1]]+","+map[result_1[4][j][2]]+","+map[result_1[4][j][3]]+","+map[result_1[4][j][4]]+","+map[result_1[4][j][5]]+","+map[result_1[4][j][6]]+"\n");
			}   //for result_1
			for(int j=0;j<result_2[4].size();j++){
			out::write(map[result_2[4][j][0]]+","+map[result_2[4][j][1]]+","+map[result_2[4][j][2]]+","+map[result_2[4][j][3]]+","+map[result_2[4][j][4]]+","+map[result_2[4][j][5]]+","+map[result_2[4][j][6]]+"\n");
			}   //for result_2
			for(int j=0;j<result_3[4].size();j++){
			out::write(map[result_3[4][j][0]]+","+map[result_3[4][j][1]]+","+map[result_3[4][j][2]]+","+map[result_3[4][j][3]]+","+map[result_3[4][j][4]]+","+map[result_3[4][j][5]]+","+map[result_3[4][j][6]]+"\n");
			}   //for result_3
			for(int j=0;j<result_4[4].size();j++){
			out::write(map[result_4[4][j][0]]+","+map[result_4[4][j][1]]+","+map[result_4[4][j][2]]+","+map[result_4[4][j][3]]+","+map[result_4[4][j][4]]+","+map[result_4[4][j][5]]+","+map[result_4[4][j][6]]+"\n");
			}   //for result_4
			for(int j=0;j<result_5[4].size();j++){
			  out::write(map[result_5[4][j][0]]+","+map[result_5[4][j][1]]+","+map[result_5[4][j][2]]+","+map[result_5[4][j][3]]+","+map[result_5[4][j][4]]+","+map[result_5[4][j][5]]+","+map[result_5[4][j][6]]+"\n");
			}   //for result_4
		// 	for(int j=0;j<result_6[4].size();j++){
		// 	  out::write(map[result_6[4][j][0]]+","+map[result_6[4][j][1]]+","+map[result_6[4][j][2]]+","+map[result_6[4][j][3]]+","+map[result_6[4][j][4]]+","+map[result_6[4][j][5]]+","+map[result_6[4][j][6]]+"\n");
		// 	}   //for result_6
		// 	for(int j=0;j<result_7[4].size();j++){
		// 	  out::write(map[result_7[4][j][0]]+","+map[result_7[4][j][1]]+","+map[result_7[4][j][2]]+","+map[result_7[4][j][3]]+","+map[result_7[4][j][4]]+","+map[result_7[4][j][5]]+","+map[result_7[4][j][6]]+"\n");
		// 	}   //for result_7
		// 	for(int j=0;j<result_8[4].size();j++){
		// 	  out::write(map[result_8[4][j][0]]+","+map[result_8[4][j][1]]+","+map[result_8[4][j][2]]+","+map[result_8[4][j][3]]+","+map[result_8[4][j][4]]+","+map[result_8[4][j][5]]+","+map[result_8[4][j][6]]+"\n");
		// 	}   //for result_7
	out::close();

   	// endss = clock();
	// cout<<(double)(endss-start)/CLOCKS_PER_SEC<<endl;

}
