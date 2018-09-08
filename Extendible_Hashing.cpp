#include<bits/stdc++.h>
#include<iostream>
using namespace std;
unsigned int bucket_size = 40;
unsigned int Total_Normal_Buckets = 1;
unsigned int Total_Overflow_Buckets = 0;
unsigned int search_cost = 0;
unsigned int Splitting_cost = 0;
unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine gen(seed1); //gen(time(NULL));
int A=0,B=5000;
uniform_int_distribution<int> dist(0,B);

class bucket{
	public:
		unsigned int Local_Dpt ;
		unsigned int empty_records ;
		unsigned int *Array_Of_Records;
		unsigned int overflow_index;
		bucket(){
			Array_Of_Records = new unsigned int[bucket_size];
			for(int i=0;i<bucket_size;i++){
				Array_Of_Records[i] = 0;
			}
			empty_records =  bucket_size;
			Local_Dpt = 0;
			overflow_index = 4294967295;
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
		Pr_Memory = new bucket[200000];
		Se_Memory = new bucket[300000];
		for(unsigned int i = 200000; i<300000; i++){
			Avail_OverFlow_Buckets.push(i);
		}
		for(unsigned int i = 100000;i<200000;i++){
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
	void Child_To_Parent();
	vector<unsigned int> SaveAll_to_vector(unsigned int new_bucket_index,unsigned int workout_index);
	void Send_Directory_Entries_to_Secondary();
	void rehash_records(vector<unsigned int>temp_records,unsigned int temp_Local_Dpt,unsigned int workout_index);
	unsigned int get_index(unsigned int record_value,unsigned int Suffix_Depth);
	void Simple_push(unsigned int workout_index,unsigned int de_address,unsigned int record_value);
	void print();
	void search(unsigned int record_value);
};

void Extendible_Hash::print(){
	for(int i=bucket_size-1;i>=0;i--){
		cout<<Se_Memory[100001].Array_Of_Records[i]<<" ";
	}
	cout<<endl;
	cout<<Se_Memory[100001].Local_Dpt<<" "<<Se_Memory[100001].overflow_index<<endl;
	for(int i=bucket_size-1;i>=0;i--){
		cout<<Se_Memory[100002].Array_Of_Records[i]<<" ";
	}
	cout<<endl;
	cout<<Se_Memory[100002].Local_Dpt<<" "<<Se_Memory[100002].overflow_index<<endl;
	for(int i=bucket_size-1;i>=0;i--){
		cout<<Se_Memory[100003].Array_Of_Records[i]<<" ";
	}
	cout<<endl;
	cout<<Se_Memory[100003].Local_Dpt<<" "<<Se_Memory[100003].overflow_index<<endl;
	for(int i=bucket_size-1;i>=0;i--){
		cout<<Se_Memory[100004].Array_Of_Records[i]<<" ";
	}
	cout<<endl;
	cout<<Se_Memory[100004].Local_Dpt<<" "<<Se_Memory[100004].overflow_index<<endl;
	for(int i=bucket_size-1;i>=0;i--){
		cout<<Se_Memory[100005].Array_Of_Records[i]<<" ";
	}
	cout<<endl;
	cout<<Se_Memory[100005].Local_Dpt<<" "<<Se_Memory[100005].overflow_index<<endl;
	cout<<endl;
	
}

void Extendible_Hash::search(unsigned int record_value){
	unsigned int directory_entry,de_position,de_bucket_position;
	directory_entry = get_index(record_value,Global_Dpt);
	de_bucket_position = (directory_entry/bucket_size);
	de_position = (directory_entry%bucket_size); 

	if(directory_entry > 1024){
		search_cost++;
	}
	unsigned int de_address = Pr_Memory[de_bucket_position].Array_Of_Records[de_position];
	unsigned int workout_index, Local_Depth, initial_de_address;
	workout_index = Avail_Workout_Buckets.top();
	Avail_Workout_Buckets.pop();
	Pr_Memory[workout_index] = Se_Memory[de_address];
	Local_Depth = Pr_Memory[workout_index].Local_Dpt; 
	initial_de_address = de_address;


	do{
		for(unsigned int i=0;i<bucket_size;i++){
			if(Pr_Memory[workout_index].Array_Of_Records[i] == record_value){
				search_cost++;
				return;
			}
		}
		search_cost++;
		de_address = Pr_Memory[workout_index].overflow_index;
		if(de_address != 4294967295)
			Pr_Memory[workout_index] = Se_Memory[de_address];
	}while(Pr_Memory[workout_index].overflow_index != 4294967295);

	search_cost++;
}
 
unsigned int Extendible_Hash::get_index(unsigned int record_value,unsigned int depth){
	if(depth == 0){
		return 0;
	}
	record_value = record_value<<(32 - depth);
	record_value = record_value>>(32 - depth);
	return record_value;
}

void Extendible_Hash::Fetch_Directory_Entries_to_Primary(){
	unsigned int T_D = (int)pow(2,Global_Dpt);
	unsigned int total_buckets;
	//cout<<Global_Dpt<<endl;
	total_buckets = (T_D)/bucket_size;
	for(unsigned int i=0;i<=total_buckets;i++){
		Pr_Memory[i] = Se_Memory[i];
	}
}


void Extendible_Hash::Send_Directory_Entries_to_Secondary(){
	unsigned int T_D = (int)pow(2,Global_Dpt);
	unsigned int total_buckets;
		total_buckets = (T_D)/bucket_size;
		for(unsigned int i=0;i<=total_buckets;i++){
			Se_Memory[i] = Pr_Memory[i] ;
		}
}


vector<unsigned int> Extendible_Hash::save_to_vector(unsigned int de_address,unsigned workout_index,unsigned int temp_Local_Dpt){
	vector<unsigned int> temp_records;
	unsigned int count = 0;
	do{
		Pr_Memory[workout_index] = Se_Memory[de_address];
		for (int i = 0; i<bucket_size; i++){
			temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[i]);
		}
		Se_Memory[de_address].empty_records = bucket_size;
		Se_Memory[de_address].Local_Dpt = temp_Local_Dpt;
		Se_Memory[de_address].overflow_index = 4294967295;
		count++;
		if(count>1){
			Se_Memory[de_address].Local_Dpt = 0;
			Avail_OverFlow_Buckets.push(de_address);
		}
		de_address = Pr_Memory[workout_index].overflow_index;
	}while(de_address != 4294967295);
	Splitting_cost = Splitting_cost + count - 1;
	return temp_records; 
}

void Extendible_Hash::Change_Pointers(unsigned int initial_de_address,unsigned int new_bucket_index,unsigned int T_D, unsigned int temp,unsigned int temp_Local_Dpt){
	int row_index = 0, column_index = 0, i=0;

	while(i<T_D){
		if(Pr_Memory[row_index].Array_Of_Records[column_index] == initial_de_address){
			unsigned int temp_pointer;
			temp_pointer = get_index(i,temp_Local_Dpt);
			if(temp_pointer == temp){
				Splitting_cost++;
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

void Extendible_Hash::Child_To_Parent(){
	unsigned int Total_directories = 1;
	for(unsigned int i=0;i<Global_Dpt;i++){
		Total_directories = Total_directories*2;
	}
	for(unsigned int i=0;i<Total_directories;i++){
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
}


vector<unsigned int> Extendible_Hash::SaveAll_to_vector(unsigned int new_bucket_index,unsigned int workout_index){
	vector<unsigned int>temp_records;
	for(unsigned int i=100001;i<=new_bucket_index;i++){       //we are adding data into a vector
		//Are there any overflow buckets
		Pr_Memory[workout_index] = Se_Memory[i];

		if(Pr_Memory[workout_index].overflow_index == 4294967295){
			unsigned int j = Pr_Memory[workout_index].empty_records;
			for(;j<bucket_size;j++){
				temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[j]);
				Pr_Memory[workout_index].Array_Of_Records[j] = 0;
			}
			Pr_Memory[workout_index].empty_records = bucket_size;
			Pr_Memory[workout_index].overflow_index = 4294967295;
			Se_Memory[i] = Pr_Memory[workout_index];
		}
		else{
			//No change in the local depth here
			unsigned int j = Pr_Memory[workout_index].empty_records;
			unsigned int next_index;
			for(;j<bucket_size;j++){
				temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[j]);
				Pr_Memory[workout_index].Array_Of_Records[j] = 0;
			}
			Pr_Memory[workout_index].empty_records = bucket_size;
			next_index = Pr_Memory[workout_index].overflow_index;
			Pr_Memory[workout_index].overflow_index = 4294967295;
			Se_Memory[i] = Pr_Memory[workout_index];

			do{
				Pr_Memory[workout_index] = Se_Memory[next_index];
				unsigned int j = Pr_Memory[workout_index].empty_records;
				for(;j<bucket_size;j++){
					temp_records.push_back(Pr_Memory[workout_index].Array_Of_Records[j]);
					Pr_Memory[workout_index].Array_Of_Records[j] = 0;
				}
				Pr_Memory[workout_index].empty_records = bucket_size;
			  unsigned int temp_next_index = next_index;
				next_index = Pr_Memory[workout_index].overflow_index;
				Pr_Memory[workout_index].overflow_index = 4294967295;
				Pr_Memory[workout_index].Local_Dpt = 0;
				Avail_OverFlow_Buckets.push(temp_next_index);
				Se_Memory[temp_next_index] = Pr_Memory[workout_index];
			}while(next_index != 4294967295);

	   }
	}
	return temp_records;
}

void Extendible_Hash::rehash_records(vector<unsigned int>temp_records,unsigned int temp_Local_Dpt,unsigned int workout_index){
	for(int i=0;i<temp_records.size();i++){
		unsigned int rehash_record_pointer;
		rehash_record_pointer = get_index(temp_records[i],temp_Local_Dpt);
		unsigned int rehash_bucket_position = (rehash_record_pointer/bucket_size);
		unsigned int rehash_de_position = (rehash_record_pointer%bucket_size);
		unsigned int new_pointer;
		new_pointer = Pr_Memory[rehash_bucket_position].Array_Of_Records[rehash_de_position]; 
		//cout<<"saia  "<<new_pointer<<endl;
		Pr_Memory[workout_index] = Se_Memory[new_pointer];

		unsigned int temp_index = new_pointer;

		while(Pr_Memory[workout_index].overflow_index != 4294967295){
				Splitting_cost++;
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
			Splitting_cost++;
			unsigned int new_hashed_bucket = Avail_OverFlow_Buckets.top();
			Pr_Memory[workout_index].overflow_index = new_hashed_bucket;
			Se_Memory[temp_index] = Pr_Memory[workout_index];
			Pr_Memory[workout_index] = Se_Memory[new_hashed_bucket];
			Pr_Memory[workout_index].empty_records = bucket_size;

			Pr_Memory[workout_index].Array_Of_Records[Pr_Memory[workout_index].empty_records - 1] = temp_records[i];
			Pr_Memory[workout_index].Local_Dpt = temp_Local_Dpt;
			Pr_Memory[workout_index].empty_records--;
			Pr_Memory[workout_index].overflow_index = 4294967295;
			Se_Memory[new_hashed_bucket] = Pr_Memory[workout_index];
		}
	}
	Pr_Memory[workout_index].empty_records = bucket_size;
	Pr_Memory[workout_index].overflow_index = 4294967295;
	Pr_Memory[workout_index].Local_Dpt = 0;
	Avail_Workout_Buckets.push(workout_index);
}


void Extendible_Hash::Simple_push(unsigned int workout_index,unsigned int de_address,unsigned int record_value){
	unsigned int em_rec = Pr_Memory[workout_index].empty_records;
	Pr_Memory[workout_index].Array_Of_Records[em_rec - 1] = record_value;
	Pr_Memory[workout_index].empty_records--;
	Se_Memory[de_address] = Pr_Memory[workout_index];
	Pr_Memory[workout_index].empty_records = bucket_size;
	Pr_Memory[workout_index].Local_Dpt = 0;
	Pr_Memory[workout_index].overflow_index = 4294967295;
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
	if(flag == 0){
		Fetch_Directory_Entries_to_Primary();//1
	}
	
	//Q3.Get the bucket from the Se_memory into PM
	unsigned int de_address = Pr_Memory[de_bucket_position].Array_Of_Records[de_position];
	unsigned int workout_index, Local_Depth, initial_de_address;
	workout_index = Avail_Workout_Buckets.top();
	Avail_Workout_Buckets.pop();
	Pr_Memory[workout_index] = Se_Memory[de_address];
	Local_Depth = Pr_Memory[workout_index].Local_Dpt; 
	initial_de_address = de_address;

	//Q4.Does that have any overflow buckets linked to it.If yes go to the last bucket
	unsigned int Temporary_Cost = 0;
	while(Pr_Memory[workout_index].overflow_index != 4294967295){
		Temporary_Cost++;
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
			//Q8.save all rcords to a vector
			//Q9.add a new bucket and change the pointers of the directories
			//Q10.send the records to the new buckets and rehash
			vector<unsigned int> temp_records;
			de_address = initial_de_address;
			temp_records = save_to_vector(de_address,workout_index,temp_Local_Dpt);
			temp_records.push_back(record_value);
			unsigned int temp;
			temp = temp_records[0]<<(32 - Local_Depth);
			temp = temp>>(32 - Local_Depth);
			new_bucket_index++; 
       
			Change_Pointers(initial_de_address,new_bucket_index,T_D,temp,temp_Local_Dpt);
			rehash_records(temp_records,temp_Local_Dpt,workout_index);
		}
		else if(Pr_Memory[workout_index].Local_Dpt == Global_Dpt && flag == 0){
			//Q11.directory expansion
			//Q12.Point all the child pointers to the parent pointer
			//Q13.Increase the Global Depth
			//Q14.Increasethe local depth of the divided
			//Q15.reinsert all the records
			//Q16.copy the PM to SM
			//Q17.Free the workout index 
			cout<<"Directory Expansion"<<endl;

			Splitting_cost = Splitting_cost + Temporary_Cost;
			Child_To_Parent();
			Splitting_cost = Splitting_cost + (unsigned int)pow(2,Global_Dpt);
			vector<unsigned int>temp_records;
			temp_records = SaveAll_to_vector(new_bucket_index,workout_index);
			temp_records.push_back(record_value);



			Pr_Memory[workout_index] = Se_Memory[initial_de_address];
			unsigned int parent_local_depth = Pr_Memory[workout_index].Local_Dpt;
			Pr_Memory[workout_index].Local_Dpt = Pr_Memory[workout_index].Local_Dpt + 1;
			Se_Memory[initial_de_address] = Pr_Memory[workout_index];
			unsigned int child_of_split = directory_entry+(int)pow(2,Global_Dpt);

			new_bucket_index++;
			Pr_Memory[workout_index] = Se_Memory[new_bucket_index];
			Pr_Memory[workout_index].Local_Dpt = parent_local_depth + 1;
			Se_Memory[new_bucket_index] = Pr_Memory[workout_index];
			
			Pr_Memory[(child_of_split/bucket_size)].Array_Of_Records[(child_of_split%bucket_size)] = new_bucket_index;
			Se_Memory[(child_of_split/bucket_size)].Array_Of_Records[(child_of_split%bucket_size)] = new_bucket_index;
			
			Pr_Memory[workout_index].empty_records = bucket_size;
			Pr_Memory[workout_index].Local_Dpt = 0;
			Pr_Memory[workout_index].overflow_index = 4294967295;
			Avail_Workout_Buckets.push(workout_index);

			Global_Dpt++;
			cout<<"Global_Dpt: "<<Global_Dpt<<endl;
			for(int i=0;i<temp_records.size();i++){
				insert_record(temp_records[i],1);
			}
			Send_Directory_Entries_to_Secondary();
		}                                        
		else{
			//Q16.inserting after expansion
			Splitting_cost = Splitting_cost + Temporary_Cost;
			while(Pr_Memory[workout_index].overflow_index != 4294967295){
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
				Pr_Memory[workout_index].overflow_index = 4294967295;
				Avail_Workout_Buckets.push(workout_index);
			}
			else{
				Splitting_cost++;
				unsigned int new_bucket_add;
				new_bucket_add = Avail_OverFlow_Buckets.top();
				Pr_Memory[workout_index].overflow_index = new_bucket_add;
				Se_Memory[de_address] = Pr_Memory[workout_index];
				Pr_Memory[workout_index] = Se_Memory[new_bucket_add];
				Pr_Memory[workout_index].empty_records = bucket_size;
				Pr_Memory[workout_index].Array_Of_Records[bucket_size - 1] = record_value;
				Pr_Memory[workout_index].Local_Dpt = Local_Depth;
				Pr_Memory[workout_index].overflow_index = 4294967295;
				Pr_Memory[workout_index].empty_records--;
				Se_Memory[new_bucket_add] = Pr_Memory[workout_index];
				//push the workout index
				Pr_Memory[workout_index].empty_records = bucket_size;
				Pr_Memory[workout_index].Local_Dpt = 0;
				Pr_Memory[workout_index].overflow_index = 4294967295;
				Avail_Workout_Buckets.push(workout_index);
			}
		}
  }
}








int main(){ 
	ifstream inFile;
	ofstream inFile1;
	int count=1;
	vector <int> All_Records;
  unsigned int record_value;
  inFile.open("dataset_uniform.txt");
  inFile1.open("EH_40_Splitting_Cost.txt");
  Extendible_Hash EH;
  while (inFile >> record_value){ 
  		 All_Records.push_back(record_value); 
       EH.insert_record(record_value,0);
       if(Splitting_cost > 0){
       		inFile1<<count<<" "<<Splitting_cost+2<<"\n";
       }
       else{
       		inFile1<<count<<" "<<0<<"\n";
       }

       count++;
       Splitting_cost = 0;
  }
  cout<<EH.new_bucket_index<<endl;
  //EH.print();
  inFile1.close();
  inFile.close();

  return 0;
}




