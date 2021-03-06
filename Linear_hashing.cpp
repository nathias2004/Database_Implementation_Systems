#include<bits/stdc++.h>
#include<iostream>
using namespace std;
int bucket_size = 40;
int Total_Normal_Buckets = 1;
int Total_Overflow_Buckets = 0;
int search_cost=0;
int Splitting_cost=0;
unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine gen(seed1); //gen(time(NULL));
int A=0,B=5000;
uniform_int_distribution<int> dist(0,B);

class bucket{
	public:
		int empty_records ;
		int *Array_Of_Records;
		int overflow_index = -1;
		bucket(){
			Array_Of_Records = new int[bucket_size];
			empty_records =  bucket_size;
		}
		
};




class Linear_Hash{
	public:
	int Next_Split;
	int hash_index;	
	bucket *Se_Memory;
	priority_queue <int, vector<int>, greater<int> > Avail_OverFlow_Buckets;
	Linear_Hash(){
		Next_Split = 0;
		hash_index = 0;
		Se_Memory = new bucket[200000];
		for(int i = 100000; i<200000; i++){
			Avail_OverFlow_Buckets.push(i);
		}
	}
	void insert_record(int record_value,int flag);
	void rehash_records(int rehash_address);
	int get_hash_value(int record_value);
	void print();
	void search(int record_value);
};


void Linear_Hash::search(int record_value){
	int temp_bucket_index;
	temp_bucket_index = get_hash_value(record_value);
	int record_index = Se_Memory[temp_bucket_index].empty_records;

	if(record_index > 0){
		search_cost++;
		Se_Memory[temp_bucket_index].Array_Of_Records[record_index - 1] = record_value;
		Se_Memory[temp_bucket_index].empty_records--;
	}
	else{
		int travel_pointer = Se_Memory[temp_bucket_index].overflow_index;
		int pre_travel_pointer = temp_bucket_index;
		while(travel_pointer != -1){
			for(int i=0;i<bucket_size;i++){
				if(Se_Memory[pre_travel_pointer].Array_Of_Records[i]==record_value){
					search_cost++;
					return;
				}
			}
			search_cost++;
			pre_travel_pointer = travel_pointer;
			travel_pointer = Se_Memory[travel_pointer].overflow_index;
		}
	}
}
void Linear_Hash::print(){
	for(int i=0;i<200000;i++){
		cout<<Se_Memory[i].Array_Of_Records[0]<<" ";
		cout<<Se_Memory[i].overflow_index<<endl;
	}
}

int Linear_Hash::get_hash_value(int record_value){
	int temp_address;
	temp_address = (record_value)%((int)pow(2,hash_index));
	if(temp_address < Next_Split){
		return (record_value)%((int)pow(2,hash_index+1));
	}
	return temp_address;
}


void Linear_Hash::rehash_records(int rehash_address){
	Total_Normal_Buckets++;
	int travel_pointer = rehash_address;
	vector <int> records_to_be_rehashed;
	int size = bucket_size;
	int count = 0;
	while(travel_pointer != -1){
		int i = size-1;
		Splitting_cost++;
		while(Se_Memory[travel_pointer].empty_records != size){
			records_to_be_rehashed.push_back(Se_Memory[travel_pointer].Array_Of_Records[i]);
			Se_Memory[travel_pointer].Array_Of_Records[i] = -1;
			Se_Memory[travel_pointer].empty_records++;
			i--;
		}
		if(count != 0){
			Avail_OverFlow_Buckets.push(travel_pointer);
			Total_Overflow_Buckets--;
		}
		count = 1;
		int pre_travel_pointer = travel_pointer;
		travel_pointer = Se_Memory[travel_pointer].overflow_index;
		Se_Memory[pre_travel_pointer].overflow_index = -1;
	}
	Next_Split++;
	for(int i=0;i<records_to_be_rehashed.size();i++){
		insert_record(records_to_be_rehashed[i],1);
	}
	if(Next_Split == (int)pow(2,hash_index)){
		Next_Split = 0;
		hash_index++;
	}

}

void Linear_Hash::insert_record(int record_value, int flag){
	int temp_bucket_index;
	temp_bucket_index = get_hash_value(record_value);
	int record_index = Se_Memory[temp_bucket_index].empty_records;

	if(record_index > 0){
		Se_Memory[temp_bucket_index].Array_Of_Records[record_index - 1] = record_value;
		Se_Memory[temp_bucket_index].empty_records--;
	}
	else{
		int travel_pointer = Se_Memory[temp_bucket_index].overflow_index;
		int pre_travel_pointer = temp_bucket_index;
		while(travel_pointer != -1){
			pre_travel_pointer = travel_pointer;
			travel_pointer = Se_Memory[travel_pointer].overflow_index;
		}
		if(Se_Memory[pre_travel_pointer].empty_records == 0){
			//Taking a new and adding the overflow bucket
			if(flag){
				Splitting_cost++;
			}
			int overflow_bucket_address =  Avail_OverFlow_Buckets.top();
			Avail_OverFlow_Buckets.pop();
			Total_Overflow_Buckets++;
			record_index = Se_Memory[overflow_bucket_address].empty_records;
			Se_Memory[overflow_bucket_address].Array_Of_Records[record_index - 1] = record_value;
			Se_Memory[overflow_bucket_address].empty_records--;
			Se_Memory[pre_travel_pointer].overflow_index = overflow_bucket_address;
			if(flag == 0){
					 rehash_records(Next_Split);
			}

		}
		else{
			record_index = Se_Memory[pre_travel_pointer].empty_records;
			Se_Memory[pre_travel_pointer].Array_Of_Records[record_index - 1] = record_value;
			Se_Memory[pre_travel_pointer].empty_records--;
		}


	}

}








int main(){ 
	ifstream inFile;	
	ofstream inFile1,inFile2,inFile3;
  int record_value;
  inFile.open("dataset_highbit_uniform.txt");
  inFile1.open("Metrics_Data/High_Bit_Data/LH_40_StorageUtil.txt");
  inFile2.open("Metrics_Data/High_Bit_Data/LH_40_Avg_Suc_Cost.txt");
  inFile3.open("Metrics_Data/High_Bit_Data/LH_40_Splitting_Cost.txt");
  
  Linear_Hash LH;
  int count=1;
  vector <unsigned int> All_Records;
  while (inFile >> record_value){  
  		All_Records.push_back(record_value);
       LH.insert_record(record_value,0);
       inFile1<<count<<" "<<(float)count/((Total_Normal_Buckets+Total_Overflow_Buckets)*bucket_size)<<"\n";
       if( (count)%5000 == 0 ){
	   		for(int i=0;i<50;i++){
				unsigned int number = dist(gen);
				LH.search(All_Records[number]);
			}
			inFile2<<B<<" "<<(float)search_cost/50<<"\n";
			search_cost = 0;
			B = B+5000;
   		}
		if(Splitting_cost > 0){
			inFile3<<count<<" "<<Splitting_cost<<"\n";
		}
		Splitting_cost=0;
   		search_cost=0;
	   count++;
  }
	inFile1.close();
	inFile2.close();
	inFile3.close();

  inFile.close();

  return 0;
}




