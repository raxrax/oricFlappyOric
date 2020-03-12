#include <lib.h>
#include "image.h"

#define SCREEN_START_ADDRES (0xbb80 + 40)
#define MAX_Y 27
#define SCREEN_LEN (MAX_Y * 40)
#define COLOR 22
#define INK 5
#define PLAYER_X 7
#define GRAVITY 17
#define FORCE 42
#define TITLE_TEXT0 (0xbb80 + 4 + 6 * 40)
#define TITLE_TEXT1 (0xbb80 + 8 + 10 * 40)
#define TITLE_TEXT2 (0xbb80 + 12 + 12 * 40)
#define TITLE_TEXT3 (0xbb80 + 12 + 13 * 40)
#define TITLE_TEXT4 (0xbb80 + 12 + 14 * 40)
#define TITLE_TEXT5 (0xbb80 + 1 + 18 * 40)
#define TITLE_TEXT7 (0xbb80 + 15 + 25 * 40)
#define TITLE_TEXT8 (0xbb80 + 1 + 26 * 40)


#define SCORE_ADDR (0xbb80)
#define DIFFICULT_TEXT (0xbb80 + 31)

#define STATE_RUNNING 1
#define STATE_DIE 2
#define STATE_END 3

#define CHAR_BIRD1 'a'
#define CHAR_BIRD2 'b'
#define CHAR_BIRD3 'c'
#define CHAR_BIRD4 'd'
#define CHAR_BIRD5 'e'
#define CHAR_BIRD6 'f'

void scroll(void);
void player(void);
void title(void);
void wait(unsigned int time);

extern unsigned char image[];

unsigned char font[12 * 8] = {0, 3, 4, 30, 33, 32, 32, 17, 63, 4, 8, 8, 8, 36, 35, 4, 0, 32, 16, 40, 40, 8, 60, 2, 14, 8, 6, 1, 0, 0, 0, 0, 11, 4, 3, 60, 0, 0, 0, 0, 60, 4, 56, 0, 0, 0, 0, 0, 58, 52, 58, 52, 58, 52, 58, 52, 34, 8, 34, 8, 34, 8, 34, 8, 11, 39, 11, 39, 11, 39, 11, 39, 63, 52, 58, 52, 58, 52, 58, 63, 63, 8, 34, 8, 34, 8, 34, 63, 63, 39, 11, 39, 11, 39, 11, 63};
static int multi40[27] = {0, 40, 80, 120, 160, 200, 240, 280, 320, 360, 400, 440, 480, 520, 560, 600, 640, 680, 720, 760, 800, 840, 880, 920, 960, 1000, 1040};

unsigned char pipe[5][28] = {
    "                           ",
    "ggggggggggggggggggggggggggg",
    "hhhhhhhhhhhhhhhhhhhhhhhhhhh",
    "iiiiiiiiiiiiiiiiiiiiiiiiiii",
    "                           ",
};

unsigned char tempPipe[5][28] = {0};

unsigned char screenBuffer[SCREEN_LEN] = {0};

unsigned char pipeInverval, keyHitCounter, hitCounterLen, state, key1, sleep;
unsigned int playerOldAddress, dy, playerY, score;

static unsigned char charset[] = {

    12, 12, 12, 12, 12, 0, 12, 0,
    54, 54, 54, 0, 0, 0, 0, 0,
    20, 20, 62, 20, 62, 20, 20, 0,
    8, 30, 44, 28, 26, 60, 8, 0,
    48, 50, 6, 12, 24, 54, 38, 0,
    24, 44, 44, 24, 46, 44, 30, 0,
    24, 24, 24, 0, 0, 0, 0, 0,
    12, 24, 48, 48, 48, 24, 12, 0,
    24, 12, 6, 6, 6, 12, 24, 0,
    8, 42, 28, 8, 28, 42, 8, 0,
    0, 24, 24, 62, 24, 24, 0, 0,
    0, 0, 0, 0, 0, 12, 12, 24,
    0, 0, 0, 62, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 4, 0,
    0, 2, 6, 12, 24, 48, 32, 0,
    28, 50, 54, 58, 50, 50, 28, 0,
    12, 28, 12, 12, 12, 12, 30, 0,
    28, 54, 6, 12, 24, 48, 62, 0,
    62, 6, 12, 28, 6, 54, 28, 0,
    4, 12, 28, 44, 62, 12, 12, 0,
    62, 48, 60, 6, 6, 38, 28, 0,
    12, 24, 48, 60, 50, 50, 28, 0,
    62, 2, 6, 12, 24, 24, 24, 0,
    28, 50, 50, 28, 50, 50, 28, 0,
    28, 54, 54, 30, 6, 12, 24, 0,
    0, 0, 12, 0, 0, 12, 0, 0,
    0, 0, 12, 0, 0, 12, 12, 24,
    6, 12, 24, 48, 24, 12, 6, 0,
    0, 0, 62, 0, 62, 0, 0, 0,
    48, 24, 12, 6, 12, 24, 48, 0,
    28, 54, 6, 12, 12, 0, 12, 0,
    28, 50, 50, 54, 54, 48, 30, 0,
    8, 28, 50, 50, 62, 50, 50, 0,
    60, 50, 50, 60, 50, 50, 60, 0,
    28, 50, 48, 48, 48, 50, 28, 0,
    60, 50, 50, 50, 50, 50, 60, 0,
    62, 48, 48, 60, 48, 48, 62, 0,
    62, 48, 48, 60, 48, 48, 48, 0,
    30, 48, 48, 48, 54, 50, 30, 0,
    50, 50, 50, 62, 50, 50, 50, 0,
    30, 12, 12, 12, 12, 12, 30, 0,
    6, 6, 6, 6, 6, 38, 28, 0,
    50, 52, 56, 48, 56, 60, 54, 0,
    48, 48, 48, 48, 48, 48, 62, 0,
    34, 54, 58, 58, 50, 50, 50, 0,
    50, 50, 50, 58, 54, 50, 50, 0,
    28, 50, 50, 50, 50, 50, 28, 0,
    60, 34, 34, 60, 48, 48, 48, 0,
    28, 50, 50, 50, 58, 52, 26, 0,
    60, 50, 50, 60, 56, 52, 50, 0,
    28, 50, 48, 28, 6, 38, 28, 0,
    62, 24, 24, 24, 24, 24, 24, 0,
    50, 50, 50, 50, 50, 50, 28, 0,
    50, 50, 50, 50, 50, 28, 8, 0,
    50, 50, 50, 58, 58, 54, 34, 0,
    54, 54, 20, 8, 20, 54, 54, 0,
    50, 50, 20, 24, 24, 24, 24, 0,
    62, 6, 12, 24, 48, 48, 62, 0,
    60, 48, 48, 48, 48, 48, 60, 0,
    0, 32, 48, 24, 12, 6, 2, 0,
    30, 6, 6, 6, 6, 6, 30, 0,
    8, 28, 54, 34, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 63, 0,
    30, 33, 45, 41, 41, 45, 33, 30,
    0, 0, 28, 2, 30, 38, 30, 0,
    32, 32, 60, 50, 50, 50, 60, 0,
    0, 0, 30, 48, 48, 48, 30, 0,
    2, 2, 30, 38, 38, 38, 30, 0,
    0, 0, 28, 50, 62, 48, 30, 0,
    12, 18, 16, 60, 24, 24, 24, 0,
    0, 0, 28, 38, 38, 30, 6, 28,
    32, 32, 60, 50, 50, 50, 50, 0,
    8, 0, 56, 24, 24, 24, 60, 0,
    4, 0, 28, 12, 12, 12, 44, 24,
    48, 48, 50, 52, 56, 52, 50, 0,
    56, 24, 24, 24, 24, 24, 60, 0,
    0, 0, 54, 58, 58, 58, 50, 0,
    0, 0, 60, 50, 50, 50, 50, 0,
    0, 0, 28, 50, 50, 50, 28, 0,
    0, 0, 60, 50, 50, 60, 48, 48,
    0, 0, 30, 38, 38, 30, 6, 6,
    0, 0, 46, 48, 48, 48, 48, 0,
    0, 0, 30, 48, 28, 6, 60, 0,
    24, 24, 60, 24, 24, 26, 12, 0,
    0, 0, 50, 50, 50, 54, 26, 0,
    0, 0, 50, 50, 50, 28, 8, 0,
    0, 0, 50, 50, 58, 58, 54, 0,
    0, 0, 54, 20, 8, 20, 54, 0,
    0, 0, 54, 54, 54, 30, 6, 28,
    0, 0, 62, 6, 12, 24, 62, 0,
    12, 24, 24, 48, 24, 24, 12, 0,
    12, 12, 12, 12, 12, 12, 12, 12,
    24, 12, 12, 6, 12, 12, 24, 0,
    42, 21, 42, 21, 42, 21, 42, 21,
    63, 63, 63, 63, 63, 63, 63, 63};

void main()
{
   text();
   paper(0);
   ink(7);
   poke(0x24e, 255);
   poke(0x24f, 255);

   // image
   hires();
   poke(0x26a, 10);
   memcpy((void *)0xa000, image, 8000);
   key();
   get();

   text();
   poke(0x26a, 10);

   while (state != STATE_END)
   {
      title();

      pipeInverval = 45;
      playerY = 700;
      dy = 0;
      keyHitCounter = 0;
      score = -1;
      state = STATE_RUNNING;

      while (state == STATE_RUNNING)
      {
         scroll();
         player();
         memcpy((void *)SCREEN_START_ADDRES, (void *)screenBuffer, SCREEN_LEN);
         wait(sleep);
      }
      key();
      get();
   }
}

void title()
{
   unsigned int addr;

   cls();
   paper(6);
   ink(4);

   //font
   memcpy((void *)0xb508, charset, sizeof(charset));

   //text
   sprintf(TITLE_TEXT0, "\xa\x1* * *  Flappy Oric :)  * * *");
   sprintf(TITLE_TEXT0 + 40, "\xa\x1* * *  Flappy Oric :)  * * *");
   sprintf(TITLE_TEXT1, "\x4 Choose your destiny:");
   sprintf(TITLE_TEXT2, "\x4 1.. Easy");
   sprintf(TITLE_TEXT3, "\x4 2.. Normal");
   sprintf(TITLE_TEXT4, "\x4 3.. Hard");

   sprintf(TITLE_TEXT5, "\04Discussion in forum.defence-force.org");

   sprintf(TITLE_TEXT7, "\05 implementation: raxiss");
   sprintf(TITLE_TEXT8, "\05 based on idea and sprites of: peacer");
   key();

   //choice
   while ((key1 = get()) != '1' && key1 != '2' && key1 != '3')
      ;

   if (key1 == '1')
   {
      hitCounterLen = 2;
      sleep = 6;
      sprintf((void *)DIFFICULT_TEXT, "     EASY");
   }
   else if (key1 == '3')
   {
      hitCounterLen = 4;
      sleep = 0;
      sprintf((void *)DIFFICULT_TEXT, "     HARD");
   }
   else
   {
      hitCounterLen = 3;
      sleep = 0;
      sprintf((void *)DIFFICULT_TEXT, "   NORMAL");
   }

   cls();

   // set sprites
   memcpy((void *)(0xb400 + 'a' * 8), (void *)font, sizeof(font));

   //clear screen buffer
   memset((void *)screenBuffer, COLOR, SCREEN_LEN);

   // invert pipe chars
   for (addr = 0xb400 + 'g' * 8; addr < 0xb400 + 'g' * 8 + 6 * 8; ++addr)
   {
      poke(addr, ~peek(addr));
   }
}

void player()
{
   static unsigned int addr;

   // delete old bird
   screenBuffer[playerOldAddress++] = COLOR;
   screenBuffer[playerOldAddress++] = COLOR;
   screenBuffer[playerOldAddress] = COLOR;
   playerOldAddress += 38;
   screenBuffer[playerOldAddress++] = COLOR;
   screenBuffer[playerOldAddress++] = COLOR;
   screenBuffer[playerOldAddress] = COLOR;

   dy += GRAVITY;

   // read key
   switch (key())
   {
   case 0:
      break;

   default:
      keyHitCounter = hitCounterLen;
      dy = dy > 0 ? 0 : dy + FORCE;
      break;
   }

   // add force
   if (keyHitCounter > 0)
   {
      dy -= FORCE;
      --keyHitCounter;
   }

   // calc y pos
   playerY += dy;

   // if player max up
   if (playerY > 60000)
   {
      playerY = 0;
   }

   // calc player addr
   addr = PLAYER_X + multi40[playerY / 100];
   playerOldAddress = addr - 1;

   //collision check
   if (screenBuffer[addr] > 'a' || screenBuffer[addr + 2] > 'a' || screenBuffer[addr + 40] > 'a' || screenBuffer[addr + 42] > 'a')
   {
      shoot();
      state = STATE_DIE;
   }

   // if player max down
   if (playerY > MAX_Y * 100 - 200)
   {
      playerY = MAX_Y * 100 - 200;
   }

   // draw bird
   screenBuffer[addr++] = CHAR_BIRD1;
   screenBuffer[addr++] = CHAR_BIRD2;
   screenBuffer[addr] = CHAR_BIRD3;
   addr += 38;
   screenBuffer[addr++] = CHAR_BIRD4;
   screenBuffer[addr++] = CHAR_BIRD5;
   screenBuffer[addr++] = CHAR_BIRD6;
}

void scroll()
{
   static unsigned char counter, drawPipe;
   unsigned char y, pos, chr, tmp;
   unsigned int addr;

   // new pipe
   if (counter == 0)
   {
      // score
      score += 1;
      sprintf(SCORE_ADDR, "SCORE: %d     ", score);

      drawPipe = 1;

      // clear pipe
      memcpy(tempPipe, pipe, sizeof(pipe));

      // new pipe in arr
      pos = peek(0x304) % 15 + 1;
      tempPipe[0][pos] = 'j';
      tempPipe[1][pos] = 'k';
      tempPipe[2][pos] = 'k';
      tempPipe[3][pos] = 'k';
      tempPipe[4][pos] = 'l';

      tempPipe[0][pos + 7] = 'j';
      tempPipe[1][pos + 7] = 'k';
      tempPipe[2][pos + 7] = 'k';
      tempPipe[3][pos + 7] = 'k';
      tempPipe[4][pos + 7] = 'l';

      for (y = 1; y < 7; ++y)
      {
         tempPipe[0][pos + y] = ' ';
         tempPipe[1][pos + y] = ' ';
         tempPipe[2][pos + y] = ' ';
         tempPipe[3][pos + y] = ' ';
         tempPipe[4][pos + y] = ' ';
      }
   }

   // if a pipe is drawn, its 6 lines
   if (drawPipe)
   {
      ++drawPipe;
      if (drawPipe > 6)
      {
         drawPipe = 0;
         counter = pipeInverval;

         if (pipeInverval > 15)
         {
            --pipeInverval;
         }
      }
   }

   // move screen buffer
   memcpy(screenBuffer, screenBuffer + 1, SCREEN_LEN - 1);

   // set color and ink in start pos
   screenBuffer[0] = COLOR;
   screenBuffer[1] = INK;

   // the last address of the first line
   addr = 39;

   // draw new line
   for (y = 0; y < MAX_Y; ++y)
   {
      if (drawPipe)
      {
         tmp = tempPipe[drawPipe - 2][y];
         chr = tmp == 32 ? 32 : tmp + 128;
      }
      else
      {
         if (peek(0x304) < 4)
         {
            chr = '.';
         }
         else
         {
            chr = COLOR;
         }
      }

      screenBuffer[addr++] = chr;
      screenBuffer[addr++] = COLOR;
      screenBuffer[addr] = INK;

      // calc new line addr
      addr += 38;
   }

   --counter;
}

void wait(unsigned int time)
{
   if (time == 0)
   {
      return;
   }

   doke(0x276, time);
   while (deek(0x276) > 0)
   {
      ;
   }
}