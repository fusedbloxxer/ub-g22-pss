#ifndef __GRID_ANIMATION__
#define __GRID_ANIMATION__

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <format>
#include <vector>

#include "../components/scene_object.h"
#include "../search/problem.h"
#include "../maze/graph.h"
#include "../maze/cell.h"

struct GridCell final : public SceneObject
{
    GridCell(sf::Vector2f pos, sf::Color color, float size)
        : _pos(pos), _color(color), _size(size), _vertices(sf::TriangleStrip, 4)
    {
        this->setPosition(pos);
        this->setColor(color);
        this->setSize(size);
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        states.transform *= this->getTransform();

        states.texture = NULL;

        target.draw(_vertices, states);
    }

    virtual void update(sf::Time elapsed) override
    {
    }

    void setColor(const sf::Color &color)
    {
        this->_vertices[0].color = color;
        this->_vertices[1].color = color;
        this->_vertices[2].color = color;
        this->_vertices[3].color = color;
    }

    void setSize(const float size)
    {
        _vertices[0].position = sf::Vector2f(0.0f, 0.0f);
        _vertices[1].position = sf::Vector2f(size, 0.0f);
        _vertices[2].position = sf::Vector2f(0.0f, size);
        _vertices[3].position = sf::Vector2f(size, size);
    }

private:
    sf::VertexArray _vertices;
    sf::Vector2f _pos;
    const sf::Color _color;
    const size_t _size;
};

struct Grid final : public SceneObject
{
    std::vector<std::vector<GridCell>> cells;

    Grid(size_t rows, size_t cols, size_t size, size_t gap)
        : cells(rows, std::vector<GridCell>())
    {
        for (int i = 0; i != rows; ++i)
        {
            for (int j = 0; j != cols; ++j)
            {
                sf::Vector2f pos = sf::Vector2f(j * (size + gap), i * (size + gap));
                cells[i].push_back(GridCell(pos, sf::Color::White, size));
            }
        }
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        states.transform *= this->getTransform();

        states.texture = NULL;

        for (int i = 0; i != cells.size(); ++i)
        {
            for (int j = 0; j != cells[i].size(); ++j)
            {
                target.draw(cells[i][j], states);
            }
        }
    }

    virtual void update(sf::Time elapsed) override
    {
    }

    void update(const std::vector<std::vector<Cell>> &cells)
    {
        for (int i = 0; i != cells.size(); ++i)
        {
            for (int j = 0; j != cells[i].size(); ++j)
            {
                this->cells[i][j].setColor(cellToColor(cells[i][j]));
            }
        }
    }

    void update(sf::Vector2u screenSize)
    {
        float cellsHeight = screenSize.y / this->cells.size();
        float cellsWidth = screenSize.x / this->cells[0].size();
        float cellSize = std::min(cellsWidth, cellsHeight);

        float size = cellSize * 0.9;
        float gap = cellSize * 0.1;

        for (int i = 0; i != cells.size(); ++i)
        {
            for (int j = 0; j != cells[i].size(); ++j)
            {
                sf::Vector2f pos = sf::Vector2f(j * (size + gap), i * (size + gap));
                this->cells[i][j].setPosition(pos);
                this->cells[i][j].setSize(size);
            }
        }
    }

    inline static sf::Color cellToColor(Cell::CellType cell)
    {
        switch (cell)
        {
        case Cell::CellType::Empty:
            return sf::Color::White;
        case Cell::CellType::Door:
            return sf::Color::Magenta;
        case Cell::CellType::Explore:
            return sf::Color::Yellow;
        case Cell::CellType::Player:
            return sf::Color::Green;
        case Cell::CellType::Visited:
            return sf::Color::Cyan;
        case Cell::CellType::Wall:
            return sf::Color::Blue;
        default:
            throw std::runtime_error(std::format("Invalid cell type: {}", static_cast<char>(cell)));
        }
    }
};

class GridAnimation final : public SceneObject
{
    using History = std::vector<std::vector<std::vector<Cell>>>;

public:
    GridAnimation(const History &history, const DetailsMap &details, std::shared_ptr<Node> solution)
        : _history(history), _details(details), _solution(solution), _grid(_history[0].size(), _history[0][0].size(), 50, 5)
    {
        this->setAnimationDuration(sf::seconds(10));
        this->setEndingDuration(sf::seconds(10));
        this->resetState();
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        states.transform *= this->getTransform();

        states.texture = NULL;

        target.draw(_grid, states);
    }

    virtual void update(sf::Time delta) override
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
        {
            if (this->isPausedFlag == false)
            {
                this->isPaused = !this->isPaused;
                this->isPausedFlag = true;
            }
        }
        else
        {
            this->isPausedFlag = false;
        }

        if (this->isPaused)
        {
            return;
        }

        this->elapsedState += delta.asMicroseconds();

        _grid.update(sf::Vector2u(1920, 1080));

        if (this->elapsedState >= this->stepDuration.asMicroseconds())
        {
            this->elapsedState = 0;

            if (this->remainingEnd <= 0)
            {
                this->index = (this->index + 1) % this->_history.size();
                this->_grid.update(this->_history[this->index]);
            }

            if (this->index == this->_history.size() - 1)
            {
                if (this->remainingEnd == 0)
                {
                    this->remainingEnd = this->endingDuration.asMicroseconds();
                }

                int len = 0;

                _solution->rchain([&](const Node &node)
                {
                    auto&& [i, j] = boost::get(_details, node.state.vertex).coords;
                    this->_grid.cells[i][j].setColor(sf::Color::Red);
                    ++len;
                });

                std::cout << "Steps: " << _history.size() << std::endl;
                std::cout << "Solution Length: " << len << std::endl;
            }
        }

        if (this->remainingEnd != 0)
        {
            this->remainingEnd = std::max(0.0f, this->remainingEnd - delta.asMicroseconds());

            std::cout << "Pausing for: " << this->remainingEnd << "us" << std::endl;
        }
    }

    void resetState()
    {
        this->elapsedState = 0;
        this->remainingEnd = 0;
        this->index = 0;
    }

    void setAnimationDuration(sf::Time time)
    {
        this->stepDuration = sf::microseconds(time.asMicroseconds() / this->_history.size());
    }

    void setEndingDuration(sf::Time time)
    {
        this->endingDuration = time;
    }

private:
    const std::vector<std::vector<std::vector<Cell>>> &_history;
    std::shared_ptr<Node> _solution;
    const DetailsMap &_details;

    sf::Time endingDuration;
    sf::Time stepDuration;

    bool isPausedFlag = false;
    bool isPaused = false;
    float elapsedState;
    float remainingEnd;

    Grid _grid;
    int index;
};

#endif