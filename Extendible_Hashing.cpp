#include<bits/stdc++.h>
#include<iostream>
using namespace std;
unsigned int bucket_size = 40;
unsigned int Total_Normal_Buckets = 1;
unsigned int Total_Overflow_Buckets = 0;

class bucket{
	public:
		unsigned int Local_Dpt ;
		unsigned int empty_records ;
		unsigned int *Array_Of_Records;
		unsigned int overflow_index = -1;
		bucket(){
			Array_Of_Records = new unsigned int[bucket_size];
			empty_records =  bucket_size;
			Local_Dpt = 0;
		}
		
};




class Extendible_Hash{
	public:
	unsigned int Global_Dpt;	
	bucket *Se_Memory;
	bucket *Pr_Memory;
	priority_queue <unsigned int, vector<unsigned int>, greater<unsigned int> > Avail_OverFlow_Buckets;
	priority_queue <unsigned int, vector<unsigned int>, greater<unsigned int> > Avail_Workout_Buckets;
	unsigned int new_bucket_index = 100001;
	Extendible_Hash(){
		Global_Dpt = 0;
		Pr_Memory = new bucket[100000];
		Se_Memory = new bucket[300000];
		for(unsigned int i = 200000; i<300000; i++){
			Avail_OverFlow_Buckets.push(i);
		}
		for(unsigned int i = 50000;i<100000;i++){
			Avail_Workout_Buckets.push(i);
		}
		Se_Memory[0].Array_Of_Records[0] = 100001;       //The directory at 0 pounsigned ints to the bucket 1000001;
		Pr_Memory[0].Array_Of_Records[0] = 100001;
		Se_Memory[100001].Local_Dpt = 0;
	}
	void insert_record(unsigned int record_value,unsigned int flag);
	void Fetch_Directory_Entries_to_Primary();
	vector<unsigned int> save_to_vector(unsigned int de_address,unsigned workout_index,unsigned int temp_Local_Dpt);
	void Change_Pointers(unsigned int initial_de_address,unsigned int new_bucket_index,unsigned int T_D, unsigned int temp,unsigned int temp_Local_Dpt);
	void rehash_records(vector<unsigned int>temp_records,unsigned int temp_Local_Dpt,unsigned int workout_index);
	unsigned int get_index(unsigned int record_value,unsigned int Suffix_Depth);
	void Simple_push(unsigned int workout_index,unsigned int de_address,unsigned int record_value);
};



unsigned int Extendible_Hash::get_index(unsigned int record_value,unsigned int depth){
	record_value = record_value<<(32 - depth);
	record_value = record_value>>(32 - depth);
	return record_value;
}

void Extendible_Hash::Fetch_Directory_Entries_to_Primary(){
	unsigned int T_D = (int)pow(2,Global_Dpt);
	unsigned int total_buckets;
	if(T_D > 1024){
		total_buckets = (T_D)/bucket_size;
		for(unsigned int i=0;i<=total_buckets;i++){
			Pr_Memory[i] = Se_Memory[i];
		}
	}
}

vector<unsigned int> Extendible_Hash::save_to_vector(unsigned int de_address,unsigned workout_index,unsigned int temp_Local_Dpt){
	vector<unsigned int> temp_records;
	int count = 0;
	do{
		Pr_Memory[workout_index] = Se_Memory[de_address];
		for (int i = 0; i<bucket_size; i++){
			temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[i]);
		}
		Se_Memory[de_address].empty_records = bucket_size;
		Se_Memory[de_address].Local_Dpt = temp_Local_Dpt;
		Se_Memory[de_address].overflow_index = -1;
		count++;
		if(count>1){
			Se_Memory[de_address].Local_Dpt = 0;
			Avail_OverFlow_Buckets.push(de_address);
		}
		de_address = Pr_Memory[workout_index].overflow_index;
	}while(de_address != -1);
	return temp_records; 
}

void Extendible_Hash::Change_Pointers(unsigned int initial_de_address,unsigned int new_bucket_index,unsigned int T_D, unsigned int temp,unsigned int temp_Local_Dpt){
	int row_index = 0, column_index = 0, i=0;

	while(i<T_D){
		if(Pr_Memory[row_index].Array_Of_Records[column_index] == initial_de_address){
			unsigned int temp_pointer;
			temp_pointer = i<<(32 - temp_Local_Dpt);
			temp_pointer = temp_pointer>>(32 - temp_Local_Dpt);
			if(temp_pointer == temp){
				Pr_Memory[row_index].Array_Of_Records[column_index] = new_bucket_index;
				Se_Memory[row_index].Array_Of_Records[column_index] = new_bucket_index;
			}
		}
		column_index++;
		if(column_index >= bucket_size){
			row_index++;
			column_index = 0;
		}
		i++;
	}

}

void Extendible_Hash::rehash_records(vector<unsigned int>temp_records,unsigned int temp_Local_Dpt,unsigned int workout_index){
	for(int i=0;i<temp_records.size();i++){
		unsigned int rehash_record_pointer;
		rehash_record_pointer = get_index(temp_records[i],temp_Local_Dpt);
		unsigned int rehash_bucket_position = (rehash_record_pointer/bucket_size);
		unsigned int rehash_de_position = (rehash_record_pointer%bucket_size);
		unsigned int new_pointer;
		new_pointer = Pr_Memory[rehash_bucket_position].Array_Of_Records[rehash_de_position]; 
		Pr_Memory[workout_index] = Se_Memory[new_pointer];
		unsigned int temp_index = new_pointer;
		while(Pr_Memory[workout_index].overflow_index != -1){
				temp_index = Pr_Memory[workout_index].overflow_index;
				Pr_Memory[workout_index] = Se_Memory[temp_index];
		}

		if(Pr_Memory[workout_index].empty_records > 0){
			Pr_Memory[workout_index].Array_Of_Records[Pr_Memory[workout_index].empty_records - 1] = temp_records[i];
			Pr_Memory[workout_index].Local_Dpt = temp_Local_Dpt;
			Pr_Memory[workout_index].empty_records--;
			Se_Memory[temp_index] = Pr_Memory[workout_index];
		}
		else{
			//create new bucket;
			unsigned int new_hashed_bucket = Avail_OverFlow_Buckets.top();
			Pr_Memory[workout_index].overflow_index = new_hashed_bucket;
			Se_Memory[temp_index] = Pr_Memory[workout_index];
			Pr_Memory[workout_index] = Se_Memory[new_hashed_bucket];
			Pr_Memory[workout_index].Array_Of_Records[Pr_Memory[workout_index].empty_records - 1] = temp_records[i];
			Pr_Memory[workout_index].Local_Dpt = temp_Local_Dpt;
			Pr_Memory[workout_index].empty_records--;
			Pr_Memory[workout_index].overflow_index = -1;
			Se_Memory[new_hashed_bucket] = Pr_Memory[workout_index];
		}
	}


}


void Extendible_Hash::Simple_push(unsigned int workout_index,unsigned int de_address,unsigned int record_value){
	unsigned int em_rec = Pr_Memory[workout_index].empty_records;
	Pr_Memory[workout_index].Array_Of_Records[em_rec - 1] = record_value;
	Pr_Memory[workout_index].empty_records--;
	Se_Memory[de_address] = Pr_Memory[workout_index];
	Pr_Memory[workout_index].empty_records = bucket_size;
	Pr_Memory[workout_index].Local_Dpt = 0;
	Pr_Memory[workout_index].overflow_index = -1;
	Avail_Workout_Buckets.push(workout_index);
}


void Extendible_Hash::insert_record(unsigned int record_value, unsigned int flag){

	//Q1. Where does the record value will go.......checking the suffix of it till Gobal depth and returning the index 
	unsigned int directory_entry,de_position,de_bucket_position;
	directory_entry = get_index(record_value,Global_Dpt);
	de_bucket_position = (directory_entry/bucket_size);
	de_position = (directory_entry%bucket_size); 

	//Q2.Does all the directory entries required are present in the Primary memory. If not bring them to the Primary Memory
	unsigned int T_D = (int)pow(2,Global_Dpt);
	Fetch_Directory_Entries_to_Primary();//1

	//Q3.Get the bucket from the Se_memory into PM
	unsigned int de_address = Pr_Memory[de_bucket_position].Array_Of_Records[de_position];
	unsigned int workout_index, Local_Depth, initial_de_address;
	workout_index = Avail_Workout_Buckets.top();
	Avail_Workout_Buckets.pop();
	Pr_Memory[workout_index] = Se_Memory[de_address];
	Local_Depth = Pr_Memory[workout_index].Local_Dpt; 
	initial_de_address = de_address;

	//Q4.Does that have any overflow buckets linked to it.If yes go to the last bucket
	while(Pr_Memory[workout_index].overflow_index != -1){
		de_address = Pr_Memory[workout_index].overflow_index;
		Pr_Memory[workout_index] = Se_Memory[de_address];
	}


	//Q5.Does the last bucket has any space to fill the new record.If yes simple push.else compre LD and GD
	if(Pr_Memory[workout_index].empty_records > 0){
		Simple_push(workout_index,de_address,record_value);                   
	}
	else{
		Pr_Memory[workout_index] = Se_Memory[initial_de_address];
		unsigned int temp_Local_Dpt = Pr_Memory[workout_index].Local_Dpt+1;

		//Q6.Check for simple rehashing or Directory Expansion
		if(Pr_Memory[workout_index].Local_Dpt < Global_Dpt){
			//Q7.Add one more bucket and change the pointers since ld<gd. To do that first save all the records in a vector
			vector<unsigned int> temp_records;
			de_address = initial_de_address;
			//Q8.save all rcords to a vector
			temp_records = save_to_vector(de_address,workout_index,temp_Local_Dpt);
			temp_records.push_back(record_value);
			unsigned int temp;
			temp = temp_records[0]<<(32 - Local_Depth);
			temp = temp>>(32 - Local_Depth);
			//Q9.add a new bucket and change the pointers of the directories
			new_bucket_index++;        
			Change_Pointers(initial_de_address,new_bucket_index,T_D,temp,temp_Local_Dpt);

			// send the records to the new buckets
			rehash_records(temp_records,temp_Local_Dpt,workout_index);

							
		}
		else if(Pr_Memory[workout_index].Local_Dpt == Global_Dpt && flag == 0){
			//complete directory exansion
			//put all the pointers child pointers pointing to the parent pointer
			Global_Dpt++;
			unsigned int Total_directories = 1;
			for(int i=0;i<Global_Dpt;i++){
				Total_directories = Total_directories*2;
			}
			for(int i=0;i<Total_directories;i++){
				unsigned int row_no,column_no;
				row_no = i/bucket_size;
				column_no = i%bucket_size;
				unsigned int parent_add = Pr_Memory[row_no].Array_Of_Records[column_no];
				unsigned int new_child;
				new_child = i+Total_directories;
				row_no = new_child/bucket_size;
				column_no = new_child%bucket_size;
				Pr_Memory[row_no].Array_Of_Records[column_no] = parent_add;
			}
			//mapping of all the new directries completed
			vector<unsigned int>temp_records;

			for(int i=100001;i<new_bucket_index;i++){       //we are adding data into a vector
				Pr_Memory[workout_index] = Se_Memory[i];

				if(Pr_Memory[workout_index].overflow_index == -1){
					//there are no overflow buckets;
					unsigned int j = Pr_Memory[workout_index].empty_records;
					for(;j<bucket_size;j++){
						temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[j]);
					}
					Pr_Memory[workout_index].empty_records = bucket_size;
					Pr_Memory[workout_index].overflow_index = -1;
					Se_Memory[i] = Pr_Memory[workout_index];
				}
				else{
					unsigned int j = Pr_Memory[workout_index].empty_records;
					unsigned int next_index;
					for(;j<bucket_size;j++){
						temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[j]);
					}
					Pr_Memory[workout_index].empty_records = bucket_size;
					next_index = Pr_Memory[workout_index].overflow_index;
					Pr_Memory[workout_index].overflow_index = -1;
					Se_Memory[i] = Pr_Memory[workout_index];
					do{
						Pr_Memory[workout_index] = Se_Memory[next_index];
						unsigned int j = Pr_Memory[workout_index].empty_records;
						unsigned int next_index;
						for(;j<bucket_size;j++){
							temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[j]);
						}
						Pr_Memory[workout_index].empty_records = bucket_size;
					  unsigned int temp_next_index = next_index;
						next_index = Pr_Memory[workout_index].overflow_index;
						Pr_Memory[workout_index].overflow_index = -1;
						Pr_Memory[workout_index].Local_Dpt = 0;
						Avail_OverFlow_Buckets.push(temp_next_index);
						Se_Memory[temp_next_index] = Pr_Memory[workout_index];
					}while(next_index != -1);
				}
			}

			Pr_Memory[workout_index] = Se_Memory[initial_de_address];
			Pr_Memory[workout_index].Local_Dpt = Pr_Memory[workout_index].Local_Dpt + 1;
			Se_Memory[initial_de_address] = Pr_Memory[workout_index];
			Pr_Memory[workout_index].empty_records = bucket_size;
			Pr_Memory[workout_index].Local_Dpt = 0;
			Pr_Memory[workout_index].overflow_index = -1;
			temp_records.push_back(record_value);

			for(int i=0;i<temp_records.size();i++){
				insert_record(temp_records[i],1);	
			}




		}                                        //inserting after expansion
		else{
			while(Pr_Memory[workout_index].overflow_index != -1){
				de_address = Pr_Memory[workout_index].overflow_index;
				Pr_Memory[workout_index] = Se_Memory[de_address];
			}

			if(Pr_Memory[workout_index].empty_records > 0){
				//simple pushing shouldn't be any problem
				unsigned int em_rec = Pr_Memory[workout_index].empty_records;
				Pr_Memory[workout_index].Array_Of_Records[em_rec - 1] = record_value;
				Pr_Memory[workout_index].empty_records--;
				Se_Memory[de_address] = Pr_Memory[workout_index];
				Pr_Memory[workout_index].empty_records = bucket_size;
				Pr_Memory[workout_index].Local_Dpt = 0;
				Pr_Memory[workout_index].overflow_index = -1;
				Avail_Workout_Buckets.push(workout_index);
			}
			else{
				unsigned int new_bucket_add;
				new_bucket_add = Avail_OverFlow_Buckets.top();
				Pr_Memory[workout_index].overflow_index = new_bucket_add;
				Pr_Memory[workout_index] = Se_Memory[new_bucket_add];
				Pr_Memory[workout_index].Array_Of_Records[bucket_size - 1] = record_value;
				Pr_Memory[workout_index].Local_Dpt = Local_Depth;
				Pr_Memory[workout_index].overflow_index = -1;
				Pr_Memory[workout_index].empty_records--;
				Se_Memory[new_bucket_add] = Pr_Memory[workout_index];
			}
		}
  }
}








int main(){ 
	ifstream inFile;
  unsigned int record_value;
  inFile.open("dataset_uniform.txt");
  Extendible_Hash EH;
  while (inFile >> record_value){  
       EH.insert_record(record_value,0);
  }

  inFile.close();

  return 0;
}




