#include "icb_gui.h"  

int FRM1;
ICBYTES panel;
bool thread_continue = false;
DWORD dw;

// Coordinates and speeds for raindrops
int bx = 50;
int by = 50;
int x[12] = { bx, bx + 50, bx + 100, bx + 150, bx + 200, bx + 250, bx + 300, bx + 350, bx + 400, bx + 450, bx + 500, bx + 550 };
int y[12] = { by, by + 50, by + 100, by, by + 50, by + 100, by, by + 50, by + 100, by, by + 50, by + 100 };
int speed[12];

// Variables for vehicle coordinates and speed
int left_x = 50;      // Vehicle moving from left to right 
int right_x = 500;    // Vehicle moving from right to left
int left_speed = 10;  // Speed of the red vehicle (faster)
int right_speed = 5;  // Speed of the blue vehicle (slower)

void static setRandomSpeeds() {
    for (int i = 0; i < 11; i++) {
        speed[i] = rand() % 20 + 15;
    }
}

void static DrawRaindrop(int x, int y) {
    HalfRect(panel, x, y, -4, -15, 0x00BFFF);
    HalfRect(panel, x, y, 4, -15, 0x00BFFF);
    FillCircle(panel, x, y, 3, 0x00BFFF);
}

void static DrawFormulaCar(int posX, int posY, int color, int kabin) {
    // Body
    FillRect(panel, posX + 5, posY - 5, 40, 10, color);  // Body color (red or blue)

    // Cabin (window)
    FillRect(panel, kabin, posY - 8, 10, 5, 0xFFFFFF);  // Cabin section (light blue)

    // Wheels
    FillCircle(panel, posX + 10, posY + 5, 6, 0x333333);  // Front wheel
    FillCircle(panel, posX + 35, posY + 5, 6, 0x333333);  // Rear wheel
}

void static DrawHorizontalObjects() {
    // Drawing the vehicle moving from left to right
    DrawFormulaCar(left_x, 300, 0xFF0000, left_x + 13);  // Red vehicle 

    // Drawing the vehicle moving from right to left
    DrawFormulaCar(right_x, 300, 0x0000FF, right_x + 25);  // Blue vehicle 
}

// Tree drawing function
void static DrawTree(int treeX, int treeY) {
    // Trunk (brown rectangle)
    FillRect(panel, treeX, treeY, 10, 30, 0x8B4513);  // Trunk color (brown)

    // Leaves (green circles)
    FillCircle(panel, treeX + 5, treeY - 10, 15, 0x228B22);  // Upper leaves (green)
    FillCircle(panel, treeX + 5, treeY - 25, 12, 0x228B22);  // Middle leaves (green)
    FillCircle(panel, treeX + 5, treeY - 35, 8, 0x228B22);   // Lower leaves (green)
}


void static Trees() {
    // Tree coordinates
    int treeCoords[25][2] = {
        {200, 500},   // 1st tree
        {50, 450},    // 2nd tree 
        {100, 520},   // 3rd tree 
        {150, 470},   // 4th tree
        {200, 510},   // 5th tree
        {250, 480},   // 6th tree
        {300, 530},   // 7th tree
        {350, 460},   // 8th tree
        {400, 500},   // 9th tree
        {450, 490},   // 10th tree 
        {500, 470},   // 11th tree 
        {550, 520},   // 12th tree 
        {550, 320},   // 13th tree 
        {200, 400},   // 14th tree 
        {50, 350},    // 15th tree 
        {100, 420},   // 16th tree 
        {150, 370},   // 17th tree 
        {200, 410},   // 18th tree 
        {250, 380},   // 19th tree 
        {300, 430},   // 20th tree 
        {350, 360},   // 21st tree 
        {400, 400},   // 22nd tree 
        {450, 390},   // 23rd tree 
        {500, 370},   // 24th tree 
        {550, 420},   // 25th tree   
    };

    // Drawing trees at fixed positions
    for (int i = 0; i < 25; i++) {
        DrawTree(treeCoords[i][0], treeCoords[i][1]);
    }
}

// Cloud drawing function
void static DrawCloud(int x, int y) {
    FillCircle(panel, x, y, 20, 0xD3D3D3); // Large gray circle
    FillCircle(panel, x - 15, y, 20, 0xD3D3D3); // Left small circle
    FillCircle(panel, x + 15, y, 20, 0xD3D3D3); // Right small circle
    FillCircle(panel, x - 7, y - 10, 20, 0xD3D3D3); // Upper left
    FillCircle(panel, x + 7, y - 10, 20, 0xD3D3D3); //  Upper right 
}

void static DrawClouds() {
    // Cloud positions, increased y-coordinates to start from above 
    DrawCloud(100, 50);
    DrawCloud(200, 40);
    DrawCloud(360, 60);
    DrawCloud(450, 30);
    DrawCloud(30, 20);
    DrawCloud(550, 70);
    DrawCloud(250, 80);
}

VOID* Crashed(PVOID lpParam) {
    int smokeY1 = 290; // Initial smoke positions 
    int smokeY2 = 285;
    int smokeY3 = 280;
    thread_continue = true;

    //bool a = true;
    while (thread_continue) {
        FillRect(panel, 0, 0, 600, 600, 0x55FFFF);  // Paint the screen white
        FillRect(panel, 0, 310, 600, 295, 0x55FF55); // Grass
        FillRect(panel, 0, 305, 600, 15, 0x808080); //  Road

        Trees();

        // Drawing raindrops
        for (int i = 0; i < 11; i++) {
            DrawRaindrop(x[i], y[i]);
            y[i] += speed[i];
            if (y[i] > 600) y[i] = 0;
        }
        DrawClouds();
        // Scattered state of the red vehicle
        FillRect(panel, left_x, 300, 30, 8, 0xFF0000);     // Body part 
        FillRect(panel, left_x + 10, 290, 10, 5, 0x00FFFF); // Window part 
        FillCircle(panel, left_x + 5, 310, 6, 0x333333);    // Left wheel
        FillCircle(panel, left_x + 20, 315, 6, 0x333333);   // Right wheel

        // Scattered state of the blue vehicle
        FillRect(panel, right_x, 300, 30, 8, 0x0000FF);    // Body part 
        FillRect(panel, right_x + 10, 290, 10, 5, 0x00FFFF); // Window part 
        FillCircle(panel, right_x + 5, 310, 6, 0x333333);   // Left wheel
        FillCircle(panel, right_x + 20, 315, 6, 0x333333);  // Right wheel

        // Smoke effects (gray circles), move upwards
        FillCircle(panel, (left_x + right_x) / 2 - 10, smokeY1, 8, 0x808080);
        FillCircle(panel, (left_x + right_x) / 2 + 10, smokeY2, 10, 0xA9A9A9);
        FillCircle(panel, (left_x + right_x) / 2, smokeY3, 12, 0xB0B0B0);

        // Enables upward movement of smoke
        smokeY1 -= 1; // Smoke rises slowly
        smokeY2 -= 1;
        smokeY3 -= 1;

        // If smoke goes off-screen, reposition it
        if (smokeY1 < 0) smokeY1 = 290;
        if (smokeY2 < 0) smokeY2 = 285;
        if (smokeY3 < 0) smokeY3 = 280;

        // Fire effects (yellow and red flames)
        for (int i = 0; i < 10; i++) {
            int fireX = (left_x + right_x) / 2 + (rand() % 20 - 10); // Random x position
            int fireY = 300 - (rand() % 20);                         // Random y position

            // Create flames with yellow and red circles
            FillCircle(panel, fireX, fireY, rand() % 5 + 3, 0xFFA500);  // Yellow flame
            FillCircle(panel, fireX + 5, fireY - 5, rand() % 4 + 2, 0xFF4500);  // Red flame
        }


        // Reflect the image on the screen
        DisplayImage(FRM1, panel);
        Sleep(30);
    }
    return NULL;
}

VOID* Animate(PVOID lpParam) {


    while (thread_continue) {
        FillRect(panel, 0, 0, 600, 600, 0x55FFFF);
        FillRect(panel, 0, 310, 600, 295, 0x55FF55);
        FillRect(panel, 0, 305, 600, 15, 0x808080);

        // Draw raindrops
        for (int i = 0; i < 11; i++) {
            DrawRaindrop(x[i], y[i]);
            y[i] += speed[i];
            if (y[i] > 600) y[i] = 0;
        }
        DrawClouds();


        DrawHorizontalObjects();


        //Trees
        Trees();
        // Vehicle movement
        left_x += left_speed;
        right_x -= right_speed;

        // Collision detection
        if (left_x + 40 >= right_x) {  // Collision detection
