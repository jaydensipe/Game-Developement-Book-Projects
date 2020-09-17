#include "Ball.h"


Ball::Ball(float startX, float startY)
{
    m_Position.x = startX;
    m_Position.y = startY;
    m_Shape.setSize(sf::Vector2f(10, 10));
    m_Shape.setPosition(m_Position);
}

// Getter method for returning position of ball
FloatRect Ball::getPosition()
{
    return m_Shape.getGlobalBounds();
}

// Getter method for returning shape of ball (used for collisions)
RectangleShape Ball::getShape()
{
    return m_Shape;
}

float Ball::getXVelocity()
{
    return m_DirectionX;
}

// Determines what happens when ball hits sides of the screen
void Ball::reboundSides()
{
    m_DirectionX = -m_DirectionX;
}

// Determines what happens when ball hits the bat or top of screen
void Ball::reboundBatOrTop()
{
    m_DirectionY = -m_DirectionY;
}

// Determines what happens when ball hits the bottom of the screen
void Ball::reboundBottom()
{
    m_Position.y = 0;
    m_Position.x = 500;
    m_DirectionY = -m_DirectionY;
}

// Update function for the ball
void Ball::update(Time delta)
{
    m_Position.y += m_DirectionY * m_Speed * delta.asSeconds();
    m_Position.x += m_DirectionX * m_Speed * delta.asSeconds();
    m_Shape.setPosition(m_Position);
}