// Copyright (c) 2013 Bg Porter

#include "PluginBlockComponent.h"
#include "ComponentDefs.h"

PluginBlockComponent::PluginBlockComponent(TrackComponent::PluginColors* colors ,PluginBlock* plugins)
:  fColors(colors)
,  fPlugins(nullptr)
{
   if (nullptr != plugins)
   {
      this->ConnectToPluginBlock(plugins);
   }

}

PluginBlockComponent::~PluginBlockComponent()
{

}

void PluginBlockComponent::paint (Graphics& g)
{
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif
}


void PluginBlockComponent::resized()
{
#if 0
   // each connector line must be at least this many pixels long. 
   const int kMinConnectorSize = 4;
   const int kPluginCount = fPlugins->Size();
   const int kConnectorCount = kPluginCount + 1;

   const float kSlotHeight = this->getHeight() * kPluginSlotHeight;
   
   // The slot bubbles can take at MOST this many pixels.
   const int kAvailableWidth = this->getWidth() - (kMinConnectorSize * kConnectorCount);

   // Figure out how many total characters we're going to need to display. This 
   // is all very approximate -- we may end up needing to use the TextLayout
   // and AttributedString classes to make this work sensibly.
   const int kMinChars = 5;
   int totalChars = 0;

   Array<int> nameLengths;

   for (int i = 0; i < fPlugins->Size(); ++i)
   {
      int len = kMinChars;
      PluginInfo pi = fPlugins->PluginInSlot(i);
      if (pi.name != String::empty)
      {
         len = pi.name.length();
      }
      nameLengths.add(len);
      totalChars += len;
   }

   Array<float> percentages;
   for (int j = 0; j < fPlugins->Size(); ++j)
   {
      percentages.add(nameLengths.getUnchecked(j) /  static_cast<float>(totalChars));
   }


#else
   int totalRequestedWidth = 0;
   for (int i = 0; i < fSlots.size(); ++i)
   {
      PluginSlotComponent* slot = fSlots.getUnchecked(i);
      int preferredWidth = slot->GetPreferredWidth();
      totalRequestedWidth += preferredWidth;
      this->SetSlotBounds(i, slot);
   }
   #endif
}


void PluginBlockComponent::SetSlotBounds(int index, PluginSlotComponent* slot)
{
   int blockWidth = this->getWidth();
   int blockHeight = this->getHeight();
   float slotHeight = blockHeight * kPluginSlotHeight;
   float elementCount = fSlots.size() + 1.0;
   float slotWidth = blockWidth / elementCount;
   float margin = slotWidth / elementCount;

   int x = (index + 1) * margin + index * slotWidth;
   int y =  (blockHeight - slotHeight) / 2.0;
   slot->setBounds(x, y, slotWidth, slotHeight);


}

void PluginBlockComponent::ConnectToPluginBlock(PluginBlock* plugins)
{
   // only do anything if we're changing something.
   if (plugins != fPlugins)
   {
      if (fPlugins)
      {
         // disconnect.
         fPlugins->removeChangeListener(this);
      }
   }
   fPlugins = plugins;

   if (fPlugins)
   {
      // connect to our new source of data.
      fPlugins->addChangeListener(this);
      if (fPlugins->Size() != fSlots.size())
      {
         fSlots.clear();
         for (int i = 0; i < fPlugins->Size(); ++i)
         {
            PluginSlotComponent* slot = new PluginSlotComponent(fPlugins, i);
            fSlots.add(slot);
            this->addAndMakeVisible(slot);
            //this->SetSlotBounds(i, slot);
         }
      }
   }
}

void PluginBlockComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (source == fPlugins)
   {
      this->repaint();
   }

}