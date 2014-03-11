//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// height and width of paddle
#define paddlew 50
#define paddleh 5
// lives
#define LIVES 3

// velocity randomizer
double drand48();
// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    GRect brick = NULL;
    
    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // initial speed of ball
    double xvelo = drand48() + 0.05 , yvelo = drand48() + 0.05;
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // define how paddle moves
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                double y = getY(event) - getWidth(paddle);
                setLocation(paddle, x, HEIGHT - 50);
            }
        }
        // ball movement
        
        move(ball, xvelo, yvelo);
        
        if ((getX(ball) + getWidth(ball) >= getWidth(window)) || (getX(ball) <= 0))
        {
            xvelo = -xvelo;
        }
        else if (getY(ball) <= 0)
        {
            yvelo = -yvelo;
        }
        else if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            yvelo = -yvelo /*0, xvelo = 0*/;
        }
        
        // collision effects
        GObject object = detectCollision(window, ball);
        if (object == paddle)
        {
            yvelo = -(yvelo + drand48());   
        }
        else if (object != NULL)
        {    
            if (strcmp(getType(object), "GRect") == 0) 
            {
                removeGWindow(window, object);
                points++;
                //git remote set-url origin git://new.url.hereupdateScoreboard(window);
                yvelo = -yvelo;
               //TODO TODO  part 9. 
            }
        }
        pause(5);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int r, c, m;
    for (r = 0; r < ROWS; r++)
    {
        for (c = 0; c < COLS; c++) //
        {
            GRect brick = newGRect(5 + c * (WIDTH / COLS), (r + 2) * (HEIGHT / 30), (WIDTH / COLS) - 10, (HEIGHT / 30) - 10);
            
            m = r % 3;
            switch (m)
            {
                case 0:
                    setColor(brick, "GRAY");
                    break;
                case 1:
                    setColor(brick, "GREEN");
                    break;
                case 2:
                    setColor(brick, "BLUE");
                    break;
            }
            setFilled(brick, true);
            add(window, brick); 
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH / 2 - (RADIUS / 2), HEIGHT / 2 - (RADIUS / 2), RADIUS, RADIUS);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
   
    // make paddle
    GRect paddle = newGRect (WIDTH / 2 - paddlew / 2, HEIGHT - 50, paddlew, paddleh);
    setColor(paddle, "RED");
    setFilled(paddle, true);
    
    // place paddle
    add(window, paddle);
    
    return paddle;
}

/**
 * TODO TODO TODO Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    //center label in window
    
    GLabel label = newGLabel("0");
    
    if (label != NULL)
    {
        double x = (getWidth(window) - getWidth(label)) / 2;
        double y = (getHeight(window) - getHeight(label)) / 2;
        setFont(label, "SansSerif-40");
        setColor(label, "LIGHT_GRAY");
        setLocation(label, x, y);
        add(window, label);
    }
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
