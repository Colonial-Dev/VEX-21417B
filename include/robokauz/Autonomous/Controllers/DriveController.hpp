#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

class DriveController
{
    private:

        const double INPUT_FACTOR = 127.0;
        std::vector<pros::Motor> left_motors;
        std::vector<pros::Motor> right_motors;
    
    public:

        DriveController(std::vector<int> left, std::vector<int> right);

        void outputLeft(int output_voltage);

        void outputRight(int output_voltage);

        void tank(int left_voltage, int right_voltage);

        void splitArcade(int translation_axis_value, int rotation_axis_value);

        void brake();
};