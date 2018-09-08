#include<bits/stdc++.h>
#include<iostream>
using namespace std;
unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine gen(seed1); //gen(time(NULL));
uniform_int_distribution<int> dist(0,800000);
int main(){
	ofstream myfile;
  myfile.open ("dataset_uniform.txt");
  for(int i=0;i<100000;i++){
	  int number = dist(gen);
	  myfile <<number<<"\n";
	}

  myfile.close();
  return 0;
}