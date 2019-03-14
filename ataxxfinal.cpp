#include <windows.h>
#include <cstdio>
#include <conio.h>
#include <iostream>
#include <cstdlib>
using namespace std;
#define KEY_DOWN  80
#define KEY_UP    72
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_ESC   27
int chess[11][11];//����ֵΪ1������ֵΪ-1����ֵΪ0��Ϊ���ٶ�ά����Խ����鷳������������11*11
int halfround;    //��¼�Ծ���
int colorRequest,colorResponse;      //ֵΪ0�ǰ��ӣ�ֵΪ1Ϊ���� colorRequestΪ���ִ����ɫ��colorResponseΪbotִ����ɫ
int ch[2]={-1,1};  //colorRequest��colorResponse��ֵ��chess��ֵ֮�������
char menuChoice;
int difficulty;
int di[8][2]={{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};
int DI[24][2]={{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},
        {-2,-2},{-2,-1},{-2,0},{-2,1},{-2,2},{-1,2},{0,2},{1,2},{2,2},{2,1},{2,0},{2,-1},{2,-2},{1,-2},{0,-2},{-1,-2}};
struct movee {int x1;int y1;int x2;int y2;};
struct mv {int x1;int y1;int x2;int y2;int sum;};
movee player;  //��¼������ҵ�����
movee choice;  //bot����ѡ��������move

//����������
void gotoxy(int x, int y);
void change(movee a, int color);
void save();
void resume();
void exit();
void menu();
void opening();
void newgame();
void game(int x,int y);
void whowins(int P,int B);
bool checkPutdown(int a1,int b1,int a2,int b2);
void print_chess(int x,int y);
void print_board();
int bestmove(int color,int layer,int diff);
void checkEnd();


int abs(int o)      //�����ֵ
{
	return (o>0)?o:-o;
}
void gotoxy(int x, int y)   // ������Ƶ�(x, y)����xΪ��ֱ����yΪˮƽ����
{
	COORD coord = { y, x };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void change(movee a, int color)     //���Ӷ����̲����ı仯  ��colorΪ��������ɫ��
{
    if(abs(a.x1-a.x2)==2||abs(a.y1-a.y2)==2){
        chess[a.x1][a.y1]=0;
    }
    chess[a.x2][a.y2]=ch[color];
    for(int i=0;i<8;i++){
        if(chess[a.x2+di[i][0]][a.y2+di[i][1]]==ch[1-color])
            chess[a.x2+di[i][0]][a.y2+di[i][1]]=ch[color];
    }
}
void save()     //�浵����
{
    freopen("save.txt","w",stdout);
    for(int i=0;i<11;++i){
    	for (int j=0;j<11;++j)cout<<chess[i][j]<<' ';
    	cout<<endl;
	}
    cout<<colorRequest<<endl;
    cout<<halfround<<endl;
    cout<<difficulty<<endl;
    fclose(stdout);
}
void resume()  //���������˵���Ϸ�Ĺ���
{
    freopen("save.txt","r",stdin);
    for(int i=0;i<11;++i)
    	for (int j=0;j<11;++j)cin>>chess[i][j];
    cin>>colorRequest;
    cin>>halfround;
    cin>>difficulty;
    fclose(stdin);
    game(0,0);
}
void exit()   //�˳���Ϸ�Ĺ���
{
    system("cls");
    cout<<"SEE YOU!"<<endl;
}
void menu()   //�˵�
{
    system("cls");
    cout<<"WELCOME TO ATAXX !!!"<<endl<<endl;
    cout<<"MENU:"<<endl;
    cout<<"a: NEW GAME"<<endl;
    cout<<"b: SAVE "<<endl;
    cout<<"c: RESUME"<<endl;
    cout<<"d: EXIT"<<endl<<endl;
    cout<<"Enter 'a', 'b', 'c' or 'd' to choose!"<<endl;
    cin>>menuChoice;
    system("cls");
    switch (menuChoice)
    {
        case 'a':newgame();break;
        case 'b':save();break;
        case 'c':resume();break;
        case 'd':exit();break;
    }
}
void opening()
{
    cout<<"Which color do you prefer? XD"<<endl;
    cout<<"a: PLAYER-FIRST mode (You will be holding BLACK'��' check pieces~)"<<endl;
    cout<<"b: COMPUTER-FIRST mode (You will be holding WHITE'��' check pieces~)"<<endl;
    cout<<"Enter 'a' or 'b' to choose!"<<endl<<endl;
    char cha=cin.get();
    cha=cin.get();
    if(cha=='a'){
        colorRequest=1;colorResponse=0;
        system("cls");
    }
    else if(cha=='b'){
        colorRequest=0;colorResponse=1;
        system("cls");
    }
    cout<<"Want a CHALLENGE? XD"<<endl;
    cout<<"a: EASY mode"<<endl;
    cout<<"b: HARD mode"<<endl;
    cha=cin.get();
    cha=cin.get();
    if(cha=='a'){
        difficulty=2;
        system("cls");
    }
    else if(cha=='b'){
        difficulty=4;
        system("cls");
    }
    cout<<"READY???"<<endl;
    cout<<"GAME BEGINS!"<<endl;
    system("cls");
}
void game(int x,int y)
{
    while(true){
        print_board();
        int hold=0;
        gotoxy(2 * (x + 1), 4 + y * 4);
        while(true) {
            int c = _getch();
            // ��ȡ���������
            if (!isascii(c)) {
                c = _getch();
                switch (c) {
                case KEY_DOWN:
                    if (x < 6) {
                        x++;
                        gotoxy(2 * (x + 1), 4 + y * 4);
                    }
                    break;
                case KEY_UP:
                    if (x > 0) {
                        x--;
                        gotoxy(2 * (x + 1), 4 + y * 4);
                    }
                    break;
                case KEY_LEFT:
                    if (y > 0) {
                        y--;
                        gotoxy(2 * (x + 1), 4 + y * 4);
                    }
                    break;
                case KEY_RIGHT:
                    if (y < 6) {
                        y++;
                        gotoxy(2 * (x + 1), 4 + y * 4);
                    }
                    break;
                default: break;
                }
            }
            // �����ַ�����
            else {
                if (c == ' '&&hold==0){           //ȡ��
                    if(chess[x+2][y+2]==ch[colorRequest]){
                        player.x1=x+2;
                        player.y1=y+2;
                        hold=1;
                    }
                    gotoxy(2 * (x + 1), 4 * (y + 1));
                }
                else if(c==' '&&hold==1){        //����
                    if((checkPutdown(player.x1,player.y1,x+2,y+2))==true){
                        player.x2=x+2;
                        player.y2=y+2;
                        chess[x+2][y+2]=ch[colorRequest];
                        change(player,colorRequest);
                        hold=-1;
                        halfround++;
                    }
                }
                else if(c==KEY_ESC){
                	menu();
                    gotoxy(18, 0);
				}
            }
            if(hold==-1){
                system("cls");
                break;
            }
        }
        checkEnd();
        bestmove(colorResponse,1,difficulty);
        change(choice,colorResponse);
        halfround++;
        system("cls");
        checkEnd();
        hold=0;
    }
}
void newgame()
{
    opening();

    chess[2][2]=chess[8][8]=1;
    chess[2][8]=chess[8][2]=-1;
    if(colorRequest==0){
        chess[2][3]=1;
        halfround=1;
        game(0,6);
    }
    else {
        halfround=0;
        game(0,0);
    }
}
bool checkPutdown(int a1,int b1,int a2,int b2)
{
    if(chess[a2][b2]!=0){
        return false;
    }
    if(abs((int)a1-a2)>2||abs((int)b1-b2)>2){
        return false;
    }
    return true;
}
void print_chess(int x,int y)
{
    if(chess[x][y]==0){
        cout<<"  ";
    }
    else if(chess[x][y]==1){
        cout<<"��";
    }
    else if(chess[x][y]==-1){
        cout<<"��";
    }
}
// ��ӡ����
void print_board()
{
    system("color 0F");
	cout << "     1   2   3   4   5   6   7   " << endl;
	cout << "  ���������ө������ө������ө������ө������ө������ө�������" << endl;
	for (int i = 1; i < 7; i++)
	{
		cout << ' ' << char('A' + i - 1) << "��";
		for(int j=2;j<8;j++){
            cout<<" ";
            print_chess(i+1,j);
            cout<<"��";
		}
		cout<<" ";
		print_chess(i+1,8);
		cout<<"��"<<endl;
		cout << "  �ġ�-�ࡪ-�ࡪ-�ࡪ-�ࡪ-�ࡪ-�ࡪ-��" << endl;
	}
	cout << " G��";
	for(int j=2;j<8;j++){
            cout<<" ";
            print_chess(8,j);
            cout<<"��";
		}
		cout<<" ";
		print_chess(8,8);
		cout<<"��"<<endl;
	cout << "  ���������۩������۩������۩������۩������۩������۩�������" << endl;
}
void whowins(int P,int B)
{
    system("cls");
    if(P>B){
            cout<<"YOU WIN :)"<<endl;
        }
        else if(P==B){
            cout<<"DRAW :|"<<endl;
        }
        else if(P<B){
            cout<<"YOU LOSE :("<<endl;
        }
}
void checkEnd()
{
    int cntPlayer=0,cntBot=0;
    for(int i=2;i<9;i++){
        for(int j=2;j<9;j++){
            if(chess[i][j]==ch[colorRequest])
                cntPlayer++;
            else if(chess[i][j]==ch[colorResponse])
                cntBot++;
        }
    }
    if(cntBot==0){
        system("cls");
        cout<<"YOU WIN :)"<<endl;
        return ;
    }
    else if(cntPlayer==0){
        system("cls");
        cout<<"YOU LOSE :("<<endl;
        return ;
    }
    if(halfround>800){
        system("cls");
        cout<<"Too many rounds! It's the end of the game."<<endl;
        whowins(cntPlayer,cntBot);
        return ;
    }
    for(int two=0;two<2;two++){
        int flg=0;
        for(int i=2;i<9;i++){
            for(int j=2;j<9;j++){
                if(chess[i][j]==ch[two]){
                    for(int dr=0;dr<24;dr++){
                        int ti=i+DI[dr][0];
                        int tj=j+DI[dr][1];
                        if(ti>1&&ti<9&&tj>1&&tj<9&&(chess[ti][tj]==0)){
                            flg=1;
                            break;
                        }
                    }
                    if(flg==1){
                        break;
                    }
                }
            }
            if(flg==1){
                break;
            }
        }
        if(flg==0){
            if(colorRequest==two){
                cntBot+=49-cntPlayer-cntBot;
                whowins(cntPlayer,cntBot);
                return;
            }
            else if(colorResponse==two){
                cntPlayer+=49-cntPlayer-cntBot;
                whowins(cntPlayer,cntBot);
                return;
            }
        }
    }
    return ;
}
int bestmove(int color,int layer,int diff)
{
    if(layer==diff)return 0;
    int simulation1[11][11];
    for(int i=0;i<11;++i)     //����һ���ԭʼ���̣��Ա����
        for(int j=0;j<11;++j)simulation1[i][j]=chess[i][j];
    int a=0;  //�߷�mv���±�
    mv all[500];
    for(int i=0;i<500;i++)
        all[i].x1=all[i].x2=all[i].y1=all[i].y2=all[i].sum=0;
    for(int ii=2;ii<9;ii++){
        for(int jj=2;jj<9;jj++){          //���ӵ��ö��
            if(chess[ii][jj]==ch[color]){
                for(int d=0;d<24;d++){
                    int i=ii+DI[d][0];
                    int j=jj+DI[d][1];
                    if(i>1&&i<9&&j>1&&j<9&&chess[i][j]==0){
                        if(d>=8){  //ʵ�� ����or���еĲ��
                            chess[ii][jj]=0;
                        }
                        all[a].x1=ii;all[a].y1=jj;all[a].x2=i;all[a].y2=j; //struct mv {int x1;int y1;int x2;int y2;int sum;} all[500]; ������Ĵ�����ά����ı��취
                        for(int k=0;k<8;k++){
                            if(chess[i+di[k][0]][j+di[k][1]]==ch[1-color]){
                                chess[i+di[k][0]][j+di[k][1]]=ch[color];
                                all[a].sum++;
                            }
                        }
                        all[a].sum-=bestmove(1-color,layer+1,diff);
                        a++;
                        for(int i=0;i<11;++i)                          //����
                          for(int j=0;j<11;++j)
                            chess[i][j]=simulation1[i][j];
                	}
                }
            }
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
    menu();
	return 0;
}
