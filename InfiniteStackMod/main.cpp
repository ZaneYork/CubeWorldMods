#include "main.h"
#include "../cwmods/cwmods.h"
//#include "logger.h"
//using namespace LOGGER;
//CLogger* logger;

EXPORT int HandleCheckInventoryFull(cube::Creature* player, cube::Item *item) {
    //logger->TraceInfo("Player:%s, Item:%d\n", player->entity_data.name, item->id);
    // 1: full, 2: not full, 0: origin logic
    return 2;
}

//EXPORT void ModInitialize() {
//    logger = new CLogger(LogLevel_Info,CLogger::GetAppPathA());
//    logger->ChangeLogLevel(LOGGER::LogLevel_Error);
//}
