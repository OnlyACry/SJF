//实验一，SJF调度算法
//计算机科学与技术1901-陈润尧
//写的很急，有空再优化

#include <iostream>
#include<queue>

using namespace std;

struct Work
{
	string ProcessName;		//作业名 
	int ArriveTime;			//到达时间 
	int ServiceTime;		//服务时间 
	string State;			//状态 

	int RemainTime;			//剩余时间 
	int Complete;			//完成时间 
	int AroundTime;			//周转时间
	double AveAroundTime;		//带权周转时间 

}works[200];

queue<struct Work> work_que;
queue<struct Work> all_work;

void ProcessRun(int n);
void DisplayWorkMassage(int n);
void SortByTime(int n);
void CalcTime(int n);

int main(int argc, char** argv)
{
	int n;  //作业数
	int max = INT_MAX;
	cout << "请输入要完成的作业数" << endl;
	cin >> n;

	cout << "依次输入作业的 名字、到达时间、服务时间。用空格隔开" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> works[i].ProcessName >> works[i].ArriveTime >> works[i].ServiceTime;
		works[i].RemainTime = works[i].ServiceTime;
		works[i].State = "W";
		all_work.push(works[i]);
	}

	ProcessRun(n);

	return 0;
}

/// <summary>
/// 排序
/// </summary>
/// <param name="n"></param>
void SortByTime(int n)
{
	int m = n;
	struct Work t;

	for (int i = 0; i < n; i++) {
		m--;
		for (int j = 0; j < m; j++) {
			if (works[j].ArriveTime > works[j + 1].ArriveTime) {
				t = works[j];
				works[j] = works[j + 1];
				works[j + 1] = t;
			}
			else if (works[j].ArriveTime == works[j + 1].ArriveTime) {
				if (works[j].ProcessName > works[j + 1].ProcessName) {
					t = works[j];
					works[j] = works[j + 1];
					works[j + 1] = t;
				}
			}
		}
	}
}

/// <summary>
/// 模拟运行
/// </summary>
/// <param name="n"></param>
void ProcessRun(int n)
{
	bool flag = true;		//处理机是否空闲 
	int time = 0, num = 0, nowtime = 0;
	SortByTime(n);

	DisplayWorkMassage(n);	//时刻0的作业状态 

	works[0].State = "R";
	flag = false;

	while (all_work.size() > 0)
	{
		nowtime++;
		for (int i = num; i < n; i++)
		{
			if (works[i].ArriveTime <= nowtime)
			{
				//if (flag == false && num != i)
				if (num != i)
				{
					bool f = false;
					works[i].State = "W";

					//判断等待队列是否已存在此作业
					for (int si = 0; si < work_que.size(); si++) {
						Work w = work_que.front();
						if (works[i].ProcessName == w.ProcessName) f = true;
						work_que.push(work_que.front());
						work_que.pop();
					}

					if(!f) work_que.push(works[i]);
				}
			}
		}

		if (flag == true && work_que.size() != 0) {
			work_que.pop();
			num++;
			flag = false;
			works[num].State = "R";
			continue;
		}

		if (works[num].State == "R")
		{
			works[num].RemainTime--;		//当前作业的剩余时间减一 
			//当前作业将执行结束,如果等待队列不空，再转换状态 
			if (works[num].RemainTime == 0)
			{
				all_work.pop();

				//加一个等待队列，判断是否为空 
				//不为空，将等待队列队首弹出，num++，指向下一个待运行作业
				if (work_que.size() != 0) {
					work_que.pop();
					num++;
					flag = false;
					works[num].State = "R";
					works[num - 1].State = "F";	//当前运行作业状态为结束 
					works[num - 1].Complete = nowtime;	//记录当前作业的完成时间、周转时间
					works[num - 1].AroundTime = nowtime - works[num - 1].ArriveTime;
					works[num - 1].AveAroundTime = (1.0) * works[num - 1].AroundTime / works[num - 1].ServiceTime;
				}
				else
				{
					flag = true;
					works[num].State = "F";	//当前运行作业状态为结束 
					works[num].Complete = nowtime;	//记录当前作业的完成时间、周转时间
					works[num].AroundTime = nowtime - works[num].ArriveTime;
					works[num].AveAroundTime = (1.0) * works[num].AroundTime / works[num].ServiceTime;
				}

				DisplayWorkMassage(n);
			}
		}
		
	}
	CalcTime(n);
	DisplayWorkMassage(n);
}

/// <summary>
/// 显示作业信息
/// </summary>
/// <param name="n"></param>
void DisplayWorkMassage(int n)
{
	cout << "进程名\t" << "到达时间\t" << "服务时间\t" << "结束时间\t" << "周转时间\t" << "带权周转时间\t" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << works[i].ProcessName << "\t\t" << works[i].ArriveTime << "\t\t" << works[i].ServiceTime << "\t\t" << works[i].Complete << "\t\t" << works[i].AroundTime << "\t\t" << works[i].AveAroundTime << "\t\t" << endl;
	}
}

/// <summary>
/// 计算带权周转时间
/// </summary>
/// <param name="n"></param>
void CalcTime(int n)
{
	for (int i = 0; i < n; i++) {
		works[i].AveAroundTime = (1.0) * works[i].AroundTime / works[i].ServiceTime;
	}
}