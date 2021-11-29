#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void display_board(int DIMENSION, char[DIMENSION][DIMENSION]);
int	 prompt(int, char[]);
void swap(char*,char*);
void get_tiles(char[],char[]);
int	 playing(int DIMENSION, char[DIMENSION][DIMENSION], int p_count, char [p_count][7],int*,char[]);
void swap_single_tile(char[],char[]);
void swap_all_tiles(char[],char[]);

int main (int argc, char* argv[]){
	FILE* dictptr = fopen("Dictionary.txt","r");
	if (!dictptr){
		printf("Dictionary file not found");
		return 0;
	}
	
	//cant use global vars but basically treaat as such
	int DIMENSION = 15;

	int p_count = 0;		// player count
	while (!(p_count >=2 && p_count <=4)){
		printf("Enter number of players [2-4]\n");
		scanf("%d",&p_count);
	}

	char tile_set[100];
	// populate tile set array for the game. ^ is for blank tiles... 
		// * is for 'missing' tiles in tileset (players have them)
	for (int i = 0; i<100; i++){
			if (i < 9)
				tile_set[i] = 'A';
			else if (i<11)
				tile_set[i] = 'B';
			else if (i<13)
				tile_set[i] = 'C';
			else if (i<17)
				tile_set[i] = 'D';
			else if (i<29)
				tile_set[i] = 'E';
			else if (i<31)
				tile_set[i] = 'F';
			else if (i<34)
				tile_set[i] = 'G';
			else if (i<36)
				tile_set[i] = 'H';
			else if (i<45)
				tile_set[i] = 'I';
			else if (i<46)
				tile_set[i] = 'J';
			else if (i<47)
				tile_set[i] = 'K';
			else if (i<51)
				tile_set[i] = 'L';
			else if (i<53)
				tile_set[i] = 'M';
			else if (i<59)
				tile_set[i] = 'N';
			else if (i<67)
				tile_set[i] = 'O';
			else if (i<69)
				tile_set[i] = 'P';
			else if (i<70)
				tile_set[i] = 'Q';
			else if (i<76)
				tile_set[i] = 'R';
			else if (i<80)
				tile_set[i] = 'S';
			else if (i<86)
				tile_set[i] = 'T';
			else if (i<90)
				tile_set[i] = 'U';
			else if (i<92)
				tile_set[i] = 'V';
			else if (i<94)
				tile_set[i] = 'W';
			else if (i<95)
				tile_set[i] = 'X';
			else if (i<97)
				tile_set[i] = 'Y';
			else if (i<98)
				tile_set[i] = 'Z';
			else // last two are blank tiles
				tile_set[i] = '^';
	}
	

	
	/*srand(time(NULL));		// randomize the tile-set... maybe not needed
	for (int i = 99; i > 0; i--){
		int j = rand() % (i+1);
		swap(&tile_set[i], &tile_set[j]);
	}*/
	// each row is a player's 7 tiles
	char player_tiles[p_count][7];
	srand(time(NULL));
	for (int i = 1; i<=p_count;i++){
		get_tiles(tile_set,player_tiles[i]);
	}
	
	/*	test print the tiles users have
	for (int i = 0; i< 7; i++){
		printf(" %c\t %c\n",player_tiles[1][i], player_tiles[2][i]);
	}*/
	
	
	char board[DIMENSION][DIMENSION];
	// initialize board
	for (int i = 0; i< DIMENSION; i++){
		for (int j = 0; j < DIMENSION; j++){
			board[i][j] = 0;
		}
	} 
	int l = 1;
	int ctr = 1;
	while (l)
		l = playing(DIMENSION,board,p_count,player_tiles,&ctr,tile_set);
	return 0;
}

// just saying this would be nicer with macros/global vars
void display_board(int DIMENSION,char board[DIMENSION][DIMENSION]){
	printf("Current state of the board\n");
	for (int i = 0; i< DIMENSION; i++){
		for (int j = 0; j < DIMENSION; j++){
			printf("|");
			if (board[i][j] == 0){
				printf("_");
			}else{
				printf(" %c",board[i][j]);
			}
		}
		printf("|\n");
	}
}

int playing(int DIMENSION, char board[DIMENSION][DIMENSION],int p_count, char player_tiles[p_count][7],int* ctr,char tile_set[100]){
	//printf("\e[1;1H\e[2J");	//regex to clear console
	display_board(DIMENSION,board);
	int res = prompt(*ctr,player_tiles[*ctr]);
	// do smth w prompt
	if (res == 0)		// user wants to quit game
		return 0;
	if (res == 3)
		swap_single_tile(tile_set,player_tiles[*ctr]);
	if (res == 4)
		swap_all_tiles(tile_set,player_tiles[*ctr]);
	
	printf("Player %d\'s turn is over... enter \'C\' to continue\n",*ctr);
	// will really work with any character except ENTER
	char cont = -1;
	scanf(" %c",&cont);
	// increment turn counter	
	*ctr = (*ctr)+1;
	if (*ctr>p_count)	// reset turn counter
		*ctr = 1;
	return 1;
}

void get_tiles(char tile_set[100],char player_set[7]){
	for (int i =0; i<7;i++){
		int j = rand() % (100);
		while (tile_set[j] == '*'){ // this will be bad near end of game... fix later
			j = rand() % (100);
		}
		//printf(" %c\n", tile_set[j]);
		player_set[i] = tile_set[j];
		tile_set[j] = '*';
	}
}

void swap_single_tile(char tile_set[100],char player_set[7]){
	int ans = -1;

	
	while (!(ans > 0 && ans <8)){
		printf("Which tile would you like to swap? Enter 1-7\n");
		scanf("%d",&ans);
	}
		// will need to check j here to see if not always getting same seed
	int j = rand() % 100;
	while (tile_set[j] == '*'){
		j = rand() % 100;
	}
	char temp = player_set[ans-1];
	player_set[ans-1] = tile_set[j];
	printf("Swapped %c for %c\n",temp,tile_set[j]);
	tile_set[j] = temp;
	

}

void swap_all_tiles(char tile_set[100], char player_set[7]){
	
	printf("Your new tiles are:\n");
	for (int i = 0; i<7;i++){
		int j = rand() % 100;
		while (tile_set[j] == '*')
			j = rand()%100;
		char temp = player_set[i];
		player_set[i] = tile_set[j];
		tile_set[j] = temp;
		printf(" %c",player_set[i]);
	}
	printf("\n");
	
}

/* swap elements in memory, gives a weird error... not needed for now anyways
void swap(char* a, char* b){
	char temp = *a;
	*a = *b
	*b = temp;
}*/

int prompt(int ctr,char player_set[7]){
	printf("Player %d, it's your turn! Here are your current tiles:\n",ctr);
	for (int i =0;i<7;i++)
		printf(" %c",player_set[i]);
	printf("\n");
	printf("What would you like to do?\n");
	printf("\t1. Pass (forfeit turn)\n\t2. Place Word\n\t3. Swap a single tile\n\t4. Swap all of your tiles\n\tPress \'0\' to quit\n");
	int in = -1;
	while (!(in >= 0 && in<5)){
		printf("Enter 1-4 or 0\n");
		scanf("%d",&in);
	}
	return in;
}
