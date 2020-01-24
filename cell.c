#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcs.h"
#include <windows.h>
/*typedef struct cell_el{
	int x;
	int y;
	char name[100];
	int energy;
	struct cell_el* next;
}cell;*/
/* this fucntion is used :
* first when we start the game to randomlyset the cells in the blocks
* and second when we split a cell and we wanna add a cell to the list
*/
extern block** map;
char * create_rand_name(void){
	//i set the names to be 5 characters
	char *str=(char *)malloc(6*sizeof(char));
	const char charset[]=
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i=0;i<6;i++){
		int temp=rand()%(sizeof(charset)/sizeof(char)-1);
		str[i]=charset[temp];
	}
	str[6]='\0';
	return str;
}
void textcolor (int color){
    static int __BACKGROUND;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
    color + (__BACKGROUND << 2));
}
void add_cell(cell* cell_list,int x,int y,char *name){
	cell* current=cell_list;
	for(;current->next!=NULL;current=current->next);
	current->next=(cell*)malloc(sizeof(cell));
	current->next->x=x;
	current->next->y=y;
	current->next->next=NULL;
}
//setting the first shits
//first it sets the first node and give it randomized x & y
//then it will create other cells using add_list function
cell* create_list(int n_cell,block** map){
	int x_temp,y_temp;
	cell* cell_list=(cell*)malloc(sizeof(cell));
	do{
		//n*n is the map blocks
		x_temp=rand()%n;
		y_temp=rand()%n;
	}while(map[y_temp][x_temp].full!=false);
	cell_list->x=x_temp;
	cell_list->y=y_temp;
	cell_list->name=create_rand_name();
	for (int i=1;i<n;i++){
		do{
			x_temp=rand()%n;
			y_temp=rand()%n;
		}while(map[y_temp][x_temp].full!=false);
		add_cell(cell_list,x_temp,y_temp,create_rand_name(	));
	}
}

void clears(void){
	system("CLS");
}
void single_player_handler (void){
	//clear screen here
	textcolor(11);
	printf("SINGLE PLAYER MODE\n");
	textcolor(15);
	int cell_num;
	printf("please enter the number of cell(s): ");
	scanf("%d",&cell_num);
	cell* player_cell=create_list(cell_num,map);
	
}
void multi_player_handler (void){
	//clear screen here
	textcolor(11);
	printf("MULTI PLAYER MODE\n");
	textcolor(15);
	int cell_num_p1,cell_num_p2;
	printf("please enter the number of cell(s) for player 1 : ");
	scanf("%d",&cell_num_p1);
	printf("please enter the number of cell(s) for player 2 : ");
	scanf("%d",&cell_num_p2);
	
	
}

//returns non zero when the client wrote the right number
int choose_player_num(void){
	int player_num;
	printf("please enter the number of player (1 or 2 ) : ");
	scanf("%d",&player_num);
	if(player_num>2 || player_num<1){
		printf("U should have choose the right number bro. =( \n");
		return 0;
	}else{
		switch (player_num){
			case 1:
				single_player_handler();
				return 1;
			case 2:
				multi_player_handler();
				return 1;
		}
	}
	
}
void move_cell(cell* cell_list,block** map){
	
	
}
void split_cell(cell* cell_list,block** map){
	
}
void boost_energy(cell* cell_list,block** map){
	
}

int main (void){
	srand(time(NULL));
	choose_player_num();
	
}










