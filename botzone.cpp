//此为可提交到botzone上的代码
#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;
int halfround;
int simulation[11][11];          //2-8有棋 值为-1是白子，值为1为黑子,值为0为空
int colorRequest,colorResponse;      //值为0是白子，值为1为黑子
int ch[2]={-1,1};
int di[8][2]={{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};
int DI[24][2]={{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},
        {-2,-2},{-2,-1},{-2,0},{-2,1},{-2,2},{-1,2},{0,2},{1,2},{2,2},{2,1},{2,0},{2,-1},{2,-2},{1,-2},{0,-2},{-1,-2}};
struct movee {int x1;int y1;int x2;int y2;};
struct mv {int x1;int y1;int x2;int y2;int sum;};
movee choice;
void swi(movee &a)  //把输入的0~6转换成模拟棋盘上的2~8
{
    a.x1+=2;
    a.y1+=2;
    a.x2+=2;
    a.y2+=2;
}
int abs(int o)
{
	return (o>0)?o:-o;
}
//在模拟棋盘上实现先前的变化
void change(movee a, int color)     //color为主动方颜色
{
    if(abs(a.x1-a.x2)==2||abs(a.y1-a.y2)==2){
        simulation[a.x1][a.y1]=0;
    }
    simulation[a.x2][a.y2]=ch[color];
    for(int i=0;i<8;i++){
        if(simulation[a.x2+di[i][0]][a.y2+di[i][1]]==ch[1-color])
            simulation[a.x2+di[i][0]][a.y2+di[i][1]]=ch[color];
    }
}
int firstGR=0;
void getRequest(movee rqst)
{
    cin>>rqst.x1>>rqst.y1>>rqst.x2>>rqst.y2;
    if(firstGR==0){
        firstGR=1;
        if(rqst.x1==-1){
            colorRequest=0;colorResponse=1;
            return;
        }
        else {
            colorRequest=1;colorResponse=0;
        }
    }
    swi(rqst);
    if(rqst.x1>1)change(rqst,colorRequest);
}
void getResponse(movee rspn)
{
    cin>>rspn.x1>>rspn.y1>>rspn.x2>>rspn.y2;
    swi(rspn);
    change(rspn,colorResponse);
}
int bestmove(int color,int layer)
{
    if(layer==4)return 0;
    int simulation1[11][11]={0};
    for(int i=2;i<9;i++){
        for(int j=2;j<9;j++){
            simulation1[i][j]=simulation[i][j];
        }
    }
    int a=0;
    mv all[500];
    for(int i=0;i<500;i++)
        all[i].x1=all[i].x2=all[i].y1=all[i].y2=all[i].sum=0;
    for(int ii=2;ii<9;ii++){
        for(int jj=2;jj<9;jj++){          //起子点的枚举
            if(simulation[ii][jj]==ch[color]){
                for(int d=0;d<24;d++){
                    int i=ii+DI[d][0];
                    int j=jj+DI[d][1];
                    if(i>1&&i<9&&j>1&&j<9&&simulation[i][j]==0){
                        if(d>=8){  //实现 复制or剪切的差别
                            simulation[ii][jj]=0;
                        }
                        all[a].x1=ii;all[a].y1=jj;all[a].x2=i;all[a].y2=j; //struct mv {int x1;int y1;int x2;int y2;int sum;} all[500]; 我想出的代替四维数组的笨办法
                        if(simulation[i][j]==0){
                            for(int k=0;k<8;k++){
                                if(simulation[i+di[k][0]][j+di[k][1]]==ch[1-color]){
                                    simulation[i+di[k][0]][j+di[k][1]]=ch[color];
                                    all[a].sum++;
                                }
                            }
                            all[a].sum-=bestmove(1-color,layer+1);
                            a++;
                            for(int i=2;i<9;i++){
                              for(int j=2;j<9;j++){
                                   simulation[i][j]=simulation1[i][j];
                              }
                            }
                        }
                    }
                }
            }
        }
    }
    for(int i=2;i<9;i++){
        for(int j=2;j<9;j++){
            simulation[i][j]=simulation1[i][j];
        }
    }
    int maxsum=-100,maxLocation=0;
    if(layer==1){
        for(int i=0;i<a;i++){
            if(all[i].sum>maxsum){
                maxsum=all[i].sum;
                maxLocation=i;
            }
        }
        choice.x1=all[maxLocation].x1;
        choice.y1=all[maxLocation].y1;
       choice.x2=all[maxLocation].x2;
        choice.y2=all[maxLocation].y2;
        return 0;
    }
    else{
        for(int i=0;i<a;i++){
            if(all[i].sum>maxsum){
                maxsum=all[i].sum;
            }
        }
        return maxsum;
    }
}
int main()
{
	for(int i=0;i<11;++i)
	  for(int j=0;j<11;++j)simulation[i][j]=0;
    simulation[2][2]=simulation[8][8]=ch[1];
    simulation[2][8]=simulation[8][2]=ch[0];
    int n;
    cin>>n;
    movee tmp;
    for(int i=0;i<n-1;i++){
        getRequest(tmp);
        getResponse(tmp);
    }
    getRequest(tmp);
    for(int i=2;i<9;++i)
      for(int j=2;j<9;++j)cin>>simulation[i][j];
    cin>>colorResponse;
    bestmove(colorResponse,1);
    cout<<choice.x1-2<<" "<<choice.y1-2<<" "<<choice.x2-2<<" "<<choice.y2-2;
    return 0;
}
