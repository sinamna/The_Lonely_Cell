#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "funcs.h"
#include <windows.h>
#include <stdbool.h>
typedef struct map_block{
    /*  ENERGY = 1,
    MITOSIS,
    FORBIDDEN,
    NORMAL
     */
    char type;
    int energy;
    /* if empty=0
    	  player1=1
    	  player2=2
    	  if type_blocked=3
    	  */
    int full;
}block;
typedef struct cell_el{
	int x;
	int y;
	char * name;
	char player;
	int energy;
	struct cell_el* next;
}cell;
int n;
block** map;
/////////////////////////
char * create_rand_name(void){
	int i;
	//i set the names to be 3 characters
	char *str=(char *)malloc(4*sizeof(char));
	const char charset[]=
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (i=0;i<4;i++){
		int temp=rand()%(sizeof(charset)/sizeof(char)-1);
		str[i]=charset[temp];
	}
	str[4]='\0';
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
void print_list(cell* player_list){
	cell* current=player_list;
	if(current!=NULL){
		printf("player %c\n",current->player);
		while(current!=NULL){
			printf("  %s  ",current->name);
			printf("(%d , %d)\n",current->x,current->y);
			current=current->next;
		}
	}
}
//U CAN ADD PREFERRED ENERGY
void add_cell(cell* cell_list,int x,int y,char *name,char c){
	cell* current=cell_list;
	while(current->next!=NULL){
		current=current->next;
	}
	current->next=(cell*)malloc(sizeof(cell));
	current->next->x=x;
	current->next->y=y;
	current->next->name=name;
	current->next->player=c;
	current->next->next=NULL;
}
//setting the first shits
//first it sets the first node and give it randomized x & y
//then it will create other cells using add_list function
cell* create_list(int n_cell,block** map,char c){
	//fullness is used when we wanna print 
	int i;
	int x_temp,y_temp;
	cell* player_list=(cell*)malloc(sizeof(cell));
	
	if(player_list==NULL){
		printf("Can not allocate space .\n");
		return NULL;
	}
	int fullness;
	if(c=='X'){
		fullness=1;
	}else{
		fullness=2;
	}
	do{
		//n*n is the map blocks
		x_temp=rand()%n;
		y_temp=rand()%n;
	}while(map[y_temp][x_temp].full==true);
	player_list->x=x_temp;
	player_list->y=y_temp;
	map[y_temp][x_temp].full=fullness;
	
	player_list->name=create_rand_name();
	player_list->player=c;
	player_list->next=NULL;
	for (i=1;i<n_cell;i++){
		do{
			x_temp=rand()%n;
			y_temp=rand()%n;
		}while(map[y_temp][x_temp].full==true);
		add_cell(player_list,x_temp,y_temp,create_rand_name(),c);
		map[y_temp][x_temp].full=fullness;
	}
	return player_list;
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
	cell* player_cell=create_list(cell_num,map,'X');
	print_list(player_cell);
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
	cell* player1_list=create_list(cell_num_p1,map,'X');
	cell* player2_list=create_list(cell_num_p2,map,'O');
	print_list(player1_list);
	print_list(player2_list);
	
	
}

////////////////////map_control
// create_map funcion opens the map file and takes the input parameters
block** create_map(void){
	int i,j;
    FILE* fp=NULL;
    fp=fopen("map6.bin","rb");
    if(fp==NULL){
        printf("can not open file .\n");
        return NULL;
    }
    fread(&n,sizeof(int),1,fp);
   // block**
	 map=(block** )calloc(n,sizeof(block *));
    for(i=0;i<n;i++){
        *(map+i)=(block*)calloc(n,sizeof(block));
    }
    // I SHOULD HANDLE PLAYERS PRESENTS IN THE ACTUALL NON_VISUAL MAP
    //I HANDLE IT WITH THE STRUCT MEMBER ((FULL))
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            fread(&map[i][j].type,sizeof(char),1,fp);
            if(map[i][j].type=='1'){
                map[i][j].energy=100;
            }else map[i][j].energy=0;
            //this part handles the forbidden block parts
            if(map[i][j].type=='3')map[i][j].full=3;
            else map[i][j].full=0;
        }
    }
    fclose(fp);
    
    return map;
}
char** visual_map_creator(block** map){
	int i,j,k,d;
	char** visual_map=(char**)calloc(n*5+2,sizeof(char*));
	if(visual_map==NULL){
		printf("can not allocate space .\n");
		return NULL;
	}
	for(i=0;i<5*n+2;i++){
		*(visual_map+i)=(char*)calloc(n*5,sizeof(char));
		if(*(visual_map+i)==NULL){
			printf("can not allocate space .\n");
			return NULL;
		}
	}
	for(i=0;i<5*n+2;i++){
		for(j=0;j<5*n;j++){
			visual_map[i][j]='0';
		}
	}
			
	
	int counter=0;
	int ip=n-1,jp=0;
	for(i=2;i<5*n+2;i+=5){
		for(j=2;j<5*n;j+=5){
				char c=map[ip][jp].type;
				if(counter%2==1){
					i+=2;
				}
			
				for(k=i-2;k<=i+2;k++){
					for(d=j-2;d<=j+2;d++){
						visual_map[k][d]=c;
					}
				}
				if(map[ip][jp].full==1)visual_map[i][j]='X';
				if(map[ip][jp].full==2)visual_map[i][j]='O';
				//visual_map[i][j]=c;
				//counter++;
				if(counter%2==1){
					i-=2;
				}
				counter++;
				jp++;
		}
		jp=0;
		if(ip!=0)ip--;
	}
	
	return visual_map;
}
//THE MOST FUCKING HARD PART
// U SHOULD FUCKING MOVE THE MAP
void print_visual_map(block** map){
	int i,j;
	int counter=0;
	char** visual_map=visual_map_creator(map);
	int ip=n-1,jp=0;
	for(i=0;i<5*n+2;i++){
		for(j=0;j<5*n;j++){
			switch (visual_map[i][j]){
				case '0':
					textcolor(15);
					printf("  ");
					break;
				case '1':
					textcolor(111);
					printf("  ");
					textcolor(15);
					break;
				case '2':
					textcolor(63);
					printf("  ");
					//textcolor(15);
					break;
				case '3':
					textcolor(143);
					printf("  ");
					//textcolor(15);
					break;
				case '4':
					textcolor(95);
					printf("  ");
					//textcolor(15);
					break;	
				case 'X':
					//if(map)
					printf("X ");
					break;
				case 'O':
					printf("O ");
					break;
				
			}
		}
		printf("\n");
	}
	textcolor(15);
}
void print_map(block** map){
	
	int i,j;
    for(i=n-1;i>=0;i--){
        for(j=0;j<n;j++){
            printf("%c   ",map[i][j].type);
        }
        printf("\n");
    }
    //printf("fuck");
}
int menu (block** map){
	printf("[1] Load\n");
	printf("[2] New single player game\n");
	printf("[3] New Multiplayer game\n");
	printf("[4] Exit\n");
	int input;
	printf("Please enter your choice : ");
	scanf("%d",&input);
	switch(input){
		case 1:
			//map_loader();
			break;
		case 2:
			single_player_handler();
			break;
		case 3:
			multi_player_handler ();
			break;
		case 4:
			break;
	}
	print_visual_map(map);
}
int main(void){
	//reads the file;
	block** map=create_map();
	srand(time(NULL));
	menu(map);
	
}