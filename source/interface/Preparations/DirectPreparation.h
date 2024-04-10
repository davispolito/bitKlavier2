//
// Created by Davis Polito on 2/13/24.
//

#ifndef BITKLAVIER2_DIRECTPREPARATION_H
#define BITKLAVIER2_DIRECTPREPARATION_H
#include "PreparationSection.h"
class DirectPreparation : public PreparationSection
{
public:
    DirectPreparation( ValueTree v, UndoManager &um);
    ~DirectPreparation();

    static PreparationSection* createDirectSection( ValueTree v,  UndoManager &um)
    {
        return new DirectPreparation(v, um);
    }
private:

};

#endif //BITKLAVIER2_DIRECTPREPARATION_H
