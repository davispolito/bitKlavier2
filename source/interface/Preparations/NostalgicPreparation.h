//
// Created by Davis Polito on 4/22/24.
//

#ifndef BITKLAVIER2_NOSTALGICPREPARATION_H
#define BITKLAVIER2_NOSTALGICPREPARATION_H
#include "PreparationSection.h"
class NostalgicPreparation : public PreparationSection
{
public:
    NostalgicPreparation( ValueTree v, UndoManager &um);
    ~NostalgicPreparation();

    static PreparationSection* createNostalgicSection( ValueTree v,  UndoManager &um)
    {
        return new NostalgicPreparation(v, um);
    }
private:

};

#endif //BITKLAVIER2_NOSTALGICPREPARATION_H
