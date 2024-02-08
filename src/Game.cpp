#include "Game.h"

#include "Component.h"
#include "Entity.h"

Game::Game()
{
    window.setFramerateLimit(constants::kFramerateLimit);
}

void Game::CreateGameComponents()
{
    create_paddle();
    create_ball();

    for (int iX{0}; iX < constants::kCountBlocksX; ++iX)
    {
        for (int iY{0}; iY < constants::kCountBlocksY; ++iY)
        {
            create_brick(sf::Vector2f{
                static_cast<float>(iX + 1) * (constants::kBlockWidth + 3) +
                    constants::kBlocksInsetX,
                static_cast<float>(iY + 2) * (constants::kBlockHeight + 3)});
        }
    }
}

Entity &Game::create_ball()
{
    auto &entity(_manager.add_entity());

    entity.add_component<PositionComponent>(
        sf::Vector2f{constants::kWindowWidth / static_cast<float>(2),
                     constants::kWindowHeight / static_cast<float>(2)});
    entity.add_component<PhysicsComponent>(
        sf::Vector2f{constants::kBallRadius, constants::kBallRadius});
    entity.add_component<CircleComponent>(this, constants::kBallRadius);

    auto &physics_component(entity.get_component<PhysicsComponent>());
    physics_component._velocity =
        sf::Vector2f{-constants::kBallVelocity, -constants::kBallVelocity};
    physics_component.on_out_of_bounds =
        [&physics_component](const sf::Vector2f &side) {
            if (side.x != 0.F)
            {
                physics_component._velocity.x =
                    std::abs(physics_component._velocity.x) * side.x;
            }
            if (side.y != 0.F)
            {
                physics_component._velocity.y =
                    std::abs(physics_component._velocity.y) * side.y;
            }
        };

    entity.add_group(ArkanoidGroup::GBall);

    return entity;
}

Entity &Game::create_brick(const sf::Vector2f &position)
{
    sf::Vector2f half_size{constants::kBlockWidth / static_cast<float>(2),
                           constants::kBlockHeight / static_cast<float>(2)};
    auto &entity(_manager.add_entity());

    entity.add_component<PositionComponent>(position);
    entity.add_component<PhysicsComponent>(half_size);
    entity.add_component<RectangleComponent>(this,
                                             sf::Color::Yellow,
                                             half_size);

    entity.add_group(ArkanoidGroup::GBrick);

    return entity;
}

Entity &Game::create_paddle()
{
    sf::Vector2f half_size{constants::kPaddleWidth / static_cast<float>(2),
                           constants::kPaddleHeight / static_cast<float>(2)};
    auto &entity(_manager.add_entity());

    entity.add_component<PositionComponent>(
        sf::Vector2f{constants::kWindowWidth / static_cast<float>(2),
                     constants::kWindowHeight - constants::kPaddleInsetBottom});
    entity.add_component<PhysicsComponent>(half_size);
    entity.add_component<RectangleComponent>(this, half_size);
    entity.add_component<PaddleControlComponent>();

    entity.add_group(ArkanoidGroup::GPaddle);

    return entity;
}

void Game::run()
{
    running = true;
    while (window.isOpen())
    {
        auto time_point1(std::chrono::high_resolution_clock::now());

        window.clear(sf::Color::Black);
        input_phase();
        update_phase();
        draw_phase();

        auto time_point2(std::chrono::high_resolution_clock::now());
        auto elapsed_time(time_point2 - time_point1);
        const FrameTime frame_time{
            std::chrono::duration_cast<
                std::chrono::duration<float, std::milli>>(elapsed_time)
                .count()};
        last_frame_time = frame_time;
        constexpr float kMillisecondsPerSecond{1000.F};
        auto frame_time_seconds(frame_time / kMillisecondsPerSecond);
        auto fps(1.F / frame_time_seconds);

        window.setTitle("Arkanoid Clone");
    }
}

void Game::input_phase()
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
}

void Game::update_phase()
{
    current_slice += last_frame_time;

    for (; current_slice >= constants::kFrameTimeSlice;
         current_slice -= constants::kFrameTimeSlice)
    {
        _manager.refresh();
        _manager.update(constants::kFrameTimestep);

        auto &paddles(_manager.get_entities_by_group(GPaddle));
        auto &bricks(_manager.get_entities_by_group(GBrick));
        auto &balls(_manager.get_entities_by_group(GBall));

        for (auto &ball : balls)
        {
            for (auto &paddle : paddles)
            {

                handle_paddle_ball_collision(*paddle, *ball);
            }
            for (auto &brick : bricks)
            {

                handle_brick_ball_collision(*brick, *ball);
            }
        }
    }
}

void Game::draw_phase()
{
    _manager.draw();
    window.display();
}

void Game::render(const sf::Drawable &drawable)
{
    window.draw(drawable);
}
