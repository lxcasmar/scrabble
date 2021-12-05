#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Declaration of outside functions
void delay(int);
char newLetter(void);

//main "game" function
int main(int argc, char **argv)
{
	char again;
	
	//Game cycle: Simon Says that will go up until 20 characters, if person gets all 20 then it is a pass
	do
	{	
		char simon[20];
		char player[20];
		int lose = 0;
		int count = 0;
		int gamemode = -1;
		int difficulty;
		
		while(gamemode < 0 || gamemode > 2)
		{
			printf("What difficulty would you like this game?\n");
			printf("0 is easy (5 char), 1 is normal (10 char), 2 is hard (20 char)\n");
			scanf("%d",&gamemode);
		}
		system("cls");
		switch (gamemode)
		{
			case 0:
				difficulty = 5;
				break;
			case 2:
				difficulty = 20;
				break;
			default:
				difficulty = 10;
		}
		
		while(lose == 0)
		{
			if(count == difficulty)
			{
				break;
			}
			if(count <= 19)
			{
				simon[count + 1] = NULL;
			}
			simon[count] = newLetter();
			system("cls");
			printf("\nRemember the sequence: ");
			printf("%s",simon);
			delay(3);
			system("cls");
			printf("\nWhat was the Sequence: ");
			scanf("%s",player);
			
			
			for(int j = 0; j < count; j++)
			{
				if(player[j] != simon[j])
				{
					lose++;
					break;
				}
			}
			count++;
			if(lose == 1)
			{
				break;
			}
			
		}
		if(lose == 1)
		{
			printf("\nYou Lose!");
		}
		else
		{
			printf("\nYou Win!");
		}
		
		printf("\nWould you like to play again? (Y/N): ");
		scanf(" %c",&again);
		while(again != 'n' && again != 'y')
		{
			printf("\n Error. Would you like to play again? (Y/N): ");
			scanf(" %c",&again);
		}
		system("cls");
	}while(again == 'y');
	
	printf("\nThanks for playing!");
	
	return 0;
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

    
char newLetter(void)
{
	int check;
    time_t t;
    char result;
   
   /* Intializes random number generator */
   srand((unsigned) time(&t));
   
   check = rand() % 3;
   
   switch (check)
   {
		case 0:
			result = 'w';
			break;
		case 1:
			result = 'a';
			break;
		case 2:
			result = 's';
			break;
		case 3:
			result = 'd';
			break;
   }
   
   return result;
}


