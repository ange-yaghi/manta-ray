#ifndef DELTA_BASIC_RIGID_BODY_SYSTEM_H
#define DELTA_BASIC_RIGID_BODY_SYSTEM_H

#include "delta_core.h"

#include "collision_object.h"
#include "rigid_body.h"
#include "collision_detector.h"
#include "rigid_body_link.h"
#include "grid_partition_system.h"

#include <Windows.h>
#include <fstream>

namespace dphysics {

    class DeltaEngine;
    class RigidBodySystem : public ysObject {
    public:
        static const int ResolutionIterationLimit = 1024;
        static float ResolutionPenetrationEpsilon;

        struct CollisionGenerationCallData {
            RigidBodySystem *System;
            int Start;
            int Count;
            int ThreadID;
        };

    public:
        RigidBodySystem();
        ~RigidBodySystem();

        // Collision Detector Interface
        CollisionDetector CollisionDetector;

        void InitializeFrictionTable(
            int materialCount, float defaultStaticFriction, float defaultDynamicFriction);
        float GetDynamicFriction(int material1, int material2);
        float GetStaticFriction(int material1, int material2);
        void SetFriction(int material1, int material2, float staticFriction, float dynamicFriction);

        void RegisterRigidBody(RigidBody *body);
        void RemoveRigidBody(RigidBody *body);

        void Update(float timeStep);

        template<typename T_Link>
        T_Link *CreateLink(RigidBody *body1, RigidBody *body2) {
            T_Link *newLink = m_rigidBodyLinks.NewGeneric<T_Link, 16>();
            newLink->SetRigidBodies(body1, body2);
            return newLink;
        }

        bool CheckState();

        void DeleteLink(RigidBodyLink *link);

        void ProcessGridCell(int x, int y);

        void OpenReplayFile(const std::string &fname);
        void CloseReplayFile();

    protected:
        bool CollisionExists(Collision *collision);

        void GenerateCollisions();
        void InitializeCollisions();
        void CleanCollisions();
        void ClearCollisions();
        void GenerateCollisions(RigidBody *body1, RigidBody *body2);

        void ResolveCollisions(float dt);
        void ResolveCollision(Collision *collision, ysVector *velocityChange, ysVector *rotationDirection, float rotationAmount[2], float penetration);

        void AdjustVelocities(float timestep);
        void AdjustVelocity(Collision *collision, ysVector velocityChange[2], ysVector rotationChange[2]);

        void GenerateForces(float timeStep);
        void Integrate(float timeStep);
        void UpdateDerivedData();
        void CheckAwake();

        void OrderPrimitives(CollisionObject **prim1, CollisionObject **prim2, RigidBody **body1, RigidBody **body2);

        void GenerateCollisions(int start, int count);

        void WriteFrameToReplayFile();

        void AttachBreakdownTimer(ysBreakdownTimer *breakdownTimer) { m_breakdownTimer = breakdownTimer; }

    protected:
        ysRegistry<RigidBody, 512> m_rigidBodyRegistry;

        ysDynamicArray<RigidBodyLink, 512> m_rigidBodyLinks;

        ysDynamicArray<Collision, 4> m_dynamicCollisions;
        ysExpandingArray<Collision *, 8192> m_collisionAccumulator;

        std::vector<std::vector<float>> m_dynamicFrictionTable;
        std::vector<std::vector<float>> m_staticFrictionTable;

        float m_defaultDynamicFriction;
        float m_defaultStaticFriction;

        int m_lastLoadMeasurement;
        int m_loadMeasurement;
        float m_currentStep;

        ysBreakdownTimer *m_breakdownTimer;

        // TEST
        GridPartitionSystem m_gridPartitionSystem;
        std::ofstream m_loggingOutput;

    protected:
        // Debug
        std::fstream m_outputFile;
        bool m_replayEnabled;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_RIGID_BODY_SYSTEM_H */
