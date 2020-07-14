//------------------------------------------------------------------------------	
// 2.
//
// You are implementing simple 'waypoint' following for an AI hover drone
// in a 3D environment. The drone will consider a destination waypoint 
// acceptable if it is both:
//    i. Less than 10 units away.
//    ii. Inside a 60 degree cone centered around the drone's 'forward' vector.
//
// Implement the function testWayPoint() shown below. If the waypoint is not
// acceptable the method should return REJECTED, otherwise it should return the
// side that the waypoint is on.  This is defined with respect to the drone's
// forward vector and the world up vector (you may choose either 'handedness').

//----

// Assume the following functions and structures are defined elsewhere
// DO NOT IMPLEMENT

#include <cmath>

struct Vector3
{

};

// Add v and w, putting the result in sum : sum = v + w
void add(const Vector3& v, const Vector3& w, Vector3& sum);

// Subtract w from v, putting the result in diff: diff = v - w
void subtract(const Vector3& v, const Vector3& w, Vector3& diff);

// Compute dot product of Vector3 v and w
float dot(const Vector3& v, const Vector3& w);

// Compute v X w (cross product): crossOut = v X w
void cross(const Vector3& v, const Vector3& w, Vector3& crossOut);

// Normalize the Vector3 v to unit length
void normalize(Vector3& v);

// Compute length of Vector3
float length(const Vector3& v);

enum Direction
{
	REJECTED = 0,
	RIGHT,
	LEFT
};

//----

// This is the function you must implement:
Direction testWayPoint(const Vector3& dronePosition, const Vector3& droneForward, const Vector3& targetWaypoint, const Vector3& worldUp)
{
    float maxDist = 10.f;
    float maxCone = 60.f * (3.14159265359f/180.f);

    // Check distance
    Vector3 pnt_vec;
    subtract(targetWaypoint, dronePosition, pnt_vec);
    if(length(pnt_vec) < maxDist) return REJECTED;

    // Check cone
    normalize(pnt_vec);
    float angle = acos(dot(pnt_vec, droneForward));
    if(angle > maxCone) return REJECTED;

    // Check side
    Vector3 rightVec;
    cross(droneForward, worldUp, rightVec);
    if(dot(pnt_vec, rightVec) < 0) return LEFT;
    else return RIGHT;
}
