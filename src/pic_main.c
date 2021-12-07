#include "solver.h"
#include "tft_master.h"
#include "tft_gfx.h"
#include <stdio.h>  // required for serial communication

// string buffer
char buffer[60];

/* DEFINE GUI SIGNALS */
#define SOLVE_SIG 1
#define NEXT_SIG 2
#define PREV_SIG 3

/* TFT STATE MACHINE */
#define INIT_STATE 1
#define SOLVING_STATE 2
#define SOLVED_STATE 3

int TFT_STATE = INIT_STATE;

/* GLOBAL STATE */
Move scramble_moves[32] = {};
Move solution_moves[MAX_MOVES] = {};

int num_scramble_moves = 0;
int num_solution_moves = 0;
int tft_move = 0;

void drawTFT() {
    // clear display
    tft_fillScreen(ILI9340_BLACK);

    if (TFT_STATE == INIT_STATE) {
      tft_setCursor(100,100);
      tft_setTextSize(3);
      char title = "RUBOT";
      tft_writeString(title);

    }
    else if (TFT_STATE == SOLVING_STATE) {
        tft_setCursor(100,100);
        char solving = "Solving..."
        tft_writeString(solving);
    }
    else if (TFT_STATE == SOLVED_STATE) {
        // Move <tft_move+1> of <num_solution_moves>
        // <solution_moves[tft_move]>
        tft_setCursor(0,0);
        sprintf(buffer,"Step: %d of %d", tft_move+1,num_solution_moves);
        tft_writeString(buffer);
        
        tft_setTextSize(4);
        tft_setCursor(100,100);
        sprintf(buffer,"%c",solution_moves[tft_move]);
        tft_writeString(buffer);
}

int initHardware() {
    ANSELA = 0; ANSELB = 0;

    // initialize TFT
    tft_init_hw();
    tft_begin();
    tft_setRotation(1); // 320x240 horizontal display

    // setup system-wide interrupts
    //INTEnableSystemMultiVectoredInt();
}

void main() {
    // initialization
    initHardware();
    drawTFT();

    // read and respond to GUI signals
    while (1) {
        if (/* SOLVE signal */) {
            // TODO:
            // set num_scrable_moves
            // set scramble_moves[]

            // scramble cube
            Cube c = scramble(cubeFactory(), scramble_moves, num_scramble_moves);

            // update TFT display
            TFT_STATE = SOLVING_STATE;
            drawTFT();

            // solve cube
            num_solution_moves = solve(c, solution_moves);

            // update TFT display
            TFT_STATE = SOLVED_STATE;
            tft_move = 0;
            drawTFT();
        }
        else if (/* NEXT signal */) {
            if (tft_move < num_solution_moves-1) {
                tft_move++;
            }
            // update TFT display
            drawTFT();
        }
        else if (/* PREV signal */) {
            if (tft_move > 0) {
                tft_move--;
            }
            // update TFT display
            drawTFT();
        }
    }
}
