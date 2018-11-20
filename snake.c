#include<stdio.h>
#include<stdlib.h>
#ifdef	WIN32
#include<conio.h>
#include<windows.h>
#else
#include<termios.h>
#include<unistd.h>
#include<fcntl.h>
#endif
#define	W	80
#define	H	30
#define	L	(H*W)
#define	A	10
#ifdef	WIN32
#define	T	200
#else
#define	T	(200*1000)
#endif
char x[W][H]={0};
int dx=0,dy=-1;
struct q{
	int x,y;
};
struct snake{
	struct q q[L];
	int l,r;
}snake;
#ifndef	WIN32
int kbhit(){
	struct termios oldt,newt;
	int c;
	int oldf;
	tcgetattr(STDIN_FILENO,&oldt);
	newt=oldt;
	newt.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	oldf=fcntl(STDIN_FILENO,F_GETFL,0);
	fcntl(STDIN_FILENO,F_SETFL,oldf|O_NONBLOCK);
	c=getchar();
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	fcntl(STDIN_FILENO,F_SETFL,oldf);
	if(c!=EOF){
		ungetc(c,stdin);
		return 1;
	}
	return 0;
}
#endif
void init(){
#ifdef	WIN32
	char a[100];
	sprintf(a,"mode con cols=%d lines=%d",W,H);
	system(a);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);
	cci.bVisible=0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);
#else
	char a[100];
	sprintf(a,"resize -s %d %d",H,W);
	system(a);
	printf("\033[2J");
	printf("\033[?25l");
	system("stty -echo");
#endif
}
void quit(){
#ifndef	WIN32
	printf("\033[%d;%dH",0,0);
	printf("\033[2J");
	printf("\033[?25h");
	system("stty echo");
#endif
}
void print(int x,int y,char a[]){
#ifdef	WIN32
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
#else
	printf("\033[%d;%dH",y,x);
#endif
	printf(a);
}
void clr(){
	while(kbhit()){
#ifdef	WIN32
		getch();
#else
		getchar();
#endif
	}
}
void control(){
	char c;
	if(!kbhit()){
		return;
	}
#ifdef	WIN32
	c=getch();
	if(c!=-32){
		clr();
		return;
	}
	c=getch();
	if(c!=72&&c!=80&&c!=75&&c!=77){
		clr();
		return;
	}
#else
	c=getchar();
	if(c!=27){
		clr();
		return;
	}
	c=getchar();
	if(c!=91){
		clr();
		return;
	}
	c=getchar();
	if(c!=65&&c!=66&&c!=68&&c!=67){
		clr();
		return;
	}
#endif
	switch(c){
#ifdef	WIN32
		case 72:
#else
		case 65:
#endif
			if(dx==0&&dy==1){
				break;
			}
			dx=0;
			dy=-1;
			break;
#ifdef	WIN32
		case 80:
#else
		case 66:
#endif
			if(dx==0&&dy==-1){
				break;
			}
			dx=0;
			dy=1;
			break;
#ifdef	WIN32
		case 75:
#else
		case 68:
#endif
			if(dx==1&&dy==0){
				break;
			}
			dx=-1;
			dy=0;
			break;
#ifdef	WIN32
		case 77:
#else
		case 67:
#endif
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
	for(i=0;i<A;i++)
	{
		food();
	}
	while(1){
		control();
		ax=snake.q[snake.r-1].x+dx;
		ay=snake.q[snake.r-1].y+dy;
		if(ax<0||ay<0||ax>W-1||ay>H-1||x[ax][ay]==1){
			print(0,0,"WA");
			getchar();
			quit();
			return;
		}
		if(x[ax][ay]!=-1){
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
			getchar();
			quit();
			return;
		}
#ifdef	WIN32
		sleep(T);
#else
		usleep(T);
#endif
	}
}
int main(){
	init();
	run();
	return 0;
}

