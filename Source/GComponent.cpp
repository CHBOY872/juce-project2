
#include "GComponent.h"

GComponent::GComponent () : juce::AudioAppComponent(otherDeviceManager)
{
    state = stopped;
    otherDeviceManager.initialise(2, 2, nullptr, true);
    audioSettings.reset(new AudioDeviceSelectorComponent(otherDeviceManager, 0, 2, 0, 2, true, true, true, true));
    addAndMakeVisible(audioSettings.get());
    setAudioChannels(2, 2);
    
    music_name.reset (new juce::Label ("Music Header",
                                       TRANS("label text")));
    addAndMakeVisible (music_name.get());
    music_name->setFont (juce::Font (30.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    music_name->setJustificationType (juce::Justification::centred);
    music_name->setEditable (false, false, false);
    music_name->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    music_name->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    music_name->setBounds (352, 296, 432, 48);

    music_slider.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (music_slider.get());
    music_slider->setRange (0, 10, 0);
    music_slider->setSliderStyle (juce::Slider::LinearHorizontal);
    music_slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    music_slider->addListener (this);

    music_slider->setBounds (247, 416, 688, 32);

    volume_slider.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (volume_slider.get());
    volume_slider->setRange (0, 10, 0);
    volume_slider->setSliderStyle (juce::Slider::LinearVertical);
    volume_slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    volume_slider->addListener (this);

    volume_slider->setBounds (792, 184, 103, 216);

    volume_label.reset (new juce::Label ("new label",
                                         TRANS("Volume")));
    addAndMakeVisible (volume_label.get());
    volume_label->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    volume_label->setJustificationType (juce::Justification::centred);
    volume_label->setEditable (false, false, false);
    volume_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    volume_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    volume_label->setBounds (800, 120, 88, 24);

    play_stop_button.reset (new juce::TextButton ("play stop button"));
    addAndMakeVisible (play_stop_button.get());
    play_stop_button->setButtonText (TRANS("Play"));
    play_stop_button->addListener (this);
    play_stop_button->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff73b7ff));
    play_stop_button->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff32542b));

    play_stop_button->setBounds (576, 368, 64, 24);

    volume_val_label.reset (new juce::Label ("new label",
                                             juce::String()));
    addAndMakeVisible (volume_val_label.get());
    volume_val_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    volume_val_label->setJustificationType (juce::Justification::centred);
    volume_val_label->setEditable (false, false, false);
    volume_val_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    volume_val_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    volume_val_label->setBounds (800, 152, 80, 24);

    music_length_label.reset (new juce::Label ("new label",
                                               juce::String()));
    addAndMakeVisible (music_length_label.get());
    music_length_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    music_length_label->setJustificationType (juce::Justification::centred);
    music_length_label->setEditable (false, false, false);
    music_length_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    music_length_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    music_length_label->setBounds (152, 416, 94, 32);

    open_button.reset (new juce::TextButton ("open button"));
    addAndMakeVisible (open_button.get());
    open_button->setButtonText (TRANS("Open"));
    open_button->addListener (this);
    open_button->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff767770));
    open_button->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff32542b));

    open_button->setBounds (496, 368, 64, 24);


    
    

    setSize (1024, 768);


    
    formatManager.registerBasicFormats();
    transport.addChangeListener(this);
    play_stop_button->setEnabled(false);
    
}

GComponent::~GComponent()
{
    
    music_name = nullptr;
    music_slider = nullptr;
    volume_slider = nullptr;
    volume_label = nullptr;
    play_stop_button = nullptr;
    volume_val_label = nullptr;
    music_length_label = nullptr;
    open_button = nullptr;

    shutdownAudio();
    
}


void GComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colour (0xff323e44));

}

void GComponent::resized()
{
}

void GComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
 
    if (sliderThatWasMoved == music_slider.get())
    {
 
        if (state == playing)
            transport.setPosition(music_slider->getValue());
 
    }
    else if (sliderThatWasMoved == volume_slider.get())
    {
 
        juce::String new_val = std::to_string((int)(volume_slider->getValue() * 10));
        new_val += " %";
        volume_val_label->setText(new_val, juce::dontSendNotification);
 
    }
}

void GComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == play_stop_button.get())
    {
        if (play_stop_button->getButtonText() == "Play") // start or stop music depending on state
        {
            transportStateChanged(starting);
            play_stop_button->setButtonText("Stop");
        }
        else
        {
            transportStateChanged(stopping);
            play_stop_button->setButtonText("Play");
        }
    }
    else if (buttonThatWasClicked == open_button.get())
    {

        FileChooser chooser ("Choose a Wav or AIFF File", File::getSpecialLocation(File::userDesktopDirectory), "*.wav; *.mp3");
        if (chooser.browseForFileToOpen())
        {
            File myFile;

            myFile = chooser.getResult();

            AudioFormatReader* reader = formatManager.createReaderFor(myFile);

            if (reader != nullptr)
            {
                std::unique_ptr<AudioFormatReaderSource> tempSource (new AudioFormatReaderSource (reader, true));

                transport.setSource(tempSource.get());
                transportStateChanged(stopped);

                playSource.reset(tempSource.release());
                music_name->setText(myFile.getFileName(), juce::dontSendNotification);
                music_length_label->setText(std::to_string(transport.getLengthInSeconds()), juce::dontSendNotification);
                play_stop_button->setEnabled(true);
                play_stop_button->setButtonText("Play");
                music_slider->setRange(0, transport.getLengthInSeconds());
            }
        }
        
    }

}

void GComponent::moved()
{
}

void GComponent::mouseMove (const juce::MouseEvent& e)
{
}

void GComponent::mouseEnter (const juce::MouseEvent& e)
{
}

void GComponent::mouseExit (const juce::MouseEvent& e)
{
}

void GComponent::mouseDown (const juce::MouseEvent& e)
{
}

void GComponent::mouseDrag (const juce::MouseEvent& e)
{
}

void GComponent::mouseUp (const juce::MouseEvent& e)
{
}

void GComponent::mouseDoubleClick (const juce::MouseEvent& e)
{
}

void GComponent::mouseWheelMove (const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)
{
}

void GComponent::focusGained (FocusChangeType cause)
{
}

void GComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transport.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void GComponent::transportStateChanged(TransportState newState)
{
    if (newState != state)
    {
        state = newState;

        switch (state) {
            case stopped:
                transport.setPosition(0.0);
                break;

            case playing:
                break;

            case starting:
                transport.start();
                break;

            case stopping:
                transport.stop();
                break;
        }

    }
}

void GComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    transport.getNextAudioBlock(bufferToFill);
}

void GComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

void GComponent::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    if (source == &transport)
    {
        if (transport.isPlaying())
            transportStateChanged(playing);
        else
            transportStateChanged(stopped);
    }
}


void GComponent::changePosition()
{

}
