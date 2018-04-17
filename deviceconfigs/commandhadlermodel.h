#ifndef COMANDHADLERMODEL_H
#define COMANDHADLERMODEL_H
#include "utility/singleton.hpp"

#define CH_MODEL singleton<CommandHadlerModel>()

class CommandHadlerModel {
    friend CommandHadlerModel& singleton<CommandHadlerModel>();

//ToDo: Не думаю, что так нужно делать. Мб переделать на friend class?
public:
    bool addItem(int type, int id);
    bool deleteItem();
    bool refreshItem();    
    
private:
    CommandHadlerModel();
     
};

#endif // COMANDHADLERMODEL_H
