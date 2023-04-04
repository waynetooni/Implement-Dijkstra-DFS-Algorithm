# include <stdio.h>
# include <iostream>
# include <fstream>
# include <string.h>
# include <vector>
# include <string>

using namespace std ;

int matrix[100][100] ;
int dist[100] ;

struct info{
	string name ;   // N1,N2,N3......N17
	int weight ;
};

struct vertex{
	vector<info> input ;
	vector<info> output ;
	string name ;   // S,V1,V2.....D
};

int getName(string token){
	for ( int i = 0; i < token.size() ; i++ ){
	  if(token[i] == '(' )
	    return i ;	  	
	} // for
} // getName

int getOutput(vector<vertex> data , string name){
	for(int i = 0 ; i < data.size() ; i++){
		if( data[i].name == name )
		  return i ;
	} // for
} // getOutput

int getVertex( vector<vertex> data , string name ){ // name:outputname N1 N2 N3.... 
	for( int i = 0 ; i < data.size() ; i++ ) {
		for( int j = 0 ; j < data[i].input.size() ; j++)
			if( data[i].input[j].name == name )	
			  return i;
	} // for
} // getVertex

int findMin(vector<info> input) {
	int  min = 0 ;
	for( int i = 0 ; i < input.size() ; i++) {
	  if ( i == 0 )
	    min = i;
		else if( input[i].weight < input[min].weight ) 
		  min = i ;
	} // for
	return min ;
	
} // findMin


bool isTraverse(vector<string> traverse, string name ){
  for(int i = 0 ; i < traverse.size(); i++){
    if( traverse[i] == name ) 
      return true ; 
  } // for
  return false ;
} // is

void dfs(vector<vertex> data, vector<vertex> data0, vector<string>& traverse){    // data0: original data || data: changed data
	vector<string> stack ;
	string temp ;
	stack.push_back(data0[0].name) ; // push S into stack 
	while(!stack.empty()){
		traverse.push_back(stack[stack.size()-1]) ;
		temp = stack[stack.size()-1] ;  // temp: 暫存目前節點的名字 
		stack.pop_back() ;
		for( ; data[getOutput(data, temp)].output.size() ; ){
		  if ( !isTraverse(traverse, data0[getVertex(data0,data[getOutput(data0,temp)].output[findMin(data[getOutput(data0,temp)].output)].name)].name)) {  
		    stack.push_back(data0[getVertex(data0,data[getOutput(data0,temp)].output[findMin(data[getOutput(data0,temp)].output)].name)].name) ; 
		  } // if
		  data[getOutput(data, temp)].output.erase(data[getOutput(data, temp)].output.begin() + findMin(data[getOutput(data,temp)].output) );

		} // for 
	} //while
} // dfs

bool isInOutput(vector<vertex> data, int i, int j){
	for(int k = 0 ; k < data[i].output.size() ; k++){
		  if( data[j].name == data[getVertex(data,data[i].output[k].name)].name){
	        matrix[i][j] = data[i].output[k].weight ;
		    return true ; 
		  } // if
		   
	} // for
	return false ;
} // bool

void initMatrix(int matrix[100][100], vector<vertex> data){
	
	for(int i = 0 ; i < data.size() ; i++) {
	  for(int j = 0 ; j < data.size(); j++) {
	  	if( i == j )    // 自己本身的距離為0 
	  	  matrix[i][j] = 0 ; 
	  	else if( isInOutput(data,i ,j) ){
//	  	  for(int k = 0 ; k < data[i].output.size() ; k++){
//			 if( data[j].name == data[getVertex(data,data[i].output[k].name)].name) 
//			   matrix[i][j] = data[i].output[k].weight ;
//		  }//for
		}  // else if
	  	else
	  	  matrix[i][j] = 99999 ; // Can't reach :99999 
		  
	  } // for
	} // for
} // init

void dijkstra( int dist[100], int martix[100][100], vector<vertex> data ){
	
  bool s[100];    // 判斷是否已存入該點到S集合中
	for( int i = 1; i < data.size(); i++)
	{
		dist[i] = matrix[0][i];
		s[i] = false;     // init
	}
	s[0] = true;
 
	for(int i=2 ; i < data.size() ; i++)
	{
		int temp = 99999;
		int u = 0;
		// 找出當前未使用的點j的dist[j]最小值
		for(int j = 1; j < data.size() ; j++)
			if( !s[j] && dist[j] < temp)
			{
				u = j;              // u保存當前鄰接點中距離最小的點的號碼
				temp = dist[j];
			}
		s[u] = true;    // 表示u點已存入S集合中
 
		// 更新dist
		for(int j = 1; j < data.size(); j++)
			if( !s[j] && matrix[u][j] < 99999 ){
				int newdist = dist[u] + matrix[u][j];
				if( newdist < dist[j] ){
					dist[j] = newdist ;
				} // if
			} // if
	} // if
} // Dijkstra

int main(){
 
  fstream file ;
  int index = 0;
  bool first = true ;
	vector<string> temp ;
	vector<string> traverse ;
	vector<vertex> data ;
	vertex tempv ;    // vertex暫存 
	info tempi ;      // info暫存 
	string token ;
    file.open("input.txt",ios::in ) ;
    if(!file)
      cout << "Can't Open File!!\n" ;
    else {
	    getline( file, token ) ;  // 換行 
	    file >> token  ; 
      while(file){
        if( strcmp(token.c_str(),"INSTANCE") == 0  || strcmp(token.c_str(),"ENDCIRCUIT") == 0){
		  if(!first) {
		    data.push_back(tempv) ;
          } //if
          file >> token ; 
	      tempv.input.clear() ;
          tempv.output.clear() ;
          tempv.name.clear() ;
          tempi.name.clear() ;
          tempi.weight = 0 ;
          first = false ;
          tempv.name = token ;  // 將vertex的名字存入
	    } // if
		    else {
		      if ( token[token.size()-2] == 'i' ) {    // input
		      	tempi.weight = token[token.size()-4] - 48;
		      	tempi.name = token.substr(0,getName(token)) ;
		      	tempv.input.push_back(tempi) ;	
		      } // if 
		      if ( token[token.size()-2] == 'o' ){     // output
		        tempi.weight = token[token.size()-4] - 48;
		        tempi.name = token.substr(0,getName(token)) ;	
		        tempv.output.push_back(tempi) ;
		      } // if
        } // else		  
		    
        file >> token ;   // 讀進一個token 
	    } // while
      file.close();
	} // else
	
	int disMatrix[data.size()][data.size()] ;  // two dimension vector to store weight 
	int disofAll[data.size()] ; 
	initMatrix(matrix, data);
	dfs( data ,data, traverse);
	dijkstra( dist, matrix, data ) ;
	for(int i = 0 ; i < traverse.size() ; i++ ) {
	  cout << traverse[i] << endl;
    } // for
  
    for(int i = 0 ; i < data.size() ; i++){
    	cout << dist[i] << endl ;
	}   

	
//  https://www.itread01.com/content/1549904612.html


} // main
