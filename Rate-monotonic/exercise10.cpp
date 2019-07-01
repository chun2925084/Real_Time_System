#include<iostream>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include<numeric>
#include<math.h>

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
	float U;
	float rate;

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
bool compare_rate(const task &a, const task &b);

int total = 0;
int Time = 0;

int main(){


	vector<task> T;
	task t;
	vector<int> schedule_list;
	
	bool schedule=0;
	
	freopen("input.txt","r",stdin);
	int m = 0;//process
	int n = 0;//task
	int count = 0;
	
	while(schedule==0 && cin>>m>>n){
		
//		cin >> m >> n;
		printf("number of process: %d\n", m);	
		printf("number of task: %d\n", n);	
		vector<processor> P;
		processor p;
		for(count=0;count<m;count++){
			scanf("%d %d\n", &p.id, &p.ability);
			print_process(p.id, p.ability);
			P.push_back(p);
	
		}
	
		vector<task> temp_T;
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
			t.U = (double)t.E_T/(double)t.period;
			printf("U = %f\n",t.U );
			t.rate = 1/(double)t.period;
			temp_T.push_back(t);
		}
		
		double total_U = 0;
		for(int i=0;i<temp_T.size();i++){
			total_U += temp_T[i].U;
		}
		printf("U = %f\n",total_U );
		
//		double U_RM = 1;
		double U_RM = temp_T.size()*(pow(2,1/(double)temp_T.size())-1);
		printf("U_RM = %f\n",U_RM );
		
		if(total_U<=U_RM){
			printf("schedulable\n");
			schedule=1;
			schedule_list.push_back(schedule);
			T = temp_T;
		}
		else{
			schedule_list.push_back(schedule);
			int sep;
			cin>>sep;
		}
//		break;
	}
	
	printf("ID = %d\n",T[0].ID);

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
	std::sort(T.begin(),T.end(), compare_rate);
	
	for(int i=0;i<T.size();i++){
		print_task(T[i].ID, T[i].R_T, T[i].E_T, T[i].deadline, T[i].period, T[i].preemption, T[i].task_type);
	
	}
	
	int F_T = frame;
	int Time = 0;
	int total_w = 0;
	int total_t = T.size();
	int idle = 0;
	int cur_num = 0,old_num =0;
	int cur_id = 0,old_id =0;
	int flag = 0;
	int same_task = 0;
	int erase = 0;
	int total_W_T = 0;
	int total_CPU_T = 0;
	int total_task = T.size();
	
	while(!T.empty() || Time<LCM){
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
//				for(int y=0;y<precedence.size();y++){
//					if(get_it){
//						if(T[i].ID==precedence[y]){
//							for(int x=0;x<y;x++){
//								for(int u=0;u<T.size();u++){
//									if(T[u].ID==precedence[x]){
//										get_it=0;
//									}	
//								}
//							}
//						}
//					}
//				}
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
	
	int sep = 0;
	cin>>sep;
	
	while(cin>>m>>n){
		
////		cin >> m >> n;
//		printf("number of process: %d\n", m);	
//		printf("number of task: %d\n", n);	
		vector<processor> P;
		processor p;
		for(count=0;count<m;count++){
			scanf("%d %d\n", &p.id, &p.ability);
//			print_process(p.id, p.ability);
			P.push_back(p);
	
		}
	
		vector<task> temp_T;
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
//			print_task(t.ID, t.R_T, t.E_T, t.deadline, t.period, t.preemption, t.task_type);
			t.U = (double)t.E_T/(double)t.period;
//			printf("U = %f\n",t.U );
			t.rate = 1/(double)t.period;
			temp_T.push_back(t);
		}
		
		double total_U = 0;
		for(int i=0;i<temp_T.size();i++){
			total_U += temp_T[i].U;
		}
//		printf("U = %f\n",total_U );
		
//		double U_RM = 1;
		double U_RM = temp_T.size()*(pow(2,1/(double)temp_T.size())-1);
//		printf("U_RM = %f\n",U_RM );
		
		if(total_U<=U_RM){
//			printf("schedulable\n");
			schedule=1;
			schedule_list.push_back(schedule);
		}
		else{
//			printf("4444\n");
			schedule_list.push_back(0);
			int sep;
			cin>>sep;
		}
//		break;
	}
	
	for(int i=0;i<schedule_list.size();i++){
		if(schedule_list[i]==1){
			printf("case%d: schedulable\n", i);
		}
		if(schedule_list[i]==0){
			printf("case%d: not schedulable\n", i);
		}
	}
	
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

bool compare_rate(const task &a, const task &b){
	return (a.rate)>(b.rate);
}
