/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  16 Dec 2012 10:55:55am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_SCUMBLERCOMPONENT_SCUMBLERCOMPONENT_A102A7FF__
#define __JUCER_HEADER_SCUMBLERCOMPONENT_SCUMBLERCOMPONENT_A102A7FF__

//[Headers]     -- You can add your own extra header files here --


#include "../Scumbler.h"

#include "../Commands.h"
#include "JuceHeader.h"
#include "StyledComponent.h"
#include "TrackComponent.h"
#include "TransportComponent.h"  

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
  @class ScumblerComponent

  We use a MVC design for the app here. The ScumblerComponent acts as the controller
  and the view, while the Scumbler itself functions as the model.
                                                                    //[/Comments]
*/
class ScumblerComponent  : public StyledComponent,
                           public ApplicationCommandTarget,
                           public ButtonListener,
                           public LabelListener, 
                           public Timer
{
public:
    //==============================================================================
    ScumblerComponent (UiStyle* style, Scumbler* scumbler);
    ~ScumblerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    /**
     * Add a new track to the scumbler object.
     * @return result code indicating success or reason for failure.
     */
    tk::Result AddTrack();

    /**
     * Set the specified track's bounds to the correct location, dependent on:
     * - current size of this component
     * - the number of tracks
     * - (future) whether that track is active??
     * @param index zero-based index of the track to set.
     * @param tc pointer to the TrackComponent to set th elocation of.
     */
    void SetTrackBounds(int index, TrackComponent* tc);

    /**
     * @name ApplicationCommandTarget overrides.
     */

    ///@{
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands(Array<CommandID>& commands);
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
    bool perform(const InvocationInfo& info);
    ///@}

    /**
      * @name ChangeListener overrides.
      */

    /**
     * Called when something we're watching calls us back with a notification.
     */
    void changeListenerCallback(ChangeBroadcaster* source);

    /**
     * Called when our timer interval elapses.
     */
    void timerCallback();

    void labelTextChanged(Label* source);

    bool keyPressed(const KeyPress& key);

    /**
     * Make any changes needed to reflect the current setting of our UiStyle object.
     */
    void UpdateStyle();

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Scumbler* fScumbler;
    OwnedArray<TrackComponent>  fTracks;
    TransportComponent* fTransport;
    TooltipWindow fTooltipWindow;
    ScopedPointer<Label> fTitle;

    // measuring redraw frequency during playback
    Time fPlaybackStart;
    int fRepaintCount;
    bool fPlaying;

    /**
     * How fast should the timer update?
     */
    int fFramesPerSecond;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScumblerComponent);
};


#endif   // __JUCER_HEADER_SCUMBLERCOMPONENT_SCUMBLERCOMPONENT_A102A7FF__
