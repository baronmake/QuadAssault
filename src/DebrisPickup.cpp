#include "DebrisPickup.h"

#include "DebrisParticle.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Explosion.h"
#include "Player.h"
#include "RenderUtility.h"

class DebrisPickupRenderer : public IRenderer
{
public:
	virtual void init()
	{
		tex = getGame()->getTextureMgr()->getTexture("SmeceDiffuse.tga");
		texN= getGame()->getTextureMgr()->getTexture("SmeceNormal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass == RP_GLOW )
			return;

		Texture* t;
		if(pass==RP_DIFFUSE)
			t=tex;
		else if(pass==RP_NORMAL)
			t=texN;

		drawSprite( object->getRenderPos() , object->getSize() , 0 , t );
		glColor3f(1.0, 1.0, 1.0);
	}

	Texture* tex;
	Texture* texN;
};

DEFINE_RENDERER( DebrisPickup , DebrisPickupRenderer )

DebrisPickup::DebrisPickup( Vec2f const& pos ) 
	:BaseClass( pos )
{

}

void DebrisPickup::init()
{
	BaseClass::init();

	mSize.x=16;
	mSize.y=16;	

	brzina=200;
	cesticaTimer=1.0;
}

void DebrisPickup::onSpawn()
{
	BaseClass::onSpawn();

	dir.x=getLevel()->random(1,10)-5;
	dir.x/=5;
	dir.y=getLevel()->random(1,10)-5;
	dir.y/=5;
	Math::normalize( dir );

	light = getLevel()->createLight( getPos() ,64 , false);
	light->setColorParam(Vec3(1.0, 0.75, 0.5),4);	
	light->isExplosion = true;
}

void DebrisPickup::onDestroy()
{
	light->destroy();
	BaseClass::onDestroy();
}

void DebrisPickup::tick()
{
	BaseClass::tick();		
	
	if(brzina>0.0)
	{
		light->setPos(getPos());
		if(cesticaTimer>=1.0)
		{
			DebrisParticle* c = new DebrisParticle( getPos() );
			c->init();	
			getLevel()->addParticle( c );
				
			cesticaTimer = 0.0;
		}
		else
		{
			cesticaTimer += TICK_TIME*10;
		}

		Vec2f off = ( brzina * TICK_TIME ) * dir;

		Vec2f offset = Vec2f( 0 , 0 );

		offset.y += off.y;
		if( testCollision( offset ) )
		{
			offset.y = 0;
			dir.y =-dir.y;
		}
		offset.x += off.x;
		if( testCollision( offset ) )
		{
			offset.x = 0;
			dir.x=-dir.x;
		}

		mPos += offset;
		brzina-=100*TICK_TIME;
	}
	else
	{
		brzina=0.0;
	}
}

bool DebrisPickup::testCollision( Vec2f const& offset )
{
	ColInfo info;
	return getLevel()->getColManager().testCollision( info , offset , mBody  , COL_TERRAIN );
}

void DebrisPickup::onPick(Player* player)
{
	getLevel()->playSound("pickup.wav");		
	
	Explosion* e= getLevel()->createExplosion( getPos(),128 );
	e->setParam(12,100,50);
	e->setColor(Vec3(1.0, 0.75, 0.5));	

	player->addHP(2);

	destroy();
}