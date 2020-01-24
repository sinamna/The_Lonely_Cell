#include <stdio.h>
#include <stdlib.h>
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
    bool full;
}block;
int n;
block** map;
void textcolor (int color){
    static int __BACKGROUND;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
    color + (__BACKGROUND << 2));
}
// create_map funcion opens the map file and takes the input parameters
block** create_map(void){
    FILE* fp=NULL;
    fp=fopen("map6.bin","rb");
    if(fp==NULL){
        printf("can not open file .\n");
        return NULL;
    }
    fread(&n,sizeof(int),1,fp);
   // block**
	 map=(block** )calloc(n,sizeof(block *));
    for(int i=0;i<n;i++){
        *(map+i)=(block*)calloc(n,sizeof(block));
    }
    // I SHOULD HANDLE PLAYERS PRESENTS IN THE ACTUALL NON_VISUAL MAP
    //I HANDLE IT WITH THE STRUCT MEMBER ((FULL))
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            fread(&map[i][j].type,sizeof(char),1,fp);
            if(map[i][j].type=='1'){
                map[i][j].energy=100;
            }else map[i][j].energy=0;
            //this part handles the forbidden block parts
            if(map[i][j].type=='3')map[i][j].full=true;
            else map[i][j].full=false;
        }
    }
    fclose(fp);
    
    return map;
}
char** visual_map_creator(block** map){
	char** visual_map=(char**)calloc(n*5+2,sizeof(char*));
	if(visual_map==NULL){
		printf("can not allocate space .\n");
		return NULL;
	}
	for(int i=0;i<5*n+2;i++){
		*(visual_map+i)=(char*)calloc(n*5,sizeof(char));
		if(*(visual_map+i)==NULL){
			printf("can not allocate space .\n");
			return NULL;
		}
	}
	for(int i=0;i<5*n+2;i++){
		for(int j=0;j<5*n;j++){
			visual_map[i][j]='0';
		}
	}
			
	
	int counter=0;
	int ip=n-1,jp=0;
	for(int i=2;i<5*n+2;i+=5){
		for(int j=2;j<5*n;j+=5){
				char c=map[ip][jp].type;
				if(counter%2==1){
					i+=2;
				}
				for(int k=i-2;k<=i+2;k++){
					for(int d=j-2;d<=j+2;d++){
						visual_map[k][d]=c;
					}
				}
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
	int counter=0;
	char** visual_map=visual_map_creator(map);
	int ip=n-1,jp=0;
	for(int i=0;i<5*n+2;i++){
		for(int j=0;j<5*n;j++){
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
					textcolor(15);
					break;
				case '3':
					textcolor(143);
					printf("  ");
					textcolor(15);
					break;
				case '4':
					textcolor(95);
					printf("  ");
					textcolor(15);
					break;	
				
			}
		}
		printf("\n");
	}
	textcolor(15);
}
void print_map(block** map){
    for(int i=n-1;i>=0;i--){
        for(int j=0;j<n;j++){
            printf("%c   ",map[i][j].type);
        }
        printf("\n");
    }
    //printf("fuck");
}


int main (void){
	//getchar();
    block** map=create_map();
    print_visual_map(map);
}
