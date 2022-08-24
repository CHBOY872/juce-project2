#pragma once
#include <JuceHeader.h>
using namespace juce;

class GComponent  : public AudioAppComponent,
                    public ChangeListener,
                    public juce::Slider::Listener,
                    public juce::Button::Listener
{
public:
    //==============================================================================
    GComponent ();
    ~GComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void moved() override;
    void mouseMove (const juce::MouseEvent& e) override;
    void mouseEnter (const juce::MouseEvent& e) override;
    void mouseExit (const juce::MouseEvent& e) override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;
    void mouseDoubleClick (const juce::MouseEvent& e) override;
    void mouseWheelMove (const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;
    void focusGained (FocusChangeType cause) override;



private:
    AudioDeviceManager otherDeviceManager;
    std::unique_ptr <AudioDeviceSelectorComponent> audioSettings;

    enum TransportState
    {
        stopped,
        playing,
        starting,
        stopping
    };

    TransportState state;


    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> playSource;
    AudioTransportSource transport;

    void transportStateChanged(TransportState newState);
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;

    void changePosition();

    //==============================================================================
    std::unique_ptr<juce::Label> music_name;
    std::unique_ptr<juce::Slider> music_slider;
    std::unique_ptr<juce::Slider> volume_slider;
    std::unique_ptr<juce::Label> volume_label;
    std::unique_ptr<juce::TextButton> play_stop_button;
    std::unique_ptr<juce::Label> volume_val_label;
    std::unique_ptr<juce::Label> music_length_label;
    std::unique_ptr<juce::TextButton> open_button;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GComponent)
};
