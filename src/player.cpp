#include "player.h"
#include "utility.h"

#include <iostream>

//Player movement functor
struct MovePlayer
{
    MovePlayer(float vx, float vy)
        : velocity(vx, vy)
    {
    }

    void operator() (WorldNode& node, sf::Time) const
    {
        Actor& actor = static_cast<Actor&>(node);
        //accelerate the actor
        actor.setVelocity(adjustVectorLength(actor.getVelocity() + velocity, actor.getSpeed()));
    }

    sf::Vector2f velocity;
};


Player::Player()
    : shooting(false)
    , speed(250.f)
{
    //Initial hardcoded keybindings
    //TODO: Make Settings class to handle this
    keyBinding[sf::Keyboard::W] = MoveUp;
    keyBinding[sf::Keyboard::A] = MoveLeft;
    keyBinding[sf::Keyboard::S] = MoveDown;
    keyBinding[sf::Keyboard::D] = MoveRight;
    keyBinding[sf::Keyboard::Escape] = Menu;
    //TODO: remove Debug keys for release
    keyBinding[sf::Keyboard::N] = Debug1;
    keyBinding[sf::Keyboard::M] = Debug2;

    //Action bindings
    actionBinding[MoveUp] = Command(MovePlayer(0.f, -speed), Category::Player);
    actionBinding[MoveLeft] = Command(MovePlayer(-speed, 0.f), Category::Player);
    actionBinding[MoveDown] = Command(MovePlayer(0.f, speed), Category::Player);
    actionBinding[MoveRight] = Command(MovePlayer(speed, 0.f), Category::Player);
    //DEBUG
    actionBinding[Debug1].action =
        [] (WorldNode& node, sf::Time)
    {
        std::cout << node.getPosition().x << ", "
                  << node.getPosition().y << std::endl;
    };
    actionBinding[Debug2].action =
        [] (WorldNode&, sf::Time)
    {
        std::cout << randomInt(100) << std::endl;
    };
}


bool Player::isShooting() const
{
    return shooting;
}

void Player::setShooting(bool shoot)
{
    shooting = shoot;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed
            && keyBinding[event.key.code])
    {
        commands.push(actionBinding[keyBinding[event.key.code]]);
    }
}

void Player::handleRealTimeInput(CommandQueue& commands)
{
    //Check if any bound keys are being pressed and act accordingly
    for (auto keyBind : keyBinding)
    {
        if (sf::Keyboard::isKeyPressed(keyBind.first)
                && isRealTimeAction(keyBind.second))
        {
            commands.push(actionBinding[keyBind.second]);
        }
    }
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
    //Check if action is already mapped and remove older mapping if it is
    for (auto it = keyBinding.begin(); it != keyBinding.end(); ++it)
    {
        if (it->second == action)
        {
            it = keyBinding.erase(it);
        }
    }
    keyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
    for (auto it = keyBinding.begin(); it != keyBinding.end(); ++it)
    {
        if (it->second == action)
            return it->first;
    }
    return sf::Keyboard::Unknown;
}


Category::Type Player::getCategory() const
{
    return Category::Player;
}

bool Player::isRealTimeAction(Action action)
{
    switch (action)
    {
    case MoveUp:
    case MoveLeft:
    case MoveDown:
    case MoveRight:
        return true;
    default:
        return false;
    }
}
