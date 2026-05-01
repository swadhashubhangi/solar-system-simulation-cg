#include <graphics.h>
#include <math.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

#define MAX_SHOOT 6
#define AST_COUNT 140
#define KUIPER_COUNT 400

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    float angle = 0;
    float speed = 0.03;
    int paused = 0;

    float zoom = 1.0;
    int offsetX = 0, offsetY = 0;

    //stars
    int stars[100][2];
    for(int s=0; s<100; s++){
        stars[s][0] = rand()%getmaxx();
        stars[s][1] = rand()%getmaxy();
    }

    //asteroid belt
    int astX[AST_COUNT], astY[AST_COUNT];
    for(int a=0; a<AST_COUNT; a++){
        float ang = (rand()%360) * 3.14 / 180;
        int r = 150 + rand()%60;
        astX[a] = r * cos(ang);
        astY[a] = r * sin(ang);
    }

    //kuiper belt
    int kx[KUIPER_COUNT], ky[KUIPER_COUNT];
    for(int k=0; k<KUIPER_COUNT; k++){
        float ang = (rand()%360) * 3.14 / 180;
        int r = 302 + rand()%40;
        kx[k] = r * cos(ang);
        ky[k] = r * sin(ang);
    }

    //comets
    int shootX[MAX_SHOOT], shootY[MAX_SHOOT];
    float dirX[MAX_SHOOT], dirY[MAX_SHOOT];
    int shootSpeed[MAX_SHOOT], shootActive[MAX_SHOOT];

    for(int ss=0; ss<MAX_SHOOT; ss++)
        shootActive[ss] = 0;

    while(1){

        //input
        if(kbhit()){
            int ch = getch();

            if(ch==0 || ch==224){
                ch = getch();
                if(ch==75) speed -= 0.01;
                if(ch==77) speed += 0.01;
                if(ch==72) speed = 0.03;
                if(ch==80) paused = !paused;
            }

            else if(ch=='i' || ch=='I') zoom += 0.1;
            else if(ch=='o' || ch=='O') zoom -= 0.1;

            else if(ch=='w' || ch=='W') offsetY += 20;
            else if(ch=='s' || ch=='S') offsetY -= 20;
            else if(ch=='a' || ch=='A') offsetX += 20;
            else if(ch=='d' || ch=='D') offsetX -= 20;

            else if(ch==27) break;
        }

        if(zoom < 0.5) zoom = 0.5;
        if(zoom > 4.0) zoom = 4.0;

        if(speed > 0.2) speed = 0.2;
        if(speed < -0.2) speed = -0.2;

        cleardevice();

        //stars
        setcolor(WHITE);
        for(int st=0; st<100; st++)
            putpixel(stars[st][0], stars[st][1], WHITE);

        //center
        int cx = getmaxx()/2 + offsetX;
        int cy = getmaxy()/2 + offsetY;

        //asteroid belt
        setcolor(DARKGRAY);
        for(int a=0; a<AST_COUNT; a++){
            float rot = angle * 0.03;
            int px = cx + (astX[a]*cos(rot) - astY[a]*sin(rot)) * zoom;
            int py = cy + (astX[a]*sin(rot) + astY[a]*cos(rot)) * zoom;
            int size = zoom * 2;
            if(size < 1) size = 1;
            circle(px, py, size);
        }

        //kuiper belt
        setcolor(DARKGRAY);
        for(int k=0; k<KUIPER_COUNT; k++){
            float rot = angle * 0.02;
            int px = cx + (kx[k]*cos(rot) - ky[k]*sin(rot)) * zoom;
            int py = cy + (kx[k]*sin(rot) + ky[k]*cos(rot)) * zoom;
            int size = zoom * 2;
            if(size < 1) size = 1;
            circle(px, py, size);
        }

        //planet positions
        int mx = cx + (50 * zoom)*cos(angle);
        int my = cy + (50 * zoom)*sin(angle);

        int vx = cx + (80 * zoom)*cos(angle*0.8);
        int vy = cy + (80 * zoom)*sin(angle*0.8);

        int ex = cx + (110 * zoom)*cos(angle*0.6);
        int ey = cy + (110 * zoom)*sin(angle*0.6);

        int marsx = cx + (140 * zoom)*cos(angle*0.5);
        int marsy = cy + (140 * zoom)*sin(angle*0.5);

        int jx = cx + (180 * zoom)*cos(angle*0.4);
        int jy = cy + (180 * zoom)*sin(angle*0.4);

        int sx = cx + (220 * zoom)*cos(angle*0.3);
        int sy = cy + (220 * zoom)*sin(angle*0.3);

        int ux = cx + (260 * zoom)*cos(angle*0.2);
        int uy = cy + (260 * zoom)*sin(angle*0.2);

        int nx = cx + (290 * zoom)*cos(angle*0.1);
        int ny = cy + (290 * zoom)*sin(angle*0.1);

        //comets
        for(int ss=0; ss<MAX_SHOOT; ss++){

            if(!shootActive[ss] && rand()%1000 < 5){
                shootActive[ss] = 1;
                shootSpeed[ss] = 5 + rand()%5;

                if(rand()%2==0){
                    shootX[ss] = rand()%getmaxx();
                    shootY[ss] = 0;
                } else {
                    float ang = (rand()%360) * 3.14 / 180;
                    shootX[ss] = cx + 340*cos(ang);
                    shootY[ss] = cy + 340*sin(ang);
                }

                float dx = cx - shootX[ss];
                float dy = cy - shootY[ss];
                float mag = sqrt(dx*dx + dy*dy);
                dirX[ss] = dx/mag;
                dirY[ss] = dy/mag;
            }

            if(shootActive[ss]){

                float dx = shootX[ss]-cx;
                float dy = shootY[ss]-cy;
                if(sqrt(dx*dx+dy*dy) < 30*zoom){
                    shootActive[ss]=0;
                    continue;
                }

                int px[8]={mx,vx,ex,marsx,jx,sx,ux,nx};
                int py[8]={my,vy,ey,marsy,jy,sy,uy,ny};
                int pr[8]={5,7,7,6,12,11,10,10};

                for(int p=0;p<8;p++){
                    dx=shootX[ss]-px[p];
                    dy=shootY[ss]-py[p];
                    if(sqrt(dx*dx+dy*dy) < pr[p]*zoom){
                        shootActive[ss]=0;
                        break;
                    }
                }

                if(!shootActive[ss]) continue;

                setcolor(WHITE);
                line(shootX[ss], shootY[ss],
                     shootX[ss]-dirX[ss]*15,
                     shootY[ss]-dirY[ss]*15);

                line(shootX[ss]-dirX[ss]*15,
                     shootY[ss]-dirY[ss]*15,
                     shootX[ss]-dirX[ss]*25,
                     shootY[ss]-dirY[ss]*25);

                putpixel(shootX[ss], shootY[ss], WHITE);

                if(!paused){
                    shootX[ss] += dirX[ss] * shootSpeed[ss] * speed * 50;
                    shootY[ss] += dirY[ss] * shootSpeed[ss] * speed * 50;
                }

                if(shootX[ss] < 0 || shootX[ss] > getmaxx() ||
                   shootY[ss] < 0 || shootY[ss] > getmaxy()){
                    shootActive[ss] = 0;
                }
            }
        }

        //sun
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        circle(cx, cy, 30 * zoom);
        floodfill(cx, cy, YELLOW);

        //orbits
        setcolor(DARKGRAY);
        setlinestyle(DOTTED_LINE, 0, 1);
        for(int r=50; r<=300; r+=30)
            circle(cx, cy, r * zoom);
        setlinestyle(SOLID_LINE, 0, 1);

        //planets
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        circle(mx, my, 3 * zoom); floodfill(mx, my, WHITE);
        outtextxy(mx+5, my, "Mercury");

        setcolor(LIGHTGRAY);
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        circle(vx, vy, 5 * zoom); floodfill(vx, vy, LIGHTGRAY);
        outtextxy(vx+5, vy, "Venus");

        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        circle(ex, ey, 6 * zoom); floodfill(ex, ey, BLUE);
        outtextxy(ex+5, ey, "Earth");

        circle(ex + (15 * zoom)*cos(angle*2),
               ey + (15 * zoom)*sin(angle*2), 2);

        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        circle(marsx, marsy, 5 * zoom); floodfill(marsx, marsy, RED);
        outtextxy(marsx+5, marsy, "Mars");

        setcolor(BROWN);
        setfillstyle(SOLID_FILL, BROWN);
        circle(jx, jy, 10 * zoom); floodfill(jx, jy, BROWN);
        outtextxy(jx+5, jy, "Jupiter");

        setcolor(LIGHTCYAN);
        setfillstyle(SOLID_FILL, LIGHTCYAN);
        circle(sx, sy, 9 * zoom); floodfill(sx, sy, LIGHTCYAN);
        ellipse(sx, sy, 0, 360, 14 * zoom, 5 * zoom);
        outtextxy(sx+5, sy, "Saturn");

        setcolor(CYAN);
        setfillstyle(SOLID_FILL, CYAN);
        circle(ux, uy, 8 * zoom); floodfill(ux, uy, CYAN);
        outtextxy(ux+5, uy, "Uranus");

        setcolor(LIGHTBLUE);
        setfillstyle(SOLID_FILL, LIGHTBLUE);
        circle(nx, ny, 8 * zoom); floodfill(nx, ny, LIGHTBLUE);
        outtextxy(nx+5, ny, "Neptune");

        outtextxy(10,10,"Arrows: Speed | I/O: Zoom | WASD: Move");

        if(!paused) angle += speed;

        delay(40);
    }

    closegraph();
    return 0;
}
