#include "LevelMaker.h"


LevelMaker::LevelMaker(const std::string& path, const std::string& filename
    , const int level_num, const std::string& extension, const double scalex, const double scaley)
    : scale_x(scalex), scale_y(scaley){
    try {
        tmxl.LoadFromFile(path + filename+ std::to_string(level_num) + extension);
    } catch (std::runtime_error& rnt){
        std::cerr << rnt.what() << std::endl;
    }

    for(const auto& x : tmxl.GetAllObjects("path")){
        path_.push_back(x);
    }
    for(const auto& x : tmxl.GetAllObjects("tower")){
        towers_.push_back(x);
    }
    sort(path_.begin(), path_.end(), [](const TmxObject& lhs, const TmxObject& rhs){ return lhs.name < rhs.name; });
    sort(towers_.begin(), towers_.end(), [](const TmxObject& lhs, const TmxObject& rhs){ return lhs.name < rhs.name; });
    start_ = tmxl.GetAllObjects("start")[0];
    finish_ = tmxl.GetAllObjects("finish")[0];
}
