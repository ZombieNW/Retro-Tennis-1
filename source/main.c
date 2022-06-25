/*===========================================
    GRRLIB Pong
    Created By ZombieNW
    So He Could Add More Games To His Wii Shelf
    :thumbs_up:
=============================================*/
#include <grrlib.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <stdio.h>

#include "FreeMonoBold_ttf.h"

//Callback for when the reset button is pressed on the console
void WiiResetPressed(){
	SYS_ResetSystem(SYS_RESTART, 0, 0);//Restart System
}

//Callback for when the power button is pressed on the console
void WiiPowerPressed(){
	SYS_ResetSystem(SYS_POWEROFF_STANDBY, 0, 0);//Power-Off System
}

/**
    Callback for when the power button on a Wiimote
    @param[in] chan The Wiimote that pressed the button
*/
void WiimotePowerPressed(s32 chan){
	SYS_ResetSystem(SYS_POWEROFF_STANDBY, 0, 0);//Power-Off System
}

int main(int argc, char **argv) {
    GRRLIB_Init();// Initialise the Graphics & Video subsystem
    WPAD_Init();// Initialise the Wiimotes
    WPAD_SetIdleTimeout(60); //Wiimote is shutdown after 60 seconds of innactivity.

    //Callbacks for when power buttons are pressed, these call voids above the main function
    SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);
    
    //Load the font from memory
    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);

    //Init variables
    int number_of_players = 1; //ammount of players, 1 by default
    int player1ypos = 200; //player one position
    int player1xpos = 50; //this shouldn't change and is just for reference
    int player1width = 10; //this shouldn't change and is just for reference
    int player1height = 60; //this shouldn't change and is just for reference
    int player1score = 0; //player 1 score
    int player2ypos = 200; //player one position
    int player2xpos = 580; //this shouldn't change and is just for reference
    int player2width = 10; //this shouldn't change and is just for reference
    int player2height = 60; //this shouldn't change and is just for reference
    int player2score = 0; //player 2 score
    int overallspeed = 5; //speed of ball and paddles
    s32 CPU_difficulty = 3; //This is the speed(in pixels) the CPU will move the bar.
    /*Hardness:
    2 = Easy
    3 = Normal
    4 = Hard
    5 = Expert
    */
    s32 ballX = 311; //Ball X position.
    s32 ballY = 190; //Ball Y position. 
    s32 ballspeedX =  rand() % 4 + 3; //Ball horitzontal speed. Changes when contacts borders or Bars.
    s32 ballspeedY = rand() % -3 + -2; //Ball vertical speed. Changes when contacts borders or Bars.
    int gamemode = 1;
    /*Gamemodes:
    1 = Main Menu
    2 = Gameplay
    3 = Pause Menu
    */
    int mainmenuselect = 1;
    /*Menumodes:
    1 = 1 Player Mode
    2 = 2 Player Mode
    3 = Exit
    */
    //Functions
    //reset the ball's position and send it flying somewhere random
    void resetball(){
        ballX = 311; //Ball X position.
        ballY = 190; //Ball Y position.
        bool rl = true;
        if ((rand() % 2) + 1 ==  2){
            rl = false;
        }
        if (rl == true){
            ballspeedX =  rand() % 4 + 3; //rand Ball horitzontal speed
            ballspeedY = rand() % -3 + -2; //rand Ball vertical speed
        }
        else{
            ballspeedX = rand() % -4 + -5; //rand Ball horitzontal speed
            ballspeedY = rand() % 3 + 2; //rand Ball vertical speed
        }
    }
    //If players are hitting the ball
    bool player1hittingball(){
        if(ballX > player1xpos + player1width || player1xpos > ballX + 15){ //the 15 is ball width, i didn't wanna make more ints
            return false;
        }
        if(ballY > player1ypos + player1height || player1ypos > ballY + 15){ //the 15 is ball width, i didn't wanna make more ints
            return false;
        }
        return true;
    }
    bool player2hittingball(){
        if(ballX > player2xpos + player2width || player2xpos > ballX + 15){ //the 15 is ball width, i didn't wanna make more ints
            return false;
        }
        if(ballY > player2ypos + player2height || player2ypos > ballY + 15){ //the 15 is ball width, i didn't wanna make more ints
            return false;
        }
        return true;
    }

    // Loop forever
    while(1) {
        if(gamemode == 1){
            //Set The Default Background Color
            GRRLIB_SetBackgroundColour(0xE0, 0xC1, 0x77, 0xFF);

            //controller stuff
            u32 player1pressed = WPAD_ButtonsDown(0);//player1 button up
            WPAD_ScanPads(); // Scan the Wiimotes
            //Title
            GRRLIB_PrintfTTF(85, 16, myFont, "Retro Tennis", 64, 0x000000FF);
            if(mainmenuselect == 1){
                GRRLIB_PrintfTTF(200, 200, myFont, ">1  2 Player Play", 24, 0x000000FF);
                GRRLIB_PrintfTTF(200, 240, myFont, " Exit", 24, 0x000000FF);
                if (player1pressed & WPAD_BUTTON_DOWN){
                    mainmenuselect = 3;
                }
                if (player1pressed & WPAD_BUTTON_RIGHT){
                    mainmenuselect = 2;
                }
                if (player1pressed & WPAD_BUTTON_A){
                    number_of_players = 1;
                    player1score = 0;
                    player2score = 0;
                    gamemode = 2;
                }
            }
            if(mainmenuselect == 2){
                GRRLIB_PrintfTTF(200, 200, myFont, " 1 >2 Player Play", 24, 0x000000FF);
                GRRLIB_PrintfTTF(200, 240, myFont, " Exit", 24, 0x000000FF);
                if (player1pressed & WPAD_BUTTON_DOWN){
                    mainmenuselect = 3;
                }
                if (player1pressed & WPAD_BUTTON_LEFT){
                    mainmenuselect = 1;
                }
                if (player1pressed & WPAD_BUTTON_A){
                    number_of_players = 2;
                    player1score = 0;
                    player2score = 0;
                    gamemode = 2;
                }
            }
            if(mainmenuselect == 3){
                GRRLIB_PrintfTTF(200, 200, myFont, " 1  2 Player Play", 24, 0x000000FF);
                GRRLIB_PrintfTTF(200, 240, myFont, ">Exit", 24, 0x000000FF);
                if (player1pressed & WPAD_BUTTON_UP){
                    mainmenuselect = 1;
                }
                if (player1pressed & WPAD_BUTTON_A){
                    GRRLIB_FreeTTF(myFont); //be a good boy and clear the font from ram
                    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
                    SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);//Return to the system menu     SYS_RETURNTOMENU
                }
            }
        }
        if(gamemode == 2){
            //Set The Default Background Color
            GRRLIB_SetBackgroundColour(0x25, 0x87, 0x1F, 0xFF);
            WPAD_ScanPads(); // Scan the Wiimotes
            u32 player1pressed = WPAD_ButtonsHeld(0);// WPAD_ButtonsDown tells us which buttons were pressed in this loop (the number is by what wiimote) player 1
            u32 player2pressed = WPAD_ButtonsHeld(1);// WPAD_ButtonsDown tells us which buttons were pressed in this loop (the number is by what wiimote) player 2
            //Home Button Return Home
            if (player1pressed & WPAD_BUTTON_HOME || player1pressed & WPAD_BUTTON_PLUS){//If button pressed and the button was the home button
                gamemode = 1;
            }
            
            //Player 1 Controls
            if (player1pressed & WPAD_BUTTON_DOWN){ //move down if down held
                if(player1ypos > 335){} //make sure you didn't hit the bottom of the screen
                else{
                    player1ypos += overallspeed;
                }
            }
            if (player1pressed & WPAD_BUTTON_UP){ //move up if up held
                if(player1ypos < 45){} //make sure you didn't hit the top of the screen
                else{
                    player1ypos -= overallspeed;
                }
            }

            //Player 2 Controls
            if (number_of_players == 1){ //Let the CPU control the right Bar
                if (player2ypos > ballY) player2ypos += - CPU_difficulty; 
                if ((player2ypos < ballY) && (player2ypos < 335)) player2ypos += CPU_difficulty; 
            }
            if (number_of_players == 2){ //Let Wiimote(1) (aka player 2) control the right Bar
                if (player2pressed & WPAD_BUTTON_DOWN){ //move down if down held
                    if(player2ypos > 335){} //make sure you didn't hit the bottom of the screen
                    else{
                        player2ypos += overallspeed;
                    }
                }
                if (player2pressed & WPAD_BUTTON_UP){ //move up if up held
                    if(player2ypos < 45){} //make sure you didn't hit the top of the screen
                    else{
                        player2ypos -= overallspeed;
                    }
                }
            }

            //Ball Controls
            ballX += ballspeedX; //Set the new speed/direction of the ball X
            ballY += ballspeedY; //Set the new speed/direction of the ball Y
            //Bounce off the top
            if(ballY >= 388){ //If the Ball Y position is bigger or equal to 388(bottom border), invert the speed and direction. This simulates a bounce off the border.
                ballspeedY = rand() % -5 + -4;
            }	
            if(ballY <= 40){//If the Ball Y position is smaller or equal to 40(top border), invert the speed and direction. This simulates a bounce off the border.
                ballspeedY = rand() % 4 + 3;
            }
            //Bounce off the player
            if (player1hittingball() == true){
                ballspeedX = rand() % 5 + 4;
                bool rl = true;
                if ((rand() % 2) + 1 ==  2){
                    rl = false;
                }
                if (rl == true){
                    ballspeedY = rand() % -3 + -2; //rand Ball vertical speed
                }
                else{
                    ballspeedY = rand() % 3 + 2; //rand Ball vertical speed
                }
            }
            if (player2hittingball() == true){
                ballspeedX = rand() % -4 + -5;
                bool rl = true;
                if ((rand() % 2) + 1 ==  2){
                    rl = false;
                }
                if (rl == true){
                    ballspeedY = rand() % -3 + -2; //rand Ball vertical speed
                }
                else{
                    ballspeedY = rand() % 3 + 2; //rand Ball vertical speed
                }
            }
            //reset ball and score
            if(ballX >= 800){
                player1score += 1;
                resetball();
            }	
            if(ballX <= -100){
                player2score += 1;
                resetball();
            }

            //Rendering
            //Tennis Court
            GRRLIB_Rectangle(25, 40, 595, 360, 0x0E4Eb5FF, true); //Floor
            GRRLIB_Line(320,40,320,400,0xFFFFFFFF); // Central line, made from 3 lines
            GRRLIB_Line(319,40,319,400,0xFFFFFFFF);
            GRRLIB_Line(321,40,321,400,0xFFFFFFFF);
            GRRLIB_Line(25,40,620,40,0xFFFFFFFF); // Top border, made from 3 lines
            GRRLIB_Line(26,41,619,41,0xFFFFFFFF);
            GRRLIB_Line(27,42,618,42,0xFFFFFFFF);
            GRRLIB_Line(27,400,618,400,0xFFFFFFFF); // Bottom border, made from 3 lines
            GRRLIB_Line(26,401,619,401,0xFFFFFFFF);
            GRRLIB_Line(25,402,620,402,0xFFFFFFFF);
            //Draw Objects
            GRRLIB_Rectangle(player1xpos, player1ypos, player1width, player1height, 0xFFFFFFFF, true);//Draw player 1
            GRRLIB_Rectangle(player2xpos, player2ypos, player2width, player2height, 0xFFFFFFFF, true);//Draw player 2
            GRRLIB_Rectangle(ballX, ballY, 15, 15, 0xFFFFFFFF, true);//Draw ball
            //Draw Text
            //Score
            char p1s [100];
            sprintf(p1s, "%d", player1score);
            char p2s [100];
            sprintf(p2s, "%d", player2score);
            GRRLIB_PrintfTTF(150, 1, myFont, p1s, 32, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(465, 1, myFont, p2s, 32, 0xFFFFFFFF);
            //Player Signs
            GRRLIB_PrintfTTF(30, 415, myFont, "Player 1", 24, 0xFFFFFFFF);
            if (number_of_players == 1){ //if there is only 1 player, display that the right is a cpu
                GRRLIB_PrintfTTF(570, 415, myFont, "CPU", 24, 0xFFFFFFFF);
            }
            if (number_of_players == 2){ //If there are 2 players, display that right is player 2
                GRRLIB_PrintfTTF(500, 415, myFont, "Player 2", 24, 0xFFFFFFFF);
            }
        }
        // Render the frame buffer to the TV
        GRRLIB_Render();
    }
    //What happens if the while loop breaks
    GRRLIB_FreeTTF(myFont);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    SYS_ResetSystem(SYS_RESTART, 0, 0);//Restart System
}