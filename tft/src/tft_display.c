#include "tft_display.h"

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

static struct Face right=   { .begin_x0 = 110, .begin_y0 = 105, .begin_x1 = 140, .begin_y1 = 67,
                            .end_x0 = 110, .end_y0 = 165, .end_x1 = 140, .end_y1 = 127,
                            .x_mult = 0, .y_mult = 1, .x_length =30, .y_length = 60 , .color0 = ILI9340_YELLOW,
                            .color1 = ILI9340_YELLOW, .color2 = ILI9340_YELLOW, .color4 = ILI9340_YELLOW,
                            .color3 = ILI9340_YELLOW, .color5 = ILI9340_YELLOW, .color6 = ILI9340_YELLOW,
                            .color7 = ILI9340_YELLOW, .color8 = ILI9340_YELLOW};
static struct Face back =   { .begin_x0 = 260, .begin_y0 = 105, .begin_x1 = 290, .begin_y1 = 67,
                            .end_x0 = 110, .end_y0 = 165, .end_x1 = 140, .end_y1 = 127,
                            .x_mult = 0, .y_mult = 1, .x_length =30, .y_length = 60 , .color0 = ILI9340_BLUE,
                            .color1 = ILI9340_BLUE, .color2 = ILI9340_BLUE, .color4 = ILI9340_BLUE,
                            .color3 = ILI9340_BLUE, .color5 = ILI9340_BLUE, .color6 = ILI9340_BLUE,
                            .color7 = ILI9340_BLUE, .color8 = ILI9340_BLUE};
static struct Face front =  { .begin_x0 = 50, .begin_y0 = 100, .begin_x1 = 110, .begin_y1 = 105,
                            .end_x0 = 50, .end_y0 = 160, .end_x1 = 110, .end_y1 = 165,
                            .x_mult = 0, .y_mult = 1, .x_length =60, .y_length = 60 , .color0 = ILI9340_GREEN,
                            .color1 = ILI9340_GREEN, .color2 = ILI9340_GREEN, .color4 = ILI9340_GREEN,
                            .color3 = ILI9340_GREEN, .color5 = ILI9340_GREEN, .color6 = ILI9340_GREEN,
                            .color7 = ILI9340_GREEN, .color8 = ILI9340_GREEN};
static struct Face left =   { .begin_x0 = 200, .begin_y0 = 100, .begin_x1 = 260, .begin_y1 = 105,
                            .end_x0 = 200, .end_y0 = 160, .end_x1 = 260, .end_y1 = 165,
                            .x_mult = 0, .y_mult = 1, .x_length =60, .y_length = 60 , .color0 = ILI9340_WHITE,
                            .color1 = ILI9340_WHITE, .color2 = ILI9340_WHITE, .color4 = ILI9340_WHITE,
                            .color3 = ILI9340_WHITE, .color5 = ILI9340_WHITE, .color6 = ILI9340_WHITE,
                            .color7 = ILI9340_WHITE, .color8 = ILI9340_WHITE};
static struct Face up =     {.begin_x0 = 80,.begin_y0 = 62, .begin_x1 = 140, .begin_y1 = 67,
                            .end_x0 = 50, .end_y0 = 100, .end_x1 = 110, .end_y1 = 105,
                            .x_mult = -1, .y_mult = 1, .x_length =60, .y_length = 30,
                            .color0 = ILI9340_RED,  .color1 = ILI9340_RED, .color2 = ILI9340_RED,
                            .color3 = ILI9340_RED,    .color4 = ILI9340_RED, .color5 = ILI9340_RED,
                            .color6 = ILI9340_RED,     .color7 = ILI9340_RED, .color8 = ILI9340_RED};
static struct Face down =   {.begin_x0 = 230,.begin_y0 = 62, .begin_x1 = 290, .begin_y1 = 67,
                            .end_x0 = 200, .end_y0 = 100, .end_x1 = 260, .end_y1 = 105,
                            .x_mult = -1, .y_mult = 1, .x_length =60, .y_length = 30,
                            .color0 = ILI9340_ORANGE,  .color1 = ILI9340_ORANGE, .color2 = ILI9340_ORANGE,
                            .color3 = ILI9340_ORANGE,    .color4 = ILI9340_ORANGE, .color5 = ILI9340_ORANGE,
                            .color6 = ILI9340_ORANGE,     .color7 = ILI9340_ORANGE, .color8 = ILI9340_ORANGE};

static int move, move_index, move_num; 

static char tft_buffer[60];

static unsigned short tftColor(FaceMask fm) {
    if (fm & MV_MASK_U) {
        return ILI9340_RED;
    }
    else if (fm & MV_MASK_D) {
        return ILI9340_ORANGE;
    }
    else if (fm & MV_MASK_F) {
        return ILI9340_GREEN;
    }
    else if (fm & MV_MASK_B) {
        return ILI9340_BLUE;
    }
    else if (fm & MV_MASK_R) {
        return ILI9340_YELLOW;
    }
    else if (fm & MV_MASK_L) {
        return ILI9340_WHITE;
    }
}

void updateTFT(Cube c, Move mv, int mv_i, int mv_n) {
    // update move state
    move = mv;
    move_index = mv_i;
    move_num = mv_n;

    // update cube state
    ColorCube cc = convertCube(c);
    front.color0 = tftColor(cc.F.UL);
    front.color1 = tftColor(cc.F.U);
    front.color2 = tftColor(cc.F.UR);
    front.color3 = tftColor(cc.F.L);
    front.color4 = tftColor(cc.F.C);
    front.color5 = tftColor(cc.F.R);
    front.color6 = tftColor(cc.F.DL);
    front.color7 = tftColor(cc.F.D);
    front.color8 = tftColor(cc.F.DR);
    right.color0 = tftColor(cc.R.UL);
    right.color1 = tftColor(cc.R.U);
    right.color2 = tftColor(cc.R.UR);
    right.color3 = tftColor(cc.R.L);
    right.color4 = tftColor(cc.R.C);
    right.color5 = tftColor(cc.R.R);
    right.color6 = tftColor(cc.R.DL);
    right.color7 = tftColor(cc.R.D);
    right.color8 = tftColor(cc.R.DR);
    up.color0 = tftColor(cc.U.UL);
    up.color1 = tftColor(cc.U.U);
    up.color2 = tftColor(cc.U.UR);
    up.color3 = tftColor(cc.U.L);
    up.color4 = tftColor(cc.U.C);
    up.color5 = tftColor(cc.U.R);
    up.color6 = tftColor(cc.U.DL);
    up.color7 = tftColor(cc.U.D);
    up.color8 = tftColor(cc.U.DR);
    back.color0 = tftColor(cc.B.DR);
    back.color1 = tftColor(cc.B.D);
    back.color2 = tftColor(cc.B.DL);
    back.color3 = tftColor(cc.B.R);
    back.color4 = tftColor(cc.B.C);
    back.color5 = tftColor(cc.B.L);
    back.color6 = tftColor(cc.B.UR);
    back.color7 = tftColor(cc.B.U);
    back.color8 = tftColor(cc.B.UL);
    left.color0 = tftColor(cc.L.DR);
    left.color1 = tftColor(cc.L.D);
    left.color2 = tftColor(cc.L.DL);
    left.color3 = tftColor(cc.L.R);
    left.color4 = tftColor(cc.L.C);
    left.color5 = tftColor(cc.L.L);
    left.color6 = tftColor(cc.L.UR);
    left.color7 = tftColor(cc.L.U);
    left.color8 = tftColor(cc.L.UL);
    down.color0 = tftColor(cc.D.UR);
    down.color1 = tftColor(cc.D.R);
    down.color2 = tftColor(cc.D.DR);
    down.color3 = tftColor(cc.D.U);
    down.color4 = tftColor(cc.D.C);
    down.color5 = tftColor(cc.D.D);
    down.color6 = tftColor(cc.D.UL);
    down.color7 = tftColor(cc.D.L);
    down.color8 = tftColor(cc.D.DL);
}

static unsigned int color_select(struct Face this_face, int i){
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
    }
}

static void drawLines() {
    // front face
    tft_drawLine(50,100,110,105,ILI9340_WHITE);
    tft_drawLine(110,105,110,165,ILI9340_WHITE);
    tft_drawLine(50,100,50,160,ILI9340_WHITE);
    tft_drawLine(110,165,50,160,ILI9340_WHITE);

    tft_drawLine(70,101,70,164,ILI9340_WHITE);      // vertical
    tft_drawLine(90,101,90,164,ILI9340_WHITE);      // vertical
    tft_drawLine(50,120,110,125,ILI9340_WHITE);     // horizontal
    tft_drawLine(50,140,110,145,ILI9340_WHITE);     // horizontal

    //additional lines for top face
    tft_drawLine(50,100,80,62,ILI9340_WHITE);
    tft_drawLine(110,105,140,67,ILI9340_WHITE);
    tft_drawLine(140,67,80,62,ILI9340_WHITE);

    tft_drawLine(70,74,130,79,ILI9340_WHITE);       // horizontal
    tft_drawLine(60,88,120,93,ILI9340_WHITE);       // horizontal
    tft_drawLine(100,63,70,101,ILI9340_WHITE);      // vertical
    tft_drawLine(120,66,90,101,ILI9340_WHITE);      // vertical

    //additional lines for right face
    tft_drawLine(140,67,140,127,ILI9340_WHITE);
    tft_drawLine(140,127,110,165,ILI9340_WHITE);

    tft_drawLine(110,125,140,87,ILI9340_WHITE);     // horizontal
    tft_drawLine(110,145,140,107,ILI9340_WHITE);    // horizontal
    tft_drawLine(120,93,120,153,ILI9340_WHITE);     // vertical
    tft_drawLine(130,79,130,139,ILI9340_WHITE);     // vertical

    // left face
    tft_drawLine(200,100,260,105,ILI9340_WHITE);
    tft_drawLine(260,105,260,165,ILI9340_WHITE);
    tft_drawLine(200,100,200,160,ILI9340_WHITE);
    tft_drawLine(260,165,200,160,ILI9340_WHITE);

    tft_drawLine(220,101,220,164,ILI9340_WHITE);    // vertical
    tft_drawLine(240,101,240,164,ILI9340_WHITE);    // vertical
    tft_drawLine(200,120,260,125,ILI9340_WHITE);    // horizontal
    tft_drawLine(200,140,260,145,ILI9340_WHITE);    // horizontal

    //additional lines for down face
    tft_drawLine(200,100,230,62,ILI9340_WHITE);
    tft_drawLine(260,105,290,67,ILI9340_WHITE);
    tft_drawLine(290,67,230,62,ILI9340_WHITE);

    tft_drawLine(220,74,280,79,ILI9340_WHITE);      // horizontal
    tft_drawLine(210,88,270,93,ILI9340_WHITE);      // horizontal
    tft_drawLine(250,63,220,101,ILI9340_WHITE);     // vertical
    tft_drawLine(270,66,240,101,ILI9340_WHITE);     // vertical

    //additional lines for back face
    tft_drawLine(290,67,290,127,ILI9340_WHITE);
    tft_drawLine(290,127,260,165,ILI9340_WHITE);

    tft_drawLine(260,125,290,87,ILI9340_WHITE);     // horizontal
    tft_drawLine(260,145,290,107,ILI9340_WHITE);    // horizontal
    tft_drawLine(270,93,270,153,ILI9340_WHITE);     // vertical
    tft_drawLine(280,79,280,139,ILI9340_WHITE);     // vertical

    // side labels
    tft_drawChar(75, 175,'F', ILI9340_CYAN, ILI9340_BLACK, 2);
    tft_drawChar(100, 45,'U', ILI9340_CYAN, ILI9340_BLACK, 2);
    tft_drawChar(150, 100,'R', ILI9340_CYAN, ILI9340_BLACK, 2);
    tft_drawChar(225, 175,'L', ILI9340_CYAN, ILI9340_BLACK, 2);
    tft_drawChar(250, 45,'D', ILI9340_CYAN, ILI9340_BLACK, 2);
    tft_drawChar(300, 100,'B', ILI9340_CYAN, ILI9340_BLACK, 2);
}

static void drawColors() {
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

static void drawMove() {
    tft_setTextSize(4);
    tft_setCursor(100,100);
    
    if (mv & MV_MASK_U) {
        sprintf(tft_buffer, "U");
    }
    else if (mv & MV_MASK_D) {
        sprintf(tft_buffer, "D");
    }
    else if (mv & MV_MASK_F) {
        sprintf(tft_buffer, "F");
    }
    else if (mv & MV_MASK_B) {
        sprintf(tft_buffer, "B");
    }
    else if (mv & MV_MASK_R) {
        sprintf(tft_buffer, "R");
    }
    else if (mv & MV_MASK_L) {
        sprintf(tft_buffer, "L");
    }
    
    if (mv & MV_MASK_I) {
        sprintf(tft_buffer+1, "'");
    }
    else if (mv & MV_MASK_H) {
        sprintf(tft_buffer+1, "2");
    }
    
    tft_writeString(tft_buffer);
}

void drawTFT(Screen sc) {
    // clear display
    tft_fillScreen(ILI9340_BLACK);
    tft_setTextColor(ILI9340_CYAN);

    if (sc == TITLE_SCREEN) {
      tft_setCursor(100,100);
      tft_setTextSize(3);
      char title[] = "RUBOT";
      tft_writeString(title);
    }
    else if (sc == SOLVING_SCREEN) {
        tft_setCursor(100,100);
        char solving[] = "Solving...";
        tft_writeString(solving);
    }
    else if (sc == CUBE_SCREEN) {
        tft_setCursor(0,0);
        sprintf(tft_buffer,"Step: %d of %d", move_index+1,move_num);
        tft_writeString(tft_buffer);

        drawLines();
        drawColors();
        drawMove();
    }
}