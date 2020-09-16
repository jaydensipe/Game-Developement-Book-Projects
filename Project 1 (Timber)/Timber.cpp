#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <sstream>
using namespace sf;

// Function declaration

void updateBranches(int seed);

// Global variables

const int NUM_BRANCHES = 6;
Sprite branches [NUM_BRANCHES];

enum class side
{
    LEFT,
    RIGHT,
    NONE
};

side branchPositions[NUM_BRANCHES];


int main()
{

    /**
     *  Initialize
     */

    VideoMode vm (1920, 1080);
    RenderWindow window(vm, "Timber!", Style::Default);

    // Initializes background image
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");

    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0,0);

    // Initializes tree
    Texture treeTexture;
    treeTexture.loadFromFile("graphics/tree.png");

    Sprite tree;
    tree.setTexture(treeTexture);
    tree.setPosition(810, 0);

    // Initializes bees and variables

    Texture beeTexture;
    beeTexture.loadFromFile("graphics/bee.png");

    Sprite bee;
    bee.setTexture(beeTexture);
    bee.setPosition(0, 800);

    bool isBeMoving = false;
    float beeSpeed = 0.0f;

    // Initializes clouds and variables

    Texture cloudTexture;
    cloudTexture.loadFromFile("graphics/cloud.png");

    Sprite cloud1, cloud2, cloud3;
    cloud1.setTexture(cloudTexture);
    cloud2.setTexture(cloudTexture);
    cloud3.setTexture(cloudTexture);

    cloud1.setPosition(0, 0);
    cloud2.setPosition(0, 250);
    cloud3.setPosition(0, 500);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Initializes branches

    Texture branchTexture;
    branchTexture.loadFromFile("graphics/branch.png");

    // Initializes player

    Texture playerTexture;
    playerTexture.loadFromFile("graphics/player.png");
    Sprite player;
    player.setTexture(playerTexture);
    player.setPosition(580, 720);
    side playerSide = side::LEFT;

    // Initializes gravestone
    Texture gravestoneTexture;
    gravestoneTexture.loadFromFile("graphics/rip.png");
    Sprite gravestone;
    gravestone.setTexture(gravestoneTexture);
    gravestone.setPosition(600, 860);

    // Initializes axe

    Texture axeTexture;
    axeTexture.loadFromFile("graphics/axe.png");
    Sprite axe;
    axe.setTexture(axeTexture);
    axe.setPosition(700, 830);
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Initializes flying log

    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite log;
    log.setTexture(textureLog);
    log.setPosition(810, 720);
    bool logFlying = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;


    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(branchTexture);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // Initializes sound

    SoundBuffer chopSoundBuffer;
    chopSoundBuffer.loadFromFile("sound/chop.wav");
    SoundBuffer deathSoundBuffer;
    deathSoundBuffer.loadFromFile("sound/death.wav");
    SoundBuffer ootSoundBuffer;
    ootSoundBuffer.loadFromFile("sound/out_of_time.wav");

    Sound chopSound;
    chopSound.setBuffer(chopSoundBuffer);
    Sound deathSound;
    deathSound.setBuffer(deathSoundBuffer);
    Sound ootSound;
    ootSound.setBuffer(ootSoundBuffer);

    // Game variables

    Clock clock;

    RectangleShape timeBar;
    float timeBarStartWidth = 400.0f;
    float timeBarHeight = 80.0f;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool gamePaused = true;
    bool acceptInput = false;

    int score = 0;

    // HUD & Text Drawing

    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    Text scoreText;
    Text messageText;

    scoreText.setFont(font);
    messageText.setFont(font);

    scoreText.setString("Score = 0");
    messageText.setString("Press 'Enter' to start!");

    scoreText.setCharacterSize(100);
    messageText.setCharacterSize(75);

    scoreText.setFillColor(Color::White);
    messageText.setFillColor(Color::White);
    scoreText.setOutlineColor(Color::Black);
    messageText.setOutlineColor(Color::Black);
    messageText.setOutlineThickness(10.0f);
    scoreText.setOutlineThickness(10.0f);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);


   /**
    *  Game Loop
    */

    while(window.isOpen()) {
        /**
         *  Inputs
         */

        Event event;

        while(window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !gamePaused)
            {
                acceptInput = true;

                axe.setPosition(2000, axe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        if (gamePaused == true)
        {
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                gamePaused = false;

                // Reset
                score = 0;
                timeRemaining = 6.0f;

                for (int i = 1; i < NUM_BRANCHES; i++)
                {
                    branchPositions[i] = side::NONE;
                }

                gravestone.setPosition(675, 2000);

                player.setPosition(580, 720);

                acceptInput = true;
            }
        }


        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;

                timeRemaining += (2 / score) + .15;

                axe.setPosition(AXE_POSITION_RIGHT, axe.getPosition().y);

                player.setPosition(1200, 720);

                updateBranches(score);
                log.setPosition(810, 720);
                logSpeedX = -5000;
                logFlying = true;

                acceptInput = false;
                chopSound.play();
            }

            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;

                timeRemaining += (2 / score) + .15;

                axe.setPosition(AXE_POSITION_LEFT, axe.getPosition().y);

                player.setPosition(580, 720);

                updateBranches(score);
                log.setPosition(810, 720);
                logSpeedX = 5000;
                logFlying = true;

                acceptInput = false;
                chopSound.play();
            }
        }



        /**
         *  Update Loop
         */

        if (!gamePaused)
        {
            Time delta = clock.restart();

            timeRemaining -= delta.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            // When game time runs out
            if (timeRemaining <= 0.0f)
            {
                gamePaused = true;
                messageText.setString("You have run out of time!");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                ootSound.play();
            }

            if (!isBeMoving)
            {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                bee.setPosition(2000, height);

                isBeMoving = true;
            }
            else
            {
                bee.setPosition(bee.getPosition().x - (beeSpeed * delta.asSeconds()), bee.getPosition().y);

                if (bee.getPosition().x < -100)
                {
                    isBeMoving = false;
                }
            }

            if(!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                srand((int)time(0) * 10);
                float height = rand() % 150;
                cloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                cloud1.setPosition((cloud1.getPosition().x + delta.asSeconds() * cloud1Speed), cloud1.getPosition().y);

                if (cloud1.getPosition().x > 1920)
                {
                    cloud1Active = false;
                }
            }

            if(!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                cloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                cloud2.setPosition((cloud2.getPosition().x + delta.asSeconds() * cloud2Speed), cloud2.getPosition().y);

                if (cloud2.getPosition().x > 1920)
                {
                    cloud2Active = false;
                }
            }

            if(!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                cloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                cloud3.setPosition((cloud3.getPosition().x + delta.asSeconds() * cloud3Speed), cloud3.getPosition().y);

                if (cloud3.getPosition().x > 1920)
                {
                    cloud3Active = false;
                }
            }

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // Updates branch position

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;

                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    branches[i].setRotation(0);

                }
                else
                {
                    branches[i].setPosition(3000, height);

                }
            }

            // Makes logs fly off in a direction

            if (logFlying)
            {
                log.setPosition(log.getPosition().x + (logSpeedX * delta.asSeconds()), log.getPosition().y + (logSpeedY * delta.asSeconds()));

                if (log.getPosition().x < -100 || log.getPosition().x > 2000)
                {
                    logFlying = false;
                    log.setPosition(810, 720);
                }
            }

            // Detects if a player is hit by a branch

            if (branchPositions[5] == playerSide)
            {
                gamePaused = true;
                acceptInput = false;

                gravestone.setPosition(525, 760);
                player.setPosition(2000, 660);

                messageText.setString("You are dead! Press 'Enter' to retry");
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                deathSound.play();
            }

        }

        /**
         * Draw
         */

        window.clear();

        window.draw(spriteBackground);
        window.draw(player);
        window.draw(axe);
        window.draw(log);
        window.draw(gravestone);
        window.draw(tree);

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(bee);
        window.draw(cloud1);
        window.draw(cloud2);
        window.draw(cloud3);
        window.draw(scoreText);
        window.draw(timeBar);

        if (gamePaused)
        {
            window.draw(messageText);
        }


        window.display();
    }

}

// Updates branch positions on tree

void updateBranches(int seed)
{
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
    {
        branchPositions[i] = branchPositions[i - 1];
    }

    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;

    }
}
