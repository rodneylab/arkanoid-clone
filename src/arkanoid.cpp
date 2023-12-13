#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <variant>

#include "Ball.h"

constexpr unsigned int kWINDOW_WIDTH{800U}, kWINDOW_HEIGHT{600U},
    kFRAMERATE_LIMIT{60U};

int main()
{
    const Ball ball{static_cast<int>(kWINDOW_WIDTH / 2),
                    static_cast<int>(kWINDOW_HEIGHT / 2)};

    sf::RenderWindow window{{kWINDOW_WIDTH, kWINDOW_HEIGHT}, "Araknoid - 1"};
    window.setFramerateLimit(kFRAMERATE_LIMIT);

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
        window.draw(ball.shape);
        window.display();
    }
    return 0;
}
