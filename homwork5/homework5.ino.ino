#include <Arduino.h>
#include <LiquidCrystal.h>

/* ===== LCD PINS ===== */
const byte rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int backlightPin = 10;
const int BUZZER_PIN = 11;

/* ===== JOYSTICK / BUTTON ===== */
const int SW_pin = 12; // fire button
const int X_pin = A1;  // joystick vertical
const int PAUSE_pin = A0; // pause button

/* ===== GAME ===== */
int starship_row = 1; // 0 = top row, 1 = bottom row
bool game_in_progress = false;
unsigned long game_score = 0;

struct Bullet {
  bool active;
  int row;
  int col;
  unsigned long lastMove;
};
Bullet bullet;

struct Enemy {
  bool active;
  int row;
  int col;
  unsigned long lastMove;
};
Enemy enemies[5];

unsigned long lastEnemySpawn = 0;
const unsigned long enemySpeed = 200;
const unsigned long bulletSpeed = 100;

/* ===== CUSTOM CHARS ===== */
byte c_starship[8] = {B10000,B10100,B01110,B10101,B01110,B10100,B10000,B00000};
byte c_enemy[8]    = {B00100,B01000,B01010,B10100,B01010,B01000,B00100,B00000};
byte c_bullet[8]   = {B00000,B00000,B00000,B00110,B00110,B00000,B00000,B00000};

/* ===== GAME STATES ===== */
enum GameState { MENU, PLAYING, PAUSED, GAMEOVER };
GameState state = MENU;

/* ===== UTILS ===== */
int readJoystickRow() {
  int val = analogRead(X_pin); // 0-1023
  if(val < 400) return 1;      // push joystick DOWN → bottom row
  if(val > 600) return 0;      // push joystick UP → top row
  return starship_row;          // no movement
}

/* ===== FUNCTIONS ===== */
void resetGame(){
  game_in_progress = true;
  starship_row = 1;
  game_score = 0;
  bullet.active = false;
  for(int i=0;i<5;i++) enemies[i].active=false;
  lastEnemySpawn = millis();
}

void spawnEnemy(){
  for(int i=0;i<5;i++){
    if(!enemies[i].active){
      enemies[i].active = true;
      enemies[i].row = random(0,2);
      enemies[i].col = 15;
      enemies[i].lastMove = millis();
      break;
    }
  }
}

void drawScreen(){
  lcd.clear();
  // clear all
  for(int row=0; row<2; row++){
    for(int col=0; col<16; col++){
      lcd.setCursor(col,row);
      lcd.print(" ");
    }
  }

  // draw starship
  lcd.setCursor(0, starship_row);
  lcd.write((uint8_t)0);

  // draw bullet
  if(bullet.active){
    lcd.setCursor(bullet.col, bullet.row);
    lcd.write((uint8_t)2);
  }

  // draw enemies
  for(int i=0;i<5;i++){
    if(enemies[i].active){
      lcd.setCursor(enemies[i].col, enemies[i].row);
      lcd.write((uint8_t)1);
    }
  }

  // HUD: score bottom right
  lcd.setCursor(10,1);
  lcd.print("S:");
  lcd.print(game_score);
}

/* ===== SETUP ===== */
void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(PAUSE_pin, INPUT_PULLUP);
  pinMode(backlightPin, OUTPUT);
  analogWrite(backlightPin, 255);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.begin(16,2);
  lcd.createChar(0, c_starship);
  lcd.createChar(1, c_enemy);
  lcd.createChar(2, c_bullet);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" STARSHIP GAME ");
  lcd.setCursor(0,1);
  lcd.print("> Press button");
  randomSeed(analogRead(A3));
}

/* ===== LOOP ===== */
void loop(){
  bool fire = (digitalRead(SW_pin)==LOW);
  bool pauseBtn = (digitalRead(PAUSE_pin)==LOW);

  switch(state){
    case MENU:
      if(fire){
        resetGame();
        state=PLAYING;
        tone(BUZZER_PIN,1000,100);
      }
      break;

    case PLAYING:
      // --- starship movement ---
      starship_row = readJoystickRow();

      // --- pause ---
      if(pauseBtn){
        state = PAUSED;
        lcd.setCursor(4,0);
        lcd.print("***PAUSE***");
        delay(500); // simple debounce
        break;
      }

      // --- fire bullet ---
      if(fire && !bullet.active){
        bullet.active = true;
        bullet.col = 1;
        bullet.row = starship_row;
        bullet.lastMove = millis();
        tone(BUZZER_PIN,1200,50);
      }

      // --- move bullet ---
      if(bullet.active && millis()-bullet.lastMove >= bulletSpeed){
        bullet.lastMove = millis();
        if(bullet.col < 15) bullet.col++;
        else bullet.active = false;
      }

      // --- spawn enemies ---
      if(millis()-lastEnemySpawn >= enemySpeed*3){
        spawnEnemy();
        lastEnemySpawn = millis();
      }

      // --- move enemies ---
      for(int i=0;i<5;i++){
        if(enemies[i].active && millis()-enemies[i].lastMove >= enemySpeed){
          enemies[i].lastMove = millis();
          enemies[i].col--;
          if(enemies[i].col < 0) enemies[i].active = false;
        }
      }

      // --- collisions ---
      for(int i=0;i<5;i++){
        if(enemies[i].active){
          // bullet hits enemy
          if(bullet.active && bullet.col==enemies[i].col && bullet.row==enemies[i].row){
            enemies[i].active=false;
            bullet.active=false;
            game_score+=10;
            tone(BUZZER_PIN,1500,50);
          }
          // enemy hits starship
          if(enemies[i].col==0 && enemies[i].row==starship_row){
            state=GAMEOVER;
            tone(BUZZER_PIN,500,200);
          }
        }
      }

      drawScreen();
      break;

    case PAUSED:
      if(fire){
        state=PLAYING;
        lcd.clear();
        drawScreen();
        tone(BUZZER_PIN,800,100);
      }
      break;

    case GAMEOVER:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" GAME OVER! ");
      lcd.setCursor(0,1);
      lcd.print("Score:");
      lcd.print(game_score);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" STARSHIP GAME ");
      lcd.setCursor(0,1);
      lcd.print("> Press button");
      state=MENU;
      break;
  }

  delay(50);
}
