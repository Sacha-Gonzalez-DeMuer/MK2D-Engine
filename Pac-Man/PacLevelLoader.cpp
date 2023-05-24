#include "PacLevelLoader.h"
#include "GridGraph.h"

namespace dae
{
    GridGraph* PacLevelLoader::LoadLevel(const PacLevelData& data)
    {
        int columns = static_cast<int>(data[0].size());
        int rows = static_cast<int>(data.size());
        GridGraph* grid = new GridGraph(columns, rows, 16, false, false);


    }
}
