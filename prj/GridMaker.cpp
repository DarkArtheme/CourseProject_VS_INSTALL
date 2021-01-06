#include "GridMaker.h"



template<class T>
sf::IntRect makeRect(const T& obj){
    return sf::IntRect(obj.rect);
    //return sf::IntRect(obj.rect.left, obj.rect.top, obj.rect.width, obj.rect.height);
}






std::vector<sf::RectangleShape> GridMaker::makeRectangles(Orientation orientation) {
	switch (orientation) {
	case Orientation::HORIZONTAL: {
		std::vector<sf::RectangleShape> hor_rectangles(window_height / scale);
		int y = scale;
		for (auto& rect : hor_rectangles) {
			rect.setPosition(0, y);
			y += scale;
			rect.setFillColor(sf::Color::Black);
			rect.setSize({ (float)window_width, 1 });
		}
		return std::move(hor_rectangles);
	}
	case Orientation::VERTICAL: {
		std::vector<sf::RectangleShape> ver_rectangles(window_width / scale);
		int x = scale;
		for (auto& rect : ver_rectangles) {
			rect.setPosition(x, 0);
			x += scale;
			rect.setFillColor(sf::Color::Black);
			rect.setSize({ 1, (float)window_height });
		}
		return std::move(ver_rectangles);
	}
	default:
		break;
	}
}

template<typename T>
void GridMaker::drawObjects(sf::RenderWindow& window, const std::vector<T>& objects) const {
	for (const auto& object : objects) {
		window.draw(object);
	}
}

void GridMaker::drawGrid(sf::RenderWindow& window) const{
	drawObjects(window, hor_rectangles);
	drawObjects(window, ver_rectangles);
}

GridMaker::GridMaker(const sf::RenderTarget &window, const int sc) : scale(sc), window_width(window.getSize().x), window_height(window.getSize().y){
    hor_rectangles = makeRectangles(Orientation::HORIZONTAL);
    ver_rectangles = makeRectangles(Orientation::VERTICAL);
    squares = std::vector<std::vector<int>>(window_height / scale, std::vector<int>(window_width / window_width));
}

void GridMaker::updateGrid(const sf::RenderTarget &window) {
    window_width = window.getSize().x;
    window_height = window.getSize().y;
    hor_rectangles = makeRectangles(Orientation::HORIZONTAL);
    ver_rectangles = makeRectangles(Orientation::VERTICAL);
    squares = std::vector<std::vector<int>>(window_height / scale, std::vector<int>(window_width / window_width));
}
