#include "entity.h"

float Entity::getSpeed() const
{
    return speed;
}

void Entity::setSpeed(float speed)
{
    this->speed = speed;
}

sf::Vector2f Entity::getVelocity() const
{
    return velocity;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    this->velocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    velocity = sf::Vector2f(vx, vy);
}

void Entity::setOrientation(Direction newOrient)
{
    orientation = newOrient;
}

/*
 * Flip actor horizontally
 */
void Entity::flip()
{
    auto scale = getScale();
    //Setting the scale to negative flips the entity about its origin
    setScale(-scale.x, scale.y);
    if (orientation == RIGHT)
        orientation = LEFT;
    else
        orientation = RIGHT;
}

/*
 * If target is behind actor, flip sprite horizontally.
 */
void Entity::flip(sf::Vector2f target)
{
    //xval is the x component of the vector going from the entity to the target
    float xval = target.x - getPosition().x;
    if ((xval < 0 && orientation == RIGHT) ||
            (xval > 0 && orientation == LEFT))
    {
        flip();
    }
}

sf::FloatRect Entity::getHitbox() const
{
    return this->sprite.getLocalBounds();
}

void Entity::updateCurrent(sf::Time dt)
{
    move(velocity * dt.asSeconds());
}

