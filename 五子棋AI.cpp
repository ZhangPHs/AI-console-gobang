#include<iostream>
#include<vector>
#include<Windows.h>
#include<math.h>

using namespace std;

#define White 1//白棋为1，●
#define WhiteShape "●"
#define Black -1//黑棋为-1，★
#define BlackShape "★"
#define BaseShape "十"
#define Base 0

#define MIN -100000000
#define MAX 100000000

#define DEPTH 3

int Five = 10000000;
int liveFour = 100000;
int sleepFour = 10000;
int liveThree = 1000;
int liveTwo = 100;
int sleepThree = 100;
int liveOne = 10;
int sleepTwo = 10;
int unknown = 0;

class point{
public:
	int i, j;
	point(){}
	point(int a, int b){
		i = a;
		j = b;
	}
};

class Checkboard
{
public:
	int Score;
	int TScore;
	vector<vector<int> > G;
	int n;
	int depth;
	Checkboard(int N){
		depth = DEPTH;
		TScore = 0;
		Score = 0;
		this->n = N;
		int i;
		G.resize(n + 1);
		for (i = 0; i <= n; i++)
			G[i].resize(n + 1);
	}

	int minF(int deep, int alpha, int beta);	//MIN层搜索
	int maxF(int deep, int alpha, int beta);	//MAX层搜索
	int getCScore();	//得到AI方分数
	int getCScoreT();	//得到玩家方分数
	void getOneStrScore(int length, int state);	//后面的状态表示眠和活
	void getOneStrScoreT(int length, int state);	//玩家方

	void setcheckboard();	//设置棋盘
	void showcurrent();		//展示当前棋局
	void play();			//玩家下棋
	void cplay();			//计算机下棋,core
	int judge();			//判定胜负
	void success();			//胜利
	void fail();			//失败
	void resetAll();		//重置棋局

	vector<point> genpoints();	//生成适合落子的位置集合
	point maxmin(int deep);		//极大极小值算法实现
	int getTotalScore(){
		int score = getCScore();
		int tscore = getCScoreT();
		Score = 0;
		TScore = 0;
		return score + tscore;
	}		//获取当前AI方分数，即叶子节点指代的棋局中，各种状态评分之和。   
};

void Checkboard::setcheckboard()
{
	int i, j;
	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			G[i][j] = Base;
		}
	}
}

bool pan(vector<point> &t, int i, int j){	
	int k;
	for (k = 0; k < t.size(); k++){
		if (t[k].i == i && t[k].j == j){
			return false;
		}
	}
	return true;
}

vector<point> Checkboard::genpoints(){
	vector<point> t;
	int i, j;
	bool flag = false;	//是否先手
	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			if (G[i][j] == Base)continue;	
			else{
				flag = true;
				if (i == 1 || j == 1) ;
				else{
					if (G[i - 1][j - 1] == Base && pan(t, i - 1, j - 1))	t.push_back(point(i - 1, j - 1));
				}
				if (i == 1);
				else{
					if (G[i - 1][j] == Base && pan(t, i - 1, j))t.push_back(point(i - 1, j));
				}
				if (i == 1 || j == n) ;
				else{
					if (G[i - 1][j + 1] == Base && pan(t, i - 1, j + 1))t.push_back(point(i - 1, j + 1));
				}
				if (j == 1);
				else{
					if (G[i][j - 1] == Base && pan(t, i, j - 1))t.push_back(point(i, j - 1));
				}
				if (j == n);
				else{
					if (G[i][j + 1] == Base && pan(t, i, j + 1))t.push_back(point(i, j + 1));
				}
				
				if (i == n || j == 1)	;
				else{
					if (G[i + 1][j - 1] == Base && pan(t, i + 1, j - 1))t.push_back(point(i + 1, j - 1));
				}
				if (i == n);
				else{
					if (G[i + 1][j] == Base && pan(t, i + 1, j ))t.push_back(point(i + 1, j));
				}
				if (i == n || j == n);
				else{
					if (G[i + 1][j + 1] == Base && pan(t, i + 1, j + 1))t.push_back(point(i + 1, j + 1));
				}
			}
		}
	}
	if (flag == false)
		t.push_back(point(n / 2, n / 2));

	return t;
}

int Checkboard::maxF(int deep,int alpha, int beta){
	int grades = getTotalScore();
	//返回当前估分
	if (deep <= 0 || judge() != 0)	return grades;//此时已经到叶子节点了,或者已经产生连五
	int best = MIN;
	int i, j;
/*	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			point p(i, j);
			if (G[p.i][p.j] != Base) continue;
			G[p.i][p.j] = Black;
			int v = minF(deep - 1,alpha, best > beta? best:beta);
			G[p.i][p.j] = Base;
			if (v > best)	best = v;
			if (v > alpha)	return best;//剪枝
		}
	}
*/
	vector<point> t = genpoints();
	for (i = 0; i < t.size(); i++){
		point p(t[i].i, t[i].j);
		if (G[p.i][p.j] != Base) continue;
		G[p.i][p.j] = Black;	//尝试下子
		int v = minF(deep - 1,alpha, best > beta ? best : beta);
		G[p.i][p.j] = Base;	//取消掉下的棋子
		if (v > best)	best = v;
		if (v > alpha)	return best;	//剪枝。若找到比alpha更大的值，则剩下节点无需再考量。

	}
	if (best < alpha)	alpha = best;
	return best;
}

int Checkboard::minF(int deep, int alpha, int beta){
	int grades = getTotalScore();
	//返回当前估分
	if (deep <= 0 || judge() != 0)	return grades;//此时已经到叶子节点，或者已经产生连五！
	
	int best = MAX;
	int i, j;
/*
	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			point p(i, j);
			if (G[p.i][p.j] != Base) continue;
			G[p.i][p.j] = White;	//尝试下子
			int v = maxF(deep - 1, best < alpha ? best : alpha, beta);
			G[p.i][p.j] = Base;	//取消掉下的棋子
			if (v < best)	best = v;
			if (v < beta)	return best;//剪枝
		
		}
	}
*/
	vector<point> t = genpoints();
	for (i = 0; i < t.size(); i++){
		point p(t[i].i, t[i].j);
			if (G[p.i][p.j] != Base) continue;
			G[p.i][p.j] = White;	//尝试下子
			int v = maxF(deep - 1, best < alpha ? best : alpha, beta);
			G[p.i][p.j] = Base;	//取消掉下的棋子
			if (v < best)	best = v;
			if (v < beta)	return best;	//剪枝。若已经找到比beta小的落子，则剩下节点无需再考虑。

		}
	if (best > beta) beta = best;
	return best;
}

point Checkboard::maxmin(int deep){//deep是预设思考层数，参数为4
	int i, j;
	int best = MIN;
	int alpha = MAX, beta = MIN;
	vector<point> points;

/*	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			point p = point(i, j);
			if (G[p.i][p.j] != Base) continue;
			G[i][j] = Black;
			//落子
			int v = minF(deep - 1, alpha, beta);//找最大值
			if (v == best)	points.push_back(p);
			else if (v > best){	//这是选最大的
				best = v;
				points.clear();
				points.push_back(p);
			}
			G[i][j] = Base;	//取消下的子
		}
	}
*/
	vector<point> t = genpoints();
	for (i = 0; i < t.size(); i++){
		point p(t[i].i, t[i].j);
		if (G[p.i][p.j] != Base) continue;		//若该位置已被落子，则跳过
		G[p.i][p.j] = Black;					//尝试落子
		int v = minF(deep - 1, alpha, beta);	//在min层节点中找最大值
		if (v == best)	points.push_back(p);	//考虑存在多种落子法得到分数相同，将这些位置放入集合points
		else if (v > best){					//若找到比当前最大值best更大的，则更新best，同时重置之前集合points
			best = v;
			points.clear();
			points.push_back(p);
		}
		G[p.i][p.j] = Base;					//还原	

	}

	int len = points.size();
	return points[rand() % len];			//对于分数相同的多种落子法，随机选择一种落子。
}

void Checkboard::cplay()
{//计算机运算,算法核心。

//评估函数
//连五、活四、眠四、活三、活二、眠三、活一、眠二
//注意眠的意思是，一侧被封死，另一侧没有。而不是直接两侧锁死。直接锁死就不考虑了。
	//单个棋子就不说了

/*输入：当前棋盘。设定思考深度为4.
	1.获得可以落子的位置集合。
	2.对每一个位置，假设落子，再假设敌方落子，再假设我方落子。遍历树。
	3.计算落子后的分数

输出：完成落子。
*/	
	point p = maxmin(depth);
	G[p.i][p.j] = Black;
}

void Checkboard::getOneStrScore(int length, int state){
//	cout << "调用！此时length" << length << "状态" << state << endl;
//	cout <<"score"<< Score << endl;
	if (length == 5)	Score += Five;
	if (state == 0){
		//眠
		switch (length)
		{
		case 4:
			Score += sleepFour;
			break;
		case 3:
			Score += sleepThree;
			break;
		case 2:
			Score += sleepTwo;
			break;
		default:
			Score += 0;
			break;
		}
	}
	else if(state == 1) {
		//活
		switch (length)
		{
			case 4:
				Score += liveFour;
				break;
			case 3:
				Score += liveThree;
				break;
			case 2:
				Score += liveTwo;
				break;
			case 1:
				Score += liveOne;
				break;
			default:
				Score += 0;
				break;
		}
	}
	else{
		//死
		Score += 0;
	}
}

void Checkboard::getOneStrScoreT(int length, int state){
	//	cout << "调用！此时length" << length << "状态" << state << endl;
	//	cout <<"score"<< Score << endl;
	if (length == 5) TScore -= Five;
	if (state == 0){
		//眠
		switch (length)
		{
		case 4:
			TScore -= sleepFour;
			break;
		case 3:
			TScore -= sleepThree;
			break;
		case 2:
			TScore -= sleepTwo;
			break;
		default:
			TScore -= 0;
			break;
		}
	}
	else if (state == 1) {
		//活
		switch (length)
		{
		case 4:
			TScore -= liveFour;
			break;
		case 3:
			TScore -= liveThree;
			break;
		case 2:
			TScore -= liveTwo;
			break;
		case 1:
			TScore -= liveOne;
			break;
		default:
			TScore -= 0;
			break;
		}
	}
	else{
		//死
		TScore -= 0;
	}
}

int Checkboard::getCScore(){
	//输入是当前的棋局G
	//输出当前计算机（black）方的棋子评估的分数
	//将每一行、每一列、每一斜进行处理么。
	int i, j;
	for (i = 1; i <= this->n; i++){// for each row.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;
		for (j = 1; j <= this->n; j++){// cols.
			point lastone(i,j-1);
			if (flag == true){
				if (j == this->n){
					switch (G[i][j]){
						case Black:{
							rightState = false;
							length++;
							if (leftState == true) {
								tmpState = 0;
								getOneStrScore(length, tmpState);
							}
							else{
								tmpState = -1;
								getOneStrScore(length, tmpState);
							}
							length = 0;
							flag = false;
							break;
						}
						case White:{
							rightState = false;
							if (leftState == true) {
								tmpState = 0;
								getOneStrScore(length, tmpState);
							}
							else{
								tmpState = -1;
								getOneStrScore(length, tmpState);
							}
							length = 0;
							flag = false;
							break;
						}
						case Base:{
							rightState = true;
							if (leftState == true) {
								tmpState = 1;
								getOneStrScore(length, tmpState);
							}
							else{
								tmpState = 0;
								getOneStrScore(length, tmpState);
							}
							length = 0;
							flag = false;
							break;
						}
						default:break;
					}
				}
				else{
					switch (G[i][j]){
						case Black:{
							length++;
//							cout << "遇到黑棋了" << endl;
							break;
						}
						case White:{
							rightState = false;
							if (leftState == true) {
								tmpState = 0;
								getOneStrScore(length, tmpState);
							}
							else{
								tmpState = -1;
								getOneStrScore(length, tmpState);
							}
							length = 0;
							flag = false;
							break;
						}
						case Base:{
							rightState = true;
							if (leftState == true) {
								tmpState = 1;
								getOneStrScore(length, tmpState);
							}
							else{
								tmpState = 0;
								getOneStrScore(length, tmpState);
							}
							length = 0;
							flag = false;
							break;
						}
						default:break;
					}
				}

			}
			else{//flag = false
				if (j == 1){
					switch (G[i][j]){
						case Black:{
							leftState = false;
							length++;
							flag = true;
							break;
						}
						case White:{
							leftState = false;
							break;
						}
						case Base:{
							leftState = true;
							break;
						}
						default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == White){
						leftState = false;
						switch (G[i][j]){
							case Black:{
								length++;
								if (j == this->n){
									rightState = false;
									if (leftState == true) {
										tmpState = 0;
										getOneStrScore(length, tmpState);
									}
									else{
										tmpState = -1;
										getOneStrScore(length, tmpState);
									}
									length = 0;
									flag = false;
								}
								else{
									flag = true;
								}
								break;
							}
							case Base:{
								leftState = true;
								break;
							}
							default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
							case Black:{
								length++;
								if (j == this->n){
									rightState = false;
									if (leftState == true) {
										tmpState = 0;
										getOneStrScore(length, tmpState);
									}
									else{
										tmpState = -1;
										getOneStrScore(length, tmpState);
									}
									length = 0;
									flag = false;
								}
								else{
									flag = true;
								}
								break;
							}
							case White:{
								leftState = false;
							}
						}
					}

				}
			}
			
		}
	}

	/*********************************************************************************************************************/


	//****cols 
	for (j = 1; j <= this->n; j++){// for each row.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;
		for (i = 1; i <= this->n; i++){// cols.
			point lastone(i - 1, j);
			if (flag == true){
				if (i == this->n){
					switch (G[i][j]){
					case Black:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case Black:{
								   length++;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (i == 1){
					switch (G[i][j]){
					case Black:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case White:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == White){
						leftState = false;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case White:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
	}



	/*********************************************************************************************************************/


	//左斜
	int k;
	for (k = 1; k <= this->n; k++){// for n times circle.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;
		
		for (i = 1, j = this->n - k + 1; j <= this->n; ++j, ++i){// 一半棋局！从右上角刷到半中央
//			cout << "左斜判定" << endl;
			if (j >= this->n - 4 + i) break;
			point lastone(i - 1, j - 1);
//			cout << "当前点" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//当前是右边缘
					switch (G[i][j]){
					case Black:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case Black:{
								   length++;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (i == 1){//注意数值边界和横向边界的不同判定方法。
					switch (G[i][j]){
					case Black:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case White:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == White){
						leftState = false;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case White:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
		length = 0;
		leftState = false;
		rightState = false;
		flag = false;
		tmpState = 0;

		for (i = this->n - k + 1, j = 1; i >= 1 && i<= this->n; j++, i++){// 一半棋局！从左下角刷到中央
			if (j <= i - (this->n - 4)) break;
			if (i == 1 && j == 1)break;
			point lastone(i - 1, j - 1);
//			cout << "另一半，当前点" << i << "," << j << endl;
			if (flag == true){
				if (i == this->n){//当前是右边缘
					switch (G[i][j]){
					case Black:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case Black:{
								   length++;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (j == 1){
					switch (G[i][j]){
					case Black:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case White:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == White){
						leftState = false;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case White:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
	}

	/*******************************************************************************/
	//右斜
	for (k = 1; k <= this->n; k++){// for n times circle.
//		cout << "开始右斜**********************************" << endl;
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;

		for (i = k, j = 1; i >= 1 && i <= this->n; j++, i--){// 一半棋局！从左上角刷到半中央
			if (j <= (-i + 5)) break;
			point lastone(i + 1, j - 1);
//			cout << "当前点" << i << "," << j << endl;
			if (flag == true){
				if (i == 1){//当前是右边缘
					switch (G[i][j]){
					case Black:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case Black:{
								   length++;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (j == 1){//注意数值边界和横向边界的不同判定方法。
					switch (G[i][j]){
					case Black:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case White:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == White){
						leftState = false;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (i == 1){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (i == 1){//右边缘
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case White:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
		length = 0;
		leftState = false;
		rightState = false;
		flag = false;
		tmpState = 0;

		for (i = this-> n, j = this->n - k + 1; j <=this->n && i <= this->n && i>= 1 && j >= 1; j++, i--){// 一半棋局！从半中央刷到右下角
			if (i >= this->n - 3 && j >= this->n - 3) break;
			if (i == this->n && j == 1)	break;
			point lastone(i - 1, j - 1);
//			cout << "另一半，当前点" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//当前是右边缘
					switch (G[i][j]){
					case Black:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case Black:{
								   length++;
								   break;
					}
					case White:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScore(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScore(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScore(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScore(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (i == this->n){//左边缘
					switch (G[i][j]){
					case Black:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case White:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == White){
						leftState = false;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (j = this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case Black:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScore(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScore(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case White:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
	}
	
	return Score;
}
int Checkboard::getCScoreT(){
	//输入是当前的棋局G
	//输出当前己（white）方的棋子评估的分数
	//将每一行、每一列、每一斜进行处理么。
	int i, j;
	for (i = 1; i <= this->n; i++){// for each row.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;
		for (j = 1; j <= this->n; j++){// cols.
			point lastone(i, j - 1);
			if (flag == true){
				if (j == this->n){
					switch (G[i][j]){
					case White:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case White:{
								   length++;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (j == 1){
					switch (G[i][j]){
					case White:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case Black:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == Black){
						leftState = false;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Black:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
	}

	/*********************************************************************************************************************/


	//****cols 
	for (j = 1; j <= this->n; j++){// for each row.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;
		for (i = 1; i <= this->n; i++){// cols.
			point lastone(i - 1, j);
			if (flag == true){
				if (i == this->n){
					switch (G[i][j]){
					case White:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case White:{
								   length++;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (i == 1){
					switch (G[i][j]){
					case White:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case Black:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == Black){
						leftState = false;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Black:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
	}



	/*********************************************************************************************************************/


	//左斜
	int k;
	for (k = 1; k <= this->n; k++){// for n times circle.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;

		for (i = 1, j = this->n - k + 1; j <= this->n; ++j, ++i){// 一半棋局！从右上角刷到半中央
			//			cout << "左斜判定" << endl;
			if (j >= this->n - 4 + i) break;
			point lastone(i - 1, j - 1);
			//			cout << "当前点" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//当前是右边缘
					switch (G[i][j]){
					case White:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case White:{
								   length++;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (i == 1){//注意数值边界和横向边界的不同判定方法。
					switch (G[i][j]){
					case White:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case Black:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == Black){
						leftState = false;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Black:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
		length = 0;
		leftState = false;
		rightState = false;
		flag = false;
		tmpState = 0;

		for (i = this->n - k + 1, j = 1; i >= 1 && i <= this->n; j++, i++){// 一半棋局！从左下角刷到中央
			if (j <= i - (this->n - 4)) break;
			if (i == 1 && j == 1)break;
			point lastone(i - 1, j - 1);
			//			cout << "另一半，当前点" << i << "," << j << endl;
			if (flag == true){
				if (i == this->n){//当前是右边缘
					switch (G[i][j]){
					case White:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case White:{
								   length++;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (j == 1){
					switch (G[i][j]){
					case White:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case Black:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == Black){
						leftState = false;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (i == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Black:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
	}

	/*******************************************************************************/
	//右斜
	for (k = 1; k <= this->n; k++){// for n times circle.
		//		cout << "开始右斜**********************************" << endl;
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;

		for (i = k, j = 1; i >= 1 && i <= this->n; j++, i--){// 一半棋局！从左上角刷到半中央
			if (j <= (-i + 5)) break;
			point lastone(i + 1, j - 1);
			//			cout << "当前点" << i << "," << j << endl;
			if (flag == true){
				if (i == 1){//当前是右边缘
					switch (G[i][j]){
					case White:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case White:{
								   length++;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (j == 1){//注意数值边界和横向边界的不同判定方法。
					switch (G[i][j]){
					case White:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case Black:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == Black){
						leftState = false;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (i == 1){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (i == 1){//右边缘
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Black:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
		length = 0;
		leftState = false;
		rightState = false;
		flag = false;
		tmpState = 0;

		for (i = this->n, j = this->n - k + 1; j <= this->n && i <= this->n && i >= 1 && j >= 1; j++, i--){// 一半棋局！从半中央刷到右下角
			if (i >= this->n - 3 && j >= this->n - 3) break;
			if (i == this->n && j == 1)	break;
			point lastone(i - 1, j - 1);
			//			cout << "另一半，当前点" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//当前是右边缘
					switch (G[i][j]){
					case White:{
								   rightState = false;
								   length++;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}
				else{
					switch (G[i][j]){
					case White:{
								   length++;
								   break;
					}
					case Black:{
								   rightState = false;
								   if (leftState == true) {
									   tmpState = 0;
									   getOneStrScoreT(length, tmpState);
								   }
								   else{
									   tmpState = -1;
									   getOneStrScoreT(length, tmpState);
								   }
								   length = 0;
								   flag = false;
								   break;
					}
					case Base:{
								  rightState = true;
								  if (leftState == true) {
									  tmpState = 1;
									  getOneStrScoreT(length, tmpState);
								  }
								  else{
									  tmpState = 0;
									  getOneStrScoreT(length, tmpState);
								  }
								  length = 0;
								  flag = false;
								  break;
					}
					default:break;
					}
				}

			}
			else{//flag = false
				if (i == this->n){//左边缘
					switch (G[i][j]){
					case White:{
								   leftState = false;
								   length++;
								   flag = true;
								   break;
					}
					case Black:{
								   leftState = false;
								   break;
					}
					case Base:{
								  leftState = true;
								  break;
					}
					default:break;
					}
				}
				else{
					if (G[lastone.i][lastone.j] == Black){
						leftState = false;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (j = this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Base:{
									  leftState = true;
									  break;
						}
						default:break;
						}
					}
					else if (G[lastone.i][lastone.j] == Base){
						leftState = true;
						switch (G[i][j]){
						case White:{
									   length++;
									   if (j == this->n){
										   rightState = false;
										   if (leftState == true) {
											   tmpState = 0;
											   getOneStrScoreT(length, tmpState);
										   }
										   else{
											   tmpState = -1;
											   getOneStrScoreT(length, tmpState);
										   }
										   length = 0;
										   flag = false;
									   }
									   else{
										   flag = true;
									   }
									   break;
						}
						case Black:{
									   leftState = false;
						}
						}
					}

				}
			}

		}
	}

	return TScore;
}

void Checkboard::play()
{
	int x, y;
	while (1){
		cout << "请依次输入下棋横、纵坐标" << endl;
		cin >> x >> y;
		if (x <= n && x >=1 && y>=1 && y <= n && G[x][y] == Base)
		{
			G[x][y] = White;
			break;
		}
		else
			cout << "坐标无效，请重新输入" << endl;
	}
}

void Checkboard::showcurrent()
{
	int i, j;
	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			if (G[i][j] == White)cout << WhiteShape;
			else if (G[i][j] == Black) cout << BlackShape;
			else if (G[i][j] == Base) cout << BaseShape;
			else cout << "something wrong happen! Stop now!";
		}
		cout << endl;
	}
}

int Checkboard::judge()
{
	int i, j;
	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			if (G[i][j] == White){
				if (j + 4 <= n&&G[i][j + 1] == White && G[i][j + 2] == White && G[i][j + 3] == White && G[i][j + 4] == White)
				{
					return 2;
				}
				if (i + 4 <= n&& G[i + 1][j] == White && G[i + 2][j] == White && G[i + 3][j] == White && G[i + 4][j] == White){
					return 2;
				}
				if (i + 4 <= n&&j + 4 <= n&&G[i + 1][j + 1] == White && G[i + 2][j + 2] == White && G[i + 3][j + 3] == White && G[i + 4][j + 4] == White){
					return 2;
				}
				if (i - 4 >= 1 && j + 4 <= n && G[i - 1][j + 1] == White && G[i - 2][j + 2] == White && G[i - 3][j + 3] == White && G[i - 4][j + 4] == White){
					return 2;
				}
			}
			if (G[i][j] == Black){
				if (j + 4 <= n&&G[i][j + 1] == Black && G[i][j + 2] == Black && G[i][j + 3] == Black && G[i][j + 4] == Black)
				{
					return 1;
				}
				if (i + 4 <= n&& G[i + 1][j] == Black && G[i + 2][j] == Black && G[i + 3][j] == Black && G[i + 4][j] == Black){
					return 1;
				}
				if (i + 4 <= n&&j + 4 <= n&&G[i + 1][j + 1] == Black && G[i + 2][j + 2] == Black && G[i + 3][j + 3] == Black && G[i + 4][j + 4] == Black){
					return 1;
				}
				if (i - 4 >= 1 && j + 4 <= n && G[i - 1][j + 1] == Black && G[i - 2][j + 2] == Black && G[i - 3][j + 3] == Black && G[i - 4][j + 4] == Black){
					return 1;
				}
			}
		}
	}
	return 0;
}

void Checkboard::success()
{
	cout << "玩家获得胜利" << endl;
}

void Checkboard::fail()
{
	cout << "游戏失败" << endl;
}

void Checkboard::resetAll(){
	int i;
	for (i = 0; i <= this->n; i++)
		G[i].clear();
	G.clear();

	G.resize(n + 1);
	for (i = 0; i <= n; i++)
		G[i].resize(n + 1);
}

int main()
{
	int N;
	while (1){
		cout << "请输入棋盘大小" << endl;
		cin >> N;
		if (N >= 5 && (N - floor(N)) == 0){
			break;
		}
		else{
			cout << "不符合规范！" << endl;
		}
	}
	Checkboard T(N);
	int k;
	cout << "玩家先手请输入1，AI先手请输入0" << endl;
	cin >> k;
	if (k == 0){
		T.setcheckboard();
		T.cplay();
		T.showcurrent();
	}
	else{
		T.setcheckboard();
		T.showcurrent();
	}
	while (1){
		T.play();
		T.cplay();
		T.showcurrent();
		int judge = T.judge();
		if (judge != 0)
		{
			if (judge == 2) T.success();
			if (judge == 1) T.fail();
			bool k;
			cout << "重开请输入1，关闭请输入0" << endl;
			cin >> k;
			if (k == 1){
				T.resetAll();
				int t;
				cout << "玩家先手请输入1，AI先手请输入0" << endl;
				cin >> t;
				if (t == 0){
					T.setcheckboard();
					T.cplay();
					T.showcurrent();
				}
				else{
					T.setcheckboard();
					T.showcurrent();
				}
			}
			else
				break;
		}
	}
	system("pause");
	return 0;
}

