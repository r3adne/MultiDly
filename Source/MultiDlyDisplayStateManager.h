/*
  ==============================================================================

    MultiDlyDisplayStateManager.h
    Created: 16 Aug 2020 10:37:33pm
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "MultiDlyTap.h"

class MultiDlyAudioProcessor;


class MultiDlyDisplayStateManagerBase {};

template<class T, int C>
class MultiDlyDisplayStateManager : public MultiDlyDisplayStateManagerBase
{
public:
    MultiDlyDisplayStateManager();
    ~MultiDlyDisplayStateManager();

    bool AddNewTap();
    


private:




};
