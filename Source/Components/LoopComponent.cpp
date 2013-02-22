// Copyright (c) 2013 Bg Porter

#include "LoopComponent.h"


LoopComponent::LoopComponent(LoopProcessor* loop)
:  fLoop(loop)
,  fFeedback(nullptr)
{
   fFeedback = new Slider("feedback");
   fFeedback->setTooltip ("Output volume");
   fFeedback->setRange (-96.0, 0.0, 0);
   fFeedback->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
   fFeedback->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
   fFeedback->setColour (Slider::thumbColourId, Colours::black);
   fFeedback->setColour (Slider::rotarySliderFillColourId, Colour (0x7f000000));
   fFeedback->setPopupDisplayEnabled(true, this);
   fFeedback->setTextValueSuffix("dB");
   fFeedback->addListener(this);     
   this->addAndMakeVisible(fFeedback);

}

LoopComponent::~LoopComponent()
{
   this->deleteAllChildren();
   
}

void LoopComponent::ConnectToLoop(LoopProcessor* loop)
{
   if (loop != fLoop)
   {
      if (fLoop)
      {
         fLoop->removeChangeListener(this);
      }
      fLoop = loop;
      if (fLoop)
      {
         fLoop->addChangeListener(this);
      }
   }
   
} 

void LoopComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (source == fLoop)
   {
      this->repaint();
   }
  
   
}


void LoopComponent::sliderValueChanged(Slider* slider)
{
   
}

void LoopComponent::resized()
{
   
}

void LoopComponent::paint(Graphics& g)
{
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif   
}