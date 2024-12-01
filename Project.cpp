
// Getting access to all files 
#include <iostream>
#include "MacUILib.h"
#include "objPos.h"
#include "objPosArrayList.h"
#include "GameMechs.h"
#include "Player.h"
#include "Food.h"
// #include "Food.cpp" cannot call on a non-header file. 


using namespace std;

// #define DELAY_CONST 100000

// used to creating a pointer to point to the heap, pointer will be returned 
Player *myPlayer; // Global pointer 
GameMechs *myGM;
Food *myFood;



// Starting with no user input. 

//  Declaring functions 
void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);



int main(void)
{

    Initialize();

    while(myGM->getExitFlagStatus() == false)  
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();

}


void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    

    // each of the global pointers will point to the heap here. 
    myGM = new GameMechs(); 
    myFood =new Food(myGM);
    myPlayer = new Player(myGM, myFood, myPlayer); 
    
    

    objPosArrayList* playerBody = myPlayer->getPlayerPos();
    objPosArrayList* foodBucket = myFood->getFoodBucket();

    myFood->generateFood(playerBody);
    // myGM->getExitFlagStatus() = false; Why dont you need this
}

void GetInput(void)
{
    myGM->getInput(); 

}

void RunLogic(void)
{
    

    myPlayer ->updatePlayerSpeed();
    myPlayer ->updatePlayerDir();  
    myPlayer ->movePlayer();
    myGM->clearInput();

}

void DrawScreen(void)
{
    MacUILib_clearScreen();  
    bool drawn;
    objPos tempBody;
    objPos tempFoodPos;

    // holds the playerPosList 
    objPosArrayList* playerBody = myPlayer->getPlayerPos();
    objPosArrayList* foodBucket = myFood->getFoodBucket();
    
// Debugging purposes. 
    // for (int i = 0; i < foodBucket->getSize(); i++)
    // {
    //     tempFoodPos = foodBucket->getElement(i);
    //     MacUILib_printf("Food position = %d\n", tempFoodPos.pos->x);
    // }

    // MacUILib_printf("Player [x, y, symbol]: [%d][%d][%c]", playerPos.pos -> x,
    //             playerPos.pos->y, playerPos.symbol);    

    static int count = 0; 
    count ++; 
    for (int i = 0; i < myGM->getBoardSizeY(); i++)
    {
        for(int j = 0; j < myGM->getBoardSizeX(); j++)
        {

            drawn = false;
            for(int k = 0; k < playerBody->getSize(); k++)
            {
               tempBody =  playerBody->getElement(k);

                if(tempBody.pos->x == j && tempBody.pos->y == i)
                {
                    MacUILib_printf("%c", tempBody.symbol);
                    drawn =true;
                    break;
                }
            }

            if(drawn) continue;
         

            for(int l = 0; l < foodBucket->getSize(); l++)
            {
                tempFoodPos = foodBucket->getElement(l);
                if(tempFoodPos.pos->x == j && tempFoodPos.pos->y == i)
                {
                    MacUILib_printf("%c", tempFoodPos.symbol);
                    drawn = true;
                }
            }

          
            if(drawn) continue;

            // draw border
            if(i == 0 || i == (myGM->getBoardSizeY() - 1)|| j == 0 || j == (myGM->getBoardSizeX() -1))
            {
                MacUILib_printf("%c", '#');
            }
            else 
            {
                MacUILib_printf("%c", ' ');
            }
            
        }
        MacUILib_printf("\n");
    }

    MacUILib_printf("Press ESC to quit\n");


    // testing if the game runs
    // MacUILib_printf("%d", count);

    //Generating Ran

    // We will be using this to print the speed on the screen. 
    
    // Adding some information for the user to see. 
   
    
    // int speed = (myPlayer->delay)/(1000000.0);

    // if(myPlayer->delay > 0)
    // {
    //     MacUILib_printf("Current Game Speed: %.3f\n", speed);
    // }
    // else MacUILib_printf(" ");

    MacUILib_printf("Fastest game speed = '%.3f', ", 0.045);
    MacUILib_printf("Slowest game speed = '%.2f'\n", 0.0);
    MacUILib_printf("To increase your speed press '=', To decrease your speed press '-'\n");
    MacUILib_printf("Mystery Food = '!': (+10 score, +0 length) OR ='@' (+5 score, +5 length) \n");
    MacUILib_printf("Score: %d\n", myGM->getScore());
    MacUILib_printf("Size: %d\n", playerBody->getSize());
    MacUILib_printf("\n");

    // Debugging purposes. 
    // for (int i = 0; i < foodBucket->getSize(); i++)
    // {
    //     tempFoodPos = foodBucket->getElement(i);
    //     MacUILib_printf("Food position = %d\n", tempFoodPos.pos->x);
    // }
    


}

void LoopDelay(void)
{
    MacUILib_Delay(myPlayer->delay); 
}


void CleanUp(void)
{
    MacUILib_clearScreen();  


    if(myGM->getLoseFlagStatus() == true){
        MacUILib_printf("You Lose! You ate yourself! \n");
    } else {
        MacUILib_printf("You Quit! You left the game! \n");
    }

    delete myGM; 
    delete myPlayer;  // deallocating 
    delete myFood; 


    MacUILib_uninit();
}
