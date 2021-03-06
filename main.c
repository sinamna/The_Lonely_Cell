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
	char* name;
	char player;
	int energy;
	struct cell_el* next;
}cell;
int n;
block** map;
int is_loaded=0;
bool turn_x=true;
/////////////////////////////////////////////////////////////////////////////////////////cell_control
void clears(void){
	system("CLS");
}
char * create_rand_name(void){
	int i;
	//i set the names to be 3 characters
	char *str=(char *)malloc(4*sizeof(char));
	const char charset[]=
	"abcdefghijklmnopqrstuvwxyz";
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
	int counter=1;
	cell* current=player_list;
	if(current!=NULL){
		printf("player %c\n",current->player);
		while(current!=NULL){
			printf("[%d]  %s  ",counter,current->name);
			printf("(%d , %d)",current->x,current->y);
			printf("  energy: %d \n",current->energy);
			current=current->next;
			counter++;
		}
	}
}
//U CAN ADD PREFERRED ENERGY
void add_cell(cell* cell_list,int x,int y,char *name,char c,int energy){
	cell* current=cell_list;
	while(current->next!=NULL){
		current=current->next;
	}
	current->next=(cell*)malloc(sizeof(cell));
	current->next->x=x;
	current->next->y=y;
	current->next->name=name;
	current->next->player=c;
	current->next->energy=energy;
	current->next->next=NULL;
}
//setting the first shits
//first it sets the first node and give it randomized x & y
//then it will create other cells using add_list function
cell* create_list(int n_cell,block** map,char c,int energy){
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
	}while(map[y_temp][x_temp].full);
	player_list->x=x_temp;
	player_list->y=y_temp;
	map[y_temp][x_temp].full=fullness;
	player_list->name=create_rand_name();
	player_list->player=c;
	player_list->energy=energy;
	player_list->next=NULL;
	for (i=1;i<n_cell;i++){
		do{
			x_temp=rand()%n;
			y_temp=rand()%n;
		}while(map[y_temp][x_temp].full);
		//map.full is 0 when its empty
		add_cell(player_list,x_temp,y_temp,create_rand_name(),c,energy);
		map[y_temp][x_temp].full=fullness;
	}
	return player_list;
}


cell* choose_cell(cell* player_list){
	print_visual_map(map);
	cell* current=player_list;
	print_list(player_list);
	printf("please choose one of your cells: \n");
	int input,i;
	scanf("%d",&input);
	for(i=1;i<input;i++){
		current=current->next;
	}
	return current;
}

void move_cell(block**map,cell* cell){
	//if the player couldnt move to a block allow them to make another move
	bool has_moved=false;
	bool border_err=false;
	bool block_err=false;
	while(!has_moved){
		printf("[1] North \n[2] South\n[3] Northeast\n[4] Northwest\n");
		printf("[5] Southeast\n[6] Southwest\n");
		printf("Where do you want to go ? : ");
		int dir_input,x,y;
		x=cell->x;
		y=cell->y;
		int fullness;
		/* fullness type
		x=1
		o=2
		block=3
		empty=0
		*/
		if(cell->player=='X'){
			fullness=1;
		}else{
			fullness=2;
		}
		scanf("%d",&dir_input);
		switch(dir_input){
			case 1:
				//north
				if(y<n-1){
					if(map[y+1][x].full==0){
						cell->y=y+1;
						map[y][x].full=0;
						map[y+1][x].full=fullness;
						has_moved=true;
					}else{
						block_err=true;
						break;
					}
				}else border_err=true;
				//print_visual_map(map);
				break;
			case 2:
				//south
				if(y>0){
						if(map[y-1][x].full==0){
							cell->y-=1;
							map[y][x].full=0;
							map[y-1][x].full=fullness;
							has_moved=true;
						}else {
							block_err=true;
							break;
						}
				}else border_err=true;
				break;
			case 3:
				//northeast
				if(x%2==0){
					if(x<n-1 && y<n-1){
						if(map[y+1][x+1].full==0){
							cell->y+=1;
							cell->x+=1;
							map[y][x].full=0;
							map[y+1][x+1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}else{
						border_err=true;
						break;
					}
				}
				else{
					if(x<n-1 && y<n-1){
						if(map[y][x+1].full==0){
							cell->x+=1;
							map[y][x].full=0;
							map[y][x+1].full=fullness;
							has_moved=true;
						}else {
							block_err=true;
							break;
						}
					}else if(x<n-1 && (y==n-1)){
						if(map[y][x+1].full==0){
							cell->x+=1;
							map[y][x].full=0;
							map[y][x+1].full=fullness;
							has_moved=true;
						}else{
							printf("fuck");
							block_err=true;
							break;
						}
					}
					else{
						border_err=true;
							break;
					}
				}
				break;
			case 4:
				//northwest
				if(x>0 &&y<n-1){
					if(x%2==0){
						if(map[y+1][x-1].full==0){
							cell->x-=1;
							cell->y+=1;
							map[y][x].full=0;
							map[y+1][x-1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}else{
						if(map[y][x-1].full==0){
							cell->x-=1;
							map[y][x].full=0;
							map[y][x-1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}
				}else if(x>0 && (y==n-1)){
					if(x%2==1){
						if(map[y][x-1].full==0){
							cell->x-=1;
							map[y][x].full=0;
							map[y][x-1].full=fullness;
							has_moved=true;
						}
						else{
							block_err=true;
							break;
						}
					}
				}else{
					border_err=true;
					break;	
				}break;
			
			case 5:
				//southeast
				if(x<n-1 &&y>0){
					if(x%2==0){
						if(map[y][x+1].full==0){
							cell->x+=1;
							map[y][x].full=0;
							map[y][x+1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}else{
						if(map[y-1][x+1].full==0){
							cell->y-=1;
							cell->x+=1;
							map[y][x].full=0;
							map[y-1][x+1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}
				}else if ( x<n-1 && (y==0)){
					if(x%2==0){
						if(map[y][x+1].full==0){
							cell->x+=1;
							map[y][x].full=0;
							map[y][x+1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}
				}else{
					border_err=true;
					break;
				}break;
				
				
			case 6:
				//southwest
				if(x>0 &&y>0){
					if(x%2==0){
						if(map[y][x-1].full==0){
							cell->x-=1;
							map[y][x].full=0;
							map[y][x-1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}else{
						if(map[y-1][x-1].full==0){
							cell->x-=1;
							cell->y-=1;
							map[y][x].full=0;
							map[y-1][x-1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}

					}
				}else if(x>0 && (y==0)){
					if(x%2==0){
						if(map[y][x-1].full==0){
							cell->x-=1;
							map[y][x].full=0;
							map[y][x-1].full=fullness;
							has_moved=true;
						}else{
							block_err=true;
							break;
						}
					}
				}
				else{
					border_err=true;
					break;
				}
				break;
		}
		//print_visual_map(map);
		if(border_err==true){
			printf("You can not get out of the map.\n");
			border_err=false;
		}
		if(block_err==true){
			printf("The block is full or forbidden . You cant go there.\n");
			block_err=false;
		}
	}	//print_list();
}
void boost_energy(cell* cell_el){
	int x=cell_el->x;
	int y=cell_el->y;
	if(map[y][x].type=='1'){
		if(map[y][x].energy>=15){
			cell_el->energy+=15;
			map[y][x].energy-=15;
		}else{
			cell_el->energy+=map[y][x].energy;
			map[y][x].energy=0;
			map[y][x].type='4';
		}
	}else{
		printf("You cant boost your energy here .\n");	
	}
}
void split_cell(cell* cell_el,cell* cell_list){
	int x,y;
	x=cell_el->x;
	y=cell_el->y;
	if(cell_el->energy<80){
		printf("You can not split Your cell. You have low energy.\n");
		return;
	}else if(map[y][x].type!='2'){
		printf("This block is no place to split.\n");
	}else{
		int x_temp,y_temp;
		int counter=0;
		int fullness;
		if(cell_el->player=='X')fullness=1;
		else fullness=2;
		do{
			x_temp=rand()%3-1;
			while((x+x_temp<0)||(x+x_temp>n-1)){
				x_temp=rand()%3-1;
			}
			y_temp=rand()%3-1;
			while((y+y_temp<0)||(y+y_temp>n-1)){
				if(x-x_temp==0){
					y_temp=rand()%3-1;
				}else if(x%2==0){
					y_temp=rand()%2;
					
				}else{
					y_temp=rand()%2-1;
				}	
			}	
			counter++;
			if(counter==120)break;
		}while(map[y+y_temp][x+x_temp].full!=0);
		add_cell(cell_list,x+x_temp,y+y_temp,create_rand_name(),cell_el->player,40);
		cell_el->name=create_rand_name();
		cell_el->energy=40;
		map[y+y_temp][x+x_temp].full=fullness;
		
	}
}




//why the .... struct cell*?
int cell_action(cell* cell_el,cell* cell_list){
	printf("[1] Move\n[2] Split a cell\n[3] Boost energy\n");
	printf("[4] Save \n[5] Exit\n Your choice? : ");
	int input;
	scanf("%d",&input);
	switch(input){
		case 1:
			//i start considering  block** map as global pointer
			move_cell(map,cell_el);
			return 1;
		case 2: 
			split_cell(cell_el,cell_list);
			return 1;
		case 3:
			boost_energy(cell_el);
			return 1;
		case 4:
			return 4;
		case 5:
			return 0;
			
	}
}
//////////////////////////////////////////////////////////////////////////////Players handlers
void show_scores(cell* player1,cell* player2){
	cell* current1=player1;
	int player1_score=0;
	while(current1!=NULL){
		//printf("fuck");
		player1_score+=current1->energy;
		current1=current1->next;
	}
	printf("Player X score : %d\n",player1_score);
	if(player2!=NULL){
		cell*current2=player2;
		int player2_score=0;
		while(current2!=NULL){
			player2_score+=current2->energy;
			current2=current2->next;
		}
		printf("Player O score : %d\n",player2_score);
		if(player1_score>player2_score)printf("Player X won :)))))))))))))))))))\n");
		else if(player1_score<player2_score)printf("Player O won :))))))))))))))))\n");
		else printf("No one won :(\n");
	}
}	

void single_player_handler (block** map,cell* player_cell){
	//clear screen here
	//clears();
	textcolor(11);
	printf("SINGLE PLAYER MODE\n");
	textcolor(15);
	if(!is_loaded){
		int cell_num,energy;
		printf("please enter the number of cell(s): ");
		scanf("%d",&cell_num);
		printf("please enter the starting energy of each cell : ");
		scanf("%d",&energy);
		player_cell=create_list(cell_num,map,'X',energy);
	}
	int return_val=1;
	while(return_val){
		return_val=cell_action(choose_cell(player_cell),player_cell);
		if(return_val==4)save_single_state(player_cell);
	}
	
	show_scores(player_cell,NULL);
	//menu(map,player_cell,NULL);
}
void multi_player_handler (block**map,cell* player1_cells,cell* player2_cells){
	textcolor(11);
	printf("MULTI PLAYER MODE\n");
	textcolor(15);
	if(!is_loaded){
		int cell_num_p1,cell_num_p2,energy;
		printf("please enter the number of cell(s) for player 1 : ");
		scanf("%d",&cell_num_p1);
		printf("please enter the number of cell(s) for player 2 : ");
		scanf("%d",&cell_num_p2);
		printf("please enter the starting energy of each cell : ");
		scanf("%d",&energy);
		//if(player1_cells==NULL)
	    player1_cells=create_list(cell_num_p1,map,'X',energy);
	    //if(player2_cells==NULL)
		player2_cells=create_list(cell_num_p2,map,'O',energy);
	}
	print_list(player1_cells);
	print_list(player2_cells);
	printf("\n");
	int return_val;
	while(1){
		if(turn_x){
			return_val=cell_action(choose_cell(player1_cells),player1_cells);
			if(return_val){
				if(return_val==4){
					save_multi_state(player1_cells,player2_cells,1);
				}
				turn_x=false;
			}
			else break;
		}else{
			return_val=cell_action(choose_cell(player2_cells),player2_cells);
		if(return_val){
			if(return_val==4){
				save_multi_state(player1_cells,player2_cells,2);
			}
				turn_x=true;
			}
			else break;
		}
	}
	show_scores(player1_cells,player2_cells);
	//menu(map,player1_cells,player2_cells);
}

//////////////////////////////////////////////////////////////////////////////////map_control
// create_map funcion opens the map file and takes the input parameters
block** create_map(void){
	int i,j;
	if(!is_loaded){
		FILE* fp=NULL;
		char map_name[200];
		printf("Please enter the name of the map: \n");
		scanf("%s",map_name);
    	getchar();
    	fp=fopen(map_name,"rb");
    	printf("\n");
    	if(fp==NULL){
        	printf("can not open file .\n");
    	    return NULL;
    	}
   		fread(&n,sizeof(int),1,fp);
    
	    // creating two dimensional array 
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
	}
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
				//this part uses the map.full to set the center of the squares
				if(map[ip][jp].full==1)visual_map[i][j]='X';
				if(map[ip][jp].full==2)visual_map[i][j]='O';
				if(counter%2==1){
					i-=2;
				}
				counter++;
				jp++;
		}
		counter=0;
		jp=0;
		if(ip!=0)ip--;
	}
	
	return visual_map;
}
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
					//textcolor(15);
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
void menu (block** map,cell* list1,cell* list2){
	printf("[1] Load\n");
	printf("[2] New single player game\n");
	printf("[3] New Multiplayer game\n");
	printf("[4] Exit\n");
	int input;
	printf("Please enter your choice : ");
	scanf("%d",&input);
	switch(input){
		case 1:
			load_main(list1,list2);
			break;
		case 2:
			single_player_handler(map,list1);
			break;
		case 3:
			multi_player_handler (map,list1,list2);
			break;
		case 4:
			break;
	}
	//print_visual_map(map);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////save & load functions
//i see huge problem
int find_list_size(cell* list){
	cell* current=list;
	int counter=0;
	while(current!=NULL){
		current=current->next;
		counter++;
	}
	return counter;
}
void save_single_state(cell* cell_list){
	cell* current=cell_list;
	int i,j,playernum=1;
	FILE* fp=NULL;
	printf("Please enter name of the saved file (*.bin): ");
	char out_name[200];
	char saved_name[4];
	scanf("%s",out_name);
	fp=fopen(out_name,"wb");
	if(fp==NULL){
		printf("Can not save the file.\n");
		return ;
	}
	//first we write the n of the map and then we save the blocks of the map
	fwrite(&playernum,sizeof(int),1,fp);
	fwrite(&n,sizeof(int),1,fp);
	for(i=0;i<n;i++){
		fwrite(map[i],sizeof(block),n,fp);
	}
	//2) we right the size of our list
	int list_size=find_list_size(cell_list);
	fwrite(&list_size,sizeof(int),1,fp);
	//3)we save the list
	
	while(current!=NULL){
		for(i=0;i<4;i++){
			saved_name[i]=current->name[i];
		}
		fwrite(current,sizeof(cell),1,fp);
		fwrite(saved_name,sizeof(char),4,fp);
		current=current->next;
	}
	printf("game has been saved.\n");
	fclose(fp);
}
void save_multi_state(cell* player1_list,cell* player2_list,int player_turn){
	char saved_name[4];
	cell* current1=player1_list;
	cell* current2=player2_list;
	int list1_size=find_list_size(player1_list);
	int list2_size=find_list_size(player2_list);
	int i=0,playernum=2;
	FILE* fp=NULL;
	printf("Please enter name of the saved file (*.bin): ");
	char out_name[200];
	scanf("%s",out_name);
	fp=fopen(out_name,"wb");
	if(fp==NULL){
		printf("Can not save the file.\n");
		return ;
	}
	//1)saving the n of the map and then saving the blocks of the map
	fwrite(&playernum,sizeof(int),1,fp);
	fwrite(&n,sizeof(int),1,fp);
	for(i=0;i<n;i++){
		fwrite(map[i],sizeof(block),n,fp);
	}
	//2)saving cell list of player one and the list
	fwrite(&list1_size,sizeof(int),1,fp);
	while(current1!=NULL){
		for(i=0;i<4;i++){
			saved_name[i]=current1->name[i];
		}
		fwrite(current1,sizeof(cell),1,fp);
		fwrite(saved_name,sizeof(char),4,fp);
		current1=current1->next;
	}
	//3)saving the second list
	fwrite(&list2_size,sizeof(int),1,fp);
	while(current2!=NULL){
		for(i=0;i<4;i++){
			saved_name[i]=current2->name[i];
		}
		fwrite(current2,sizeof(cell),1,fp);
		fwrite(saved_name,sizeof(char),4,fp);
		current2=current2->next;
	}
	//4)saving player turn 
	fwrite(&player_turn,sizeof(int),1,fp);
	printf("game has been saved.\n");
	fclose(fp);
}

void load_main(cell* list1,cell* list2){
	int i,j;
	char input_name[200];
	printf("Enter the name of the loading file : ");
	scanf("%s",input_name);
	FILE* fp=fopen(input_name,"rb");
	if(fp==NULL){
		printf("File could not be loaded.\n");
		return ;
	}
	int playernum;
	fread(&playernum,sizeof(int),1,fp);
	fread(&n,sizeof(int),1,fp);
	//map=(block**)malloc(n*sizeof(block*));
	for(i=0;i<n;i++){
		fread(map[i],sizeof(block),n,fp);
	}
	
	if(playernum==1)single_load(list1,fp);
	else multi_load(list1,list2,fp);
}
void single_load (cell* list1,FILE* fp){
	int size,i;
	char* saved_name_space=NULL;
	fread(&size,sizeof(int),1,fp);
	//we use dummy header add_cell function needs an first cell to be able to add to end of it
	cell* dummy_header=(cell*)malloc(sizeof(cell));
	dummy_header->next=NULL;
	cell* current=(cell*)malloc(sizeof(cell));
	for(i=1;i<=size;i++){
		saved_name_space=(char*)malloc(4*sizeof(char));
		//i cant allocate memory for current.name 
		fread(current,sizeof(cell),1,fp);
		fread(saved_name_space,sizeof(char),4,fp);
		current->next=NULL;
		//add_cell name part takes only pointer to string
		add_cell(dummy_header,current->x,current->y,saved_name_space,current->player,current->energy);
	}
	//printf("fuck.\n");
	list1=dummy_header->next;
	is_loaded=1;
	printf("loaded successfully. :) \n");
	fclose(fp);
	single_player_handler(map,list1);
}
void multi_load(cell* list1,cell* list2,FILE* fp){
	int i,size1,size2,turn;
	cell* current1=(cell*)malloc(sizeof(cell));
	cell* current2=(cell*)malloc(sizeof(cell));
	cell* dummy_head1=(cell*)malloc(sizeof(cell));
	cell* dummy_head2=(cell*)malloc(sizeof(cell));
	dummy_head1->next=NULL;
	dummy_head2->next=NULL;
	char* saved_name_space=NULL;
	fread(&size1,sizeof(int),1,fp);
	for(i=1;i<=size1;i++){
		saved_name_space=(char*)malloc(4*sizeof(char));
		fread(current1,sizeof(cell),1,fp);
		fread(saved_name_space,sizeof(char),4,fp);
		current1->next=NULL;
		add_cell(dummy_head1,current1->x,current1->y,saved_name_space,current1->player,current1->energy);
	}
	fread(&size2,sizeof(int),1,fp);
	for(i=1;i<=size2;i++){
		saved_name_space=(char*)malloc(4*sizeof(char));
		fread(current2,sizeof(cell),1,fp);
		fread(saved_name_space,sizeof(char),4,fp);
		current2->next=NULL;
		add_cell(dummy_head2,current2->x,current2->y,saved_name_space,current2->player,current2->energy);
	}
	list1=dummy_head1->next;
	list2=dummy_head2->next;
	fread(&turn,sizeof(int),1,fp);
	if(turn==1)turn_x=true;
	else turn_x=false;
	is_loaded=1;
	printf("loaded successfully. :) \n");
	fclose(fp);
	multi_player_handler(map,list1,list2);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////// main function
int main(void){
	//reads the file;
	cell* list1=NULL;
	cell* list2=NULL;
	map=create_map();
	srand(time(NULL));
	if(map!=NULL)
		menu(map,list1,list2);
	return 0;
}
