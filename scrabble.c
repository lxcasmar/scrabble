/** @file scrabble.c
 * @brief A console scrabble game. 
 * Follows these <a href="https://www.scrabblepages.com/scrabble/rules/">rules</a>
 * <p>
 * Is rebust in terms of user interaction.
 * No use of global variables according to project specification, which
 * makes the file more verbose than it could be.
 * Pending for future implementation:
 * &emsp;Diagonal word placement
 * &emsp;Efficient storage and search of dictionary (DelaBrandais Trie would do fine)
 * &emsp;Detect existing tiles on board when placing a new word
 * &emsp;Detect when game-ending conditions have been met
 * 
 * @author Luis Castellanos
 * @author Jacob Jones
 * @author Jacqueline Dion
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Function prototypes
void display_board(int DIMENSION, char[DIMENSION][DIMENSION],int*);
int	 prompt(int, char[]);
void swap(char*,char*);
void get_tiles(char[],char[]);
int	 playing(int DIMENSION, char[DIMENSION][DIMENSION], int p_count, char [p_count][7],int*,char[],FILE*);
void swap_single_tile(char[],char[]);
void swap_all_tiles(char[],char[]);
int instance_of(int,char[],char);
void place_word(int DIMENSION, char[DIMENSION][DIMENSION],char[],char[],FILE*);
void  display_board_w(int DIMENSION,char board[DIMENSION][DIMENSION],int* ctr);
int check_word_in_dict(char*,FILE*,int);
int map_char(char c);

/**
 * @brief Initializes board,tile set, and runs the main game loop
 */
int main (int argc, char* argv[]){
	// Dictionary file should be in same directory as this file
	FILE* dictptr = fopen("Dictionary.txt","r");		
	if (!dictptr){
		printf("Dictionary file not found");
		return 0;
	}
	
	int DIMENSION = 15;		//15x15 board

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
	
	// each row is a player's 7 tiles
	char player_tiles[p_count][7];
	srand(time(NULL));
	for (int i = 1; i<=p_count;i++){
		get_tiles(tile_set,player_tiles[i]);
	}
	
	char board[DIMENSION][DIMENSION];
	// initialize board
	for (int i = 0; i< DIMENSION; i++){
		for (int j = 0; j < DIMENSION; j++){
			board[i][j] = 0;
		}
	} 
	int l = 1;		// playing boolean
	int ctr = 1;	// user to track player turn
	while (l)
		l = playing(DIMENSION,board,p_count,player_tiles,&ctr,tile_set,dictptr);
	fclose(dictptr);
	return 0;
}

/**
 * @brief prints the current state of the board to the console
 * 
 * Important: This function in unused at the moment... It uses ANSI escape
 * sequences that only work in UNIX systems.
 */
void display_board(int DIMENSION,char board[DIMENSION][DIMENSION],int* ctr){
	printf("Current state of the board:\n");
	for (int i = 0; i <DIMENSION; i++){	// first line stuff
		printf("\e[4(;4)m    \e[0(;0)m");
	}
	printf("\e[4(;4)m \e[0(;0)m\n");
	//printf("\n");
	for (int i = 0; i< DIMENSION; i++){
		if (i != 0 && i % 15 == 0)
			printf("\e[4(;4)m|\e[0(;0)m");
		if (i != DIMENSION ){						// top spacing
			for (int k =0; k<=DIMENSION; k++){
				printf("|   ");
			}
		}
		printf("\n");
		for (int j = 0; j < DIMENSION; j++){
			//printf("\e[4(;4)m|  \e[0(;0)m");
			printf("| "); 
			if (board[i][j] == 0){
				//printf("\e[4(;4)m \e[0(;0)m");
				printf("  ");
			}else if (*ctr == 1){
				printf("\e[36m%c\e[0(;0)m ",board[i][j]);
				//printf("%c",board[i][j]);
			}else if (*ctr == 2){
				printf("\e[34m%c\e[0(;0)m ",board[i][j]);
			}else if (*ctr == 3){
				printf("\e[33m%c\e[0(;0)m ",board[i][j]);
			}else{
				printf("\e[31m%c\e[0(;0)m ",board[i][j]);
			}
		}
		printf("\e[4(;4)m|\e[0(;0)m\n");
		
		//if (i != DIMENSION - 1){
			for (int k =0; k<DIMENSION; k++){	
				//printf("|   ");
				printf("\e[4(;4)m|   \e[0(;0)m");
			}
			//printf("\n");
			//for (int k = 0; k <= DIMENSION; k++){
			//	printf("|   ");
			//}
		//}
		printf("\e[4(;4)m|\e[0(;0)m\n");
		
	}
}

/**
 * @brief Prints the current state of the board to the console
 */
void  display_board_w(int DIMENSION,char board[DIMENSION][DIMENSION],int* ctr){
	printf("Current state of the board:\n");
	for (int i = 0; i<DIMENSION;i++)
		printf(" ___");
	printf("\n");
	for (int i = 0; i<DIMENSION;i++){
		printf("|");
		for (int j = 0; j<DIMENSION;j++){
			if (board[i][j] == 0)		// print empty space if no tile at current location
				printf("   |");
			else
				printf(" %c |",board[i][j]);		// place the letter at current location
		}
		printf("\n|");
		for (int k = 0; k<DIMENSION; k++)
			printf("___|");
		printf("\n");
	}
			
}

/**
 * @brief Main game loop. Asks player how they would like to play their turn
 */
int playing(int DIMENSION, char board[DIMENSION][DIMENSION],int p_count, char player_tiles[p_count][7],int* ctr,char tile_set[100],FILE* dictptr){
	display_board_w(DIMENSION,board,ctr);		// print current state of the board
	int res = prompt(*ctr,player_tiles[*ctr]);
	
	// do nothing if user wants to pass
	if (res == 0)		// user wants to quit game
		return 0;
	if (res == 2)		// user wants to place a word
		place_word(DIMENSION,board,player_tiles[*ctr],tile_set,dictptr);
	if (res == 3)		// user wants to swap one tile from their tileset
		swap_single_tile(tile_set,player_tiles[*ctr]);
	if (res == 4)		// user wants to swap all their tiles 
		swap_all_tiles(tile_set,player_tiles[*ctr]);
	
	// Pause until user input so they can observe results of their turn
	printf("Player %d\'s turn is over... enter any character to continue\n",*ctr);
	char cont = -1;
	scanf(" %c",&cont);
	// increment turn counter	
	*ctr = (*ctr)+1;
	if (*ctr>p_count)	// reset turn counter
		*ctr = 1;
	return 1;
}

/**
 * @brief Count the instances of <em>c</em> in <em>arr</em>
 */
int instance_of(int size, char arr[size], char c){
	int sum = 0;
	for (int i = 0; i< size; i++){
		if (arr[i] == c)	//increment sum if found element
			sum++;
	}
	return sum;
}

/**
 * @brief Get index of first instace of <em>c</em> in <em>arr</em>. -1 If not found
 */
int index_of(int size, char arr[size], char c){
	for (int i = 0; i<size; i++){
		if (arr[i] == c)
			return i;
	}
	return -1;
}

/**
 * @brief Process for user to place a word on the board
 */
void place_word(int DIMENSION, char board[DIMENSION][DIMENSION], char player_set[7],char tile_set[100],FILE* dictptr){
	begin: ;
	int num_let = 0;
	while (!(num_let>0 && num_let<8)){		//prompt for number of leters
		printf("How many letters are in your word?\n");
		scanf("%d",&num_let);
	}
	char word[num_let];
	for (int i = 0; i< num_let; i++){ // need to re-initialize each time cuz garbage
		word[i] = 0;
	}
	int counts[7];
	for (int i = 0; i <7; i++){
		counts[i] = instance_of(7,player_set,player_set[i]);
	}
	// Get the word from user, letter by letter & error check
	char t = -1;
	int bool,bool2,single_letter = 0;
	int i;
	for (i = 0; i< num_let; i++){
		loop_start: ;
		bool = 0;	// force the loop
		bool2 = 1;
		while (!bool | bool2){
			printf("Enter letter: %d\n", i+1);
			scanf(" %c", &t);
			bool = instance_of(7,player_set,t);	
			bool2 = instance_of(num_let,word,t);
			
			// two types of error check
			if (!bool)
				printf("The letter you entered is not in your tile set. Try again\n");
			if (bool2 && counts[index_of(7,player_set,t)] > 1){
				counts[index_of(7,player_set,t)] = counts[index_of(7,player_set,t)] - 1;
				bool2 = 0;
			}else if (bool2)
				printf("You already entered this/these letter(s). Try again\n");
		}	
		word[i] = t;		// add char to word 
		
		if (single_letter)		// user was only swapping single letter
			goto skip;
	}
	skip: ;
	word[num_let] = '\0';		
	// check if word is in dictionary
	printf("\n\t%s\n",word);
	
	int in_dict = check_word_in_dict(word,dictptr,num_let);
	if (!in_dict){
		//in_dict = check_word_in_dict(word,dictptr,num_let);
		printf("Word \"%s\" is not in dictionary\n",word);
		int ot = 0;
		// provide 3 options to resolve conflict
		while (!(ot>0 && ot<4)){	//error check
			printf("Would you like to:\n\t1.Replace entire word?\n\t2.Replace a letter and try again?\n\t3.Skip Turn?\n");
			scanf("%d",&ot);
		}
		if (ot == 1)		// replace entire word
			goto begin;
		else if (ot == 2){		// replace single letter
			i = -1; 
			while (!(i>=0 && i < num_let)){		// ask which letter to replace
				printf("Enter letter number to replace [1-%d]\n",num_let);
				scanf("%d",&i);
				i--;
			}
			single_letter = 1;
			goto loop_start;
		}
		else if (ot == 3){	// skip turn 
			goto end;
		}
	}
	
	// Now need to actually place the word 
	int x = -1, y = -1,dir=0;	
	printf("%c will be the reference character.\n",word[0]);
	
	// prompt and error check for Dimensions 
	while ( !(x>0 && x<=DIMENSION)){
		printf("Enter desired X coordinate of the reference tile [1-15]\n");
		scanf("%d",&x);
	}
	while ( !(y>0 && y <= DIMENSION)){
		printf("Enter desired Y coordinate of the reference tile [1-15]\n");
		scanf("%d",&y);
	}
	
	// Get one of four (currently) Directions... also doesn't yet check if will cause ouyt of bounds
	printf("Which direction will the word be placed in?\n");
	while ( !(dir>0 && dir <=4)){
		printf("Enter [1-4]\n\t1. North->South\n\t2. South->North\n\t3. East->West\n\t4. West->East\n");
		scanf("%d",&dir);
	}
	
	// place word in direction specified by user. Then replace the tile
	if (dir == 1){		// North->South
		for (int i =0;i<num_let;i++){
			board[y+i-1][x-1] = word[i];
			int j = rand() % 100;
			while (tile_set[j] == '*')
				j = rand() % 100;
			player_set[i] = tile_set[j];
			tile_set[j] = '*';
			printf("Replaced %c with %c\n",word[i],player_set[i]);
		}
	}else if (dir == 2){ 	//South->north
		for (int i =0;i<num_let;i++){
			board[y-i-1][x-1] = word[i];
			int j = rand() % 100;
			while (tile_set[j] == '*')
				j = rand() % 100;
			player_set[i] = tile_set[j];
			tile_set[j] = '*';
			printf("Replaced %c with %c\n",word[i],player_set[i]);
		}
	}else if (dir == 3){	// East->West
		for (int i =0;i<num_let;i++){
			board[y-1][x+i-1] = word[i];
			int j = rand() % 100;
			while (tile_set[j] == '*')
				j = rand() % 100;
			player_set[i] = tile_set[j];
			tile_set[j] = '*';
			printf("Replaced %c with %c\n",word[i],player_set[i]);
		}
	}else{		// West->East
		for (int i =0;i<num_let;i++){
			board[y-1][x-i-1] = word[i];
			int j = rand() % 100;
			while (tile_set[j] == '*')
				j = rand() % 100;
			player_set[i] = tile_set[j];
			tile_set[j] = '*';
			printf("Replaced %c with %c\n",word[i],player_set[i]);
		}
	}
	end: ;
}

/**
 * @brief Gets whether a word is in the dictionary
 */
int check_word_in_dict(char* word, FILE* dictptr,int num_let){
	rewind(dictptr);
	char temp [16] = "                ";	// words are at most 15 characters (+ null terminator)
	word[num_let] = '\0';
	
	for (int status = fscanf(dictptr,"%s",temp); status != EOF; status = fscanf(dictptr,"%s",temp)){
		
		if (strcmp(temp,word) == 0)		// return 1 if found
			return 1;
		
	}
	return 0;		// not found
}

/**
 * @brief Initializes a player's tile set by drawing 7 ranom tiles from the tile pile
 */
void get_tiles(char tile_set[100],char player_set[7]){
	for (int i =0; i<7;i++){
		int j = rand() % (100);
		while (tile_set[j] == '*'){ // this will be bad near end of game... fix later 
			j = rand() % (100);	// keep getting random num until find a letter
		}
		player_set[i] = tile_set[j];
		tile_set[j] = '*';		// * is used to indicate empty space
	}
}

/**
 * @brief Swaps a single tile from a player's tileset with a random tile from the tile pile
 */
void swap_single_tile(char tile_set[100],char player_set[7]){
	int ans = -1;

	while (!(ans > 0 && ans <8)){		// ask for tile to swap and error check
		printf("Which tile would you like to swap? Enter 1-7\n");
		scanf("%d",&ans);
	}
		
	int j = rand() % 100;
	while (tile_set[j] == '*'){		// find non-empty space in tile pile
		j = rand() % 100;
	}
	char temp = player_set[ans-1];
	player_set[ans-1] = tile_set[j];	// print status after
	printf("Swapped %c for %c\n",temp,tile_set[j]);
	tile_set[j] = temp;
	

}

/**
 * @brief Swaps every tile from a player's tileset with seven random tiles from the tile pile
 */
void swap_all_tiles(char tile_set[100], char player_set[7]){
	
	printf("Your new tiles are:\n");
	for (int i = 0; i<7;i++){
		int j = rand() % 100;
		while (tile_set[j] == '*')		// find non-empty space in tile-pile
			j = rand()%100;
		// swap the tiles
		char temp = player_set[i];
		player_set[i] = tile_set[j];
		tile_set[j] = temp;
		printf(" %c",player_set[i]);	// print the current swapped tile
	}
	printf("\n");
	
}

/**
 * @brief Asks the user how they would like to use their turn.
 */
int prompt(int ctr,char player_set[7]){
	printf("Player %d, it's your turn! Here are your current tiles:\n",ctr);
	for (int i =0;i<7;i++)
		printf(" %c",player_set[i]);
	printf("\n");
	printf("What would you like to do?\n");
	printf("\t1. Pass (forfeit turn)\n\t2. Place Word\n\t3. Swap a single tile\n\t4. Swap all of your tiles\n\tPress \'0\' to quit\n");
	int in = -1;
	while (!(in >= 0 && in<5)){		//error check options
		printf("Enter 1-4 or 0\n");
		scanf("%d",&in);
	}
	return in;
}

int map_char(char c){
	return 0;
}
