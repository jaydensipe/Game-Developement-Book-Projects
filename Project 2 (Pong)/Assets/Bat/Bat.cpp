#include "Bat.h"

Bat::Bat(float startX, float startY)
{
    m_Position.x = startX;
    m_Position.y = startY;

    m_Shape.setSize(sf::Vector2f(50, 5));
    m_Shape.setPosition(m_Position);
}

// Getter method for returning bat position
FloatRect Bat::getPosition()
{
    return m_Shape.getGlobalBounds();
}

// Getter method for returning bat shape (used for collisions)
RectangleShape Bat::getShape()
{
    return m_Shape;
}

// Methods used for input
void Bat::moveLeft()
{
    m_MovingLeft = true;
}

void Bat::moveRight()
{
    m_MovingRight = true;
}

void Bat::stopLeft()
{
    m_MovingLeft = false;
}

void Bat::stopRight()
{
    m_MovingRight = false;
}

// Update method for bat
void Bat::update(Time delta)
{
    if (m_MovingLeft)
        m_Position.x -= m_Speed * delta.asSeconds();

    if (m_MovingRight)
        m_Position.x += m_Speed * delta.asSeconds();

    m_Shape.setPosition(m_Position);
}
