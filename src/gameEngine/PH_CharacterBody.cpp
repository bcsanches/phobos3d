#include "PH_CharacterBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "PH_CollisionShape.h"

#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtils.h"
#include "PH_RigidBody.h"

#define GROUND_CHECK -0.025f
#define MIN_WALK_NORMAL 0.7f
#define	MAX_CLIP_PLANES	5
#define MAX_LOOPS 4
#define EPSILON 0.01f

#define OVERCLIP 1.001f;

namespace Phobos
{
	namespace Physics
	{
		CharacterBody_c::CharacterBody_c(RigidBodyPtr_t rigidBody, Float_t stepHeight):
			fpStepHeight(stepHeight * PhysicsManager_c::GetInstance()->GetScale()),
			spRigidBody(rigidBody),
			v3WalkDirection(0, 0, 0),
			fGroundPlane(false),
			fWalking(false)
		{
			//empty
		}

		CharacterBody_c::~CharacterBody_c()
		{
			this->Unregister();
		}

		void CharacterBody_c::Register()
		{
			spRigidBody->Register();
			PhysicsManager_c::GetInstance()->AddAction(*this);
		}
		
		void CharacterBody_c::Unregister()
		{
			PhysicsManager_c::GetInstance()->RemoveAction(*this);
			spRigidBody->Unregister();
		}

		void CharacterBody_c::SetWalkDirection(const Ogre::Vector3 &walkDirection)
		{
			v3WalkDirection = MakeVector3(walkDirection, PhysicsManager_c::GetInstance()->GetScale());
		}

		Ogre::Vector3 CharacterBody_c::GetPosition() const
		{
			return spRigidBody->GetTransform().GetOrigin();
		}

		void CharacterBody_c::Teleport(const Ogre::Vector3 &position)
		{			
			//spCharacterController->warp(MakeVector3(position, PhysicsManager_c::GetInstance()->GetScale()));
		}

		void CharacterBody_c::updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
		{
			const btTransform &fromTransform = spRigidBody->GetRigidBody().getWorldTransform();			

			//if not on ground
			this->GroundTrace(fromTransform.getOrigin());			

			//Apply gravity to movement
			btVector3 movement = v3WalkDirection;
			//movement += PhysicsManager_c::GetInstance()->GetPhysicsGravity() * deltaTimeStep;
			
			btTransform toTransform;
			this->Move(fromTransform, movement, toTransform, 1.0f);

			spRigidBody->SetKinematicTransform(toTransform);
		}

		void CharacterBody_c::debugDraw(btIDebugDraw* debugDrawer)
		{
			//empty
		}

		bool CharacterBody_c::GroundTrace(const btVector3 &position)
		{
			btTransform toTransform(btQuaternion::getIdentity(), position + btVector3(0, GROUND_CHECK, 0));
	
			SweepCollisionResult_s collisionResult;

			PhysicsManager_c::GetInstance()->ConvexSweepTest(
				collisionResult, 
				spRigidBody->GetRigidBody(),
				btTransform(btQuaternion::getIdentity(), position),
				toTransform
			);

			if(collisionResult.fHasHit)
			{
				fGroundPlane = true;
				v3GroundNormal = collisionResult.v3HitNormalWorld;

				fWalking = v3GroundNormal.y() >= MIN_WALK_NORMAL;				
			}
			else
			{
				fGroundPlane = false;
				fWalking = false;
			}	

			return collisionResult.fHasHit;
		}

		void ClipVelocity( btVector3 &out, const btVector3 &in, const btVector3 &normal, Float_t overbounce)
		{	
			/*
			Float_t	backoff;
			Float_t	change;		
			
			backoff = in.dotProduct(normal) * overbounce;

			if(overbounce != 1.0)
			{
				if(backoff < 0)
					backoff *= overbounce;
				else
					backoff /= overbounce;
			}*/
			
			Float_t	backoff = in.dot(normal) * overbounce;
			for( int i = 0; i < 3; ++i )
			{
				Float_t change = normal[i]*backoff;
				out[i] = in[i] - change;
				if (out[i] > -EPSILON && out[i] < EPSILON)
					out[i] = 0;
			}
		}

		void CharacterBody_c::Move(const btTransform &originalfromTransform, const btVector3 &linearVel, btTransform &toTransform, Float_t timeStep)
		{
			btVector3 colNormals[MAX_CLIP_PLANES];
			Float_t timeLeft = timeStep;
		
			int numplanes = 0;

			Float_t len = linearVel.length();

			btVector3 velocity = linearVel;

			PhysicsManagerPtr_t physicsManager = PhysicsManager_c::GetInstance();
				
			if(fGroundPlane)
			{
				btVector3 v;
				ClipVelocity(v, velocity, v3GroundNormal, 1.0f);
				velocity = v;

				numplanes = 1;
				colNormals[0] = v3GroundNormal;
			}	

			btTransform fromTransform = originalfromTransform;

			for( int iloops = 0 ; iloops < MAX_LOOPS ; ++iloops )
			{
				if(velocity.isZero())	
				{
					toTransform = fromTransform;
					break;
				}

				//manager.IntegrateTransform(fromTransform, velocity, Vector3::ZERO, time_left, toTransform);
				toTransform.setOrigin(fromTransform.getOrigin() + (velocity * timeLeft));

				SweepCollisionResult_s cb;
				physicsManager->ConvexSweepTest(cb, spRigidBody->GetRigidBody(), fromTransform, toTransform);			
			
				if( !cb.fHasHit )
				{				
					break;		
				}
			
 				float ratio  = cb.fpFraction;
				float usedTime = timeLeft * ratio;

				//move the distance
				toTransform.setOrigin(fromTransform.getOrigin() + (velocity * usedTime));		
				timeLeft -= usedTime;
			
				//move it away a bit from the plane
				toTransform.setOrigin(toTransform.getOrigin() + (cb.v3HitNormalWorld * 0.01f));		
				fromTransform = toTransform;

				// cliped to another plane
				if (numplanes >= MAX_CLIP_PLANES)
				{
					// this shouldn't really happen				
					break;
				}

				colNormals[numplanes] = cb.v3HitNormalWorld;
				numplanes++;

				// modify original_velocity so it parallels all of the clip planes
				btVector3 new_vel;
				int i = 0;
				for( i = 0; i < numplanes; ++i )
				{
					ClipVelocity( new_vel, velocity, colNormals[i], 1.0f );
					int j = 0;
					for( j = 0; j < numplanes; ++j )
					{
						if (j != i)
						{
							if( new_vel.dot( colNormals[j] ) < 0.0f )
								break;	// not ok
						}
					}
					if( j == numplanes )
						break;
				}


				if (i != numplanes)
				{
					velocity = new_vel;
					velocity.normalize();
					velocity *= len;
				}
				else
				{
					if( numplanes != 2 )
					{					
						break;
					}
					btVector3 dir = colNormals[0].cross(colNormals[1]);
					dir.normalize();
					velocity = dir * len;
				}

				if( velocity.dot( linearVel ) <= 0.0f )
				{
					//heading in the opposite direction to original vel, just stop as this 								
					break;
				}		
			}	
		}
	}
}
