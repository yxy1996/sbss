#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <set>
#include <pthread.h>
#include <ctime>
clock_t start,end;
#define Input "/2020HuaweiCodecraft-TestData-master/3512444/test_data.txt"
//#define Input "/test_data.txt"
//#define Input "/456.txt"

#define Output "/pre_result/result.txt"
#define N 4200000

using namespace std;
vector<int> map;

vector<int> map_record[N][2];
vector<int> inverse_record[N][2]; 
int key_list2[N];
int key_list_check[3][N];

vector<vector<int > >  result_1[5];
vector<vector<int > >  result_2[5];
vector<vector<int > >  result_3[5];
vector<vector<int > >  result_4[5];

vector<vector<pair< pair<int, int>, int> > > visitA[N], visitB[N];



struct yxy{
  int start;
  int end;
};

bool comp(pair<pair<int, int>,int> x, int y){
  return x.first.first < y;
}

bool comp2(pair<pair<int,int>,int> x, pair<pair<int,int>,int> y){
  return x.first.second < y.first.second;
}
void DeepSearch1(int p_start, int tmp){
    visitA[p_start].resize(3);
//     sort(tmp.begin(),tmp.end());
        
    for(int i=0;i<map_record[p_start][0].size();i++){
      int index1 = map_record[p_start][0][i];
      
      if (index1<=tmp )  continue;
      
      visitA[p_start][0].push_back(make_pair(make_pair(index1,0), map_record[p_start][1][i])); 
      
       for(int j=0;j<map_record[index1][0].size();j++){
	 int index2 = map_record[index1][0][j];
	 if(index2<=tmp || index2==p_start)  continue;
	 visitA[p_start][1].push_back(make_pair(make_pair(index2,index1), map_record[index1][1][j]));
	 
	   for(int k=0;k<map_record[index2][0].size();k++){
	     if(map_record[index2][0][k] <= tmp || map_record[index2][0][k] == p_start || map_record[index2][0][k] == index1  ) continue;
	     visitA[p_start][2].push_back(make_pair(make_pair(map_record[index2][0][k],index2), map_record[index2][1][k]));   
	}
      }
    }
    sort(visitA[p_start][0].begin(),visitA[p_start][0].end());
    sort(visitA[p_start][1].begin(),visitA[p_start][1].end());
    sort(visitA[p_start][2].begin(),visitA[p_start][2].end());
        
    for (int i=0;i<visitA[p_start][1].size();i++)
		visitA[p_start][1][i].first.second = std::lower_bound(visitA[p_start][0].begin(), visitA[p_start][0].end(), visitA[p_start][1][i].first.second,
			comp) - visitA[p_start][0].begin();  
		
    for (int i=0;i<visitA[p_start][2].size();i++)
		visitA[p_start][2][i].first.second = std::lower_bound(visitA[p_start][1].begin(), visitA[p_start][1].end(), visitA[p_start][2][i].first.second,
			comp) -visitA[p_start][1].begin();

}
void DeepSearch2(int p_start){
    visitB[p_start].resize(3);
    // sort(tmp.begin(),tmp.end());
    
//     if(p_start>=tmp)  return;
    
    for(int i=0;i<inverse_record[p_start][0].size();i++){
      int index1 = inverse_record[p_start][0][i];
      
      if (index1<=p_start) continue;
      
      visitB[p_start][0].push_back(make_pair(make_pair(index1,0), inverse_record[p_start][1][i])); 
      
       for(int j=0;j<inverse_record[index1][0].size();j++){
	 int index2 = inverse_record[index1][0][j];
	 if(index2<=p_start) continue;
	 visitB[p_start][1].push_back(make_pair(make_pair(index2,index1), inverse_record[index1][1][j]));
	 
	   for(int k=0;k<inverse_record[index2][0].size();k++){
	     if(inverse_record[index2][0][k] <= p_start ||inverse_record[index2][0][k]== index1) continue;
	     visitB[p_start][2].push_back(make_pair(make_pair(inverse_record[index2][0][k],index2), inverse_record[index2][1][k]));   
	}
      }
    }
    sort(visitB[p_start][0].begin(),visitB[p_start][0].end());
    sort(visitB[p_start][1].begin(),visitB[p_start][1].end());
    sort(visitB[p_start][2].begin(),visitB[p_start][2].end());
        
    for (int i=0;i<visitB[p_start][1].size();i++)
		visitB[p_start][1][i].first.second = std::lower_bound(visitB[p_start][0].begin(), visitB[p_start][0].end(), visitB[p_start][1][i].first.second,
			comp) - visitB[p_start][0].begin();  
		
    for (int i=0;i<visitB[p_start][2].size();i++)
		visitB[p_start][2][i].first.second = std::lower_bound(visitB[p_start][1].begin(), visitB[p_start][1].end(), visitB[p_start][2][i].first.second,
			comp) -visitB[p_start][1].begin();

}

vector<vector<pair<int,int> > >  get1( int x, int cnt, int p, vector< vector<pair<pair<int, int>, int> > >* list, int xx) {
  
  vector<vector<pair<int,int> > > multi_path;  
  for (int i=p;i<list[x][cnt].size() && list[x][cnt][i].first.first == list[x][cnt][p].first.first;i++)
  {       

  if ((i!=p && list[x][cnt][i].first.second == list[x][cnt][i-1].first.second)|| list[x][cnt][i].first.first == xx) continue;
 
  
 if (cnt<=0){
    vector<pair<int,int> > path;
    path.push_back(  make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second ));    
    multi_path.push_back(path);
     continue;}

   
  int *index = &list[x][cnt][i].first.second;
  
  for (int j=*index; j<list[x][cnt-1].size() && list[x][cnt-1][j].first.first== list[x][cnt-1][*index].first.first; j++){
    if((j!=*index && list[x][cnt-1][j].first.second == list[x][cnt-1][j-1].first.second)||list[x][cnt-1][j].first.first == xx) continue;
    if(cnt<=1) {
    vector<pair<int,int> > path;
    path.push_back(make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second));
    path.push_back(make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second));
    multi_path.push_back(path);
      continue;}
    
    int *index1 = &list[x][cnt-1][j].first.second;
      for (int k=*index1;k<list[x][cnt-2].size() && list[x][cnt-2][k].first.first == list[x][cnt-2][*index1].first.first; k++ ){
	if((k!=*index1 && list[x][cnt-2][k].first.second == list[x][cnt-2][k-1].first.second)||list[x][cnt-2][k].first.first == xx ) continue;

	  vector<pair<int,int> > path;
	  path.push_back(make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second));
	  path.push_back(make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second));
	  path.push_back(make_pair(list[x][cnt-2][k].first.first,list[x][cnt-2][k].second));
	  multi_path.push_back(path);
      } //for k 
  }//for j
  }//for i
  
  return multi_path;
	
}
vector<vector<pair<int,int> > >  get0( int x, int cnt, int p, vector< vector<pair<pair<int, int>, int> > >* list, int xx) {
  
  vector<vector<pair<int,int> > > multi_path;  
  for (int i=p;i<list[x][cnt].size() && list[x][cnt][i].first.first == list[x][cnt][p].first.first;i++)
  {    
  if ((i!=p && list[x][cnt][i].first.second == list[x][cnt][i-1].first.second) || list[x][cnt][i].first.first <= xx ) continue;
 

    if (cnt<=0){
    vector<pair<int,int> > path;
    path.push_back(  make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second ));    
    multi_path.push_back(path);
    continue;}
   
  int *index = &list[x][cnt][i].first.second;
  
  for (int j=*index; j<list[x][cnt-1].size() && list[x][cnt-1][j].first.first== list[x][cnt-1][*index].first.first; j++){
    if((j!=*index && list[x][cnt-1][j].first.second == list[x][cnt-1][j-1].first.second)||list[x][cnt-1][j].first.first<= xx) continue;
        if(cnt<=1) {
    vector<pair<int,int> > path;
    path.push_back(make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second));
    path.push_back(make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second));
    multi_path.push_back(path);
         continue;}
    
    int *index1 = &list[x][cnt-1][j].first.second;
      for (int k=*index1;k<list[x][cnt-2].size() && list[x][cnt-2][k].first.first == list[x][cnt-2][*index1].first.first; k++ ){
	if((k!=*index1 && list[x][cnt-2][k].first.second == list[x][cnt-2][k-1].first.second)||list[x][cnt-2][k].first.first<=xx) continue;
	  
	  vector<pair<int,int> > path;
	  path.push_back(make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second));
	  path.push_back(make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second));
	  path.push_back(make_pair(list[x][cnt-2][k].first.first,list[x][cnt-2][k].second));
	  multi_path.push_back(path);

      } //for k 
  }//for j
  }//for i
  
  return multi_path;
	
}

void check1(int x, int y,  int lx,int ly, int money) {
	int p1=0, p2=0;

	while(p1<visitB[x][lx].size() && p2 < visitA[y][ly].size()){
	  int f1 = visitB[x][lx][p1].first.first, f2 = visitA[y][ly][p2].first.first;
	   
	  if( !(f1>x && f2>x)){
	      if (f1<=f2) p1++;
 	      if (f1>=f2) p2++;
	      continue;
	  }
	  
	  if (f1==f2) {

	    vector<vector<pair<int,int> > >   routB = get1(x,lx,p1,visitB,y);
	    vector<vector<pair<int,int> > >   routA = get0(y,ly,p2,visitA,x);
   
	    for(int m=0;m<routB.size();m++){
	      for(int n=0;n<routA.size();n++){
		vector<pair<int,int> >   path;	  
	        bool least=1;
		path.push_back(make_pair(x, 0)); 
		path.push_back(make_pair(y, money));  
		for (int i=routA[n].size()-1;i>0;i--)    path.push_back(routA[n][i]);
		for (int i=0;i<routB[m].size();i++)      path.push_back(routB[m][i]);
		path[0].second = routB[m][routB[m].size()-1].second;
  		path[1+routA[n].size()].second = routA[n][0].second;
		
		for (int i=0;i<routB[m].size()-1;i++)   path[2+i+routA[n].size()].second = routB[m][i].second;
		
		int ss = path.size();
		for (int i=1;i<ss;i++)  {
// 		  if (path[i].first<path[0].first) least=0; 		
		   for (int j=i+1;j<ss;j++){		
		      if (path[i].first==path[j].first) least=0;			
		    } // for i
		 }// for j
		 
		 if(!least) continue;
		 
		 float X,Y; 
		 for (int i=1;i<ss;i++) {
		  Y = (float)path[i].second;
		  X = (float)path[i-1].second;
		   if(   Y/X < 0.2 ||  Y/X > 3 ){
		   least = 0;
		   break;
		  }
		}
		
		if(least){
		  Y = (float)path[0].second;
		  X = (float)path[ss-1].second;
		   if(   Y/X < 0.2 ||  Y/X> 3 ) least = 0;
		 
		}
		 
		 if(least)   {
		 vector<int> path_copy;
		 for(int i=0;i<ss;i++) path_copy.push_back(map[path[i].first]);
 		 result_1[ss-3].push_back(path_copy);  
// 		 result[ss-3].push_back(path);
		}
	      }// for n
	    }// for m
	    
	    while(p1<visitB[x][lx].size() && visitB[x][lx][p1].first.first == f1 ) p1++;
	    while(p2<visitA[y][ly].size() && visitA[y][ly][p2].first.first == f2 ) p2++;

	  }
	  if (f1<f2) p1++;
 	  if (f1>f2) p2++;
	  }
	}//function
void check2(int x, int y,  int lx,int ly, int money) {
	int p1=0, p2=0;

	while(p1<visitB[x][lx].size() && p2 < visitA[y][ly].size()){
	  int f1 = visitB[x][lx][p1].first.first, f2 = visitA[y][ly][p2].first.first;
	   
	  if( !(f1>x && f2>x)){
	      if (f1<=f2) p1++;
 	      if (f1>=f2) p2++;
	      continue;
	  }
	  
	  if (f1==f2) {

	    vector<vector<pair<int,int> > >   routB = get1(x,lx,p1,visitB,y);
	    vector<vector<pair<int,int> > >   routA = get0(y,ly,p2,visitA,x);
   
	    for(int m=0;m<routB.size();m++){
	      for(int n=0;n<routA.size();n++){
		vector<pair<int,int> >   path;	  
	        bool least=1;
		path.push_back(make_pair(x, 0)); 
		path.push_back(make_pair(y, money));  
		for (int i=routA[n].size()-1;i>0;i--)    path.push_back(routA[n][i]);
		for (int i=0;i<routB[m].size();i++)      path.push_back(routB[m][i]);
		path[0].second = routB[m][routB[m].size()-1].second;
  		path[1+routA[n].size()].second = routA[n][0].second;
		
		for (int i=0;i<routB[m].size()-1;i++)   path[2+i+routA[n].size()].second = routB[m][i].second;
		
		int ss = path.size();
		for (int i=1;i<ss;i++)  {
// 		  if (path[i].first<path[0].first) least=0; 		
		   for (int j=i+1;j<ss;j++){		
		      if (path[i].first==path[j].first) least=0;			
		    } // for i
		 }// for j
		 
		 if(!least) continue;
		  float X,Y; 

		 for (int i=1;i<ss;i++) {
		   Y = (float)path[i].second;
		   X = (float)path[i-1].second;
		   if(   Y/X < 0.2 ||  Y/X > 3 ){
		   least = 0;
		   break;
		  }
		}
		
		if(least){
		  Y = (float)path[0].second;
		  X = (float)path[ss-1].second;
		   if(   Y/X < 0.2 ||  Y/X > 3 ) least = 0;
		 
		}
		 
		 if(least)   {
		 vector<int> path_copy;
		 for(int i=0;i<ss;i++) path_copy.push_back(map[path[i].first]);
		 result_2[ss-3].push_back(path_copy);  
// 		 result[ss-3].push_back(path);
		}
	      }// for n
	    }// for m
	    
	    while(p1<visitB[x][lx].size() && visitB[x][lx][p1].first.first == f1 ) p1++;
	    while(p2<visitA[y][ly].size() && visitA[y][ly][p2].first.first == f2 ) p2++;

	  }
	  if (f1<f2) p1++;
 	  if (f1>f2) p2++;
	  }
	}//function
void check3(int x, int y,  int lx,int ly, int money) {
	int p1=0, p2=0;

	while(p1<visitB[x][lx].size() && p2 < visitA[y][ly].size()){
	  int f1 = visitB[x][lx][p1].first.first, f2 = visitA[y][ly][p2].first.first;
	   
	  if( !(f1>x && f2>x)){
	      if (f1<=f2) p1++;
 	      if (f1>=f2) p2++;
	      continue;
	  }
	  
	  if (f1==f2) {

	    vector<vector<pair<int,int> > >   routB = get1(x,lx,p1,visitB,y);
	    vector<vector<pair<int,int> > >   routA = get0(y,ly,p2,visitA,x);
   
	    for(int m=0;m<routB.size();m++){
	      for(int n=0;n<routA.size();n++){
		vector<pair<int,int> >   path;	  
	        bool least=1;
		path.push_back(make_pair(x, 0)); 
		path.push_back(make_pair(y, money));  
		for (int i=routA[n].size()-1;i>0;i--)    path.push_back(routA[n][i]);
		for (int i=0;i<routB[m].size();i++)      path.push_back(routB[m][i]);
		path[0].second = routB[m][routB[m].size()-1].second;
  		path[1+routA[n].size()].second = routA[n][0].second;
		
		for (int i=0;i<routB[m].size()-1;i++)   path[2+i+routA[n].size()].second = routB[m][i].second;
		
		int ss = path.size();
		for (int i=1;i<ss;i++)  {
// 		  if (path[i].first<path[0].first) least=0; 		
		   for (int j=i+1;j<ss;j++){		
		      if (path[i].first==path[j].first) least=0;			
		    } // for i
		 }// for j
		 
		 if(!least) continue;
		 float X,Y; 

		 for (int i=1;i<ss;i++) {
		  Y = (float)path[i].second;
		  X = (float)path[i-1].second;
		   if(   Y/X < 0.2 ||  Y/X > 3 ){
		   least = 0;
		   break;
		  }
		}
		
		if(least){
		  Y = (float)path[0].second;
		  X = (float)path[ss-1].second;
		   if(   Y/X < 0.2 ||  Y/X > 3 ) least = 0;
		 
		}
		 
		 if(least)   {
		 vector<int> path_copy;
		 for(int i=0;i<ss;i++) path_copy.push_back(map[path[i].first]);
		 result_3[ss-3].push_back(path_copy);  
// 		 result[ss-3].push_back(path);
		}
	      }// for n
	    }// for m
	    
	    while(p1<visitB[x][lx].size() && visitB[x][lx][p1].first.first == f1 ) p1++;
	    while(p2<visitA[y][ly].size() && visitA[y][ly][p2].first.first == f2 ) p2++;

	  }
	  if (f1<f2) p1++;
 	  if (f1>f2) p2++;
	  }
	}//function
void check4(int x, int y,  int lx,int ly, int money) {
	int p1=0, p2=0;

	while(p1<visitB[x][lx].size() && p2 < visitA[y][ly].size()){
	  int f1 = visitB[x][lx][p1].first.first, f2 = visitA[y][ly][p2].first.first;
	   
	  if( !(f1>x && f2>x)){
	      if (f1<=f2) p1++;
 	      if (f1>=f2) p2++;
	      continue;
	  }
	  
	  if (f1==f2) {

	    vector<vector<pair<int,int> > >   routB = get1(x,lx,p1,visitB,y);
	    vector<vector<pair<int,int> > >   routA = get0(y,ly,p2,visitA,x);
   
	    for(int m=0;m<routB.size();m++){
	      for(int n=0;n<routA.size();n++){
		vector<pair<int,int> >   path;	  
	        bool least=1;
		path.push_back(make_pair(x, 0)); 
		path.push_back(make_pair(y, money));  
		for (int i=routA[n].size()-1;i>0;i--)    path.push_back(routA[n][i]);
		for (int i=0;i<routB[m].size();i++)      path.push_back(routB[m][i]);
		path[0].second = routB[m][routB[m].size()-1].second;
  		path[1+routA[n].size()].second = routA[n][0].second;
		
		for (int i=0;i<routB[m].size()-1;i++)   path[2+i+routA[n].size()].second = routB[m][i].second;
		
		int ss = path.size();
		for (int i=1;i<ss;i++)  {
// 		  if (path[i].first<path[0].first) least=0; 		
		   for (int j=i+1;j<ss;j++){		
		      if (path[i].first==path[j].first) least=0;			
		    } // for i
		 }// for j
		 
		 if(!least) continue;
		 float X,Y; 
		 for (int i=1;i<ss;i++) {
		  Y = (float)path[i].second;
		  X = (float)path[i-1].second;
		   if(   Y/X < 0.2 ||  Y/X > 3 ){
		   least = 0;
		   break;
		  }
		}
		
		if(least){
		  Y = (float)path[0].second;
		  X = (float)path[ss-1].second;
		   if(   Y/X < 0.2 ||  Y/X > 3 ) least = 0;
		 
		}
		 
		 if(least)   {
		 vector<int> path_copy;
		 for(int i=0;i<ss;i++) path_copy.push_back(map[path[i].first]);
		 result_4[ss-3].push_back(path_copy);  
// 		 result[ss-3].push_back(path);
		}
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

 for(int i = L;i<*length;i++)  if(!i || key_list_check[0][i]!=key_list_check[0][i-1])   if(*std::max_element(map_record[key_list_check[0][i]][0].begin(),map_record[key_list_check[0][i]][0].end()) > key_list_check[0][i] ) DeepSearch2(key_list_check[0][i]);
 pthread_exit(0); 
 return NULL;
  
}
void* D3(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);
 for(int i = L;i<*length;i++)    if(!i || key_list2[i]!=key_list2[i-1])     if(inverse_record[key_list2[i]][0][0]<key_list2[i]) DeepSearch1(key_list2[i],inverse_record[key_list2[i]][0][0] );
 pthread_exit(0); 
 return NULL;
  
}
void* D2(void* args)
  {
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);

 for(int i = 0;i<L;i++)  if(!i || key_list_check[0][i]!=key_list_check[0][i-1]) if(*std::max_element(map_record[key_list_check[0][i]][0].begin(),map_record[key_list_check[0][i]][0].end()) > key_list_check[0][i] )   DeepSearch2(key_list_check[0][i]);
 pthread_exit(0); 
 return NULL;
  
}
void* D1(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);
 for(int i = 0;i<L;i++)    if(!i || key_list2[i]!=key_list2[i-1])   if(inverse_record[key_list2[i]][0][0]<key_list2[i]) DeepSearch1(key_list2[i],inverse_record[key_list2[i]][0][0]);
 pthread_exit(0); 
 return NULL;
}

void* DD1(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){

			check1(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i]);
			check1(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i]);
			check1(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i]);
			check1(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i]);
			check1(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i]);
		}
	 
	}
	 pthread_exit(0);  
 return NULL;
}
void* DD2(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){
			check2(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i]);
			check2(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i]);
			check2(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i]);
			check2(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i]);
			check2(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i]);
		}
	 
	}
	 pthread_exit(0);  
 return NULL;
}
void* DD3(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){
			check3(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i]);
			check3(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i]);
			check3(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i]);
			check3(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i]);
			check3(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i]);
		}
	 
	}
	 pthread_exit(0);  
 return NULL;
}
void* DD4(void* args)
{
	 struct yxy* part;
	 part = (struct yxy*) args;
	 
         for (int i=part->start;i<part->end;i++) {

		       if (key_list_check[0][i]< key_list_check[1][i]){
			check4(key_list_check[0][i], key_list_check[1][i], 0, 0, key_list_check[2][i]);
			check4(key_list_check[0][i], key_list_check[1][i], 0, 1, key_list_check[2][i]);
			check4(key_list_check[0][i], key_list_check[1][i], 0, 2, key_list_check[2][i]);
			check4(key_list_check[0][i], key_list_check[1][i], 1, 2, key_list_check[2][i]);
			check4(key_list_check[0][i], key_list_check[1][i], 2, 2, key_list_check[2][i]);
		}
	 
	}
	 pthread_exit(0);  
 return NULL;
}
int main(int argc, char **argv) {
  vector<pair<int, int> > all;
  vector<pair<pair<int,int>,int > > graph;
   vector<int> key_list1;

   freopen(Input, "r", stdin);
   freopen(Output, "w", stdout);
   int x, y, z;
   while (~scanf("%d,%d,%d", &x, &y, &z)) {
		 
		graph.push_back(make_pair(make_pair(x,y),z));
		all.push_back(make_pair(x, graph.size() * 2 - 2));		
		all.push_back(make_pair(y, graph.size() * 2 - 1));	
   }
//    
   start = clock();
   sort(all.begin(), all.end());
   
   for (int i=0; i<all.size();i++) {
	  if (!i || all[i].first != all[i - 1].first)  map.push_back(all[i].first);	  
	   int index = all[i].second / 2, left = all[i].second % 2;
	   (left? graph[index].first.second : graph[index].first.first) = map.size() - 1;
	}
   
   int F = graph.size();
   vector<int> outs;
   sort(graph.begin(),graph.end(),comp2);
   for(int i=0;i<F;i++)   if(!i || graph[i].first.second != graph[i-1].first.second) outs.push_back(graph[i].first.second);  //outs is a set of ouput nodes
   sort(graph.begin(),graph.end());
   

       int length = 0;
       int sub_length=0;
   for(int i=0;i<F;i++){
     if(!i || graph[i].first.first!= graph[i-1].first.first || (graph[i].first.first == graph[i-1].first.first && graph[i].first.second != graph[i-1].first.second ) ){
       
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
    
    pthread_t t1,t2,t3,t4;
      clock_t start_1 = clock();

    pthread_create(&t1,&attr, D1, (void*)&sub_length);
    pthread_create(&t3,&attr, D3, (void*)&sub_length);
    pthread_create(&t2,&attr, D2, (void*)&sub_length);
    pthread_create(&t4,&attr, D4, (void*)&sub_length);
    
    pthread_join(t1, NULL);
    pthread_join(t3, NULL);
    pthread_join(t2, NULL);
    pthread_join(t4, NULL);
     clock_t  start_2 = clock();
             clock_t start_3 = clock();

       int line1,line2,line3;
       struct yxy part1,part2,part3,part4;
       line1 = (int)(0.25*sub_length);  //0, 0.0546
       line2 = (int)(0.5*sub_length);   //0.13
       line3 = (int)(0.75*sub_length); //0.2496
       part1.start = 0; 		part1.end = line1;
       part2.start = line1;		part2.end = line2;
       part3.start = line2;		part3.end = line3;
       part4.start = line3;		part4.end = sub_length;


       pthread_create(&t1,&attr, DD1, (void*)&part1);

       pthread_create(&t2,&attr, DD2, (void*)&part2);

       pthread_create(&t3,&attr, DD3, (void*)&part3);
// 
       pthread_create(&t4,&attr, DD4, (void*)&part4);      

      pthread_join(t1, NULL);
      pthread_join(t2, NULL);
      pthread_join(t3, NULL);
      pthread_join(t4, NULL);
      
            clock_t start_4 = clock();

    for(int i=0;i<5;i++) sort(result_1[i].begin(),result_1[i].end());
    for(int i=0;i<5;i++) sort(result_2[i].begin(),result_2[i].end());
    for(int i=0;i<5;i++) sort(result_3[i].begin(),result_3[i].end());
    for(int i=0;i<5;i++) sort(result_4[i].begin(),result_4[i].end());

    end = clock();
   int sum=0;
   for(int i=0;i<5;i++)  sum  = sum + result_1[i].size();
   for(int i=0;i<5;i++)  sum  = sum + result_2[i].size();
   for(int i=0;i<5;i++)  sum  = sum + result_3[i].size();
   for(int i=0;i<5;i++)  sum  = sum + result_4[i].size();

   printf("%d\n",sum);
           cout<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
	   cout<<"dfs:"<<(double)(start_2-start_1)/CLOCKS_PER_SEC<<endl;
	   cout<<"check:"<<(double)(start_4-start_3)/CLOCKS_PER_SEC<<endl;
   for(int i=0;i<5;i++){
      for(int j1=0;j1<result_1[i].size();j1++){
	for(int k1=0;k1<result_1[i][j1].size();k1++){
	  if(k1==0){
	    printf("%d",result_1[i][j1][k1]);
	  }
	  else{
	    printf(",%d",result_1[i][j1][k1]);
	  }
	}
	printf("\n");
      }   //for result_1

    for(int j2=0;j2<result_2[i].size();j2++){
	for(int k2=0;k2<result_2[i][j2].size();k2++){
	  if(k2==0){
	    printf("%d",result_2[i][j2][k2]);
	  }
	  else{
	    printf(",%d",result_2[i][j2][k2]);
	  }
	}
	printf("\n");
      }//for result_2
    for(int j3=0;j3<result_3[i].size();j3++){
	for(int k3=0;k3<result_3[i][j3].size();k3++){
	  if(k3==0){
	    printf("%d",result_3[i][j3][k3]);
	  }
	  else{
	    printf(",%d",result_3[i][j3][k3]);
	  }
	}
	printf("\n");
      }//for result_3
    for(int j4=0;j4<result_4[i].size();j4++){
	for(int k4=0;k4<result_4[i][j4].size();k4++){
	  if(k4==0){
	    printf("%d",result_4[i][j4][k4]);
	  }
	  else{
	    printf(",%d",result_4[i][j4][k4]);
	  }
	}
	printf("\n");
      }//for result_4
   }
}
