extern volatile uint8_t p1Score;
extern volatile uint8_t p2Score;

extern volatile uint8_t delX;
extern volatile uint8_t delY;
extern volatile uint8_t positionX;
extern volatile uint8_t positionY;
extern enum gameState state;
enum gameState {HOMESCREEN, MODESELECTION,SPEEDSELECTION, PLAYGAME, PAUSEGAME, ENDSCREEN};
enum gameMode {SINGLEPLAYER, MULTIPLAYER};
enum gameSpeed {EASY, MEDIUM, HARD};
extern enum gameSpeed gSpeed;
extern enum gameMode gMode;

void moveBall(uint8_t color);
bool checkCollision(uint8_t ballX, uint8_t ballY, uint8_t pad1Y, uint8_t pad2Y, uint8_t rectWidth, uint8_t rectHeight) ;
bool hasP1Scored();
bool hasP2Scored();
void handlePointDrop(uint8_t color);
void controlPaddle();
void checkGameStatus();
void waitForSpeedSelection();
void waitForModeSelection();
void waitForStart();

