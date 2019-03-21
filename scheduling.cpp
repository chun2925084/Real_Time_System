#include<iostream>
#include<stdio.h>
#include<vector>
	
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

};

void print_process(int id, int ab);
void print_task(int id, int R_T, int E_T, int deadline, int period, int preemption, int task_type);

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
	for(count=0;count<n;count++){

		scanf("%d %d %d %d %d %d %d\n", &t.ID, &t.R_T, &t.E_T, &t.deadline, &t.period, &t.preemption, &t.task_type);
		print_task(t.ID, t.R_T, t.E_T, t.deadline, t.period, t.preemption, t.task_type);
		T.push_back(t);
	}

	int Time = 0;
	int first = 0;
	int min = 0;
	vector<task>:: iterator cur = T.begin();
	int total = 0;

	first = T[0].R_T;
	min = T[0].E_T;
	total = T[0].E_T;
	

	/* Find the first process by release tome*/
	for(vector<task>::iterator iter = T.begin();iter!=T.end();iter++){
	
		if(first>iter->R_T){
			cur = iter;
			printf("%d\n", T.begin()->ID);
			first = iter->R_T;
		}
	
	}
	
	/*do the first job*/
	Time = cur->E_T;
	printf("%d task%d %d\n", Time-cur->E_T, cur->ID, Time);
	T.erase(cur);

	min = T.begin()->E_T;
	cur = T.begin();

	while(!T.empty()){
	
		/* Find the next task to execute */
		min = T.begin()->E_T;
		cur = T.begin();
		for(vector<task>::iterator iter=T.begin();iter!=T.end();iter++){
			if(min>iter->E_T && iter->R_T<Time){
				min = iter->E_T;
				cur = iter;
			}	
		}

		Time+=cur->E_T;
		printf("%d task%d %d\n", Time-cur->E_T, cur->ID, Time);
		T.erase(cur);
	
	}

}

void print_process(int id, int ab){

	printf("processor %d: ability: %d\n", id, ab);	

}

void print_task(int id, int R_T, int E_T, int deadline, int period, int preemption, int task_type){

		printf("task %d: release time: %d, execuion time:%d, deadline: %d, period: %d, preemtion: %d, type: %d\n",id, R_T, E_T, deadline, period, preemption, task_type);

} 
