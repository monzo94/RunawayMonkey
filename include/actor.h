/**
 * @file actor.h
 * @author Daniel Monzonís
 */

#pragma once

#include <memory>

#include "command_queue.h"
#include "projectile.h"

/**
 * @brief An Entity that is "alive" and interact with the world.
 *
 * Actor is a subclass of Entity for entities that are "alive". Actors have
 * health, and they get destroyed when their health reaches 0.
 * Actors have Category types depending on the internal Actor::Type, which
 * will determine which types of commands can be executed by them.
 * Some actors may also be able to shoot projectiles, so the class also has
 * methods for projectile shooting.
 *
 * @see Command Depending on its type, actors can execute commands targeted for
 * them.
 */
class Actor : public Entity {
 public:
    enum Type {
        None,
        Monkey,
        Snatcher,
        Dog,
    };

 public:
    /**
     * @param type Define actor type from Actor::Type.
     * @param textures Reference to TextureHolder to grab textures from.
     * @param score Amount of score points the actor awards when killed.
     */
    explicit Actor(Type type, const TextureHolder &textures);

    /**
     * @return Category::Player if Actor::Type is monkey, Category::Enemy
     * otherwise.
     */
    virtual Category::Type getCategory() const;

    /**
     * @return Current actor's health.
     */
    int getHealth() const;

    /**
     * Sets the health of the actor to the given value.
     *
     * @param health Value to set the actor's health to.
     */
    void setHealth(int health);

    /**
     * Decrease actor's health by a given amount. If the amount is a
     * negative value, the actor's health is increased instead.
     * If the actor's health reaches 0 or less after the damage,
     * the @ref die() method is called. If the actor's health surpasses
     * the maximum health permitted after the damage, the actor's health
     * is set to the maximum health permitted.
     * Then, returns the actor's score value if it died, or 0 otherwise.
     *
     * @param amount Damage to be dealt to the actor.
     *
     * @return The actor's score if it died by the damage, 0 otherwise.
     */
    int damage(int amount);

    /**
     * Destroy actor. Calls WorldNode::destroy().
     */
    void die();

    /**
     * Gives ammo to the actor equal to the amount.
     *
     * @param Amount of ammo to give to the actor. Use a negative value to
     * take ammo from it.
     */
    void giveAmmo(int amount);

    /**
     * @return The amount of ammo the actor has.
     */
    int getAmmo() const;

    /**
     * @return true if the actor is in a shooting state, false otherwise.
     */
    bool isShooting() const;

    /**
     * Set the shooting state to flag.
     */
    void setShooting(bool flag);

    /**
     * If the actor can, shoot a projectile.
     * If the actor hasn't shot a projectile in at least a time equal to its
     * shooting cooldown, shoot a projectile of a given type.
     *
     * @param type Type of the projectile to be instantiated.
     * @param commands Pointer to the world's CommandQueue.
     */
    void shoot(Projectile::Type type, CommandQueue *commands);

    /**
     * Creates a Command of category Category::Sound (so that it is only
     * executed by a SoundNode object) with the action of playing the
     * requested sound at the actor's current position in the world.
     *
     * @param id Sound to be played.
     * @param commands Pointer to the world's CommandQueue
     */
    void playSound(Sounds::ID id, CommandQueue *commands);

 private:
    bool canShoot() const;
    virtual void drawCurrent(sf::RenderTarget &, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time);

 private:
    const TextureHolder &textures;
    Type type;
    int health, maxHealth;
    bool shooting;
    sf::Time shootRate, cooldown;
    int score, ammo;
};
