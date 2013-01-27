// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginSlotComponent
#define h_PluginSlotComponent

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"


class PluginSlotComponent : public Component 
{
public:
   PluginSlotComponent(PluginBlock* block, int index);
   ~PluginSlotComponent();

   void paint (Graphics& g);
   void resized();   
   void mouseDown(const MouseEvent& e);

private:
   PluginBlock*   fPluginBlock;
   int            fIndex;
};


#endif // this must be the last line in this file.