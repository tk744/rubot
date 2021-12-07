#include "solver.h"
#include "tft_master.h"
#include "tft_gfx.h"
#include <stdio.h>  // required for serial communication

// string buffer
char buffer[60];

struct Face {
    int begin_x0;
    int begin_y0;
    int begin_x1;
    int begin_y1;

    int end_x0;
    int end_y0;
    int end_x1;
    int end_y1;

    int x_mult;
    int y_mult;

    int x_length;
    int y_length;
    unsigned short color0;
    unsigned short color1;
    unsigned short color2;
    unsigned short color3;
    unsigned short color4;
    unsigned short color5;
    unsigned short color6;
    unsigned short color7;
    unsigned short color8;
};

struct Face right= { .begin_x0 = 110, .begin_y0 = 105, .begin_x1 = 140, .begin_y1 = 67,
                          .end_x0 = 110, .end_y0 = 165, .end_x1 = 140, .end_y1 = 127,
                          .x_mult = 0, .y_mult = 1, .x_length =30, .y_length = 60 , .color0 = ILI9340_YELLOW,
                          .color1 = ILI9340_YELLOW, .color2 = ILI9340_YELLOW, .color4 = ILI9340_YELLOW,
                          .color3 = ILI9340_YELLOW, .color5 = ILI9340_YELLOW, .color6 = ILI9340_YELLOW,
                          .color7 = ILI9340_YELLOW, .color8 = ILI9340_YELLOW};
struct Face back = { .begin_x0 = 260, .begin_y0 = 105, .begin_x1 = 290, .begin_y1 = 67,
                          .end_x0 = 110, .end_y0 = 165, .end_x1 = 140, .end_y1 = 127,
                          .x_mult = 0, .y_mult = 1, .x_length =30, .y_length = 60 , .color0 = ILI9340_BLUE,
                          .color1 = ILI9340_BLUE, .color2 = ILI9340_BLUE, .color4 = ILI9340_BLUE,
                          .color3 = ILI9340_BLUE, .color5 = ILI9340_BLUE, .color6 = ILI9340_BLUE,
                          .color7 = ILI9340_BLUE, .color8 = ILI9340_BLUE};

struct Face front = { .begin_x0 = 50, .begin_y0 = 100, .begin_x1 = 110, .begin_y1 = 105,
                          .end_x0 = 50, .end_y0 = 160, .end_x1 = 110, .end_y1 = 165,
                          .x_mult = 0, .y_mult = 1, .x_length =60, .y_length = 60 , .color0 = ILI9340_GREEN,
                          .color1 = ILI9340_GREEN, .color2 = ILI9340_GREEN, .color4 = ILI9340_GREEN,
                          .color3 = ILI9340_GREEN, .color5 = ILI9340_GREEN, .color6 = ILI9340_GREEN,
                          .color7 = ILI9340_GREEN, .color8 = ILI9340_GREEN};

struct Face left = { .begin_x0 = 200, .begin_y0 = 100, .begin_x1 = 260, .begin_y1 = 105,
                          .end_x0 = 200, .end_y0 = 160, .end_x1 = 260, .end_y1 = 165,
                          .x_mult = 0, .y_mult = 1, .x_length =60, .y_length = 60 , .color0 = ILI9340_WHITE,
                          .color1 = ILI9340_WHITE, .color2 = ILI9340_WHITE, .color4 = ILI9340_WHITE,
                          .color3 = ILI9340_WHITE, .color5 = ILI9340_WHITE, .color6 = ILI9340_WHITE,
                          .color7 = ILI9340_WHITE, .color8 = ILI9340_WHITE};

struct Face up = {.begin_x0 = 80,.begin_y0 = 62, .begin_x1 = 140, .begin_y1 = 67,
                          .end_x0 = 50, .end_y0 = 100, .end_x1 = 110, .end_y1 = 105,
                          .x_mult = -1, .y_mult = 1, .x_length =60, .y_length = 30,
                          .color0 = ILI9340_RED,  .color1 = ILI9340_RED, .color2 = ILI9340_RED,
                          .color3 = ILI9340_RED,    .color4 = ILI9340_RED, .color5 = ILI9340_RED,
                          .color6 = ILI9340_RED,     .color7 = ILI9340_RED, .color8 = ILI9340_RED};

struct Face down = {.begin_x0 = 230,.begin_y0 = 62, .begin_x1 = 290, .begin_y1 = 67,
                          .end_x0 = 200, .end_y0 = 100, .end_x1 = 260, .end_y1 = 105,
                          .x_mult = -1, .y_mult = 1, .x_length =60, .y_length = 30,
                          .color0 = ILI9340_ORANGE,  .color1 = ILI9340_ORANGE, .color2 = ILI9340_ORANGE,
                          .color3 = ILI9340_ORANGE,    .color4 = ILI9340_ORANGE, .color5 = ILI9340_ORANGE,
                          .color6 = ILI9340_ORANGE,     .color7 = ILI9340_ORANGE, .color8 = ILI9340_ORANGE};

/* TFT STATE MACHINE */
#define INIT_STATE 1
#define SOLVING_STATE 2
#define SOLVED_STATE 3

int TFT_STATE = INIT_STATE;

/* GLOBAL STATE */
Cube c = cubeFactory();

Move scramble_moves[32] = {};
Move solution_moves[MAX_MOVES] = {};

int num_scramble_moves = 0;
int num_solution_moves = 0;
int tft_move = 0;

unsigned int color_select(struct Face this_face, int i){
    switch (i) {
        case 0:
            return this_face.color0;
        case 1:
            return this_face.color1;
        case 2:
            return this_face.color2;
        case 3:
            return this_face.color3;
        case 4:
            return this_face.color4;
        case 5:
            return this_face.color5;
        case 6:
            return this_face.color6;
        case 7:
            return this_face.color7;
        case 8:
            return this_face.color8;
        default:
            return;
    }
}

void color_change(Cube c) {
    ColorCube cc = convertCube(c);

    //front
    front.color0 = cc.F.UL;
    front.color1 = cc.F.U;
    front.color2 = cc.F.UR;
    front.color3 = cc.F.L;
    front.color4 = cc.F.C;
    front.color5 = cc.F.R;
    front.color6 = cc.F.DL;
    front.color7 = cc.F.D;
    front.color8 = cc.F.DR;

    //right
    right.color0 = cc.R.UL;
    right.color1 = cc.R.U;
    right.color2 = cc.R.UR;
    right.color3 = cc.R.L;
    right.color4 = cc.R.C;
    right.color5 = cc.R.R;
    right.color6 = cc.R.DL;
    right.color7 = cc.R.D;
    right.color8 = cc.R.DR;

    //up
    up.color0 = cc.U.UL;
    up.color1 = cc.U.U;
    up.color2 = cc.U.UR;
    up.color3 = cc.U.L;
    up.color4 = cc.U.C;
    up.color5 = cc.U.R;
    up.color6 = cc.U.DL;
    up.color7 = cc.U.D;
    up.color8 = cc.U.DR;

    //back
    back.color0 = cc.B.DR;
    back.color1 = cc.B.D;
    back.color2 = cc.B.DL;
    back.color3 = cc.B.R;
    back.color4 = cc.B.C;
    back.color5 = cc.B.L;
    back.color6 = cc.B.UR;
    back.color7 = cc.B.U;
    back.color8 = cc.B.UL;

    //left
    left.color0 = cc.L.DR;
    left.color1 = cc.L.D;
    left.color2 = cc.L.DL;
    left.color3 = cc.L.R;
    left.color4 = cc.L.C;
    left.color5 = cc.L.L;
    left.color6 = cc.L.UR;
    left.color7 = cc.L.U;
    left.color8 = cc.L.UL;

    //down
    down.color0 = cc.D.UR;
    down.color1 = cc.D.R;
    down.color2 = cc.D.DR;
    down.color3 = cc.D.U;
    down.color4 = cc.D.C;
    down.color5 = cc.D.D;
    down.color6 = cc.D.UL;
    down.color7 = cc.D.L;
    down.color8 = cc.D.DL;
}

void draw(){
    int j;
    int k;
    struct Face face = right;
    for (j = 0; j < 9; j++){
        int third_x = face.x_length/3;
        int third_y = (face.begin_y0 - face.begin_y1)/3;

        int x_off = (j%3)*third_x;
        int y_off = (j%3)*third_y;
        int row_off = (j/3) *(face.y_length/3);

        int begin_x = face.begin_x0 + x_off;
        int begin_y = face.begin_y0 - y_off + row_off;

        int end_x = face.begin_x0 + ((j%3) + 1)*(third_x);
        int end_y = face.begin_y0 - ((j%3)+1)*(third_y) + row_off;

        for (k = 1; k < face.y_length/3; k++){
            tft_drawLine(begin_x,begin_y + (k),end_x,end_y + (k),color_select(right,j));
        }

    }
    face = back;
    for (j = 0; j < 9; j++){
        int third_x = face.x_length/3;
        int third_y = (face.begin_y0 - face.begin_y1)/3;
        int k;
        int x_off = (j%3)*third_x;
        int y_off = (j%3)*third_y;
        int row_off = (j/3) *(face.y_length/3);

        int begin_x = face.begin_x0 + x_off;
        int begin_y = face.begin_y0 - y_off + row_off;

        int end_x = face.begin_x0 + ((j%3) + 1)*(third_x);
        int end_y = face.begin_y0 - ((j%3)+1)*(third_y) + row_off;

        for (k = 1; k < face.y_length/3; k++){
            tft_drawLine(begin_x,begin_y + (k),end_x,end_y + (k),color_select(face,j));
        }

    }
    face = front;
    for (j = 0; j < 9; j++){
            int third_x = front.x_length/3;
            int third_y = (front.begin_y0 - front.begin_y1)/3;
            int k;
            int x_off = (j%3)*third_x;
            int y_off = (j%3)*third_y;
            int row_off = (j/3) *(front.y_length/3);

            int begin_x = front.begin_x0 + x_off;
            int begin_y = front.begin_y0 - y_off + row_off;

            int end_x = front.begin_x0 + ((j%3) + 1)*(third_x);
            int end_y = front.begin_y0 - ((j%3)+1)*(third_y) + row_off;

            for (k = 1; k < (front.y_length/3) + 2; k++){
                tft_drawLine(begin_x ,begin_y + (k),end_x,end_y + (k),color_select(front,j));
            }

       }
    face = left;
    for (j = 0; j < 9; j++){
            int third_x = face.x_length/3;
            int third_y = (face.begin_y0 - face.begin_y1)/3;
            int k;
            int x_off = (j%3)*third_x;
            int y_off = (j%3)*third_y;
            int row_off = (j/3) *(face.y_length/3);

            int begin_x = face.begin_x0 + x_off;
            int begin_y = face.begin_y0 - y_off + row_off;

            int end_x = face.begin_x0 + ((j%3) + 1)*(third_x);
            int end_y = face.begin_y0 - ((j%3)+1)*(third_y) + row_off;

            for (k = 1; k < (face.y_length/3) + 2; k++){
                tft_drawLine(begin_x ,begin_y + (k),end_x,end_y + (k),color_select(face,j));
            }
       }

    // top faces
    face = up;
    for (j = 0; j<9;j++){
        int third_x = face.x_length / 3;

        int third_y = face.y_length / 3;
        int col_off = (j%3);
        int row_off = (j/3) ;

        int begin_x = face.begin_x0 + (col_off*third_x) - (row_off * third_y);
        int begin_y = face.begin_y0 + (col_off*col_off) + (row_off*12);

        int end_x = begin_x + 20;
        int end_y = begin_y + 1 +(col_off%2)*2;
        for (k = 1; k < 10; k++){
            tft_drawLine(begin_x-k,begin_y+(k *1.3),end_x-k,end_y+(k *1.3),color_select(face,j));
        }
    }
    face = down;
    for (j = 0; j<9;j++){
        int third_x = face.x_length / 3;

        int third_y = face.y_length / 3;
        int col_off = (j%3);
        int row_off = (j/3) ;

        int begin_x = face.begin_x0 + (col_off*third_x) - (row_off * third_y);
        int begin_y = face.begin_y0 + (col_off*col_off) + (row_off*12);

        int end_x = begin_x + 20;
        int end_y = begin_y + 1 +(col_off%2)*2;
        for (k = 1; k < 10; k++){
            tft_drawLine(begin_x-k,begin_y+(k *1.3),end_x-k,end_y+(k *1.3),color_select(face,j));
        }
    }
}

void drawTFT() {
    // clear display
    tft_fillScreen(ILI9340_BLACK);
    tft_setTextColor(ILI9340_CYAN);

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
