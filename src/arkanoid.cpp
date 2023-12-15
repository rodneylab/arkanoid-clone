#include <SFML/Window.hpp>

#include <variant>

#include "Ball.h"
#include "Paddle.h"

int main()
{
    Ball ball{static_cast<int>(constants::kWindowWidth / 2),
              static_cast<int>(constants::kWindowHeight / 2)};
    Paddle paddle{static_cast<int>(constants::kWindowWidth / 2),
                  constants::kWindowHeight - constants::kPaddleInsetBottom};
    sf::RenderWindow window{{constants::kWindowWidth, constants::kWindowHeight},
                            "Arkanoid - 1"};
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
        paddle.update();

        window.draw(ball.shape);
        window.draw(paddle.shape);
        window.display();
    }
    return 0;
}
