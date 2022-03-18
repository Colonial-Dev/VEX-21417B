#pragma once

struct Frame
{
    int encoder_right;
    int encoder_middle;
    int encoder_left;
    int lift_potentiometer;
    int drive_right_back;
    int drive_right_middle;
    int drive_right_front;
    int drive_left_back;
    int drive_left_middle;
    int drive_left_front;
    int motivator_lift;
    int motivator_conveyor;
    int pneumatics_front_clamp;
    int pneumatics_back_clamp;
};

class OperatorTape
{
    private:
        int tapeLength;
        int tapeType;
        std::string tapeName;
        std::vector<Frame> frameData;
    
    public:

        OperatorTape(int length, int type, std::string name)
        {

        }

        OperatorTape(std::string serialized_tape)
        {

        }

        int getLength() { return tapeLength; }
        int getType() { return tapeType; }
        std::string GetName() { return tapeName; }

        void writeFrame(Frame nextFrame)
        {

        }

        Frame readFrame(int frameIndex)
        {
            return frameData.at(frameIndex);
        }

        std::string serializeRecording()
        {
            return "";
        }
};

class TapePlayer
{
    private:
        OperatorTape currentTape;
        pros::Task playbackTask;
        int status;
        int currentFramestamp;
    
        void playbackTape() 
        {

        }

    public:

        int getStatus() { return status; }

        void insertTape(OperatorTape tape)
        {
            currentTape = tape;
        }

        void startPlayback()
        {
        
        }

        void pausePlayback()
        {

        }

        void stopPlayback()
        {

        }

};