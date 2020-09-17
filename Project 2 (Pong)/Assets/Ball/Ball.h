#ifndef PONG_BALL_H
#define PONG_BALL_H
#include <SFML/Graphics.hpp>
#pragma once

using namespace sf;

class Ball
{

private:
    Vector2f m_Position;
    RectangleShape m_Shape;
    float m_Speed = 1500.0f;
    float m_DirectionX = 0.2f;
    float m_DirectionY = 0.2f;

public:
    Ball(float startX, float startY);

    FloatRect getPosition();
    RectangleShape getShape();

    float getXVelocity();

    void reboundSides();
    void reboundBatOrTop();
    void reboundBottom();

    void update(Time delta);
};

#endif //PONG_BALL_H
