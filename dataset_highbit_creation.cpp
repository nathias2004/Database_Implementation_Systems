#include<bits/stdc++.h>
#include<iostream>
using namespace std;
unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine gen(seed1); //gen(time(NULL));
uniform_int_distribution<int> dist(700000,800000);
uniform_int_distribution<int> dist1(0,700000);
int main(){
	ofstream myfile;
  myfile.open ("dataset_highbit_uniform.txt");
  for(int i=0;i<60000;i++){
	  int number = dist(gen);
	  myfile <<number<<"\n";
	}

	for(int i=0;i<40000;i++){
		int number = dist1(gen);
	  myfile <<number<<"\n";
	}

  myfile.close();
  return 0;
}