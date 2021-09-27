#include <SFML/Graphics.hpp>
#include <math.h>
#include <cstdlib>

struct Boid {
    sf::Vector2f position;
    sf::Vector2f speed;

    Boid() :
        position(0.0f, 0.0f),
        speed(0.0f, 0.0f)
    {
    }
};

sf::ConvexShape buildBoidShape() {
    sf::ConvexShape shape;
    
    shape.setPointCount(4);

    shape.setPoint(0, sf::Vector2f(3, 0));
    shape.setPoint(1, sf::Vector2f(-1, 2));
    shape.setPoint(2, sf::Vector2f(0, 0));
    shape.setPoint(3, sf::Vector2f(-1, -2));

    return shape;
}

void seedRandom() {
    srand(time(NULL));
}

float getRandom() {
    return (float)rand() / RAND_MAX;
}

float getRandom(float min, float max) {
    return min + (max - min) * getRandom();
}

float distance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    sf::Vector2f d = v2 - v1;

    return sqrtf(d.x * d.x + d.y * d.y);
}

void drawBoid(sf::RenderWindow& window,
              sf::ConvexShape& shape,
              const Boid& boid) {
    float angle = atan2f(boid.speed.y, boid.speed.x);

    shape.setRotation((angle / M_PI) * 180.0f);
    shape.setPosition(boid.position);

    window.draw(shape);
}

void initBoids(std::vector<Boid>& boids) {
    for (auto& boid : boids) {
        boid.speed.x = getRandom(-1.0f, 1.0f);
        boid.speed.y = getRandom(-1.0f, 1.0f);
        
        boid.position.x = getRandom(-100.0f, 100.0f);
        boid.position.y = getRandom(-100.0f, 100.0f);
    }
}

std::vector<const Boid*> getBoidNeighborhood(const std::vector<Boid>& allBoids,
                                       size_t boidIndex,
                                       float maxDistance) {
    std::vector<const Boid*> result;
    result.reserve(allBoids.size());
    
    const Boid& centerBoid = allBoids[boidIndex];
    
    for (size_t i = 0; i < allBoids.size(); i++) {
        const Boid& candidate = allBoids[i];
        if (i != boidIndex && distance(candidate.position, centerBoid.position) < maxDistance) {
            result.push_back(&candidate);
        }
    }

    return result;
}

sf::Vector2f computeCentroidForce(const Boid& myBoid,
                                  const std::vector<const Boid*>& neighbors) {
    if (neighbors.empty()) {
        return sf::Vector2f(0.0f, 0.0f);
    } else {
        sf::Vector2f neighborCentroid(0.0f, 0.0f);
        for (const Boid* neighbor : neighbors) {
            neighborCentroid += neighbor->position;
        }
        neighborCentroid /= (float)neighbors.size();
        
        return (neighborCentroid - myBoid.position);
    }
}

sf::Vector2f computeRepulsiveForce(const Boid& myBoid,
                                   const std::vector<const Boid*>& neighbors) {
    sf::Vector2f force;
    float distanceThreshold = 2.0f;
    
    for (const Boid* neighbor : neighbors) {
        if (distance(neighbor->position, myBoid.position) < distanceThreshold) {
            
        }
        neighborCentroid += neighbor->position;
    }

    return force;
}

sf::Vector2f computeAllForces(const std::vector<Boid>& boids, size_t boidIndex) {
    const Boid& myBoid = boids[boidIndex];
    std::vector<const Boid*> neighbors = getBoidNeighborhood(boids, boidIndex, 20.0f);

    sf::Vector2f totalForce(0.0f, 0.0f);

    totalForce += .1f * computeCentroidForce(myBoid, neighbors);
    
    return totalForce;
}

void updateBoids(std::vector<Boid>& boids, float dt) {
    std::vector<Boid*> neighborhood;
    neighborhood.reserve(boids.size());
    
    for (size_t i = 0; i < boids.size(); i++) {
        sf::Vector2f force = computeAllForces(boids, i);

        boids[i].speed += force * dt;
        boids[i].position += boids[i].speed * dt;
    }
}

int main()
{
    seedRandom();
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Boids boids boids");

    float areaSize = 100.0f;
    sf::View view(sf::FloatRect(-areaSize, -areaSize, 2.0f * areaSize, 2.0f * areaSize));

    window.setView(view);

    sf::ConvexShape boidShape = buildBoidShape();

    std::vector<Boid> boids(100);
    initBoids(boids);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        updateBoids(boids, .003f);

        window.clear();
        for (const auto& boid : boids) {
            drawBoid(window, boidShape, boid);
        }

        window.display();
    }

    return 0;
}
