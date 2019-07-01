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
bool compare_execute_time(const task &a, const task &b);

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
		if(T[i].period!= 0){
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
	
	int F1 = 0;
	for(int i=0;i<n;i++){
		if(F1<T[i].E_T){
			F1 = T[i].E_T;
		}
	}
	
	int F2 = F1;
	vector<int> frame_gcd;
	for(int i=F2;i<=LCM;i++){
		if(LCM%i==0){
			frame_gcd.push_back(i);
		}
	}
	
	int frame=0;
	for(int i=0;i<frame_gcd.size();i++){
		frame = frame_gcd[i];
		int times = 0;
		for(int j=0;j<n;j++){
			if(2*frame-gcd(T[j].period,frame)<=T[j].deadline){
			
				times++;
			}
		}
		if(times==n)
			break;
	}
	printf("frame = %d\n", frame);
	

	//std::sort(T.begin(),T.end(), compare_id);
	std::sort(T.begin(),T.end(), compare_execute_time);
	
//	for(int i=0;i<T.size();i++){
//		print_task(T[i].ID, T[i].R_T, T[i].E_T, T[i].deadline, T[i].period, T[i].preemption, T[i].task_type);
//	
//	}
	
	int F_T = frame;
	int Time = 0;
	int total_w = 0;
	int total_t = T.size();
	int idle = 0;
	while(!T.empty() || Time<LCM){
		
		int cur_t = -1;
		int cur_num = -1;
		for(int i=0;i<T.size();i++){
			if(T[i].R_T<=Time && T[i].period!=0){
				cur_t = i;
				cur_num = T[i].num;
				break;
			}
		}
		
//		printf("cur= %d, F_T = %d\n", cur_t, F_T);
		if(cur_t>=0){
			F_T-=T[cur_t].E_T;
			if(F_T>=0){
//				printf("%d %d\n", cur_t, cur_num);
				printf("%d task%d %d\n", Time, T[cur_t].ID, Time+T[cur_t].E_T); 
				T[cur_t].W_T-=T[cur_t].E_T;
				T[cur_t].W_T+=(Time-T[cur_t].R_T);
				Time+=T[cur_t].E_T;
				total_w+=T[cur_t].W_T;
				T.erase(T.begin()+cur_t);
				if(F_T==0){
					F_T = frame;
				}
				
			}
			else if(F_T<0){//this task is out of frame
				F_T += T[cur_t].E_T;
				Time++;
				F_T--;
			}
		}
		else if(cur_t<0){ //waiting for task
			int aper = -1;
			for(int i=0;i<T.size();i++){
				if(T[i].R_T<=Time && T[i].period==0&&T[i].E_T<=F_T){
					aper = i;
					break;
				}
			}
			if(aper>0){
				F_T-=T[aper].E_T;
//				printf("%d %d\n", cur_t, cur_num);
				printf("%d task%d %d\n", Time, T[aper].ID, Time+T[aper].E_T);
				Time+=T[aper].E_T; 
				T[aper].W_T-=T[aper].E_T;
				T[aper].W_T+=(Time-T[aper].R_T);
				total_w+=T[aper].W_T;
				T.erase(T.begin()+aper);
				if(F_T==0){
					F_T = frame;
				}
			}
			else{
				Time++;
				F_T--;
				idle++;
				if(F_T==0){
					F_T = frame;
				}
				for(int w=0;w<T.size();w++){
					T[w].W_T++;	
				}	
			}
			
		}
		
	
	}
	
	printf("Average waiting time = %.2f\n", (double)total_w/total_t);
	printf("CPU utilization %.2f\n", 1-(double)idle/Time);
//	printf("%d\n", idle);
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

bool compare_execute_time(const task &a, const task &b){
	return (a.E_T)<(b.E_T);
}
