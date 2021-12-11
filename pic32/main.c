#include "solver.h"
#include "port_expander_brl4.h"
#include "config_1_3_2.h"
#include "pt_cornell_1_3_2.h"
#include "display.h"
#include <stdio.h>  // required for serial communication

/* Global state */

Cube c;

Move scramble_moves[32] = {};
Move solution_moves[MAX_MOVES] = {};

int num_scramble_moves = 0;
int num_solution_moves = 0;
int tft_move = 0;

/* Serial thread */

static PT_THREAD (protothread_serial(struct pt *pt)) {
    PT_BEGIN(pt);
    static char junk;   
    
    while(1) {
        PT_terminate_char = '\r' ; 
        PT_terminate_count = 0 ; 
        PT_terminate_time = 0 ;
        PT_SPAWN(pt, &pt_input, PT_GetMachineBuffer(&pt_input) );
        
        // PREV signal
        if (PT_term_buffer[0] == 'p' && tft_move > -1) {
            c = applyMove(c, inverseMove(solution_moves[tft_move--]));
            updateTFT(c, solution_moves[tft_move], tft_move, num_solution_moves);
            drawTFT(CUBE_SCREEN);
        }
        // NEXT signal
        else if (PT_term_buffer[0] == 'n' && tft_move < num_solution_moves-1) {
            c = applyMove(c, solution_moves[++tft_move]);
            updateTFT(c, solution_moves[tft_move], tft_move, num_solution_moves);
            drawTFT(CUBE_SCREEN);
        }
        // SOLVE signal
        else if (PT_term_buffer[0] == 's') {
            // get scramble moves
            num_scramble_moves = PT_term_buffer[1];
            int i;
            for (i=0 ; i<num_scramble_moves ; i++) {
                scramble_moves[i] = PT_term_buffer[2+i];
            }

            // get cube
            c = applyMoves(cubeFactory(), scramble_moves, num_scramble_moves);

            // solve cube
            drawTFT(SOLVING_SCREEN);
            num_solution_moves = solve(c, solution_moves);
            tft_move = -1;
            updateTFT(c, tft_move, num_solution_moves);
            drawTFT(CUBE_SCREEN);
        }                         
    }

    PT_END(pt);  
}

void main() {
    // initialize hardware
    // ANSELA = 0; ANSELB = 0;

    // initialize TFT
    tft_init_hw();
    tft_begin();
    tft_setRotation(1); // 320x240 horizontal display
    drawTFT(TITLE_SCREEN);

    // initialize serial communication thread
    PT_setup();
    INTEnableSystemMultiVectoredInt();
    pt_add(protothread_serial, 0);
    PT_INIT(&pt_sched);
    pt_sched_method = SCHED_ROUND_ROBIN;
    PT_SCHEDULE(protothread_sched(&pt_sched));
}
