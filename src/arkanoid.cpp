#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <variant>

#include "Ball.h"
#include "constants.h"

int main()
{
    Ball ball{static_cast<int>(constants::kWindowWidth / 2),
              static_cast<int>(constants::kWindowHeight / 2)};

    sf::RenderWindow window{{constants::kWindowWidth, constants::kWindowHeight},
                            "Araknoid - 1"};
    window.setFramerateLimit(constants::kFramerateLimit);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    window.close();
                }
                break;

            default:
                break;
            }
        }


        window.clear(sf::Color::Black);
        ball.update();
        window.draw(ball.shape);
        window.display();
    }
    return 0;
}
