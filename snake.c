#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#define	W	80
#define	H	30
#define	L	(H*W)
#define	A	10
#define	T	200
char x[W][H]={0};
int dx=0,dy=-1;
struct q{
	int x,y;
};
struct snake{
	struct q q[L];
	int l,r;
}snake;
void init(){
	char a[100];
	sprintf(a,"mode con cols=%d lines=%d",W,H);
	system(a);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);
	cci.bVisible=0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);
}
void print(int x,int y,char a[]){
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	printf(a);
}
void clr(){
	while(kbhit()){
		getch();
	}
}
void control(){
	char c;
	if(!kbhit()){
		return;
	}
	c=getch();
	if(c!=-32)
	{
		clr();
		return;
	}
	c=getch();
	if(c!=72&&c!=80&&c!=75&&c!=77){
		clr();
		return;
	}
	switch(c){
		case 72:
			if(dx==0&&dy==1){
				break;
			}
			dx=0;
			dy=-1;
			break;
		case 80:
			if(dx==0&&dy==-1){
				break;
			}
			dx=0;
			dy=1;
			break;
		case 75:
			if(dx==1&&dy==0){
				break;
			}
			dx=-1;
			dy=0;
			break;
		case 77:
			if(dx==-1&&dy==0){
				break;
			}
			dx=1;
			dy=0;
			break;
	}
	clr();
}
void food(){
	int i,j;
	while(1){
		i=rand()%W;
		j=rand()%H;
		if(x[i][j]!=0){
			continue;
		}
		x[i][j]=-1;
		print(i,j,"0");
		break;
	}
}
void run(){
	int i;
	int ax,ay;
	snake.l=snake.r=0;
	snake.q[snake.r].x=rand()%W;
	snake.q[snake.r].y=rand()%H;
	x[snake.q[snake.r].x][snake.q[snake.r].y]=1;
	print(snake.q[snake.r].x,snake.q[snake.r].y,"1");
	snake.r=(snake.r+1)%L;
	food();
	while(1){
		control();
		ax=snake.q[snake.r-1].x+dx;
		ay=snake.q[snake.r-1].y+dy;
		if(ax<0||ay<0||ax>W-1||ay>H-1||x[ax][ay]==1){
			print(0,0,"WA");
			sleep(-1);
			return;
		}
		i=0;
		if(x[ax][ay]==-1){
			i=1;
		}
		if(i==0){
			x[snake.q[snake.l].x][snake.q[snake.l].y]=0;
			print(snake.q[snake.l].x,snake.q[snake.l].y," ");
			snake.l=(snake.l+1)%L;
		}
		snake.q[snake.r].x=ax;
		snake.q[snake.r].y=ay;
		x[snake.q[snake.r].x][snake.q[snake.r].y]=1;
		print(snake.q[snake.r].x,snake.q[snake.r].y,"1");
		snake.r=(snake.r+1)%L;
		if(snake.r-snake.l>A){
			print(0,0,"AC");
			sleep(-1);
			return;
		}
		if(i==1){
			food();
		}
		sleep(T);
	}
}
int main(){
	init();
	run();
	return 0;
}
