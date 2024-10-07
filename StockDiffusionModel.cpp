#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>

// Libraries for Particle simulation
//#include "./lib/Air.h"
//#include "./lib/Particle.h"

// Libraries for Stock simulation
#include "./lib/PriceSimulation.h"

#include <SFML/Graphics.hpp>

void drawBag(sf::RenderWindow& window,
    float lineWidth,
    float edge,
    float height,
    float width,
    sf::Color bagColor)
{
    sf::RectangleShape  left(sf::Vector2f(lineWidth, height));
    left.setFillColor(bagColor);
    left.setPosition(edge, edge);

    sf::RectangleShape  right(sf::Vector2f(lineWidth, height));
    right.setFillColor(bagColor);
    right.setPosition(edge + width, edge);

    sf::RectangleShape  base(sf::Vector2f(width + lineWidth, lineWidth));
    base.setFillColor(bagColor);
    base.setPosition(edge, edge + height);

    window.draw(left);
    // Add below for particle simulation
    //window.draw(right);
    window.draw(base);
}

// Particle Simulations
//std::vector<Particle> createParticles(size_t count,
//    float start_x,
//    float start_y,
//    float lineWidth,
//    float edge,
//    float height,
//    float width,
//    bool breakout)
//{
//    std::vector<Particle> particles;
//    for (size_t i = 0; i < count; ++i)
//    {
//        particles.emplace_back(
//            start_x,
//            start_y,
//            edge + lineWidth,
//            edge / 2 + width - 2 * lineWidth,
//            edge / 2 + height - 2 * lineWidth,
//            breakout
//        );
//    }
//    return particles;
//}
//
//void action(size_t count, float step, bool breakout)
//{
//    std::stringstream title;
//    title << "2D Brownian motion " << count << ", breakout " << breakout;
//
//    const float height = 500.0f;
//    const float width = 500.0f;
//    const float edge = 50.0f;
//    const float lineWidth = 5.0f;
//    const auto bagColor = sf::Color(180, 120, 60);
//
//    int max_x = static_cast<int>(width + edge);
//    int max_y = static_cast<int>(height + edge);
//    sf::RenderWindow window(sf::VideoMode(max_x, max_y),
//        title.str());
//
//    std::vector<Particle> particles =
//        createParticles(count, max_x / 2.0f, max_y / 2.0f,
//            lineWidth, edge,
//            height, width, breakout);
//
//    std::random_device rd;
//    Air air(step, rd());
//
//    bool paused = false;
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            if (event.type == sf::Event::KeyPressed)
//                paused = !paused;
//        }
//
//        window.clear();
//
//        drawBag(window, lineWidth, edge / 2, height, width, bagColor);
//
//        sf::CircleShape shape(lineWidth);
//        shape.setFillColor(sf::Color::Green);
//        for (auto& particle : particles)
//        {
//            if (!paused)
//                particle.Move(air.Bump(), air.Bump());
//            shape.setPosition(particle.X(), particle.Y());
//            window.draw(shape);
//        }
//        window.display();
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }
//}

void action_stock(const std::vector<std::vector<sf::Vertex>>& sims, float time, float height, std::string title) {
    const float edge = 30.0f;
    const float lineWidth = 5.0f;
    const float width = 500.0f;
    const float x_scale = width / time;
    const auto bagColor = sf::Color::White;

    sf::RenderWindow window(sf::VideoMode(static_cast<int>(width + 2 * edge), static_cast<int>(height + 2 * edge)), title);
    size_t last = 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            break;
        }

        window.clear();
        drawBag(window, lineWidth, edge, height, width, bagColor);

        last = std::min(++last, sims.begin()->size() - 1);
        for (const auto& points : sims) {
            bool out = false;
            for (size_t i = 0; i < last; i++) {
                auto scaled_start = sf::Vertex(
                    sf::Vector2f(points[i].position.x * x_scale + edge, height - points[i].position.y), sf::Color::White);
                auto scaled_point = sf::Vertex(
                    sf::Vector2f(points[i + 1].position.x * x_scale + edge, height - points[i + 1].position.y), sf::Color::White);
                sf::Vertex line[] = { scaled_start, scaled_point };
                window.draw(line, 2, sf::Lines);
            }
            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

std::vector<sf::Vertex> price_demo(unsigned int seed, double drift, double vol, double time, double dt, double jump, double mean_jump_per_unit_time) {
    const double start_price = 50.0;
    Diffuse::PriceSimulation price(start_price, drift, vol, dt, seed, jump, mean_jump_per_unit_time);
    
    std::vector<sf::Vertex> points;
    const int count = static_cast<int>(time / dt);
    points.push_back(sf::Vector2f(0.0f, static_cast<float>(start_price)));
    for (int i = 1; i < count; i++) {
        auto point = sf::Vector2f(static_cast<float>(i * dt), static_cast<float>(price.Next()));
        points.push_back(point);
    }
    return points;
}

int main(int argc, char** argv)
{
    // For Particle Simluation
    /*int particles = 100;
    bool breakout = true;
    const float step = 7.5f;
    action(particles, step, breakout);*/

    int sims = 5;
    double dt = 0.5;
    double drift = 0.2;
    double vol = 0.3;
    double jump = 0.0;
    double mean_jump_per_unit_time = 0.1;

    std::vector<std::vector<sf::Vertex>> allPoints;
    float time = 4.0f;
    float final_height = 0.0;
    int out = 0;
    const float height = 400.0f;
    using std::chrono::high_resolution_clock;
    for (int i = 0; i < sims; ++i)
    {
        unsigned int seed = static_cast<unsigned int>(high_resolution_clock::now().time_since_epoch().count()+i);
        auto sim = price_demo(seed, drift, vol, time, dt, jump, mean_jump_per_unit_time);
        allPoints.push_back(sim);
        auto price = sim.back().position.y;
        final_height += price;
        if (price > height) {
            ++out;
        }
    }
    std::cout << "average " << final_height / sims << "\nnumber out " << out << '\n';;

    std::stringstream title;
    title << "Stock prices. sims = " << sims;
    action_stock(allPoints, time, height, title.str());
}
