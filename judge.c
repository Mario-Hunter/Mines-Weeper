
#include <stdio.h>
#include "graph.h"
#include "judge.h"
#include <time.h>
#include "timeHandler.h"
#include "fileHandler.h"
#include <string.h>
#include "mergeSort.h"
#include "binarySearch.h"
#include <pthread.h>
#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif
time_t start;

void resetAlarm(void) {
    time(&start);
}

void *alarmThread(void *x_void_ptr) {

    // casting the judge from a void pointer
    judge_t *judge = (judge_t*) x_void_ptr;
    //printf("starting the count");

    time(&start); // start time of idle time
    while (getTime(time(NULL), start) != 60.0) {
        /*Checks whether a minute passes,and loops until it reaches the limit*/
        
        if(judge->gameState != 1){
            pthread_exit(0);
        }
    }
    // a minute has passed
    calculateTime(judge);
    printf("\nYou have been idle for 1 minute. Time elapsed now  %d min %d  s\n", judge->gameTime / 60, judge->gameTime % 60);
    //setting the alarm again
    alarmThread(judge);
    /* the function must return something - NULL will do */
    return NULL;

}

void *gamePlayThread(void *x_void_ptr) {
    //Creating the judge 
    judge_t *judge = (judge_t*) x_void_ptr;
    // game start time stored in judge
    setTime(&judge->gameStartTime);

    printGrid(judge);

    printf("\nGame is starting...\n");

    while (judge->gameState == 1) {
        // reseting the idle time 

        takeInput(judge); // take  the input
        printGrid(judge);
        printf("I will check you now,opened cells =%d \n", getGraph(judge)->openedCells);

        getScore(judge); // calculate the score
        printf("Your score Now is %ld \n\n", judge->gameScore);
        printf("Number of moves: %d \n",judge->moves);
        printf("time elapsed  %d min %d s \n", judge->gameTime / 60, (judge->gameTime) % 60);
    }
}

char * setUser(void) {
    /* getting the user name */
    printf("Enter Your name:\nYou are permitted 9 characters any more characters will be truncated\n");
    char user[10];
    scanf("%s", &user);
    char userLow[10];
    for(int i=0;i<10;i++){
        userLow[i] =tolower(user[i]);
    }
    //printf("%s",user);
    /*char c ;
    for (int i=0;i<9;i++){
        
        scanf("%c",&c);
        strcat(user,c);
    }
    while (strcmp(c,'\0') != 0){
        scanf("%c",&c);
    }*/
    return userLow;
}

void setLose(judge_t *judge) {
    /* losing the game*/
    judge->gameState = 0; // change the game flag to 0 game ended
    return;
}

void openEmpty(graph_t *graph, judge_t *judge, int v) {
    /* open an empty cell*/
    depthFirstSearch(graph, v);
    return;
}

graph_p createJudge(void) {
    /* Create a judge setting score to 0,game state to 1(game is on)*/
    judge_p judge = (judge_p) malloc(sizeof (judge_t));
    judge->gameScore = 0;
    judge->gameState = 1;
    return judge;
}

__inline graph_p getGraph(judge_t *judge) {

    return judge->grid;
}

void judgeCreateGraph(judge_t *judge, int width, int height, graph_type_e type) {
    /*creating a directed graph with the scanned width and height*/
    judge->grid = createGraph(width*height, DIRECTED);

    judge->grid->height = height;
    judge->grid->width = width;
}

void gamePlay(judge_t *judge) {
    /*Creating 2 threads one for the game the other for the alarm*/
    pthread_t game;
    pthread_t alarm;
    if (pthread_create(&alarm, NULL, alarmThread, judge)) {

        fprintf(stderr, "Error creating thread\n");
        return 1;

    }

    if (pthread_create(&game, NULL, gamePlayThread, judge)) {

        fprintf(stderr, "Error creating thread\n");
        return 1;

    }
    if (pthread_join(alarm, NULL)) {

        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
    if (pthread_join(game, NULL)) {

        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
}

void loadGame() {
    /* opening a saved file*/
    judge_p judge = openFile();
    gamePlay(judge); // continuing the gameplay
}

void startGame(void) {
    /* Creating a judge containing all the user's data*/
    printf("\nGame is starting .. \n");
    printf("MAY THE ODDS BE EVER IN YOUR FAVOR!\n\n");

    judge_p judge = createJudge();
    strcpy(judge->user, setUser());

    int width, height;
    //Read the width and height of the grid
    printf("Please enter the width followed by the height."
            "\nRow and coloumns limitations are ]2,670]\n"
            "CAUTION! large grid size would need a wide screen\n");
    fflush(stdin);
    scanf("%d %d", &width, &height);
    //check the limits of the grid 
    while ((width <= 2 || width > 670) || (height <= 2 || height > 670)) {

        printf("Inappropriate Inputs try again:\n");
        fflush(stdin);
        scanf("%d %d", &width, &height);

    }


    printf("\n\nThe grid is gonna have %d coloumns and %d rows.\n\n", width, height);

    // creating the graph
    judgeCreateGraph(judge, width, height, DIRECTED);
    populateGraph(judge); // connecting the nodes of the graph
    
    /*Setting initial values of the game*/
    getGraph(judge)->mines = 1 + (getGraph(judge)->width * getGraph(judge)->height) / 10;
    getGraph(judge)->flags = 0;
    getGraph(judge)->openedCells = 0;
    judge->gameTime = 0;
    getGraph(judge)->startingNode = -1;
    judge->moves = 0;
    judge->gameScore = 0;
    gamePlay(judge);
    return;
}

void openCell(judge_p *judge, graph_t *graph, int v) {
    printf("\nOpening..\n ");
    int state = getState(graph, v); // state of the cell
    int mines, flags;
    switch (state) {
        case 0:
            if (getMine(graph, v)) {
                /* opening a closed mine*/
                setState(graph, v, 1);
                lose(judge);
            } else {
                /*opening an oridinary cell*/
                setState(graph, v, 2);
                graph->openedCells++;
                if (getAdjacentMine(graph, v) == 0) {
                    //if empty do DFS
                    depthFirstSearch(graph, v);
                }
            }
            break;
        case 2:
            /* opening an open cell*/
            mines = getAdjacentMine(graph, v);
            flags = getAdjacentFlag(graph, v);
            if (mines != 0 && flags == mines) {
                //if number of mines around the cell = the number of flags
                adjlist_node_p node = graph->adjListArr[v].head;
                //open adjacent cells
                while (node) {
                    if (node->state == 0) {
                        openCell(judge, graph, node->vertex);
                    }
                    node = node->next;
                }
            }
            break;
        case 3:
            /*open a flagged cell*/
            if (getMine(graph, v)) {
                // the cell is a mine,lose
                setState(graph, v, 1);
                lose(judge);
            } else {
                setState(graph, v, 2);
                graph->openedCells++;
                if (getAdjacentMine(graph, v) == 0) {
                    //if empty do DFS
                    depthFirstSearch(graph, v);
                }
            }
            break;
        case 4:
            /*open a questioned cell*/
            if (getMine(graph, v)) {
                //the cell is a mine,lose
                setState(graph, v, 1);
                lose(judge);
            } else {
                setState(graph, v, 2);
                graph->openedCells++;
                if (getAdjacentMine(graph, v) == 0) {
                    //if empty do DFS
                    depthFirstSearch(graph, v);
                }
            }
            break;

    }
    return;
}

void flagCell(graph_t *graph, int v) {
    int state = getState(graph, v);
    switch (state) {
        case 0:

            setState(graph, v, 3);
            graph->flags++;
            break;
        case 2:
            printf("You can NOT flag an open cell.\n");
            break;
        case 3:
            printf("You can NOT flag a flagged cell.\n");
            break;
        case 4:
            setState(graph, v, 3);
            graph->flags++;
            break;


    }
}

void questionCell(graph_t *graph, int v) {
    int state = getState(graph, v);
    switch (state) {
        case 0:

            setState(graph, v, 4);
            break;



        case 2:
            printf("You can NOT question an open cell.\n");
            break;

        case 4:

            setState(graph, v, 4);
            break;


    }
}

void checkWin(judge_t *judge) {
    /*check The win condition every move*/
    
    graph_p grid = judge->grid;
    if ((grid ->openedCells + grid->mines == grid->num_vertices) || (grid->mines == grid->flags && grid->flags + grid->openedCells == grid->num_vertices)) {
        long int score = getScore(judge);
        judge->gameState = 2;

        FILE *scores = fopen("./saved/tmp/leaderboard.dat", "r+");
        int users;
        fscanf(scores, "%d", &users);
        //printf("the users in users = %d\n", users);
        int scoresList[users + 1];
        for (int i = 0; i < users; i++) {
            //checks the leaderboard for the user
            char user[100];
            long int score;
            fscanf(scores, "%s %ld", &user, &score);
            if (strcmp(user, judge->user) == 0) {
                //the user exists in the leaderboard 
                printf("\n\n old score:%ld\n\n new score = %ld", score, judge->gameScore);
                //int pos = log(score)+1;
                score += judge->gameScore;
                fseek(scores, -10, SEEK_CUR);
                //prining he total score
                fprintf(scores, "%10ld\n", score);
                fclose(scores);
                finishGame(judge, score);
            }

        }

        fseek(scores, 0, SEEK_END);
        fprintf(scores, "\n%s %10ld", judge->user, judge->gameScore);
        fseek(scores, 0, SEEK_SET);
        users++;
        fprintf(scores, "%d", users);
        fclose(scores);
        finishGame(judge, score);
    }
}

void finishGame(judge_t *judge, int score) {
    printGrid(judge);
    printf("\nYOU WIN!\n\a");
    printf("Time elapsed : %d min %d s\n", judge->gameTime / 60, (judge->gameTime) % 60);
    printf("Number of moves: %d\n", judge->moves);
    printf("Your Score : %ld \n", judge->gameScore);
    judge->gameState = 0;
    /*Get the rank of the user*/
    FILE *leaderboard = fopen("./saved/tmp/leaderboard.dat", "r");
    int users;
    fscanf(leaderboard, "%d", &users);
    player players[users];
    
    for (int i = 0; i < users; i++) {

        fscanf(leaderboard, "%s", &players[i].name);
        fscanf(leaderboard, "%d", &players[i].score);

        //printf("%d  ", scoresList[i]);
    }
    fclose(leaderboard);
    printf("Total Score = %ld\n", score);
    mergeSort(players, users);
    printf("\nYour rank is %d out of %d.\n", users - binarySearch(players, 0, users - 1, score), users);
    free(judge);
    printf("Try again?\n[Y/N]\n");
    char choice;
    fflush(stdin);
    scanf("%c", &choice);
    choice = tolower(choice);

    while (choice != 'y' && choice != 'n') {

        scanf("%c", &choice);

    }
    printf("you chose %c\n", choice);
    switch (choice) {
        case 'y':
            printf("choose y");
            main();
            break;
        case 'n':
            exit(0);

    }
}

void lose(judge_t *judge) {
    judge->gameState = 3; // change game state to 3 for the printing
    judge->gameScore = 0; //score is reset
    printGrid(judge);
    printf("\nYOU LOST\n");
    printf("Time elapsed : %d min %d s\n", judge->gameTime / 60, (judge->gameTime) % 60);
    printf("Number of moves: %d\n", judge->moves);
    printf("Score : %ld \n", judge->gameScore);
    judge->gameState = 0;
    free(judge);
    printf("Try again?\n[Y/N]\n"); 
    char choice;
    fflush(stdin);
    scanf("%c", &choice);
    choice = tolower(choice);

    while (choice != 'y' && choice != 'n') {

        scanf("%c", &choice);

    }
    printf("you chose %c", choice);
    switch (choice) {
        case 'y':
            printf("choose y"); // play a new game
            main();
            break;
        case 'n':
            exit(0); // exit the game

    }

}

void unmark(graph_t *graph, int v) {
    int state = getState(graph, v);
    switch (state) {
        case 0:
            //unmark a closed cell
            printf("This cell is already unmarked");
            break;
        case 2:
            // unmark an open cell
            printf("You can NOT unmark an open cell.\n");
            break;
        case 3:
            // unmark a flagged cell
            setState(graph, v, 0);
            break;
        case 4:
            // unmark a questioned cell
            setState(graph, v, 0);
            break;


    }
}
