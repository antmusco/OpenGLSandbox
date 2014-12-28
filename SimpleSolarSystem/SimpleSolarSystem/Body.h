#ifndef BODY_H
#define BODY_H

#include <glm\glm.hpp>
#include <string>
#include <vector>

class Body
{
public:
	Body(const std::string name, int xPos, int yPos, int xVel, int yVel, float mass, float radius);
	~Body();
	void calculateForce(std::vector<Body> bodies);
	void move(float delta_t);
	void draw();
	inline std::string& getName() { return name; }
	inline glm::vec2& getPosition() { return position; }
	inline glm::vec2& getVelocity() { return velocity; }
	inline glm::vec2& getForce() { return force; }
	inline float getMass() { return mass; }
	inline float getRadius() { return radius; }
private:
/* Private members. */
	std::string name;
	glm::vec2	position;
	glm::vec2	velocity;
	glm::vec2	force;
	float		mass;
	float		radius;
};

#endif
