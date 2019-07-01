#include<iostream>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include<numeric>

using namespace std;

struct processor{

	int id;
	int ability;

};

struct task{

	int ID;
	int R_T;
	int E_T;
	int deadline;
	int period;
	int preemption;
	int task_type;
	int W_T;
	int num;
	int CPU_T;

};

void print_process(int id, int ab);
void print_task(int id, int R_T, int E_T, int deadline, int period, int preemption, int task_type);
int gcd(int a, int b);
int lcm(int a, int b);
bool compare_id(const task &a, const task &b);
bool compare_deadline(const task &a, const task &b);
bool compare_precedence(int &a, int &b);
bool compare_slack_time(const task &a, const task &b);

int total = 0;
int Time = 0;

int main(){

	freopen("input.txt","r",stdin);
	int m = 0;//process
	int n = 0;//task
	int count = 0;
	cin >> m >> n;
	printf("number of process: %d\n", m);	
	printf("number of task: %d\n", n);	
	vector<processor> P;
	processor p;
	for(count=0;count<m;count++){
		scanf("%d %d\n", &p.id, &p.ability);
		print_process(p.id, p.ability);
		P.push_back(p);

	}
	vector<task> T;
	task t;
	int total = 0;
	for(count=0;count<n;count++){

		scanf("%d %d %d %d %d %d %d\n", &t.ID, &t.R_T, &t.E_T, &t.deadline, &t.period, &t.preemption, &t.task_type);
		t.W_T = 0;
		total++;
		t.num = total;
		t.CPU_T = 0;
		if(t.deadline == 0){
			t.deadline = t.R_T+t.period;
		}
		print_task(t.ID, t.R_T, t.E_T, t.deadline, t.period, t.preemption, t.task_type);
		T.push_back(t);
	}

	/*Read precedence constraint*/
	int num_of_precedence = 0;
	int con1 = 0,con2 = 0;
	vector<int> precedence;
	scanf("%d\n", &num_of_precedence);
	for(count=0;count<num_of_precedence;count++){
		scanf("%d %d\n", &con1, &con2);
		int exist = 0;
		for(int i=0;i<precedence.size();i++){
			/*already exist*/
			if(precedence[i]==con1){
				exist = 1;
				break;
			}
		}
		if(exist == 0){
			precedence.push_back(con1);
		}
		exist = 0;
		for(int i=0;i<precedence.size();i++){
			/*already exist*/
			if(precedence[i]==con2){
				exist = 1;
				break;
			}
		}
		if(exist == 0){
			precedence.push_back(con2);
		}
		std::sort(precedence.begin(), precedence.end(), compare_precedence);
	}

	for(int i=0;i<precedence.size();i++){
		printf("%d\n", precedence[i]);
	}

	int per[n];
	int k = 0;
	for(int i=0;i<n;i++){
		if(T[i].period != 0){
			per[i]=T[i].period;
			k++;
		}
	}
	
	int LCM = std::accumulate(per, per+k,1,lcm);
	printf("Hyperperiodic = %d\n", LCM);

	for(int i=0;i<n;i++){
		if(T[i].period!=0){
			for(int j=1;j<LCM/T[i].period;j++){
				t = T[i];
				t.R_T += j*T[i].period;
				total++;
				t.num = total;
				t.deadline = t.R_T+t.period;
				t.CPU_T = 0;
				T.push_back(t); 
			}
		}
	}

	//std::sort(T.begin(),T.end(), compare_id);
	std::sort(T.begin(),T.end(), compare_slack_time);

	int idle = 0;
	int cur_num = 0,old_num =0;
	int cur_id = 0,old_id =0;
	int flag = 0;
	int same_task = 0;
	int erase = 0;
	int total_W_T = 0;
	int total_CPU_T = 0;
	int total_task = T.size();
	while(Time<LCM || !T.empty()){

		int get_it = 0;
		old_num = cur_num;
		old_id = cur_id;
		int i = 0;
		int temp = 0;
		int temp_id = 0;
		for(;i<T.size();i++){
			if(T[i].R_T<=Time){
				get_it = 1;
				cur_num = T[i].num;
				cur_id = T[i].ID;
				if(flag==0){
					old_num = cur_num;
					old_id = cur_id;
					flag = 1;
				}
				for(int y=0;y<precedence.size();y++){
					if(get_it){
						if(T[i].ID==precedence[y]){
							for(int x=0;x<y;x++){
								for(int u=0;u<T.size();u++){
									if(T[u].ID==precedence[x]){
										get_it=0;
									}	
								}
							}
						}
					}
				}
				if(get_it==1){
					break;
				}
			}
		}

		if(get_it){
			T[i].E_T -= 1;
			T[i].CPU_T++;
			for(int u = 0;u<T.size();u++){
				if(T[u].R_T<=Time && T[u].num!=cur_num){
					T[u].W_T++;
				}
			}
			if(cur_num == old_num){
				same_task++;
				erase = 0;
				if(T[i].E_T==0){
					printf("%d task%d %d\n", Time+1-same_task, T[i].ID, Time+1);
					total_W_T += T[i].W_T;
					total_CPU_T += T[i].CPU_T;
					T.erase(T.begin()+i);
					same_task = 1;
					erase = 1;
				}
			}
			else if(cur_num!=old_num && erase == 0){
				printf("%d task%d %d\n", Time-same_task, old_id, Time);
				same_task = 1;
				if(T[i].E_T==0){
					printf("%d task%d %d\n", Time, T[i].ID, Time+1);
					total_W_T += T[i].W_T;
					total_CPU_T += T[i].CPU_T;
					T.erase(T.begin()+i);
					same_task = 1;
					erase = 1;
				}
			}
			else if(T[i].E_T==0){
				printf("%d task%d %d\n", Time, T[i].ID, Time+1);
				total_W_T += T[i].W_T;
				total_CPU_T += T[i].CPU_T;
				T.erase(T.begin()+i);
				same_task = 1;
				erase = 1;
			}
			Time++;
		}

		else{
			idle++;
			Time++;	
		}
		std::sort(T.begin(), T.end(), compare_slack_time);
	}
	printf("Average Waiting Time = %.2f\n", (double)total_W_T/total_task);
	printf("CPU = %2f\n",(double)(Time-idle)/Time);

}

void print_process(int id, int ab){

	printf("processor %d: ability: %d\n", id, ab);	

}

void print_task(int id, int R_T, int E_T, int deadline, int period, int preemption, int task_type){

	printf("task %d: release time: %d, execuion time:%d, deadline: %d, period: %d, preemtion: %d, type: %d\n",id, R_T, E_T, deadline, period, preemption, task_type);

}

int gcd(int a, int b){
	for(;;){

		if(a==0) return b;
		b%=a;
		if(b==0) return a;
		a%=b;

	}
}

int lcm(int a, int b){

	int temp = gcd(a, b);
	return temp ?(a/temp*b):0;

}


bool compare_id(const task &a, const task &b){

	return a.ID<b.ID;
}

bool compare_deadline(const task &a, const task &b){

	return a.deadline<b.deadline;
}

bool compare_precedence(int &a, int &b){
	return a<b;
}

bool compare_slack_time(const task &a, const task &b){
	return (a.deadline-Time-a.E_T)<(b.deadline-Time-b.E_T);
}
