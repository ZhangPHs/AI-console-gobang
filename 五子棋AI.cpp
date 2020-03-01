#include<iostream>
#include<vector>
#include<Windows.h>
#include<math.h>

using namespace std;

#define White 1//����Ϊ1����
#define WhiteShape "��"
#define Black -1//����Ϊ-1����
#define BlackShape "��"
#define BaseShape "ʮ"
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

	int minF(int deep, int alpha, int beta);	//MIN������
	int maxF(int deep, int alpha, int beta);	//MAX������
	int getCScore();	//�õ�AI������
	int getCScoreT();	//�õ���ҷ�����
	void getOneStrScore(int length, int state);	//�����״̬��ʾ�ߺͻ�
	void getOneStrScoreT(int length, int state);	//��ҷ�

	void setcheckboard();	//��������
	void showcurrent();		//չʾ��ǰ���
	void play();			//�������
	void cplay();			//���������,core
	int judge();			//�ж�ʤ��
	void success();			//ʤ��
	void fail();			//ʧ��
	void resetAll();		//�������

	vector<point> genpoints();	//�����ʺ����ӵ�λ�ü���
	point maxmin(int deep);		//����Сֵ�㷨ʵ��
	int getTotalScore(){
		int score = getCScore();
		int tscore = getCScoreT();
		Score = 0;
		TScore = 0;
		return score + tscore;
	}		//��ȡ��ǰAI����������Ҷ�ӽڵ�ָ��������У�����״̬����֮�͡�   
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
	bool flag = false;	//�Ƿ�����
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
	//���ص�ǰ����
	if (deep <= 0 || judge() != 0)	return grades;//��ʱ�Ѿ���Ҷ�ӽڵ���,�����Ѿ���������
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
			if (v > alpha)	return best;//��֦
		}
	}
*/
	vector<point> t = genpoints();
	for (i = 0; i < t.size(); i++){
		point p(t[i].i, t[i].j);
		if (G[p.i][p.j] != Base) continue;
		G[p.i][p.j] = Black;	//��������
		int v = minF(deep - 1,alpha, best > beta ? best : beta);
		G[p.i][p.j] = Base;	//ȡ�����µ�����
		if (v > best)	best = v;
		if (v > alpha)	return best;	//��֦�����ҵ���alpha�����ֵ����ʣ�½ڵ������ٿ�����

	}
	if (best < alpha)	alpha = best;
	return best;
}

int Checkboard::minF(int deep, int alpha, int beta){
	int grades = getTotalScore();
	//���ص�ǰ����
	if (deep <= 0 || judge() != 0)	return grades;//��ʱ�Ѿ���Ҷ�ӽڵ㣬�����Ѿ��������壡
	
	int best = MAX;
	int i, j;
/*
	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			point p(i, j);
			if (G[p.i][p.j] != Base) continue;
			G[p.i][p.j] = White;	//��������
			int v = maxF(deep - 1, best < alpha ? best : alpha, beta);
			G[p.i][p.j] = Base;	//ȡ�����µ�����
			if (v < best)	best = v;
			if (v < beta)	return best;//��֦
		
		}
	}
*/
	vector<point> t = genpoints();
	for (i = 0; i < t.size(); i++){
		point p(t[i].i, t[i].j);
			if (G[p.i][p.j] != Base) continue;
			G[p.i][p.j] = White;	//��������
			int v = maxF(deep - 1, best < alpha ? best : alpha, beta);
			G[p.i][p.j] = Base;	//ȡ�����µ�����
			if (v < best)	best = v;
			if (v < beta)	return best;	//��֦�����Ѿ��ҵ���betaС�����ӣ���ʣ�½ڵ������ٿ��ǡ�

		}
	if (best > beta) beta = best;
	return best;
}

point Checkboard::maxmin(int deep){//deep��Ԥ��˼������������Ϊ4
	int i, j;
	int best = MIN;
	int alpha = MAX, beta = MIN;
	vector<point> points;

/*	for (i = 1; i <= n; i++){
		for (j = 1; j <= n; j++){
			point p = point(i, j);
			if (G[p.i][p.j] != Base) continue;
			G[i][j] = Black;
			//����
			int v = minF(deep - 1, alpha, beta);//�����ֵ
			if (v == best)	points.push_back(p);
			else if (v > best){	//����ѡ����
				best = v;
				points.clear();
				points.push_back(p);
			}
			G[i][j] = Base;	//ȡ���µ���
		}
	}
*/
	vector<point> t = genpoints();
	for (i = 0; i < t.size(); i++){
		point p(t[i].i, t[i].j);
		if (G[p.i][p.j] != Base) continue;		//����λ���ѱ����ӣ�������
		G[p.i][p.j] = Black;					//��������
		int v = minF(deep - 1, alpha, beta);	//��min��ڵ��������ֵ
		if (v == best)	points.push_back(p);	//���Ǵ��ڶ������ӷ��õ�������ͬ������Щλ�÷��뼯��points
		else if (v > best){					//���ҵ��ȵ�ǰ���ֵbest����ģ������best��ͬʱ����֮ǰ����points
			best = v;
			points.clear();
			points.push_back(p);
		}
		G[p.i][p.j] = Base;					//��ԭ	

	}

	int len = points.size();
	return points[rand() % len];			//���ڷ�����ͬ�Ķ������ӷ������ѡ��һ�����ӡ�
}

void Checkboard::cplay()
{//���������,�㷨���ġ�

//��������
//���塢���ġ����ġ��������������������һ���߶�
//ע���ߵ���˼�ǣ�һ�౻��������һ��û�С�������ֱ������������ֱ�������Ͳ������ˡ�
	//�������ӾͲ�˵��

/*���룺��ǰ���̡��趨˼�����Ϊ4.
	1.��ÿ������ӵ�λ�ü��ϡ�
	2.��ÿһ��λ�ã��������ӣ��ټ���з����ӣ��ټ����ҷ����ӡ���������
	3.�������Ӻ�ķ���

�����������ӡ�
*/	
	point p = maxmin(depth);
	G[p.i][p.j] = Black;
}

void Checkboard::getOneStrScore(int length, int state){
//	cout << "���ã���ʱlength" << length << "״̬" << state << endl;
//	cout <<"score"<< Score << endl;
	if (length == 5)	Score += Five;
	if (state == 0){
		//��
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
		//��
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
		//��
		Score += 0;
	}
}

void Checkboard::getOneStrScoreT(int length, int state){
	//	cout << "���ã���ʱlength" << length << "״̬" << state << endl;
	//	cout <<"score"<< Score << endl;
	if (length == 5) TScore -= Five;
	if (state == 0){
		//��
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
		//��
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
		//��
		TScore -= 0;
	}
}

int Checkboard::getCScore(){
	//�����ǵ�ǰ�����G
	//�����ǰ�������black���������������ķ���
	//��ÿһ�С�ÿһ�С�ÿһб���д���ô��
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
//							cout << "����������" << endl;
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


	//��б
	int k;
	for (k = 1; k <= this->n; k++){// for n times circle.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;
		
		for (i = 1, j = this->n - k + 1; j <= this->n; ++j, ++i){// һ����֣������Ͻ�ˢ��������
//			cout << "��б�ж�" << endl;
			if (j >= this->n - 4 + i) break;
			point lastone(i - 1, j - 1);
//			cout << "��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//��ǰ���ұ�Ե
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
				if (i == 1){//ע����ֵ�߽�ͺ���߽�Ĳ�ͬ�ж�������
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

		for (i = this->n - k + 1, j = 1; i >= 1 && i<= this->n; j++, i++){// һ����֣������½�ˢ������
			if (j <= i - (this->n - 4)) break;
			if (i == 1 && j == 1)break;
			point lastone(i - 1, j - 1);
//			cout << "��һ�룬��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (i == this->n){//��ǰ���ұ�Ե
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
	//��б
	for (k = 1; k <= this->n; k++){// for n times circle.
//		cout << "��ʼ��б**********************************" << endl;
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;

		for (i = k, j = 1; i >= 1 && i <= this->n; j++, i--){// һ����֣������Ͻ�ˢ��������
			if (j <= (-i + 5)) break;
			point lastone(i + 1, j - 1);
//			cout << "��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (i == 1){//��ǰ���ұ�Ե
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
				if (j == 1){//ע����ֵ�߽�ͺ���߽�Ĳ�ͬ�ж�������
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
									   if (i == 1){//�ұ�Ե
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

		for (i = this-> n, j = this->n - k + 1; j <=this->n && i <= this->n && i>= 1 && j >= 1; j++, i--){// һ����֣��Ӱ�����ˢ�����½�
			if (i >= this->n - 3 && j >= this->n - 3) break;
			if (i == this->n && j == 1)	break;
			point lastone(i - 1, j - 1);
//			cout << "��һ�룬��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//��ǰ���ұ�Ե
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
				if (i == this->n){//���Ե
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
	//�����ǵ�ǰ�����G
	//�����ǰ����white���������������ķ���
	//��ÿһ�С�ÿһ�С�ÿһб���д���ô��
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


	//��б
	int k;
	for (k = 1; k <= this->n; k++){// for n times circle.
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;

		for (i = 1, j = this->n - k + 1; j <= this->n; ++j, ++i){// һ����֣������Ͻ�ˢ��������
			//			cout << "��б�ж�" << endl;
			if (j >= this->n - 4 + i) break;
			point lastone(i - 1, j - 1);
			//			cout << "��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//��ǰ���ұ�Ե
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
				if (i == 1){//ע����ֵ�߽�ͺ���߽�Ĳ�ͬ�ж�������
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

		for (i = this->n - k + 1, j = 1; i >= 1 && i <= this->n; j++, i++){// һ����֣������½�ˢ������
			if (j <= i - (this->n - 4)) break;
			if (i == 1 && j == 1)break;
			point lastone(i - 1, j - 1);
			//			cout << "��һ�룬��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (i == this->n){//��ǰ���ұ�Ե
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
	//��б
	for (k = 1; k <= this->n; k++){// for n times circle.
		//		cout << "��ʼ��б**********************************" << endl;
		int length = 0;
		bool leftState = false;
		bool rightState = false;
		bool flag = false;
		int tmpState = 0;

		for (i = k, j = 1; i >= 1 && i <= this->n; j++, i--){// һ����֣������Ͻ�ˢ��������
			if (j <= (-i + 5)) break;
			point lastone(i + 1, j - 1);
			//			cout << "��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (i == 1){//��ǰ���ұ�Ե
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
				if (j == 1){//ע����ֵ�߽�ͺ���߽�Ĳ�ͬ�ж�������
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
									   if (i == 1){//�ұ�Ե
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

		for (i = this->n, j = this->n - k + 1; j <= this->n && i <= this->n && i >= 1 && j >= 1; j++, i--){// һ����֣��Ӱ�����ˢ�����½�
			if (i >= this->n - 3 && j >= this->n - 3) break;
			if (i == this->n && j == 1)	break;
			point lastone(i - 1, j - 1);
			//			cout << "��һ�룬��ǰ��" << i << "," << j << endl;
			if (flag == true){
				if (j == this->n){//��ǰ���ұ�Ե
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
				if (i == this->n){//���Ե
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
		cout << "��������������ᡢ������" << endl;
		cin >> x >> y;
		if (x <= n && x >=1 && y>=1 && y <= n && G[x][y] == Base)
		{
			G[x][y] = White;
			break;
		}
		else
			cout << "������Ч������������" << endl;
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
	cout << "��һ��ʤ��" << endl;
}

void Checkboard::fail()
{
	cout << "��Ϸʧ��" << endl;
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
		cout << "���������̴�С" << endl;
		cin >> N;
		if (N >= 5 && (N - floor(N)) == 0){
			break;
		}
		else{
			cout << "�����Ϲ淶��" << endl;
		}
	}
	Checkboard T(N);
	int k;
	cout << "�������������1��AI����������0" << endl;
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
			cout << "�ؿ�������1���ر�������0" << endl;
			cin >> k;
			if (k == 1){
				T.resetAll();
				int t;
				cout << "�������������1��AI����������0" << endl;
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

