/*
Aliens!!1
Parachute clone


*/

#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones audio(arduboy.audio.enabled);

float turretAngle = 0; //angle of the turret for display and ammo direction
bool canShoot = true;

Rect hitbox = Rect(10,10,50,5);

class Ball{
  private:
    float x;
    float y;
    float xInterval;
    float yInterval;
    int distanceMoved;

public:
  void init(){
    x=0;
    y=0;
    xInterval=1;
    yInterval=1;
 }

  void shoot(float pos){
    pos += 180;

     x = sin(radians(pos)) * 15 + 64;
     y = cos(radians(pos)) * 15 + 55;

     xInterval = sin(radians(pos));
     yInterval = cos(radians(pos));

    drawBall();
  }

  void updatePos(){
    x += xInterval;
    y += yInterval;
    drawBall();
  }

  void drawBall(){
    arduboy.fillCircle(x, y, 3);
  }

  Rect getCollider(){
    return Rect(x-2, y-2, 4, 4);
  }
};

Ball testBall;

Ball ammo[] = {Ball(), Ball(),Ball(),Ball(),Ball(),Ball(),Ball(),Ball(),Ball(),Ball()};
int ammoIndex = 0;

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();
  
  Serial.begin(115200);

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(30);
}

void initGame(){
  arduboy.clear();

  arduboy.fillCircle(64, 55, 5);
  
  arduboy.fillRect(59, 55, 11, 10);
}

void renderTurret(float pos){

  pos += 180;
  
  float origin1X = 64;
  float origin1Y = 55;

  float end1X = sin(radians(pos)) * 15 + origin1X;
  float end1Y = cos(radians(pos)) * 15 + origin1Y;

  arduboy.drawLine((int)origin1X, (int)origin1Y, (int)end1X, (int)end1Y);
  
  float origin2X = 63;
  float origin2Y = 55;

  float end2X = sin(radians(pos)) * 15 + origin2X;
  float end2Y = cos(radians(pos)) * 15 + origin2Y;

  arduboy.drawLine((int)origin2X, (int)origin2Y, (int)end2X, (int)end2Y);
  
  float origin3X = 65;
  float origin3Y = 55;

  float end3X = sin(radians(pos)) * 15 + origin3X;
  float end3Y = cos(radians(pos)) * 15 + origin3Y;

  arduboy.drawLine((int)origin3X, (int)origin3Y, (int)end3X, (int)end3Y);
  
}

// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  initGame();

  if(arduboy.pressed(RIGHT_BUTTON) && turretAngle > -90)
    turretAngle -=2;

  if(arduboy.pressed(LEFT_BUTTON) && turretAngle < 90)
    turretAngle +=2;

  if(arduboy.pressed(A_BUTTON)) {
    if(canShoot) {
      ammo[ammoIndex].shoot(turretAngle);

      ammoIndex ++;
      if (ammoIndex >= 9) {
        ammoIndex = 0;
      }
    }

    canShoot = false;
    
  }

  if(!arduboy.pressed(A_BUTTON))
    canShoot = true;
  
  renderTurret(turretAngle);

  arduboy.drawRect(10,10,50,5);
  
  for(int i = 0; i < 9; i ++){
    ammo[i].updatePos();
  if(arduboy.collide(hitbox, ammo[i].getCollider())){
    Serial.println("BOOM");
    audio.tone(NOTE_C5, 250);
  }
  }


  
  
  arduboy.display();
  
}
