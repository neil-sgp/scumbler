// Copyright (c) 2013 Bg Porter

#include "Loop.h"

#include "Track.h"

#define mMin(x, y) (x) < (y) ? (x) : (y)
#define mMax(x, y) (x) < (y) ? (y) : (x)


LoopProcessor::LoopProcessor(Track* track, int channelCount)
:  PassthroughProcessor(channelCount)
,  fTrack(track)
,  fSampleRate(44100.0)
,  fLoopDuration(4000)
,  fFeedback(0.9f)
,  fLoopBuffer(nullptr)
,  fLoopPosition(0)
,  fLoopCount(0)
{

}

LoopProcessor::~LoopProcessor()
{

}

tk::Result LoopProcessor::SetLoopDuration(int milliseconds)
{
   tk::Result retval = tk::kNotWhenPlaying;
   if (!fTrack->IsPlaying())
   {
      // Hey! If this can't be called while we're playing, why are we worried about
      // thread safety? I'm concerned that it's possible to start playback while the 
      // buffer reallocation is in progress and BOOM.
      ScopedLock sl(fMutex);
      // ensure that our duration falls within an accept20able range
      fLoopDuration = mMax((int)kMinDuration, milliseconds);
      fLoopDuration = mMin(fLoopDuration,  kMaxDuration);

      int sampleCount = static_cast<int>(fSampleRate * fLoopDuration / 1000.0);

      if (nullptr == fLoopBuffer)
      {
          fLoopBuffer = new AudioSampleBuffer(fChannelCount, sampleCount);
          this->Reset();
      }
      else if (sampleCount != fLoopBuffer->getNumSamples())
      {
         // resizing
         fLoopBuffer->setSize(fChannelCount, sampleCount);
         this->Reset();
      }
      retval = tk::kSuccess;

   }
   return retval;
}

int LoopProcessor::GetLoopDuration() const
{
   ScopedLock sl(fMutex);
   return fLoopDuration;
}


void LoopProcessor::SetFeedback(float gain)
{
   ScopedLock sl(fMutex);
   fFeedback = gain;
   this->sendChangeMessage();

}

float LoopProcessor::GetFeedback() const
{
   ScopedLock sl(fMutex);
   return fFeedback;

}

bool LoopProcessor::IsPlaying() const
{
   bool retval = false;
   if (fTrack)
   {
      retval = fTrack->IsPlaying();
   }
   return retval;
}

void LoopProcessor::Reset()
{
   ScopedLock sl(fMutex);
   fLoopBuffer->clear();
   fLoopPosition = 0;
   fLoopCount = 0;    // ?
   this->sendChangeMessage();
}

void LoopProcessor::GetLoopInfo(LoopInfo& info) const
{
   ScopedLock sl(fMutex);   
   info.fSampleRate = fSampleRate;
   info.fLoopSample = fLoopPosition;
   info.fLoopLength = fLoopBuffer->getNumSamples();
   info.fLoopCount = fLoopCount;
   info.fIsPlaying = fTrack->IsPlaying();
}

const String LoopProcessor::getName() const
{
   return "LoopProcessor";
}

void LoopProcessor::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock)
{
   PassthroughProcessor::prepareToPlay(sampleRate, estimatedSamplesPerBlock);

   fSampleRate = sampleRate;
   // we may need to resize our internal buffers.
   this->SetLoopDuration(fLoopDuration);

}

void LoopProcessor::releaseResources()
{

}

void LoopProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
   if (fTrack->IsPlaying())
   {
      int sampleCount = buffer.getNumSamples();
      int loopSampleCount = fLoopBuffer->getNumSamples();
      float feedbackGain = fFeedback;
      for (int channel = 0; channel < fChannelCount; ++channel)
      {
         // this is easy if we don't need to wrap around the loop buffer 
         // when processing this block
         if (fLoopPosition + sampleCount < loopSampleCount)
         {
            // Add samples from 1 loop ago, multiplying them by the feedback gain.
            buffer.addFrom(channel, 0, *fLoopBuffer, channel, fLoopPosition,
                           sampleCount, feedbackGain);
            // ... and copy the mixed samples back into the loop buffer so we can 
            // play them // back out in one loop's time.
            fLoopBuffer->copyFrom(channel, fLoopPosition, buffer, channel, 0, sampleCount);

         }
         else
         {
            // first, process as many samples as we can fit in at the end of the loop
            // buffer.
            int roomAtEnd = loopSampleCount - fLoopPosition;
            // and we need to put this many samples back at the beginning.
            int wrapped = sampleCount - roomAtEnd;

            // add samples from a loop ago, adjusting feedback gain.
            // part 1:
            buffer.addFrom(channel, 0, *fLoopBuffer, channel, fLoopPosition,
                            roomAtEnd, feedbackGain);
            // part 2:
            buffer.addFrom(channel, roomAtEnd, *fLoopBuffer, channel, 0,
                            wrapped, feedbackGain);

            // and now copy the mixed samples back into the loop buffer:
            // part 1:
            fLoopBuffer->copyFrom(channel, fLoopPosition, buffer, channel, 0, roomAtEnd);
            // part 2:
            fLoopBuffer->copyFrom(channel, 0, buffer, channel, roomAtEnd, wrapped);

         }
      }
   
      // set the loop position for the next block of data.
      fLoopPosition = fLoopPosition + sampleCount; 
      if (fLoopPosition >= loopSampleCount)
      {
         fLoopPosition -= loopSampleCount;
      }
   }
}

bool LoopProcessor::silenceInProducesSilenceOut() const
{
   return false;
}
