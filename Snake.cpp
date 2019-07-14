#include<iostream>
#include<conio.h>
#include<windows.h>
#include<ctime>
#define LENGTH 1000
#include <cstdlib> 
using namespace std;

void SetColor(int f=7,int b=0)
{
	unsigned short ForeColor=f+16*b;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon,ForeColor);
}

void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; 
	scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput,scrn);
} 

class Map
{
private:
	char wall1;
	char wall2;
public:
	Map();
	void MapWall();	
};

Map::Map(){
	wall1='=';
	wall2='|'; 
}

void Map::MapWall(){
	SetColor(10,0);
	for(int i=0;i<60;i++){
		cout <<wall1;
	}
	cout <<endl;
	for(int i=0;i<27;i++){
		cout <<wall2;
		SetColor();
		for(int j=0;j<58;j++){
			cout <<' ';
		}
		SetColor(10,0);
		cout <<wall2;
		cout <<endl;
	}
	for(int i=0;i<60;i++){
		cout <<wall1;
	}
	cout <<endl;
	SetColor();
}



class Body{
private:	
	char body_figure;
	int x,y;
public:
	Body();
	void CreateBody(int a,int b);
	void RemoveBody();
	void SnakeLocation(int &a,int &b);	
}; 

Body::Body(){
	body_figure = '@';
}

void Body::CreateBody(int a,int b){
	x=a;
	y=b;
	gotoxy(x,y);
	cout << body_figure;
}

void Body::RemoveBody(){
	gotoxy(x,y);
	cout << " ";
}

void Body::SnakeLocation(int &a,int &b){
	a=x;
	b=y;
}

class EggAppear{
private:	
	char Egg_figure;
	int x,y;
public:
	EggAppear();
	void CreateEgg(int a,int b);
	void EggLocation(int &a,int &b);
};

EggAppear::EggAppear(){
	Egg_figure='*';
}

void EggAppear::CreateEgg(int a,int b){
	x=a;
	y=b;
	gotoxy(x,y);
	SetColor(14,0);
	cout << Egg_figure;
	SetColor();
}

void EggAppear::EggLocation(int &a,int &b){
	a=x;
	b=y;
}

class Egg{
private:
	EggAppear egg;
	int x,y;
public:
	void ProduceEgg(int a,int b);
	void SearchEgg(int &a,int &b);
};

void Egg::ProduceEgg(int a,int b){
	egg.CreateEgg(a,b);
}

void Egg::SearchEgg(int &a,int &b){
		egg.EggLocation(x,y);
		a=x;
		b=y;	
}

class Snake:public Egg{
private:
	int x,y;
	int x1,x2,x3,x4,y1,y2,y3,y4;
	int direction;
protected:
	Body SnakeBody[LENGTH];
	int head;
	int tail;
public:
	Snake();
	int SearchSnakeHead();
	int SearchSnakeTail();
	int Direction(int Key);
	int DetermineProduceEgg(int k,int PRO); 
	int DetermineLose(int k,int Lose);
	int Move(int dir,int a,int d,int H,int n);
};

Snake::Snake(){
	head=3;
	tail=0;
	for(int i=1;i<4;i++){
		static int n=30;
		SnakeBody[i].CreateBody(n++,15); 
	}
	gotoxy(61,8);
	cout<<"A:speed++"<<endl;
	gotoxy(61,9);
	cout<<"Z:speed--"<<endl;
}

int Snake::SearchSnakeHead(){
	int a=head;
	return a;	
}
int Snake::SearchSnakeTail(){
	int b=tail;
	return b;	
}
int Snake::Direction(int Key){
	switch(Key){
		case 72:
			if(direction==1||direction==3)
			direction=0;
			break;
		case 77:
			if(direction==0||direction==2)
			direction=1;
			break;
		case 80:
			if(direction==1||direction==3)
			direction=2;
			break;
		case 75:
			if(direction==0||direction==2)
				direction=3;
			break;
		default:
			break;	
	}
	return direction;	

}

int Snake::DetermineProduceEgg(int k,int PRO){
	while(1){
		x4=rand()%58+1;
		y4=rand()%27+1;
		int j=0;
		k=head-tail;
		for(int i=head;i>(head-k);i--){
			//為了不讓蛋在蛇的位置 
			SnakeBody[i].SnakeLocation(x3,y3);
			if((x4==x3)&&(y4==y3)){
				j++;
				break;									
			}	
		}
		if(j==0){
			ProduceEgg(x4,y4);
			PRO=1;
			break;
		}
	}
	return PRO; 
}

int Snake::DetermineLose(int k,int Lose){
	for(int i=(head-1);i>(head-k);i--){
		SnakeBody[head].SnakeLocation(x1,y1);
		SnakeBody[i].SnakeLocation(x2,y2);
		if((x1==x2)&&(y1==y2)){
			Lose++;
			break;
		}
	}
	if(Lose==1){
		SetColor(12,0);
		gotoxy(x1,y1);
		cout<<"@";
	}
	SnakeBody[head].SnakeLocation(x1,y1);
	if((x1==0)||(x1==59)||(y1==0)||(y1==28)){
		SetColor(12,0);
		gotoxy(x1,y1);
		cout<<"@";
		Lose++;
	}
	return Lose;	
}

int Snake::Move(int dir,int a,int d,int H,int n){
	//L:判斷輸贏 
	int L=0;
	//K,H :身體長 
	static int k=H;
	gotoxy(61,10);
	cout<<"Speed:"<<(20-n)*5<<endl;
	gotoxy(61,0);
	cout <<"Length="<<k<<endl;
	SnakeBody[head].SnakeLocation(x,y);
	if(d==0){
		d=DetermineProduceEgg(k,d);
	}
	switch(dir){
		case 0:
			k=(head-tail);
			L=DetermineLose(k,L);
			if(L==1)return 2;
			SnakeBody[head].SnakeLocation(x1,y1);
			SearchEgg(x2,y2);
			if((x1==x2)&&(y1==y2)){
				d=0;
				a=1;
				if(a==1){
					tail = (tail)%LENGTH;						
					a=0;
				}
				k=head-tail;
				gotoxy(61,0);
				cout <<"Length="<<k<<endl;
				if(d==0){
					d=DetermineProduceEgg(k,d);
				}
			}else{
				tail = (tail+1)%LENGTH;
				SnakeBody[tail].RemoveBody();
			}	
			if(y>0) y--;
			head = (head+1)%LENGTH;
			SnakeBody[head].CreateBody(x,y);
			Sleep(n*5);
			break;
		case 1:
			k=(head-tail);
			L=DetermineLose(k,L);
			if(L==1)return 2;
			SnakeBody[head].SnakeLocation(x1,y1);
			SearchEgg(x2,y2);
			if((x1==x2)&&(y1==y2)){
				d=0;
				a=1;
				if(a==1){
					tail = (tail)%LENGTH;						
					a=0;
				}
				k=head-tail;
				gotoxy(61,0);
				cout <<"Length="<<k<<endl;
				if(d==0){
					d=DetermineProduceEgg(k,d);
				}
			}else{
				tail = (tail+1)%LENGTH;
				SnakeBody[tail].RemoveBody();
			}		
			if(x<99) x++;
			head = (head+1)%LENGTH;
			SnakeBody[head].CreateBody(x,y);
			Sleep(n*5);
			break;
		case 2:	
			k=(head-tail);
			L=DetermineLose(k,L);
			if(L==1)return 2;
			SnakeBody[head].SnakeLocation(x1,y1);
			SearchEgg(x2,y2);
			if((x1==x2)&&(y1==y2)){
				d=0;
				a=1;
				
				if(a==1){
					tail = (tail)%LENGTH;						
					a=0;
				}
				k=head-tail;
				gotoxy(61,0);
				cout <<"Length="<<k<<endl;
				if(d==0){
					d=DetermineProduceEgg(k,d);
				}
			}else{
				tail = (tail+1)%LENGTH;
				SnakeBody[tail].RemoveBody();
			}		
			if(y<28) y++;
			head = (head+1)%LENGTH;
			SnakeBody[head].CreateBody(x,y);
			Sleep(n*5);
			break;
		case 3:
			k=(head-tail);
			L=DetermineLose(k,L);
			if(L==1)return 2;
			SnakeBody[head].SnakeLocation(x1,y1);
			SearchEgg(x2,y2);
			if((x1==x2)&&(y1==y2)){
				d=0;
				a=1;
				if(a==1){
					tail = (tail)%LENGTH;						
					a=0;
				}
				k=head-tail;
				gotoxy(61,0);
				cout <<"Length="<<k<<endl;
				if(d==0){
					d=DetermineProduceEgg(k,d);
				}
			}else{
				tail = (tail+1)%LENGTH;
				SnakeBody[tail].RemoveBody();
			}	
			if(x>0) x--;
			head = (head+1)%LENGTH;
			SnakeBody[head].CreateBody(x,y);
			Sleep(n*5);
			break;
	}
	return d;
}

int main(){
	int D=1,k=77,key,n=20;
	static int d=0;
	int a=0,L=0;
	int H=3;
	Map Wall;
	Wall.MapWall();
	
	srand (time(NULL));
	Snake snake;
	
	while(1){
		if(kbhit()){
			switch(key=getch()){
				case 224:
					switch(key=getch()){
						case 72:
							k=72;
							break;
						case 77:
							k=77;
							break;
						case 80:
							k=80;
							break;
						case 75:
							k=75;
							break;
						default:
							break;		
					}
					break;
				case 97:
					if(n>0){
						n--;
					}
					break;
				case 122:
					if(n<20){
						n++;
					}
					break;		
				default:
					gotoxy(61,20);
					SetColor(14,0);
					cout <<"WRONG!!!"<<endl;
					SetColor();
					break;
			}	
		}
		//D:direction a:判斷身體是否加長 d:判斷蛋是否有 H:身體長度 n:速度 
		D=snake.Direction(k);
		d=snake.Move(D,a,d,H,n);
		if(d==2){
			gotoxy(101,15);
			cout << "GAMEOVER!"<<endl;
			SetColor();
			break;
		}
	}
	Sleep(3000);
	system("cls");
	SetColor(14,0);
	cout << "GAMEOVER!"<<endl;
	int P=snake.SearchSnakeHead()-snake.SearchSnakeTail();
	cout << "Length="<<P<<endl;
	SetColor();
	return 0;
	system("pause"); 
}
