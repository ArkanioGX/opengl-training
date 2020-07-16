#include "Bullet.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleCollisionComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "ResourceManager.h"

Bullet::Bullet()
{
	SpriteComponent* sc = new SpriteComponent(*this, ResourceManager::getTexture("Projectile"));
	MoveComponent* mc = new MoveComponent(*this);
	mc->setForwardSpeed(400.0f);
	circle = new CircleCollisionComponent(*this);
	circle->setRadius(5.0f);
	liveTime = 1.0f;
}

void Bullet::updateActor(float dt)
{
	Actor::updateActor(dt);

	for (Enemy* e : getGame().getEnemies())
	{
		if (Intersect(*circle, e->getCircle()))
		{
			// Both die on collision
			e->setState(ActorState::Dead);
			setState(ActorState::Dead);
			break;
		}
	}

	liveTime -= dt;
	if (liveTime <= 0.0f)
	{
		setState(ActorState::Dead);
	}
}