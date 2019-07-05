#include <Adafruit_NeoPixel.h>

#define LEFT_BUTTON_PIN 2
#define LED_PIN 4
#define RIGHT_BUTTON_PIN 3
#define SCREEN_SIZE 4
#define GNOME_COLOR screen.Color(255,0,0)
#define COLOR_WHEN_KILLED screen.Color(0,255,0)
#define ROCK_COLOR screen.Color(0,0,255)
#define INITIAL_NUMBER_OF_PASSES 10
#define NUMBER_OF_ROCKS 3
#define SCORE_DISPLAY_PIN 13
#define SCORE_DISPLAY_SIZE 6
#define SCORE_COLOR scoreDisplay.Color(255,255,0)
#define BONUS_COLOR scoreDisplay.Color(255,0,255)

Adafruit_NeoPixel screen = Adafruit_NeoPixel(SCREEN_SIZE*SCREEN_SIZE, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel scoreDisplay = Adafruit_NeoPixel(SCORE_DISPLAY_SIZE, SCORE_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
unsigned int gnomeX,gnomeY; 
unsigned int rockX[NUMBER_OF_ROCKS],rockY[NUMBER_OF_ROCKS]; 
unsigned int bonusX,bonusY;
bool dead;
int leftButton,rightButton;
unsigned int numberOfPasses,passesBeforeNextStep;
unsigned int score;

void newGame()
{
    numberOfPasses=0;
  	passesBeforeNextStep=INITIAL_NUMBER_OF_PASSES;
	gnomeX=0;
  	gnomeY=SCREEN_SIZE-1;
  	bonusX=rand()%SCREEN_SIZE;
  	bonusY=rand()%(SCREEN_SIZE-2);
  	for(int i=0;i<NUMBER_OF_ROCKS;i++)
  	{
  		rockX[i]=rand()%SCREEN_SIZE;
    	rockY[i]=rand()%(SCREEN_SIZE-2);
  	}
   	leftButton=LOW;
  	rightButton=LOW;
  	score=0;
}

void setup()
{
  scoreDisplay.begin();
  screen.begin();
  newGame();
  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);
}

void loop()
{
    control();
  	if(numberOfPasses>=passesBeforeNextStep){
    	rockMovement();
  		numberOfPasses=0;
    }
  	collisionDetection();
    visualize();
  	delay(5);
  	showScore();
  	numberOfPasses++;
  	bonusMovement();
}

void collisionDetection()
{
	if(gnomeX==bonusX&&gnomeY==bonusY)
    {
    	score++;
    }
	showS()
  	dead=false;
  	for(int i=0;i<NUMBER_OF_ROCKS&&!dead;i++)
    {
    	dead=dead||(rockX[i]==gnomeX&&rockY[i]==gnomeY);
    }
	if(dead)
    {
      	for(int i=0;i<SCREEN_SIZE*SCREEN_SIZE;i++)
        {
          	screen.setPixelColor(i,COLOR_WHEN_KILLED);
        }
      	screen.show();
      	for(int i=0;i<SCREEN_SIZE*SCREEN_SIZE;i++)
        {
          	screen.setPixelColor(i,0);
        }
      	delay(5000);
        newGame();
    }
  	
}

void bonusMovement()
{
	if(bonusY==SCREEN_SIZE-1)
    {
    	bonusY=rand()%(SCREEN_SIZE-2);
        bonusX=rand()%SCREEN_SIZE;
    }
  	else
    {
    	bonusY++;
  	}
}

void control()
{
  	
    if(digitalRead(LEFT_BUTTON_PIN)!=leftButton)
    {
      if(leftButton==HIGH){
    	gnomeX--;
     	gnomeX%=SCREEN_SIZE;
      }      	
      leftButton=digitalRead(LEFT_BUTTON_PIN);
    }
    if(digitalRead(RIGHT_BUTTON_PIN)!=rightButton)
    {
      if(rightButton==HIGH){
    	gnomeX++;
      	gnomeX%=SCREEN_SIZE;
      }
      rightButton=digitalRead(RIGHT_BUTTON_PIN);
    }
}

void visualize()
{
  	for(int i=0;i<NUMBER_OF_ROCKS;i++){
		screen.setPixelColor(rockY[i]*SCREEN_SIZE+rockX[i],ROCK_COLOR);
  	}    
  	screen.setPixelColor(gnomeY*SCREEN_SIZE+gnomeX,GNOME_COLOR);
    screen.setPixelColor(bonusY*SCREEN_SIZE+bonusX,BONUS_COLOR);
    
  	screen.show();
  
  	screen.setPixelColor(gnomeY*SCREEN_SIZE+gnomeX,0);
    for(int i=0;i<NUMBER_OF_ROCKS;i++)
    {
  		screen.setPixelColor(rockY[i]*SCREEN_SIZE+rockX[i],0);
    }
  	screen.setPixelColor(bonusY*SCREEN_SIZE+bonusX,0);
}

void showScore()
{
	for(int j=0;j<SCORE_DISPLAY_SIZE;j++)
        {
            if(score&(1<<j))
            {
                scoreDisplay.setPixelColor(j,SCORE_COLOR);
            }
            else
            {
                scoreDisplay.setPixelColor(j,0);
            }
        }
    scoreDisplay.show();
}

void rockMovement()
{
  	for(int i=0;i<NUMBER_OF_ROCKS;i++)
  	{
  		if(rockY[i]==SCREEN_SIZE-1)
    	{
    		rockY[i]=rand()%(SCREEN_SIZE-2);
        	rockX[i]=rand()%SCREEN_SIZE;
          	if(passesBeforeNextStep>0){
          		passesBeforeNextStep--;
          	}
          	score++;
        }
  		else
    	{
    		rockY[i]++;
  		}
    }
}