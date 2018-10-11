# PONG GAME
## Group 1
This is a read-me file for the lab assignment 3. Design a Pong Game using a GLCD, TFT Touchscreen and an acclerometer.

### Methods
**void lcd_init(void)**
This method initializes the GLCD. 

**void lcd_command(uint8_t c)**
This method is used to send commands to the LCD. 

**void lcd_data(uint8_t c)**
This method is used to send data to the LCD. 

**void lcd_set_brightness(uint8_t val)**
This methods sets the brightness of the LCD.
**void clear_buffer(uint8_t *buff)**
This method is used to clear the LCD buffer. Clears the buffer passed in the parameter.

**void clear_screen()**
This method clears the LCD screen.
**void write_buffer(uint8_t *buff)**
This method writes the the buffer.

**void setpixel(uint8_t *buff, uint8_t x, uint8_t y, uint8_t color)**
This methods sets a pixel on the LCD. Accepts 4 parameters, the buffer in which pixel needs to be set, the x and y positions of the pixel and the color of the pixel.

**void clearpixel(uint8_t *buff, uint8_t x, uint8_t y)**
This methods clears the pixel in the buffer for the positions x & y passed in the parameter.

**void drawstring(uint8_t *buff, uint8_t x, uint8_t line, uint8_t *c)**
This method is used to write a string. Accepts the x position, the line number and the string to be printed in addition to the buffer in which the string needs to be printed.

**void drawchar(uint8_t *buff, uint8_t x, uint8_t line, uint8_t c)**
This methods writes a single character to the buffer at the x position and the line number passed in the arguments. Each character is 5 pixels wide.

**void drawrect(uint8_t *buff,
uint8_t x, uint8_t y, uint8_t w, uint8_t h,
uint8_t color)**

This method draws a rectangle in the buffer starting at position x and y. It accepts the width and height of the rectangle to be drawn. 

**void fillrect(uint8_t *buff,
uint8_t x, uint8_t y, uint8_t w, uint8_t h,
uint8_t color)**
This similar to the drawrect method. But fills the rectangle completely with black pixels.

**void drawline(uint8_t *buff,
uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
uint8_t color)**
This method draws a line in the buffer. Accepts the 2 starting points x0 and y0 and the end points of the line x1 and y1 between which the line needs to be drawn.

**void drawcircle(uint8_t *buff,
uint8_t x0, uint8_t y0, uint8_t r,
uint8_t color)**

This method draws a circle with the center at location x0 and y0 with a radius of r.

**void fillcircle(uint8_t *buff,
uint8_t x0, uint8_t y0, uint8_t r,
uint8_t color)**
This is similar to drawcircle method. But it fills the circle with black pixels completely.

**void drawBorders(uint8_t color)**
This method draws the borders at the edges of the LCD screen.

**void drawDiagonals(uint8_t color)**
This method draws the diagonals from the edges of the LCD screen.

**void drawHomePage(uint8_t BLACK)**
This method draws the home page of the Pong game. The game can be started from this screen.

**void playerInfo(uint8_t color)**
This method draws the player information on top of the screen. Updates the scoring information for each player.

**void drawModeSelection(uint8_t color)**
This screen draws the mode selection screen. The mode of the game (Single or Multiplayer) can be selected from this screen.

**void drawSpeedSelection(uint8_t color)**
This screen draws the speed selection screen. The speed or the difficulty of the game can be selected from this screen.

**void drawPaddle(uint8_t paddle, uint8_t color)**
This method draws the paddles for playing the game. Accepts the parameter for which paddle needs to be drawn. The position of the paddle is taken from the mapped ADC values when the user touches a particular section of the screen.

**void startGame(uint8_t BLACK)**
This is the method where the game play begins. Draws the paddles and the balls and the continues to run till the state of the game is in "Play" mode.

**void drawPauseScreen(uint8_t color)**
This method draws the pause screen. The gameplay can be resumed from this screen.
*This is in Beta version.*

**void drawEndScreen(uint8_t color)**
This method draws the end screen once the game is over. Displays the player who won the game and returns to the home page.

**void adcInit()**
This method initialises the ADC. The clock rate of the ADC is prescaled by 128. ADC is configured to run in the interrupt mode.

**uint8_t adc_read()**
This method is used to read the ADC readings from the selected channel. This method starts the ADC conversion. The converted digital value is read in the ISR(ADC_vect) sub routine.

**uint8_t mapTocuhValues(char axis)**
This method is used to map the ADC values to the corresponding locations on the LCD. IT accepts a parameter which can be 'x' or 'y' to specify which axis needs to be mapped.

**void moveBall(uint8_t color)**
This function moves the position of the ball. The ball always starts from the center of the screen and moves randomly in x and y direction. The new position of the ball is calculated by adding an offset delta to the existing position of the ball.

**bool checkCollision(uint8_t ballX, uint8_t ballY, uint8_t pad1Y, uint8_t pad2Y, uint8_t rectWidth, uint8_t rectHeight)**
This method checks the if there is a collision between the paddles and the ball. If a collision is detected the method returns a true.

**bool hasP1Scored()**
This method returns true if the player 1 scores a point.

**bool hasP2Scored()**
This method returns true if the player 2 scores a point.

**void handlePointDrop(uint8_t color)**
This function handles when a player drops a point. Changes the background color and plays a beep sound using PWM.

**void controlPaddle()**
This method controls the movement of the paddles. Depending on the mode of the game.

**void checkGameStatus()**
This method checks the state of the game. Monitors the points scored. Player to score 3 points first wins the game.

**void waitForSpeedSelection()**
Waits for the user to select speed levels of the game.

**void waitForModeSelection()**
Waits for the user to select the mode of the game.

**void waitForStart()**
Waits for the user to start the game.

