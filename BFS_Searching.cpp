/*
*  Homework #1 Search Problem
*  HW 2-1 Programming Assignment (Missionary and Carnivals)
*  Due Date : 10/05 (수)
*  Using Algorithm : BFS
*
*  Name : 최영근
*  Student Number : 20113337
*/

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <vector>
using namespace std;

#define LEN 5

/* 각 노드의 fn, gn, hn, state를 담는 구조체 변수 
	BFS 알고리즘에서는 fn, gn, hn 을 사용하지 않는다.*/
struct Node{
	int fn;
	int gn;
	int hn;
	vector<int> state;
};

Node* newNode = NULL;	

vector<Node> openQ;			//방문하지 않은 것들
vector<Node> closeQ;		//방문한 것들

int init[LEN] = {3, 3, 0, 0, 1};	//초기 설정 값
int goal[LEN] = {0, 0, 3, 3, 0};	//목표 값 
int openQueueIndex = 0;					

/*
*	- Method description -
*  	void printOpenQueue() : 방문한 노드의 리스트 출력
*	void printCloseQueue() : 방문하지 않은 노드의 리스트 출력
*	bool checkInQueue(vector<int> temp)	: 스테이트를 인자로 받아 방문한 노드들과 비교
*											-> 중복검사  
*	bool solutionCompare(vector<int> state) : 스테이트를 인자로 받아 정답과 비교
*	void createNode(vector<int> state) : 스테이트를 인자로 받아 노드 생성
*	void insertOpenQueue(Node* newNode) : 노드를 오픈큐에 삽입 
*	void insertCloseQueue(Node* newNode) : 노드를 클로스큐에 삽입 
*	void BFS(Node* newNode) : 노드가 인자로 들어가 자식노드를 생성하고 BFS 형태로 탐색 
*/
void printOpenQueue();
void printCloseQueue();
bool checkInQueue(vector<int> temp);
bool solutionCompare(vector<int> state);
void createNode(vector<int> state);
void insertOpenQueue(Node* newNode);
void insertCloseQueue(Node* newNode);
void BFS(Node* newNode);


/* Operations */
void moveRight(vector<int> state);
void moveLeft(vector<int> state);

void Right01(vector<int> temp);
void Right02(vector<int> temp);
void Right11(vector<int> temp);
void Right10(vector<int> temp);
void Right20(vector<int> temp);

void Left01(vector<int> temp);
void Left02(vector<int> temp);
void Left11(vector<int> temp);
void Left10(vector<int> temp);
void Left20(vector<int> temp);

int main(void)
{
	newNode = new Node;
	for(int i=0; i<LEN; i++)
		newNode->state.push_back(init[i]);		//초기값 저장 

	insertOpenQueue(newNode);			//시작 노드의 정보 오픈큐에 저장 
	printOpenQueue();
	printCloseQueue();
	cout<<endl;

	openQueueIndex=0;
	while(!solutionCompare(openQ[openQueueIndex].state))	//정답을 찾을 때 까지 
	{
		BFS(&openQ[openQueueIndex]);		//다음 노드 삽입 

		openQueueIndex++;
		printOpenQueue();
		printCloseQueue();
		cout<<endl;
	}

	insertCloseQueue(&openQ[openQueueIndex]);		//정답을 클로스큐에 저장 
	openQueueIndex++;
	printOpenQueue();								  
	cout<<endl;	
	printCloseQueue();								
	cout<<endl;

	return 0;
}

/* 노드가 들어오면 그 노드는 방문한 것이므로 클로스큐에 저장,
	그리고 그 노드가 가지는 자식 노드들을 생성하기 위해 
	배의 위치에 따른 해당 매소드 호출 */
void BFS(Node* newNode)				
{

	insertCloseQueue(newNode);

	if(newNode->state[4] == 1)
		moveRight(newNode->state);

	else
		moveLeft(newNode->state);
}

/* 배가 오른쪽으로 이동하기 위해 가능한 모든 오퍼레이션을 찾는다 
	왼쪽에 존재하는 사람의 수를 기준으로 잡았다 
	이 함수 안에 존재하지 않는 경우는 식인종이 선교사보다 
	많이 존재하여 선교사가 잡아먹힐 경우이므로 다룰 필요가 없다 */
void moveRight(vector<int> state)
{
	if(state[0] == 3 && state[1] == 3)		//6명 모두 존재 
	{
		Right01(state);
		Right02(state);
		Right11(state);
	}

	else if(state[0] == 3 && state[1] == 2)	//선교사 3, 식인종 2
	{
		Right01(state);
		Right02(state);
		Right10(state);
	}

	else if(state[0] == 3 && state[1] == 1)	//선교사 3, 식인종 1
	{
		Right01(state);
		Right20(state);
	}

	else if(state[0] == 2 && state[1] == 2)	//선교사 2, 식인종 2
	{
		Right11(state);
		Right20(state);
	}

	else if(state[0] == 1 && state[1] == 1)	//선교사 1, 식인종 1
	{
		Right11(state);
		Right10(state);
	}

	else if(state[0] == 0 && state[1] == 3)	//선교사 0, 식인종 3
	{
		Right01(state);
		Right02(state);
	}

	else if(state[0] == 0 && state[1] == 2)	//선교사 0, 식인종 2
	{
		Right01(state);
		Right02(state);
	}
}

/* 배가 왼쪽으로 이동하기 위해 가능한 모든 오퍼레이션을 찾는다 
	오른쪽에 존재하는 사람의 수를 기준으로 잡았다 
	이 함수 안에 존재하지 않는 경우는 식인종이 선교사보다 
	많이 존재하여 선교사가 잡아먹힐 경우이므로 다룰 필요가 없다 */
void moveLeft(vector<int> state)
{
	if(state[2] == 3 && state[3] == 2)		//선교사 3, 식인종 2
	{
		Left01(state);
		Left02(state);
		Left10(state);
	}

	else if(state[2] == 3 && state[3] == 1)	//선교사 3, 식인종 1
	{
		Left01(state);
		Left20(state);
	}

	else if(state[2] == 2 && state[3] == 2)	//선교사 2, 식인종 2
	{
		Left11(state);
		Left20(state);
	}

	else if(state[2] == 1 && state[3] == 1)	//선교사 1, 식인종 1
	{
		Left11(state);
		Left10(state);
	}

	else if(state[2] == 0 && state[3] == 3)	//선교사 0, 식인종 3
	{
		Left01(state);
		Left02(state);
	}

	else if(state[2] == 0 && state[3] == 2)	//선교사 0, 식인종 2
	{
		Left01(state);
		Left02(state);
	}
}

/* 큐 안에 해당 스테이트가 존재하는지 검사한다 (중복검사)
	큐의 사이즈만큼 비교하고 하나의 원소라도 같지않을 때마다
	cnt를 증가시켜 횟수를 세어 큐의 사이즈와 같다면
	중복이 없는 것이기 때문에 true를 반환한다. */
bool checkInQueue(vector<int> temp)
{
	int cnt = 0;
	for(int i=0; i<openQ.size(); i++)
	{
		for(int j=0; j<LEN; j++)
		{
			if(openQ[i].state[j] != temp[j])
			{
				cnt++;
				break;
			}
		}
	}
	if(cnt == openQ.size())
		return true;
	return false;
}

/* 스테이트를 인자로 받아 새로운 노드를 생성한다 */
void createNode(vector<int> state)
{
	newNode = new Node;
	for(int i=0; i<LEN; i++)
		newNode->state.push_back(state[i]);

	insertOpenQueue(newNode);
}

/* 노드를 인자로 받고 그 노드를 오픈큐 마지막에 삽입한다 */
void insertOpenQueue(Node* newNode)
{
	openQ.push_back(*newNode);
}

/* 노드를 인자로 받고 그 노드를 클로스큐 마지막에 삽입한다 */
void insertCloseQueue(Node* newNode)
{
	closeQ.push_back(*newNode);
}

/* 오픈큐에 존재하는 모든 리스트 출력 */
void printOpenQueue()
{	
	cout<<"Open Queue : ";
	for(int i=openQueueIndex; i<openQ.size(); i++)
	{
		for(int j=0; j<LEN; j++)
		{
			cout<<openQ[i].state[j];
		}
		cout<<"   ";
	}
	cout<<endl;
}

/* 클로스큐에 존재하는 모든 리스트 출력 */
void printCloseQueue()
{
	cout<<"Close Queue : ";
	for(int i=0; i<closeQ.size(); i++)
	{
		for(int j=0; j<LEN; j++)
		{
			cout<<closeQ[i].state[j];
		}
		cout<<"   ";
	}
	cout<<endl;	
}

/* 우리가 찾고자 하는 솔루션과 비교 */
bool solutionCompare(vector<int> state)
{
	for(int i=0; i<LEN; i++)
	{
		if(goal[i] != state[i])
			return false;
	}
	return true;
}

/* 10개의 오퍼레이션 */
void Right01(vector<int> temp)
{
	temp[1] -= 1;
	temp[3] += 1;
	temp[4] = 0;

	if(temp[2] != 0 && temp[2] >= temp[3] && checkInQueue(temp))
		createNode(temp);
	else if(temp[2] == 0 && checkInQueue(temp))
		createNode(temp);
}
void Right02(vector<int> temp)
{
	temp[1] -= 2;
	temp[3] += 2;
	temp[4] = 0;

	if(temp[2] != 0 && temp[2] >= temp[3] && checkInQueue(temp))
		createNode(temp);
	else if(temp[2] == 0 && checkInQueue(temp))
		createNode(temp);
}
void Right11(vector<int> temp)
{
	temp[0] -= 1;
	temp[1] -= 1;
	temp[2] += 1;
	temp[3] += 1;
	temp[4] = 0;
	
	if(temp[2] >= temp[3] && checkInQueue(temp))
		createNode(temp);
}
void Right10(vector<int> temp)
{
	temp[0] -= 1;
	temp[2] += 1;
	temp[4] = 0;

	if(temp[2] >= temp[3] && checkInQueue(temp))
		createNode(temp);
}
void Right20(vector<int> temp)
{
	temp[0] -= 2;
	temp[2] += 2;
	temp[4] = 0;

	if(temp[2] >= temp[3] && checkInQueue(temp))
		createNode(temp);
}

void Left01(vector<int> temp)
{
	temp[1] += 1;
	temp[3] -= 1;
	temp[4] = 1;

	if(temp[0] != 0 && temp[0] >= temp[1] && checkInQueue(temp))
		createNode(temp);
	else if(temp[0] == 0 && checkInQueue(temp))
		createNode(temp);
}
void Left02(vector<int> temp)
{
	temp[1] += 2;
	temp[3] -= 2;
	temp[4] = 1;	

	if(temp[0] != 0 && temp[0] >= temp[1] && checkInQueue(temp))
		createNode(temp);
	else if(temp[0] == 0 && checkInQueue(temp))
		createNode(temp);
}
void Left11(vector<int> temp)
{
	temp[0] += 1;
	temp[1] += 1;
	temp[2] -= 1;
	temp[3] -= 1;
	temp[4] = 1;

	if(temp[0] >= temp[1] && checkInQueue(temp))
		createNode(temp);
}
void Left10(vector<int> temp)
{
	temp[0] += 1;
	temp[2] -= 1;
	temp[4] = 1;

	if(temp[0] >= temp[1] && checkInQueue(temp))
		createNode(temp);
}
void Left20(vector<int> temp)
{
	temp[0] += 2;
	temp[2] -= 2;
	temp[4] = 1;

	if(temp[0] >= temp[1] && checkInQueue(temp))
		createNode(temp);
}

