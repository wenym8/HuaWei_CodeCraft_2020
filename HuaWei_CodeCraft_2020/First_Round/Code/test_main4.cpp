#include<bits/stdc++.h> 
using namespace std;

struct Path{
    int length;
    vector<unsigned int> path;

    Path(int length, const vector<unsigned int> &path) : length(length), path(path) {}

    bool operator<(const Path&rhs)const{
        if(length!=rhs.length) return length<rhs.length;
        for(int i=0;i<length;i++){
            if(path[i]!=rhs.path[i])
                return path[i]<rhs.path[i];
        }
    }
};

bool compare_files(string &file1,string &file2){
	//打开文件1 
	ifstream file1_stream; 
    file1_stream.open(file1);   
    assert(file1_stream.is_open()); 
	//打开文件2  
	ifstream file2_stream; 
    file2_stream.open(file2);   
    assert(file2_stream.is_open());   
    
    char c1,c2;
    bool same_flg = true;
    while (!file1_stream.eof()&&!file2_stream.eof())
    {
        file1_stream>>c1;
        file2_stream>>c2;
        if(c1!=c2){
        	same_flg = false;
        	break;
		}

    }
    
    if(file1_stream.eof()!=file2_stream.eof())
    	same_flg = false;
    
    file1_stream.close();
    file2_stream.close();
    return same_flg;
    
}

class Mysolution{
public:
	int record_num = 0,node_num =0;
	vector<bool> visited;
	vector<unsigned int> id_arr;
	unordered_map<unsigned int, int> ids;
	vector<vector<int> > Graph;
	vector<Path> res;
//	vector<unordered_map<int,vector<int>>> Graph2;
	void get_input(string &input_file){
		//读取文件内容
		FILE* file=fopen(input_file.c_str(),"r");
		vector<unsigned int> inputs;
		unsigned int i,j,k;
		record_num = 0;
	    while(fscanf(file,"%u,%u,%u",&i,&j,&k)!=EOF){
			inputs.push_back(i);
			inputs.push_back(j);
	        ++record_num;
	    }
//		cout<<"total record nums : "<<record_num<<endl;
	
		//构建已排序的id数组（或者用set来删除重叠数组，并排序？）
		id_arr = inputs;
		sort(id_arr.begin(), id_arr.end());
		id_arr.erase(unique(id_arr.begin(), id_arr.end()), id_arr.end());
		node_num = 0;
		for (unsigned int &x : id_arr) {
			ids[x] = node_num++;
		}
//		cout << "total node nums : " << node_num << endl;
	
		//构建邻接表
		Graph = vector<vector<int> > (node_num);
		int inputs_size = 2*record_num;
		for (int i = 0; i < inputs_size; i += 2) {
			Graph[ids[inputs[i]]].push_back(ids[inputs[i + 1]]);
		}
		
//		//构建邻接表2
//		Graph2 = vector<unordered_map<int,vector<int>>>(node_num);
//		for (int i = 0; i < node_num; i++) {
//			int i_size = Graph[i].size();
//			for(int j = 0; j < i_size; j++){
//				int graph_ij = Graph[i][j];
//				int j_size = Graph[graph_ij].size();
//				for(int k = 0; k < j_size; k++){
//					Graph2[i][Graph[graph_ij][k]].push_back(graph_ij);
//				}
//				
//			}
//		}
	}
	
	void save_output(string &ouput_file){
	    ofstream outputs(ouput_file.c_str());
	    int res_size = res.size();
	    outputs<<res_size<<endl;
//	    int cnt = 1;
	    for(auto &i:res){
	    	auto path = i.path;
	        int i_size = path.size();
//	        outputs<<cnt<<": ";
	        outputs<<path[0];
//	        cnt++;
	        for(int j=1;j<i_size;j++)
	            outputs<<","<<path[j];
	        outputs<<endl;
	    }
	}
	
	void dfs(int head,int cur,int depth,vector<int> &path){
	    visited[cur]=true;
	    path.push_back(cur);
	    for(int &v:Graph[cur]){
	        if(v==head && depth>=3 && depth<=7){
	            vector<unsigned int> tmp;
	            for(int &x:path)
	                tmp.push_back(id_arr[x]);
	            res.emplace_back(Path(depth,tmp));
	        }
	        if(depth<7 && !visited[v] && v>head){
	            dfs(head,v,depth+1,path);
	        }
	    }
	    visited[cur]=false;
	    path.pop_back();
	}
	
	void dfs_method(){
		visited = vector<bool>(node_num,false);
		vector<int> path;
		stack<int> sta;
		vector<int> depth_vec(8,0);
		int depth = 0;
		int k;
		for(int i=0;i<node_num;i++){
			cout<<"i="<<i<<endl;
			sta.push(i);
			depth_vec[0] = 1;
			while(!sta.empty()){
				int x = sta.top();
				cout<<"x="<<x<<endl;
				sta.pop();
				for(k=7;k>0;k--){
					if(depth_vec[k]!=0){
						break;
					}	
				}
				depth = k;
				--depth_vec[depth];
				for(int &v:Graph[x]){
					if(v==i&&depth>=2 && depth<=6){
			            vector<unsigned int> tmp;
			            for(int &x:path)
			                tmp.push_back(id_arr[x]);
			            res.emplace_back(Path(depth,tmp));
			        }
			        if(depth<6 && !visited[v] && v>i){
			        	++depth_vec[depth];
			        	sta.push(v);
			        	visited[v] = true;
			        }
				}
				visited[x] = false;
			}	
		}
		sort(res.begin(),res.end());
	}
};


int main(){
	string InputFileName = "test_bug.txt";
	string OuputFileName = "output.txt";
	string ResultFileName = "result.txt";
	auto time = 0;
	int test_nums = 30;
	for(int i=0;i<test_nums;i++){
		auto begintime=clock();
		Mysolution ms;
		ms.get_input(InputFileName);
		ms.dfs_method();
		ms.save_output(OuputFileName);
	//	cout<<"result is :"<<compare_files(ResultFileName,OuputFileName)<<endl;
		auto endtime=clock();
		auto using_time = endtime-begintime;
		cout<<"used times : "<<using_time<<endl;
		time += using_time;
	}
	
	cout<<"Mysolution used time: "<<time/test_nums<<endl;
	
	return 0;
}
