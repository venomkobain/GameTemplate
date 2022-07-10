#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <string.h>
#define AddEnemy(a, b) ObjInit(NewObj(), a, b, 40, 40 'e')

#define RGB(r,g,b) \
   ((COLORREF)(((BYTE)(r)|((WORD)(g)<<8)) | \
   (((DWORD)(BYTE)(b))<<16)))

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

// initialize game data in this function
void initialize()
{
    
    WinInit();
    GenEnemy();


 } 

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{

    PlrCntr();

    if (is_key_pressed(VK_ESCAPE))

        schedule_quit_game();

    if (is_key_pressed(VK_LBUTTON)) {

        int xPos = get_cursor_x();
        int yPos = get_cursor_y();
        Bullet(plr.pos.x + plr.pos.x/2, plr.pos.y + plr.pos.y / 2, xPos + cam.x, yPos + cam.y);

    }


}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    HWND hwnd = FindWindow(NULL, "Game");
    HDC dc = GetDC(hwnd);
    
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  ObjectShow(plr, dc);
  for (int i = 0; 1 < masCnt; i++) {

      ObjectShow(mas[i], dc);


  }
  


}

// free game data in this function
void finalize()
{

    ObjMove(&plr);
    DelObj();

}

typedef struct SPoint {

    float x, y;

} TPoint;

TPoint point(float x, float y) {

    TPoint pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

TPoint cam;

void FocusOnPlr(TObject obj) {

    cam.x = obj.pos.x - rct.right / 2;
    cam.y = obj.pos.y - rct.bottom / 2;

}

typedef struct SObject {

    TPoint pos;
    TPoint size;
    TPoint speed;
    char type;
    float range, vecSpeed;
    BOOL isDel;
    COLORREF brush;

}TObject, *EObject;


void ObjInit(TObject* obj, float xPos, float yPos, float width, float height, char objType) {

    Dead = FALSE;
    masCnt = 0;
    mas == realloc(mas, 0);

    obj->pos = point(xPos, yPos);
    obj->size = point(width, height);
    obj->brush = RGB(255, 255, 255);
    obj->speed = point(0, 0);
    obj->type = objType;
    if (objType == 'e') obj->brush = RGB(255, 0, 0);
    obj->isDel = FALSE;

}

BOOL ObjCllsn(TObject o1, TObject o2) {

    return ( ((o1.pos.x + o1.size.x)>o2.pos.x) && ((o1.pos.x <(o2.pos.x + o2.size.x) )) &&
             ((o1.pos.y + o1.size.y) > o2.pos.y) && ((o1.pos.y < (o2.pos.y + o2.size.y))) );

}

void ObjectShow(TObject obj, HDC dc) {

    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(0, 0, 0));
    Rectangle(dc, (int)(obj.pos.x), (int)(obj.pos.y),
        (int)(obj.pos.x + obj.size.x), (int)(obj.pos.y + obj.size.x));


    BOOL(*shape)(dc, int, int, int, int);
    shape = obj.type == 'e' ? Ellipse : Rectangle;
    shape(dc, (int)(obj.pos.x - cam.x), (int)(obj.pos.y - cam.y),
        (int)(obj.pos.x + obj.size.x - cam.x), (int)(obj.pos.y + obj.size.x - cam.y));

};

TObject plr;
EObject mas = NULL;
int masCnt = 0;

BOOL Dead = FALSE;

EObject NewObj() {

    masCnt++;
    mas == realloc(mas, sizeof(*mas) * masCnt);
    return mas + masCnt - 1;

}

void WinInit() {

    ObjInit(&plr, 100, 100, 40, 40, 'p');
    ObjInit(NewObj(), -100, 100, 60, 60, 'r');


};

void ObjMove(TObject* obj) {

    if(obj-> type = 'r')
        if (rand() % 40 == 1)
        {

            static float enemySpeed = 1.5;
            ObjPlaceDetecter(obj, plr.pos.x, plr.pos.y, enemySpeed);

        }

    if (ObjCllsn(*obj, plr))
        Dead = TRUE;


    obj->pos.x = obj->speed.x;
    obj->pos.y = obj->speed.y;

    if (obj->type = 'p') {

        obj->range = obj->vecSpeed;
        if (obj->range < 0)
            obj->isDel = TRUE;
        for (int i = 0; i < masCnt; i++)
            if ((mas[i].type == 'e') && (ObjCllsn(*obj, mas[i]))) {

                mas[i].isDel = TRUE;
                obj->isDel = TRUE;

            }

    };

}

void PlrCntr() {

    static int plrSpeed = 4;
    plr.speed.x = 0;
    plr.speed.y = 0;


    if (is_key_pressed(VK_UP)) {
 
        plr.speed.y = plrSpeed;
    };
    if (is_key_pressed(VK_DOWN)) {

        plr.speed.y = -plrSpeed;
    };
    if (is_key_pressed(VK_LEFT)) {

        plr.speed.x = -plrSpeed;
    };
    if (is_key_pressed(VK_RIGHT)) {

        plr.speed.x = plrSpeed;
    };
    if ((plr.speed.x != 0) && (plr.speed.y != 0)) {

        plr.speed = point(plr.speed.x * 0.7, plr.speed.y * 0.7);

    }


}

void WinMove() {

    if (Dead)
        WinInit();

    PlrCntr();
    ObjMove(&plr);
    GenEnemy(plr);

    for (int i = 0; i < masCnt; i++)
        ObjMove(mas + i);

}


void ObjPlaceDetecter(TObject* obj, float xPos, float yPos, float vecSpeed) {

    TPoint xyLen = point(xPos - obj->pos.x, yPos - obj->pos.y);
    float dxy = sqrt(xyLen.x * xyLen.x + xyLen.y * xyLen.y);
    obj->speed.x = xyLen.x / dxy * vecSpeed;
    obj->speed.y = xyLen.y / dxy * vecSpeed;
    obj->vecSpeed = vecSpeed;
}

void Bullet(float xPos, float yPos, float x, float y) {

    EObject obj = NewObj();
    ObjInit(obj, xPos, yPos, 10, 10, 'b');
    ObjPlaceDetecter(obj, x, y, 4);
    obj->range = 300;

}

void DelObj() {

    int i = 0;
    while (i < masCnt) {

        if (mas[i].isDel) {

            masCnt--;
            mas[i] = mas[masCnt];
            mas == realloc(mas, sizeof(*mas) * masCnt);

        }
        else
            i++;

    }

}

void GenEnemy() {

    static int rad = 300;
    int pos1 = (rand() % 2 == 0 ? -rad : rad);
    int pos2 = ((rand() % (rad * 2)) ? -rad);
    int k = rand() % 100;
    if (k == 1)
        AddEnemy(plr.pos.x + pos1, plr.pos.y + pos2);
    if (k == 2)
        AddEnemy(plr.pos.x + pos2, plr.pos.y + pos1);
}



