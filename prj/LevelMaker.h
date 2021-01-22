#pragma once
#ifndef LEVELMAKER_H
#define LEVELMAKER_H
#include "TmxLevel.h"
//#include "GridMaker.h"
#include <iostream>
#include <algorithm>


///@brief Класс загрузки и отрисовки уровня
class LevelMaker {
public:
    LevelMaker() = delete;
    LevelMaker(const LevelMaker&) = delete;
    LevelMaker& operator= (const LevelMaker&) = delete;
    /**
     * @brief Конструктор.
     * @param path Путь к директории с уровнем.
     * @param filename Название файла с уровнем (без номера).
     * @param level_num Номер уровня.
     * @param extension Расширение файла уровня.
     * @param scalex Коэффициент масштабирования по X.
     * @param scaley Коэффициент масштабирования по Y.
     */
    LevelMaker(const std::string& path, const std::string& filename, int level_num
    , const std::string& extension, double scalex, double scaley);
    ~LevelMaker() = default;
    /**
     * @brief Отрисовка уровня.
     * @param target Окно для отрисовки.
     */
    void drawLevel(sf::RenderTarget& target) { tmxl.Draw(target, scale_x, scale_y); }
    /**
     * @return Область старта для врагов.
     */
    TmxObject getStart() const { return start_; }
    /**
     * @return Маршрут врагов.
     */
    const std::vector<TmxObject>& returnPath() const { return path_; }
    /**
     * @return Позиции застройки башен.
     */
    const std::vector<TmxObject>& returnTowers() const { return towers_; }
    /**
     * @return Конец маршрута врагов.
     */
    TmxObject getFinish() const { return finish_; }
private:
    const double scale_x;
    const double scale_y;
    TmxLevel tmxl;
    TmxObject start_;
    TmxObject finish_;
    std::vector<TmxObject> path_;
    std::vector<TmxObject> towers_;

};


#endif //LEVELMAKER_H
