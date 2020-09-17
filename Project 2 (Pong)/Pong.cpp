#include "Assets/Bat/Bat.h"
#include "Assets/Ball/Ball.h"
#include <sstream>
#include <iostream>

int main()
{
    /**
     * Initialize
     */

    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Pong", Style::Default);

    int score = 0;
    int lives = 3;

    Bat bat(1920 / 2, 1080 - 20);
    Ball ball(1920 / 2, 0);

    Text hud;
    Font font;
    font.loadFromFile("Assets/Fonts/ka1.ttf");
    hud.setFont(font);
    hud.setCharacterSize(75);
    hud.setFillColor(Color::White);

    Clock clock;

    while (window.isOpen())
    {
        /**
         * Player Input
         */

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left) || bat.getPosition().left + bat.getPosition().width > window.getSize().x)
        {
            bat.moveLeft();
        }
        else
        {
            bat.stopLeft();
        }

        if (Keyboard::isKeyPressed(Keyboard::Right) || bat.getPosition().left < 0)
        {
            bat.moveRight();
        }
        else
        {
            bat.stopRight();
        }

        /**
         * Update Loop
         */

        Time delta = clock.restart();
        bat.update(delta);
        ball.update(delta);

        // Handles HUD updating and concatenation
        std::stringstream ss;
        ss << "Score:" << score << "  " << " Lives:" << lives;
        hud.setString(ss.str());

        // Handles ball

        if (ball.getPosition().top > window.getSize().y)
        {
            ball.reboundBottom();

            lives--;

            if (lives < 1)
            {
                score = 0;
                lives = 3;
            }
        }

        if (ball.getPosition().top < 0)
        {
            ball.reboundBatOrTop();
            score++;
        }

        if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > window.getSize().x)
        {
            ball.reboundSides();
        }

        if (ball.getPosition().intersects(bat.getPosition()))
        {
            ball.reboundBatOrTop();
        }

        /**
         * Draw
         */

        window.clear();
        window.draw(hud);
        window.draw(bat.getShape());
        window.draw(ball.getShape());
        window.display();
    }

    return 0;
}