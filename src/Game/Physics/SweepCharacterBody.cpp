#include "Phobos/Game/Physics/SweepCharacterBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "Phobos/Game/Physics/CollisionShape.h"

#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/Conv.h"
#include "Phobos/Game/Physics/RigidBody.h"

#define GROUND_CHECK -0.1f
#define MIN_WALK_NORMAL 0.7f
#define	MAX_CLIP_PLANES	5
#define MAX_LOOPS 4
#define EPSILON 0.00001f

#define OVERCLIP 1.001f;

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			SweepCharacterBody::SweepCharacterBody(RigidBodyPtr_t rigidBody, Float_t stepHeight):
				m_fpStepHeight(stepHeight * Manager::GetInstance().GetScale()),
				m_spRigidBody(rigidBody),
				m_v3WalkDirection(0, 0, 0),
				m_fGroundPlane(false),
				m_fWalking(false)
			{
				//empty
			}

			SweepCharacterBody::~SweepCharacterBody()
			{
				this->Unregister();
			}

			void SweepCharacterBody::Register()
			{
				m_spRigidBody->Register();
				Manager::GetInstance().AddAction(*this);
			}
		
			void SweepCharacterBody::Unregister()
			{
				Manager::GetInstance().RemoveAction(*this);
				m_spRigidBody->Unregister();
			}

			void SweepCharacterBody::SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal)
			{
				m_v3WalkDirection = MakeVector3(velocity, Manager::GetInstance().GetScale());
			}

			Ogre::Vector3 SweepCharacterBody::GetPosition() const
			{
				return m_spRigidBody->GetTransform().GetOrigin();
			}

			void SweepCharacterBody::Teleport(const Ogre::Vector3 &position)
			{			
				//spCharacterController->warp(MakeVector3(position, Manager::GetInstance()->GetScale()));
			}

			void SweepCharacterBody::updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
			{
				const btTransform &fromTransform = m_spRigidBody->GetRigidBody().getWorldTransform();			

				//if not on ground
				this->GroundTrace(fromTransform.getOrigin());			

				//Apply gravity to movement
				btVector3 movement = m_v3WalkDirection;

				if(!m_fGroundPlane)
					movement += Manager::GetInstance().GetPhysicsGravity() * deltaTimeStep;
			
				btTransform toTransform;
				this->Move(fromTransform, movement, toTransform, 1.0f);

				m_spRigidBody->SetKinematicTransform(toTransform);
			}

			void SweepCharacterBody::debugDraw(btIDebugDraw* debugDrawer)
			{
				//empty
			}

			bool SweepCharacterBody::GroundTrace(const btVector3 &position)
			{
				btTransform toTransform(btQuaternion::getIdentity(), position + btVector3(0, GROUND_CHECK, 0));			
	
				SweepCollisionResult_s collisionResult;

				Manager::GetInstance().ConvexSweepTest(
					collisionResult, 
					m_spRigidBody->GetRigidBody(),
					btTransform(btQuaternion::getIdentity(), position),
					toTransform
				);

				if(collisionResult.m_fHasHit)
				{
					m_fGroundPlane = true;
					m_v3GroundNormal = collisionResult.m_v3HitNormalWorld;

					m_fWalking = m_v3GroundNormal.y() >= MIN_WALK_NORMAL;		

					btTransform toTransform(btQuaternion::getIdentity(), position + btVector3(0, GROUND_CHECK*0.1f, 0));
						Manager::GetInstance().ConvexSweepTest(
						collisionResult, 
						m_spRigidBody->GetRigidBody(),
						btTransform(btQuaternion::getIdentity(), position),
						toTransform
					);
				}
				else
				{
					m_fGroundPlane = false;
					m_fWalking = false;
				}	

				return collisionResult.m_fHasHit;
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

			void SweepCharacterBody::Move(const btTransform &originalfromTransform, const btVector3 &linearVel, btTransform &toTransform, Float_t timeStep)
			{
				btVector3 colNormals[MAX_CLIP_PLANES];
				Float_t timeLeft = timeStep;
		
				int numplanes = 0;

				Float_t len = linearVel.length();

				btVector3 velocity = linearVel;

				Physics::Manager &physicsManager = Manager::GetInstance();
				
				if(m_fGroundPlane)
				{
					btVector3 v;
					ClipVelocity(v, velocity, m_v3GroundNormal, 1.0f);
					velocity = v;

					numplanes = 1;
					colNormals[0] = m_v3GroundNormal;
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
					physicsManager.ConvexSweepTest(cb, m_spRigidBody->GetRigidBody(), fromTransform, toTransform);			
			
					if( !cb.m_fHasHit || (cb.m_fHasHit && cb.m_fpFraction == 0))
					{				
						if(cb.m_fHasHit)
							toTransform = fromTransform;
						break;		
					}
			
 					float ratio  = cb.m_fpFraction;
					float usedTime = timeLeft * ratio;

					//move the distance
					toTransform.setOrigin(fromTransform.getOrigin() + (velocity * usedTime));		
					timeLeft -= usedTime;
			
					//move it away a bit from the plane
					toTransform.setOrigin(toTransform.getOrigin() + (cb.m_v3HitNormalWorld * 0.01f));		
					fromTransform = toTransform;

					// cliped to another plane
					if (numplanes >= MAX_CLIP_PLANES)
					{
						// this shouldn't really happen				
						break;
					}

					colNormals[numplanes] = cb.m_v3HitNormalWorld;
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
}
