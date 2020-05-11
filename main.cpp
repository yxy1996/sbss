#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <pthread.h>
// #include <ctime>
// clock_t start,end;
#define Input "/sbss/test_data.txt"
#define Output "/sbss/result.txt"

#define N 4200000

using namespace std;

vector<pair<int,int> > map_record[N];
vector<pair<int,int> > inverse_record[N]; 
int key_list[3][N];
int key_list2[N];
int key_list_check[3][N];
vector<int> map;

vector<vector<pair< pair<int, int>, int> > > visitA[N], visitB[N];

struct yxy{
  int start;
  int end;
  vector<vector<int > >* result;
};

void DeepSearch1(int p_start, int tmp){
    visitA[p_start].resize(3);
    int index1, index2;
    
    for(int i=0;i<map_record[p_start].size();i++){
        index1 = map_record[p_start][i].first;
        
        if (index1<=tmp)  continue;
        
        visitA[p_start][0].push_back(make_pair(make_pair(index1,0), map_record[p_start][i].second)); 
        
        for(int j=0;j<map_record[index1].size();j++){
            index2 = map_record[index1][j].first;
            if(index2==p_start || index2<=tmp)  continue;
            visitA[p_start][1].push_back(make_pair(make_pair(index2,index1), map_record[index1][j].second));
            
            for(int k=0;k<map_record[index2].size();k++){
                if(map_record[index2][k].first == p_start || map_record[index2][k].first == index1 || map_record[index2][k].first <= tmp  ) continue;
                visitA[p_start][2].push_back(make_pair(make_pair(map_record[index2][k].first,index2), map_record[index2][k].second));   
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
    for(int i=0;i<inverse_record[p_start].size();i++){
        index1 = inverse_record[p_start][i].first;
        
        if (index1<p_start) continue;
        
        visitB[p_start][0].push_back(make_pair(make_pair(index1,0), inverse_record[p_start][i].second)); 
        
        for(int j=0;j<inverse_record[index1].size();j++){
            index2 = inverse_record[index1][j].first;
            if(index2<=p_start)  continue;
            visitB[p_start][1].push_back(make_pair(make_pair(index2,index1), inverse_record[index1][j].second));
            
            for(int k=0;k<inverse_record[index2].size();k++){
                if(inverse_record[index2][k].first <= p_start || inverse_record[index2][k].first== index1)  continue;
                visitB[p_start][2].push_back(make_pair(make_pair(inverse_record[index2][k].first,index2), inverse_record[index2][k].second));   
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

vector<vector<pair<int,int> > >  find_path(bool flag, int x, int lx, int p, vector< vector<pair<pair<int, int>, int> > >* list, int xx) {
  
  vector<vector<pair<int,int> > > multi_path;  
  vector<pair<int,int> > path;

  int cnt, index, index1;
  bool which_case;

  for (int i=p;i<list[x][lx].size() && list[x][lx][i].first.first == list[x][lx][p].first.first;i++)
  {  
    if (i!=p && list[x][lx][i].first.second == list[x][lx][i-1].first.second ) continue;
    
    cnt = lx;
    index = i;
    which_case = flag?(list[x][cnt][i].first.first == xx):(list[x][cnt][i].first.first <= xx);

    if (which_case)  continue;

    if (cnt<=0) {
        path.clear();
        path.push_back(  make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second ));    
        multi_path.push_back(path);
        continue;
    }
    
    index = list[x][cnt][index].first.second;
    
    for (int j=index; j<list[x][cnt-1].size() && list[x][cnt-1][j].first.first== list[x][cnt-1][index].first.first; j++){

        which_case = flag?( list[x][cnt-1][j].first.first == xx):(list[x][cnt-1][j].first.first <= xx);
        
        if((j!=index && list[x][cnt-1][j].first.second == list[x][cnt-1][j-1].first.second) || which_case) continue;

        if(cnt<=1){
        path.clear();
        path.push_back(make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second));
        path.push_back(make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second));
        multi_path.push_back(path);
        continue;
        }//if cnt==0
        
        index1 = list[x][cnt-1][j].first.second;
        for (int k=index1;k<list[x][cnt-2].size() && list[x][cnt-2][k].first.first == list[x][cnt-2][index1].first.first; k++ ){
            which_case = flag?(list[x][cnt-2][k].first.first == xx):(list[x][cnt-2][k].first.first <= xx);

            if((k!=index1 && list[x][cnt-2][k].first.second == list[x][cnt-2][k-1].first.second) || which_case) continue;
            
            if(cnt<=2){
                path.clear();
                path.push_back(make_pair(list[x][cnt][i].first.first, list[x][cnt][i].second));
                path.push_back(make_pair(list[x][cnt-1][j].first.first,list[x][cnt-1][j].second));
                path.push_back(make_pair(list[x][cnt-2][k].first.first,list[x][cnt-2][k].second));
                multi_path.push_back(path);
                continue;
            } //if	
        } //for k 
    }//for j
  }//for i
  
  return multi_path;
	
}

void find_circuit(int x, int y,  int lx,int ly, int money, vector<vector<int> > * result) {
    vector<vector<pair<int,int> > >   routB, routA;
    vector<int> path_copy;
    vector<pair<int,int> >   path;	
    int p1=0, p2=0, f1, f2, ss;
    bool least;
    float X,Y;

	while(p1<visitB[x][lx].size() && p2 < visitA[y][ly].size()){
	  f1 = visitB[x][lx][p1].first.first;
      f2 = visitA[y][ly][p2].first.first;
	   
	  if( !(f1>x && f2>x)){
	      if (f1<=f2) p1++;
 	      if (f1>=f2) p2++;
	      continue;
	  }
	  
	  if (f1==f2) {
        
        routB.clear();
        routA.clear();
	    routB = find_path(1,x,lx,p1,visitB,y);
	    routA = find_path(0,y,ly,p2,visitA,x);
   
	    for(int m=0;m<routB.size();m++){
	      for(int n=0;n<routA.size();n++){
            path.clear();	  
            least=1;
            path.push_back(make_pair(x, 0)); 
            path.push_back(make_pair(y, money));  
            for (int i=routA[n].size()-1;i>0;i--)    path.push_back(routA[n][i]);
            for (int i=0;i<routB[m].size();i++)      path.push_back(routB[m][i]);
            path[0].second = routB[m][routB[m].size()-1].second;
            path[1+routA[n].size()].second = routA[n][0].second;
		
            for (int i=0;i<routB[m].size()-1;i++)   path[2+i+routA[n].size()].second = routB[m][i].second;
            
            ss = path.size();
            for (int i=0;i<ss;i++)  {
            for (int j=i+1;j<ss;j++){		
                if (path[i].first==path[j].first) least=0;			
                } // for i
            }// for j
            
            if(!least) continue;
            
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
                if(   Y/X < 0.2 ||  Y/X > 3 )  least = 0;
            }
            
            if(least)   {
                path_copy.clear();
                for(int i=0;i<ss;i++) path_copy.push_back(map[path[i].first]);
                result[ss-3].push_back(path_copy);  
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

 for(int i = L;i<*length;i++)  if(!i || key_list[0][i]!=key_list[0][i-1])    DeepSearch2(key_list[0][i]);
 pthread_exit(0); 
 return NULL;
  
}
void* D3(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);
 for(int i = L;i<*length;i++)    if(!i || key_list2[i]!=key_list2[i-1])    DeepSearch1(key_list2[i],inverse_record[key_list2[i]][0].first);
 pthread_exit(0); 
 return NULL;
  
}
void* D2(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);

 for(int i = 0;i<L;i++)  if(!i || key_list[0][i]!=key_list[0][i-1])    DeepSearch2(key_list[0][i]);
 pthread_exit(0); 
 return NULL;
  
}
void* D1(void* args)
{
 int* length;
 length = (int*) args;
 int L = (int)0.5*(*length);
 for(int i = 0;i<L;i++)    if(!i || key_list2[i]!=key_list2[i-1])    DeepSearch1(key_list2[i],inverse_record[key_list2[i]][0].first);
 pthread_exit(0); 
 return NULL;
}

void* DD1(void* args)
{
	struct yxy* part;
	part = (struct yxy*) args;
	vector<vector<int > >* result = part->result;

    for (int i=part->start;i<part->end;i++) {
		if (key_list_check[0][i] < key_list_check[1][i]){
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
		if (key_list_check[0][i] < key_list_check[1][i]){
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
		if (key_list_check[0][i] < key_list_check[1][i]){
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
		if (key_list_check[0][i] < key_list_check[1][i]){
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
int main(int argc, char **argv) {
    vector<pair<pair<int,int>,int > > graph;
    vector<pair<int, int> > all;

    vector<int> key_list1;
    vector<vector<int > >  result_1[5];
    vector<vector<int > >  result_2[5];
    vector<vector<int > >  result_3[5];
    vector<vector<int > >  result_4[5];


   freopen(Input, "r", stdin);
   freopen(Output, "w", stdout);
   int x, y, z;
   while (~scanf("%d,%d,%d", &x, &y, &z)) {
		 
		graph.push_back(make_pair(make_pair(x,y),z));
		all.push_back(make_pair(x, graph.size() * 2 - 2));		
		all.push_back(make_pair(y, graph.size() * 2 - 1));	
   }
    //    start = clock();
   sort(all.begin(), all.end());
   
   for (int i=0; i<all.size();i++) {
	  if (!i || all[i].first != all[i - 1].first)  map.push_back(all[i].first);	  
	   int index = all[i].second / 2, left = all[i].second % 2;
	   (left? graph[index].first.second : graph[index].first.first) = map.size() - 1;
	}
   
   int F = graph.size();
   vector<int> outs;
   sort(graph.begin(),graph.end(),[](pair<pair<int, int>,int> x, pair<pair<int, int>,int> y) { return x.first.second < y.first.second; });
   for(int i=0;i<F;i++)   if(!i || graph[i].first.second != graph[i-1].first.second) outs.push_back(graph[i].first.second);  //outs is a set of ouput nodes
   sort(graph.begin(),graph.end());
   

   int length = 0;
   int index_record = 0;
   for(int i=0;i<F;i++){
     if(!i || graph[i].first.first!= graph[i-1].first.first || (graph[i].first.first == graph[i-1].first.first && graph[i].first.second != graph[i-1].first.second ) ){
       
      int index1 = lower_bound(outs.begin()+index_record,outs.end(),graph[i].first.first)-outs.begin();
     if(outs[index1]== graph[i].first.first){
       index_record = index1;
       if(!i || graph[i].first.first!= graph[i-1].first.first) key_list1.push_back(graph[i].first.first);

       key_list[0][length] = graph[i].first.first;
       key_list[1][length] = graph[i].first.second;
       key_list[2][length] = graph[i].second;
       length++;
	 
	  }
     }
    }  
       int sub_length;
       for(int i=0;i<length;i++){
        map_record[key_list[0][i]].push_back(make_pair(key_list[1][i], key_list[2][i])); 
        inverse_record[key_list[1][i]].push_back(make_pair(key_list[0][i], key_list[2][i]));
        
        int index = lower_bound(key_list1.begin() ,key_list1.end(),key_list[1][i])-key_list1.begin();
        if (key_list1[index] == key_list[1][i]) {
            key_list_check[0][sub_length] = key_list[0][i];
            key_list_check[1][sub_length] = key_list[1][i];
            key_list_check[2][sub_length] = key_list[2][i];
            key_list2[sub_length]   =  key_list[1][i];
            sub_length++;
        }
       }
    sort(key_list2,key_list2+sub_length);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 
    
    pthread_t t1,t2,t3,t4;
    //     clock_t start_1 = clock();

    pthread_create(&t1,&attr, D1, (void*)&sub_length);
    pthread_create(&t3,&attr, D3, (void*)&sub_length);
    pthread_create(&t2,&attr, D2, (void*)&length);
    pthread_create(&t4,&attr, D4, (void*)&length);
    
    pthread_join(t1, NULL);
    pthread_join(t3, NULL);
    pthread_join(t2, NULL);
    pthread_join(t4, NULL);
    //     clock_t  start_2 = clock();
    //     clock_t start_3 = clock();

    int line1,line2,line3;
    struct yxy part1,part2,part3,part4;
    line1 = (int)(0.25*sub_length);  //0, 0.0546
    line2 = (int)(0.5*sub_length);   //0.13
    line3 = (int)(0.75*sub_length); //0.2496
    part1.start = 0; 		    part1.end = line1;         part1.result = result_1;
    part2.start = line1;		part2.end = line2;         part2.result = result_2;
    part3.start = line2;		part3.end = line3;         part3.result = result_3;
    part4.start = line3;		part4.end = sub_length;    part4.result = result_4;


    pthread_create(&t1,&attr, DD1, (void*)&part1);
    pthread_create(&t2,&attr, DD2, (void*)&part2);
    pthread_create(&t3,&attr, DD3, (void*)&part3);
    pthread_create(&t4,&attr, DD4, (void*)&part4);      

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
      
    //     clock_t start_4 = clock();

    for(int i=0;i<5;i++) sort(result_1[i].begin(),result_1[i].end());
    for(int i=0;i<5;i++) sort(result_2[i].begin(),result_2[i].end());
    for(int i=0;i<5;i++) sort(result_3[i].begin(),result_3[i].end());
    for(int i=0;i<5;i++) sort(result_4[i].begin(),result_4[i].end());

    //     end = clock();
    int sum=0;
    for(int i=0;i<5;i++)  sum  = sum + result_1[i].size()+ result_2[i].size()+ result_3[i].size()+ result_4[i].size();

    printf("%d\n",sum);
    //     cout<<(double)(endss-start)/CLOCKS_PER_SEC<<endl;
    // 	   cout<<"dfs:"<<(double)(start_2-start_1)/CLOCKS_PER_SEC<<endl;
    // 	   cout<<"check:"<<(double)(start_4-start_3)/CLOCKS_PER_SEC<<endl;
   for(int i=0;i<5;i++){
      for(int j1=0;j1<result_1[i].size();j1++){
	for(int k1=0;k1<result_1[i][j1].size();k1++){
	  printf("%d", result_1[i][j1][k1]); 
	  putchar(k1 == (result_1[i][j1].size()-1) ? '\n' : ',');
	}
      }   //for result_1

    for(int j2=0;j2<result_2[i].size();j2++){
	for(int k2=0;k2<result_2[i][j2].size();k2++){
       	  printf("%d", result_2[i][j2][k2]); 
	  putchar(k2 == (result_2[i][j2].size()-1) ? '\n' : ',');
	}
      }//for result_2
    for(int j3=0;j3<result_3[i].size();j3++){
	for(int k3=0;k3<result_3[i][j3].size();k3++){
	  printf("%d", result_3[i][j3][k3]); 
	  putchar(k3 == (result_1[i][j3].size()-1) ? '\n' : ',');
	}
      }//for result_3
    for(int j4=0;j4<result_4[i].size();j4++){
	for(int k4=0;k4<result_4[i][j4].size();k4++){
	  printf("%d", result_4[i][j4][k4]); 
	  putchar(k4 == (result_4[i][j4].size()-1) ? '\n' : ',');
	}
      }//for result_4
   }
}

