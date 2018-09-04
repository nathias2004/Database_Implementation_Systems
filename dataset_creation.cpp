#include<bits/stdc++.h>
#include<iostream>
using namespace std;


int main(){
	default_random_engine generator;
  uniform_int_distribution<int> distribution(0,800000);
	ofstream myfile;
  myfile.open ("dataset_uniform.txt");
  for(int i=0;i<100000;i++){
	  int number = distribution(generator);
	  myfile <<number<<"\n";
	}
  myfile.close();
  return 0;
}