/*
*  Homework #1 Search Problem
*  HW 1-2 Programming Assignment (8-Puzzle)
*  Due Date : 10/05 (수)
*  Using Algorithm : A*
*  H(n) = of tiles in wrong position
*
*  Name : 최영근
*  Student Number : 20113337
*/

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <algorithm>
#include <string.h>
using namespace std;

#define MAX 256
#define LEN 9

//각 노드들의 fn gn hn 값을 가질 구조체 변수
//이 구조체 변수를 소팅에 이용한다.
struct NODE         
{   
    int fn, gn, hn; 
};

NODE *node;

int initState[LEN] = { 2, 8, 3, 1, 6, 4, 7, 0, 5 };     //초기상태
int solution[LEN] = {1, 2, 3, 8, 0, 4, 7, 6, 5};        //정답 
int zeroLoc = 7;                                        //0의 위치 
int *openQ[MAX], *closeQ[MAX];            //openQueue, closeQueue
int openQSize = 0, closeQSize = 0, nodeSize = 0;    //큐와 노드의 사이     

static int cnt = 0;
static int gn = 0;

/**** operation ****/
void up(int *temp, int index);          
void down(int *temp, int index);
void right(int *temp, int index);
void left(int *temp, int index);

/*
*   - Method Description -
*   BFS() - 오픈큐의 가장 앞의 노드로부터 자식노드를 생성하여
*           Breath first 형태로 오픈큐에 저장하는 함수
*   printCloseQueue(), printOpenQueue() - 큐를 출력해주는 함수
*   stateCompare() - 전역으로 선언한 솔루션과 비교하여 맞으면 true반환 
*   checkZero() - 스테이트 안의 0의 위치를 파악하여 그 인덱스를 반환 
*   check() - 오픈큐에 중복으로 저장되는걸 방지하기 위하여 오픈큐에 저장하기 전에 
*             현재의 스테이트가 오픈큐에 들어있는지 확인해 주는 함수
*   sort() - 오픈큐에 있는 스테이트들을 f(n) 값으로 오름차순 소팅해주는 함수 
*   qswap() - sort에서 이용되는 swap 함수, 큐 안에 있는 스테이트 단위로 스왑 
*/

void BFS(int state[], int i);              //자식노드를 생성하여 오픈큐의 마지막에 계속 삽입해주는 함수  
void printCloseQueue(int *queue[], int size);   //클로스 큐를 출력해주는 함수
void printOpenQueue(int *queue[], int size);    //오픈 큐를 출력해주는 함수 
bool stateCompare(int *state);              //solution과 비교해주는 함수 
int checkZero(int *state);          //현재 스테이트의 0의 위치 반환해주는 함수
bool check(int *queue[], int *state);   //생성되는 스테이트가 오픈큐에 이미 존재하는지 검사 
int getHeuristic(int *state);           //h(n)을 구하는 함수
void insertNode(int gn, int hn);            
void sort(int i);
void qswap ( int *state1, int *state2 );

int main(void)
{
    node = (struct NODE*)malloc(sizeof(struct NODE) * MAX);
    openQ[openQSize++] = initState;             //오픈큐에 초기값 저장
    
    int hn = getHeuristic(initState);    //초기 h(n) 값 저장 
    gn = 0;                              //초기 g(n) 값 저장 
    insertNode(gn, hn);                  //저장된 h(n), g(n)값을 구조체 배열에 삽입  

    int i=0;                             
    while(!stateCompare(openQ[i]))      //오픈큐의 첫번째 노드가 솔루션값과 같다면 루프 탈출 
    {
        cout<<"    f(n) : "<<node[i].fn<<endl;  
        printOpenQueue(openQ, openQSize);       //f(n), openQueue 출력 
        zeroLoc = checkZero(openQ[i]);          //현재 상태의 0의 위치 저장 
        BFS(openQ[i], i);                  //BFS 실행  
        
        sort(i);                   //BFS로부터 얻어진 큐를 f(n) 값으로 오름차순 정렬 
        
        i++;
    }
    closeQ[closeQSize] = (int*)malloc(sizeof(int)*LEN);     //솔루션을 클로스 큐에 저장 
    memcpy(closeQ[closeQSize++], openQ[i], sizeof(int)*LEN);

    cout<<"    f(n) : "<<node[i].fn<<endl;          //마지막 부분의 f(n) 값 출력 
    printOpenQueue(openQ, openQSize);               //마지막 오픈큐 상태 출력
    printCloseQueue(closeQ, closeQSize);            //마지막 클로스큐 상태 출력
    
    return 0;
}

void BFS(int state[], int i)
{   
    gn++;                   //자식노드로 파생되면서 계속 gn값 1증가 
    int temp[LEN];          //함수로 들어온 스테이트를 저장할 임시 배열 
    memset(temp, 0, sizeof(temp));

    closeQ[closeQSize] = (int*)malloc(sizeof(int)*LEN);     //방문한 스테이트들 출력 
    memcpy(closeQ[closeQSize++], state, sizeof(int)*LEN);   
         
    printCloseQueue(closeQ, closeQSize);

    /* switch 역할 : 0의 위치를 가지고 가능한 오퍼레이션들로 현재 스테이트값으로
                    저장된 temp로 미리 움직여보고 오픈큐에 있는 스테이트와 
                    중복되는지 검사.
                    중복이 아니라면 다음 스테이트를 오픈큐의 마지막에 저장하고
                    스테이트가 가지는  f(n) g(n) h(n) 값 저장      */ 
    switch(zeroLoc)     
    {
    case 0:
        memcpy(temp, state, sizeof(temp));
        right(temp, zeroLoc);      

        /* 그전의 스테이트와 오픈큐에 들어가 있는 스테이트와 비교하여 존재하지 않는다면 
                오픈큐에 스테이트 저장, 구조체 배열에 fn gn hn 값 저장 */

        // 밑의 if문 모두 동일한 기능
        if(check(openQ, temp))        
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        down(temp, zeroLoc);
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    case 1: 
        memcpy(temp, state, sizeof(temp));
        right(temp, zeroLoc);           
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);          
        }
        memcpy(temp, state, sizeof(temp));
        left(temp, zeroLoc);
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        down(temp, zeroLoc);
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    case 2:
        memcpy(temp, state, sizeof(temp));
        left(temp, zeroLoc);
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        down(temp, zeroLoc);
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    case 3: 
        memcpy(temp, state, sizeof(temp));
        right(temp, zeroLoc);           
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);          
        }
        memcpy(temp, state, sizeof(temp));
        up(temp, zeroLoc);
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        down(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    case 4: 
        memcpy(temp, state, sizeof(temp));
        right(temp, zeroLoc);           
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);          
        }
        memcpy(temp, state, sizeof(temp));
        left(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        up(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        down(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }       
        break;
    case 5: 
        memcpy(temp, state, sizeof(temp));
        down(temp, zeroLoc);            
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);          
        }
        memcpy(temp, state, sizeof(temp));
        left(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        up(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    case 6:
        memcpy(temp, state, sizeof(temp));
        right(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        up(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    case 7: 
        memcpy(temp, state, sizeof(temp));
        right(temp, zeroLoc);           
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);          
        }
        memcpy(temp, state, sizeof(temp));
        left(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        up(temp, zeroLoc);
        if(check(openQ, temp))
        {
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    case 8:
        memcpy(temp, state, sizeof(temp));
        left(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }
        memcpy(temp, state, sizeof(temp));
        up(temp, zeroLoc);
        if(check(openQ, temp))
        {   
            insertNode(gn, getHeuristic(temp));
            openQ[openQSize] = (int*)malloc(sizeof(int)*LEN);
            memcpy(openQ[openQSize++], temp, sizeof(int)*LEN);
        }   
        break;
    }
}

/* 클로스큐에 있는 모든 스테이트를 출력한다. */
void printCloseQueue(int *queue[], int size)    
{
    cout<<"closed queue list-->   ";
    for (int j=0; j<size; j++)
    {
        for (int i = 0; i < 9; i++)
        {
        cout << queue[j][i];
        }
        cout.width(3);
    }
    cout<<endl;
}

/* 오픈큐에 있는 모든 스테이트를 출력한다. */
void printOpenQueue(int *queue[], int size)     
{
    cout<<"  open queue list-->   ";
    for (int j=cnt; j<size; j++)
    {
        for (int i = 0; i < 9; i++)
        {
        cout << queue[j][i];
        }
        cout.width(3);
    }
    cout<<endl;
    cnt++;
}

/* 스테이트 배열을 인자로 받아 정답과 비교해 그 값을 반환한다. */
bool stateCompare(int *state)       
{
    for(int i=0; i<LEN; i++)
    {
        if(state[i] != solution[i])
            return false;
    }
    return true;
}

/* 인자로 들어오는 스테이트의 0의 위치를 반환한다. */
int checkZero(int *state)           
{
    int loc=0;
    for(int i=0; i<9; i++)
    {
        if(state[i] == 0)
        {
            loc = i;
            break;
        }
    }
    return loc;
}

/* 큐에 중복된 스테이트를 저장하지 않기 위해
    인자로 들어오는 큐 안에 현재의 스테이트가 있는지 파악한다. */
bool check(int *queue[], int *state)        
{
    int cnt = 0;
    for(int i=0; i<openQSize; i++)
    {
        for(int j=0; j<LEN; j++)
        {
            if(queue[i][j] == state[j])
                cnt++;
        }
        if(cnt == LEN)
            return false;
        cnt = 0;
    }
    return true;
}

/* 0을 제외하고 나머지 들의 잘못된 위치 파악 */
int getHeuristic(int *state)
{
    int heuristic = 0;
    for(int i=0; i<LEN; i++)
    {
        if(state[i] != 0 && state[i] != solution[i])
        {
            heuristic++;            
        }
    }
    return heuristic;
}

/* 노드에 삽입 */
void insertNode(int gn, int hn)
{
    int fn = gn + hn;

    node[nodeSize].fn = fn;
    node[nodeSize].gn = gn;
    node[nodeSize].hn = hn;
    nodeSize++;
}

/* 오픈 큐 안의 스테이트들을 fn값으로 오름차순 정렬 한다. */
void sort(int i)
{
    int index = i;
    for(int i=index+1; i<nodeSize-1; i++)
    {
        for(int j=index+2; j<nodeSize; j++)
        {
            if(node[i].fn >= node[j].fn)
            {   
                std::swap(node[i].fn, node[j].fn);
                qswap(openQ[i], openQ[j]);
            }
        }
    }
}

/* sort() 함수에서 사용하는 swap 함수 */
void qswap ( int *state1, int *state2 )
{
  int tmp[LEN];
  memcpy(tmp, state1, sizeof(int)*LEN);
  memcpy(state1, state2, sizeof(int)*LEN);
  memcpy(state2, tmp, sizeof(int)*LEN);
}

/* 오픈큐의 맨 처음 원소를 제거 한다. */
void dequeue()          
{
    for(int i=0; i<openQSize; i++)
    {
        if(i == openQSize-1)
            break;
        memcpy(openQ[i], openQ[i+1], sizeof(int)*LEN);
    }

    // for(int i=0; i<9; i++)
        free(openQ[openQSize]);
    openQSize--;
}

/* operations */
void up(int *temp, int index)
{
 int temp1;
 temp1 = temp[index - 3];
 temp[index - 3] = temp[index];
 temp[index] = temp1;
}
void down(int *temp, int index)
{
 int temp1;
 temp1 = temp[index + 3];
 temp[index + 3] = temp[index];
 temp[index] = temp1;
}
void right(int *temp, int index)
{
 int temp1;
 temp1 = temp[index + 1];
 temp[index + 1] = temp[index];
 temp[index] = temp1;
}
void left(int *temp, int index)
{
 int temp1;
 temp1 = temp[index - 1];
 temp[index - 1] = temp[index];
 temp[index] = temp1;
}
/* end */
