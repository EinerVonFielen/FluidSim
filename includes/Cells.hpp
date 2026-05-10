#include "Cell.hpp"
#include "glm/glm.hpp"

using glm::vec2;

class Cells{

    public:

    vec2 size;
    Cell* cells;
    int cellcount;

    Cells(vec2 size);
    ~Cells();
};