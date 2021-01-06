#ifndef GRIDMAKER_H
#define GRIDMAKER_H

#include <vector>
#include <SFML/Graphics.hpp>

template<class T>
sf::IntRect makeRect(const T& obj);

enum Orientation {
	HORIZONTAL,
	VERTICAL,
};


/**
 * @brief Вспомогательный класс.
 * @details Отрисовывает на экране сетку, хранит в себе параметры экрана.
 */
class GridMaker{
public:
    GridMaker() = delete;
    GridMaker(const GridMaker&) = delete;
    GridMaker& operator=(const GridMaker&) = delete;
    /**
     * @brief Конструктор.
     * @param window Окно игры.
     * @param sc Размер квадрата сетки.
     */
    explicit GridMaker(const sf::RenderTarget& window, const int sc);
    /**
     * @brief Обновляет сетку при изменении размера окна.
     * @param window Окно игры.
     */
    void updateGrid(const sf::RenderTarget& window);
    /**
     * @return Размер квадрата сетки.
     */
	int getScale() const { return scale; }
	/**
	 * @brief Отрисовывает сетку.
	 * @param window Окно игры.
	 */
	void drawGrid(sf::RenderWindow& window) const;

private:
    int scale;
    int window_width;
    int window_height;
    std::vector<sf::RectangleShape> makeRectangles(Orientation orientation);
	std::vector<sf::RectangleShape> hor_rectangles;
	std::vector<sf::RectangleShape> ver_rectangles;
	template<typename T>
	void drawObjects(sf::RenderWindow& window, const std::vector<T>& objects) const;
	std::vector<std::vector<int>> squares;

};

#endif