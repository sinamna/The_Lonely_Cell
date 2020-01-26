#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>


typedef struct map_block{
    /*  ENERGY = 1,
    MITOSIS,
    FORBIDDEN,
    NORMAL
     */
    char type;
}block;
int n;
block** map;
///////////////////////////////////////////////////
void textcolor (int color){
    static int __BACKGROUND;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
    color + (__BACKGROUND << 2));
}
void save_map(void){
	FILE* fp=NULL;
	int i,j;
	char out_name[200];
	printf("Please enter the name of map (+.bin): ");
	scanf("%s",out_name);
	fp=fopen(out_name,"wb");
	if(fp==NULL){
		printf("Files couldnt be created.\n");
		return ;
	}
	fwrite(&n,sizeof(int),1,fp);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			fwrite(map[i][j].type,sizeof(char),1,fp);
		}
	}
	printf("map saved successfully. :) \n");

}
int menu(){
	print_visual_map();
	printf("[1]Add energy block (yellow).\n");
	printf("[2]Add mistosis block (blue).\n");
	printf("[3]Add forbidden block (dark gray).\n");
	printf("[4]Make map and exit map editor.\n");
	int x,y;
	char input;
	printf("  What do you want? : ");
	scanf(" %c",&input);
	if((input<'5')&&(input>'0')){
		if(input!='4'){
		printf("  please enter x and y of the block : ");
		scanf(" %d %d",&x,&y);	
		}
		switch(input){
			case '1':
				map[y][x].type='1';
				return 1;
			case '2':
				map[y][x].type='2';
				return 1;
			case '3':
				map[y][x].type='3';
				return 1;
			case '4':
				return 0;
			//default:
		}
	}else{
		printf("\nplease enter the correct number .");
		return 1;
	}
}

char** visual_map_creator(void){
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
void print_visual_map(void){
	int i,j;
	int counter=0;
	char** visual_map=visual_map_creator();
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
					textcolor(112);
					printf("  ");
					//textcolor(15);
					break;	
			}
		}
		printf("\n");
	}
	textcolor(15);
}
block** create_map(void){
	int i,j;
	printf("please enter the size of you map : ");
	scanf("%d",&n);
	map=(block** )calloc(n,sizeof(block *));
   	for(i=0;i<n;i++){
   	   	*(map+i)=(block*)calloc(n,sizeof(block));
   	}
   	// setting all the blocks as normal block
   	for(i=0;i<n;i++){
   		for(j=0;j<n;j++){
   			map[i][j].type='4';
   		}
   	}
   	return map;
}

int main (void){
	create_map();
	while(menu());
	save_map();
	return 0;
	
}
