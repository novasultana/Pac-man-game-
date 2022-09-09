#include <stdio.h> // printf()
#include <conio.h> // getch()
#include <windows.h>
#include <stdbool.h>
#include<time.h>
#define H 30
#define W 60
#define NR_GHOSTS 15

   //for color on the game

int colorPrint(char color[],char str[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
    if(strcmp(color,"red")==0)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    }

    if(strcmp(color,"blue")==0)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    }

    if(strcmp(color,"green")==0)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    }


    printf("%s",str);
    SetConsoleTextAttribute(hConsole, saved_attributes);

    return 0;
}

int count =0;
int lives=3;

 //starting Easy Mode
EasyMode()
{
    struct coord
    {
        int x;
        int y;
    };

    struct PacMan
    {
        struct coord position;
        int vx;
        int vy;
        int lives;
        bool chasing;
        int food_collected;
    };
    struct Ghost
    {
        struct coord position;
        int vx;
        int vy;
        bool chasing;
    };
    struct Ghost allGhosts[0];

    struct PacMan myPacMan =
    {
        {
            .x = 1,
            .y = 1,
        },
        .vx = 0,
        .vy = 0,

        .chasing = false,
        .food_collected = 0
    };


    char playfield[H][W] =
    {

        { "############################################################" },
        { "#                                                          #" },
        { "########################################################## #" },
        { "##                                                    #### #" },
        { "## ################################################## #### #" },
        { "## ###                                             ## #### #" },
        { "## ### ########################################### ## #### #" },
        { "## ### ###                                     ### ## #### #" },
        { "## ### ### ################################### ### ## #### #" },
        { "## ### ### ###                             ### ### ## #### #" },
        { "## ### ### ### ########################### ### ### ## #### #" },
        { "## ### ### ### ####                    ### ### ### ## #### #" },
        { "## ### ### ### #### ################## ### ### ### ## #### #" },
        { "## ### ### ### #### ################## ### ### ### ## #### #" },
        { "## ### ### ### #### ###                ### ### ### ## #### #" },
        { "## ### ### ### #### ###################### ### ### ## #### #" },
        { "## ### ### ### ####                        ### ### ## #### #" },
        { "## ### ### ### ############################### ### ## #### #" },
        { "## ### ### ###                                 ### ## #### #" },
        { "## ### ### ####################################### ## #### #" },
        { "## ### ###                                         ## #### #" },
        { "## ### ############################################## #### #" },
        { "## ###                                                #### #" },
        { "## ####################################################### #" },
        { "##                                                         #" },
        { "############################################################" },

    }; // <-- CAUTION! Semicolon necessary!

    void initialize()
    {
        // 1. replace each empty field in the playfield
        //    with a food field
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (playfield[i][j]==' ')
                    playfield[i][j] = '.';
            }
        }

        srand(time(0));

        // 2. initialize all ghosts
        int N_GHOSTS=0;
        for (int i = 0; i < N_GHOSTS; i++)
        {
            allGhosts[i].vx = 0;
            allGhosts[i].vy = 0;
            allGhosts[i].chasing = true;

            // try to find a (x,y) coordinate randomly where a food piece is
            int x,y;
            do
            {
                //define field
                x = 1 + rand() % (W-1);
                y = 1 + rand() % (H-1);

            }
            while (playfield[y][x] != '.');
            allGhosts[i].position.x = x;
            allGhosts[i].position.y = y;
            playfield[y][x] = 'G';



        }


    } // initialize


    void user_input()
    {
        if (_kbhit())
        {
            char c1 = _getch();

            if (c1 == -32)
            {
                char c2 = _getch();

                myPacMan.vx = 0;
                myPacMan.vy = 0;

                switch (c2)
                {
                case 72:
                    myPacMan.vy = -1;
                    break; // cursor up
                case 80:
                    myPacMan.vy = +1;
                    break; // cursor down
                case 75:
                    myPacMan.vx = -1;
                    break; // cursor left
                case 77:
                    myPacMan.vx = +1;
                    break; // cursor right
                }
            }


            //printf("c1=%d c2=%d\n", c1, c2);

        }
    }


    void move_figures()
    {


        // 1. delete PacMan from old position
        playfield[myPacMan.position.y][myPacMan.position.x] = ' ';

        // 2. compute new desired coordinate (nx,ny)
        int nx = myPacMan.vx + myPacMan.position.x;
        int ny = myPacMan.vy + myPacMan.position.y;
        if ( playfield[ny][nx]=='G')
        {
            myPacMan.vx = 0;
            myPacMan.vy = 0;


            lives--;
        }
        if(lives<=0)
        {
            count=1;
        }
        // 3. test whether there is a wall at (nx,ny)
        if (playfield[ny][nx] == '#')
        {
            // Damn! There is a wall! Stop PacMan!
            myPacMan.vx = 0;
            myPacMan.vy = 0;
        }

        // 4. update PacMan's coordinate
        myPacMan.position.x += myPacMan.vx;
        myPacMan.position.y += myPacMan.vy;

        // 5. is there a food piece at the new location?
        if (playfield[ny][nx] == '.')
        {
            myPacMan.food_collected++;
        }

        // 6. put PacMan back again to playfield
        playfield[myPacMan.position.y][myPacMan.position.x] = 'P';



    }


    void show_playfield()
    {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if(playfield[i][j]=='#')
                {
                    colorPrint("red", "#");
                }
                else if(playfield[i][j]=='G')
                {
                    colorPrint("blue", "G");
                }
                else if(playfield[i][j]=='P')
                {
                    colorPrint("green", "P");
                }
                else
                {
                    printf("%c", playfield[i][j]);
                }
            }
            printf("\n");
        }

        printf("Total Score: %d\n", myPacMan.food_collected);
        int score=myPacMan.food_collected;
        if(score==591)
        {
            system("cls");

            colorPrint("green","\nCongratulations!!!\nYou Win the match.\n");
            exit(1);
        }
        //printf("Lives: %d",lives);
    }


// Set cursor position in console
    void set_cursor_position(int x, int y)
    {
        //Initialize the coordinates
        COORD coord = { x, y };
        //Set the position
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    } // set_cursor_position



    void hidecursor()
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    system("cls");
    hidecursor();
    initialize();

    while (1)
    {
        user_input();
        move_figures();
        show_playfield();

        Sleep( 1000 / 30 );
        set_cursor_position(0,0);
        if(count==1)
        {
            break;
        }

    }
    system("cls");
    printf("GAME OVER \n");


}//end Easy mode


    // starting of Moderate Mode

ModerateMode()
{

    struct coord
    {
        int x;
        int y;
    };

    struct PacMan
    {
        struct coord position;
        int vx;
        int vy;
        int lives;
        bool chasing;
        int food_collected;
    };
    struct Ghost
    {
        struct coord position;
        int vx;
        int vy;
        bool chasing;
    };
    struct Ghost allGhosts[NR_GHOSTS];


    struct PacMan myPacMan =
    {
        {
            .x = 1,
            .y = 1,
        },
        .vx = 0,
        .vy = 0,

        .chasing = false,
        .food_collected = 0
    };
    char playfield[H][W] =
    {
        { "############################################################" },
        { "#                                                         ###" },
        { "###     ############     ############      ############  ###" },
        { "###     ############     ############      ############  ###" },
        { "###     ############     ############      ############  ###" },
        { "###                                                      ###" },
        { "###                                                      ###" },
        { "###              #########          #########            ###" },
        { "###              #########          #########            ###" },
        { "###                   ######## #########                 ###" },
        { "###                   ######## #########                 ###" },
        { "###                      ##### ######                    ###" },
        { "###                                                      ###" },
        { "###                      ##### ######                    ###" },
        { "###                   ######## #########                 ###" },
        { "###                   ######## #########                 ###" },
        { "###              #########         #########             ###" },
        { "###              #########         #########             ###" },
        { "### #########                                  ######### ###" },
        { "### #########                                  ######### ###" },
        { "###    ###                                        ###    ###" },
        { "###    ###                                        ###    ###" },
        { "###                                                      ###" },
        { "###  ###############                    ###############  ###" },
        { "###  ###############                    ###############  ###" },
        { "############################################################" },

    }; // <-- CAUTION! Semicolon necessary!

    void initialize()
    {
        // 1. replace each empty field in the playfield
        //    with a food field
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (playfield[i][j]==' ')
                    playfield[i][j] = '.';
            }
        }
        srand(time(0));
        int N_GHOSTS=7;
        // 2. initialize all ghosts
        for (int i = 0; i < N_GHOSTS; i++)
        {
            allGhosts[i].vx = 0;
            allGhosts[i].vy = 0;
            allGhosts[i].chasing = true;

            // try to find a (x,y) coordinate randomly where a food piece is
            int x,y;
            do
            {
                x = 1 + rand() % (W-1);
                y = 1 + rand() % (H-1);

            }
            while (playfield[y][x] != '.');
            allGhosts[i].position.x = x;
            allGhosts[i].position.y = y;
            playfield[y][x] = 'G';



        }


    } // initialize


    void user_input()
    {
        if (_kbhit())
        {
            char c1 = _getch();

            if (c1 == -32)
            {
                char c2 = _getch();

                myPacMan.vx = 0;
                myPacMan.vy = 0;

                switch (c2)
                {
                case 72:
                    myPacMan.vy = -1;
                    break; // cursor up
                case 80:
                    myPacMan.vy = +1;
                    break; // cursor down
                case 75:
                    myPacMan.vx = -1;
                    break; // cursor left
                case 77:
                    myPacMan.vx = +1;
                    break; // cursor right
                }
            }


            //printf("c1=%d c2=%d\n", c1, c2);

        }
    }


    void move_figures()
    {


        // 1. delete PacMan from old position
        playfield[myPacMan.position.y][myPacMan.position.x] = ' ';

        // 2. compute new desired coordinate (nx,ny)
        int nx = myPacMan.vx + myPacMan.position.x;
        int ny = myPacMan.vy + myPacMan.position.y;
        if ( playfield[ny][nx]=='G')
        {
            myPacMan.vx = 0;
            myPacMan.vy = 0;


            lives--;

        }
        if(lives<=0)
        {
            count=1;
        }
        // 3. test whether there is a wall at (nx,ny)
        if (playfield[ny][nx] == '#')
        {
            // Damn! There is a wall! Stop PacMan!
            myPacMan.vx = 0;
            myPacMan.vy = 0;
        }

        // 4. update PacMan's coordinate
        myPacMan.position.x += myPacMan.vx;
        myPacMan.position.y += myPacMan.vy;

        // 5. is there a food piece at the new location?
        if (playfield[ny][nx] == '.')
        {
            myPacMan.food_collected++;
        }

        // 6. put PacMan back again to playfield
        playfield[myPacMan.position.y][myPacMan.position.x] = 'P';



    }


    void show_playfield()
    {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if(playfield[i][j]=='#')
                {
                    colorPrint("green", "#");
                }
                else if(playfield[i][j]=='G')
                {
                    colorPrint("red", "G");
                }
                else if(playfield[i][j]=='P')
                {
                    colorPrint("blue", "P");
                }
                else
                {
                    printf("%c", playfield[i][j]);
                }
            }
            printf("\n");
        }

        printf("Total Score: %d\n", myPacMan.food_collected);
        printf("Lives: %d",lives);
        int score=myPacMan.food_collected;

        if(score==913)
        {
            system("cls");
            colorPrint("green","\n\tCongratulations!!!\n\nYou Win this Level of the GAME!!!.\n");
            exit(1);
        }
    }


// Set cursor position in console
    void set_cursor_position(int x, int y)
    {
        //Initialize the coordinates
        COORD coord = { x, y };
        //Set the position
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    } // set_cursor_position



    void hidecursor()
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    system("cls");
    hidecursor();
    initialize();

    while (1)
    {
        user_input();
        move_figures();
        show_playfield();

        Sleep( 1000 / 30 );
        set_cursor_position(0,0);
        if(count==1)
        {
            break;
        }

    }
    system("cls");
    colorPrint("red","\tGAME OVER \n\n\nOops!!!!YOU LOSE THE GAME...\n\n\n");


}   //end moderate

//Starting of Hard Mode

HardMode()
{

    struct coord
    {
        int x;
        int y;
    };

    struct PacMan
    {
        struct coord position;
        int vx;
        int vy;
        int lives;
        bool chasing;
        int food_collected;
    };
    struct Ghost
    {
        struct coord position;
        int vx;
        int vy;
        bool chasing;
    };
    struct Ghost allGhosts[NR_GHOSTS];


    struct PacMan myPacMan =
    {
        {
            .x = 1,
            .y = 1,
        },
        .vx = 0,
        .vy = 0,

        .chasing = false,
        .food_collected = 0
    };

    char playfield[H][W] =
    {

        { "#######################################################" },
        { "#                         ###                        ##" },
        { "##   ####  ############   ###   ############  ####   ##" },
        { "##   ####  ############   ###   ############  ####   ##" },
        { "##   ####  ############   ###   ############  ####   ##" },
        { "##                                                   ##" },
        { "##    #####        ##  ############  ##    #####     ##" },
        { "##    #####        ##       ##       ##    #####     ##" },
        { "##    #####        #######  ##   ######    #####     ##" },
        { "##    #####        ##       ##       ##    #####     ##" },
        { "##                 ##                ##              ##" },
        { "######### ###          #####   #####         ### ######" },
        { "#          ##   ####   ##         ##  ####   ##       #" },
        { "#           #   ####   #############  #  #   ##       #" },
        { "#          ##   ####                  ####   ##       #" },
        { "#############   ####   #############         ##########" },
        { "##                           ##                      ##" },
        { "##       ########            ##    #########         ##" },
        { "##       ########            ##    #########         ##" },
        { "##       ########            ##    #########         ##" },
        { "##                                                   ##" },
        { "##                                                   ##" },
        { "#######################################################" },
        { "#######################################################" }
    };
    void initialize()
    {
        // 1. replace each empty field in the playfield
        //    with a food field
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (playfield[i][j]==' ')
                    playfield[i][j] = '.';
            }
        }
        srand(time(0));
        int N_GHOSTS=15;
        // 2. initialize all ghosts
        for (int i = 0; i < N_GHOSTS; i++)
        {
            allGhosts[i].vx = 0;
            allGhosts[i].vy = 0;
            allGhosts[i].chasing = true;

            // try to find a (x,y) coordinate randomly where a food piece is
            int x,y;
            do
            {
                x = 1 + rand() % (W-1);
                y = 1 + rand() % (H-1);

            }
            while (playfield[y][x] != '.');
            allGhosts[i].position.x = x;
            allGhosts[i].position.y = y;

            playfield[y][x] = 'G';


        }


    } // initialize


    void user_input()
    {
        if (_kbhit())
        {
            char c1 = _getch();

            if (c1 == -32)
            {
                char c2 = _getch();

                myPacMan.vx = 0;
                myPacMan.vy = 0;

                switch (c2)
                {
                case 72:
                    myPacMan.vy = -1;
                    break; // cursor up
                case 80:
                    myPacMan.vy = +1;
                    break; // cursor down
                case 75:
                    myPacMan.vx = -1;
                    break; // cursor left
                case 77:
                    myPacMan.vx = +1;
                    break; // cursor right
                }
            }


            //printf("c1=%d c2=%d\n", c1, c2);

        }
    }


    void move_figures()
    {


        // 1. delete PacMan from old position
        playfield[myPacMan.position.y][myPacMan.position.x] = ' ';

        // 2. compute new desired coordinate (nx,ny)
        int nx = myPacMan.vx + myPacMan.position.x;
        int ny = myPacMan.vy + myPacMan.position.y;
        if ( playfield[ny][nx]=='G')
        {
            myPacMan.vx = 0;
            myPacMan.vy = 0;


            lives--;
        }
        if(lives<=0)
        {
            count=1;
        }
        // 3. test whether there is a wall at (nx,ny)
        if (playfield[ny][nx] == '#')
        {
            // Damn! There is a wall! Stop PacMan!
            myPacMan.vx = 0;
            myPacMan.vy = 0;
        }

        // 4. update PacMan's coordinate
        myPacMan.position.x += myPacMan.vx;
        myPacMan.position.y += myPacMan.vy;

        // 5. is there a food piece at the new location?
        if (playfield[ny][nx] == '.')
        {
            myPacMan.food_collected++;
        }

        // 6. put PacMan back again to playfield
        playfield[myPacMan.position.y][myPacMan.position.x] = 'P';



    }


    void show_playfield()
    {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if(playfield[i][j]=='#')
                {
                    colorPrint("red", "#");
                }
                else if(playfield[i][j]=='G')
                {
                    colorPrint("blue", "G");
                }
                else if(playfield[i][j]=='P')
                {
                    colorPrint("green", "P");
                }
                else
                {
                    printf("%c", playfield[i][j]);
                }

            }
            printf("\n");
        }

        printf("Score: %d\n", myPacMan.food_collected);
        printf("Lives: %d",lives);
        int score=myPacMan.food_collected;

        if(score==913)
        {
            system("cls");
            colorPrint("Green","\n\tCongratulations!!!\n\nYou Win the match.\n");
            // printf("\n\tCongratulations!!!\n\nYou Win the match.\n");
            exit(1);
        }
    }


// Set cursor position in console
    void set_cursor_position(int x, int y)
    {
        //Initialize the coordinates
        COORD coord = { x, y };
        //Set the position
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    } // set_cursor_position



    void hidecursor()
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    system("cls");
    hidecursor();
    initialize();

    while (1)
    {
        user_input();
        move_figures();
        show_playfield();

        Sleep( 1000 / 30 );
        set_cursor_position(0,0);
        if(count==1)
        {
            break;
        }

    }
    system("cls");
    colorPrint("red","\tGAME OVER \n\n\nOops!!!!YOU LOSE THE GAME...\n\n\n");

}  //end hard mode



int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    printf("$$$                 $$$ $$$$$$$$  $$         $$$$$$$    $$$$$   $$$     $$$ $$$$$$$$ \n");
    printf("  $$       $       $$   $$        $$        $$        $$     $$ $$$$   $$$$ $$       \n");
    printf("   $$     $$$     $$    $$        $$        $$        $$     $$ $$ $$ $$ $$ $$       \n");
    printf("    $$   $$ $$   $$     $$$$$$    $$        $$        $$     $$ $$  $$$  $$ $$$$$$   \n");
    printf("     $$ $$   $$ $$      $$        $$        $$        $$     $$ $$       $$ $$       \n");
    printf("      $$$     $$$       $$        $$        $$        $$     $$ $$       $$ $$       \n");
    printf("       $       $        $$$$$$$$  $$$$$$$$   $$$$$$$    $$$$$   $$       $$ $$$$$$$$ \n");
    printf("                                                                                     \n");
    SetConsoleTextAttribute(hConsole, saved_attributes);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    printf("                            $$$$$$$$$$$$   $$$$$                                     \n");
    printf("                                 $$       $$   $$                                    \n");
    printf("                                 $$       $$   $$                                    \n");
    printf("                                 $$       $$   $$                                    \n");
    printf("                                 $$       $$   $$                                    \n");
    printf("                                 $$        $$$$$                                     \n");
    printf("                                                                                     \n");

    SetConsoleTextAttribute(hConsole, saved_attributes);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("             $$$$$$    $$$$$$   $$$$$$  $$      $$$  $$$$$$  $$     $$                \n");
    printf("             $$   $$  $$    $$ $$       $$$    $$$$ $$    $$ $$$    $$                \n");
    printf("             $$   $$  $$    $$ $$       $$ $$ $$ $$ $$    $$ $$ $$  $$                \n");
    printf("             $$$$$    $$$$$$$$ $$       $$  $$$  $$ $$$$$$$$ $$  $$ $$                \n");
    printf("             $$       $$    $$ $$       $$       $$ $$    $$ $$   $$$$                \n");
    printf("             $$       $$    $$  $$$$$$  $$       $$ $$    $$ $$    $$$                \n");
    printf("\n\n\n");


    SetConsoleTextAttribute(hConsole, saved_attributes);


    int level;

    printf("Which level you want to play ?\n");
    colorPrint("green"," 1. Easy\n");
    colorPrint("green"," 2. Moderate\n");
    colorPrint("green"," 3. Hard\n");

    printf("Enetr Level: ");
    scanf("%d",&level);



    if(level==1)
    {
        EasyMode();
    }
    else if(level==2)
    {
        ModerateMode();
    }

    else if(level==3)
    {
        HardMode();
    }

    else
    {
        printf("WRONG INPUT ....!!!!");
    }


}


