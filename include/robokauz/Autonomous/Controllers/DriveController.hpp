#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

/**
 * @brief Wrapper class for control of a skid-steer drive via split-arcade controller input or programmatic tank input.
 * @note Best stored within a shared_ptr for global use, as pros::Motor objects do NOT like being referenced or copied.
 */
class DriveController
{
    private:
        
        /**
         * @brief Constant representing the max value that can be output by a V5 analog stick.
         * @details Used to transform controller inputs into percentage outputs.
         */
        const double INPUT_FACTOR = 127.0;

        /**
         * @brief Vectors of the left and right side motors.
         */
        std::vector<pros::Motor> left_motors;
        std::vector<pros::Motor> right_motors;
    
    public:
        
        /**
         * @brief Construct a new DriveController object.
         * @param left A brace-enclosed list containing the V5 port numbers of the left side motors.
         * @param right A brace-enclosed list containing the V5 port numbers of the right side motors.
         * @note It is necessary to refer to the motors from their port numbers as pros::Motor objects cannot be passed within brace-enclosed lists.
         * The constructor uses the port numbers to reconstitute pros::Motor instances for use by the DriveController.
         */
        DriveController(std::vector<int> left, std::vector<int> right);

        /**
         * @brief Shunts the specified amount of voltage to the left side motors.
         * @param output_voltage The desired output voltage, from -12000 to 12000 millivolts.
         */
        void outputLeft(int output_voltage);

        /**
         * @brief Shunts the specified amount of voltage to the right side motors.
         * @param output_voltage The desired output voltage, from -12000 to 12000 millivolts.
         */
        void outputRight(int output_voltage);
        
        /**
         * @brief Wrapper around outputLeft and outputRight for basic tank control.
         * @param left_voltage The desired output voltage for the left motors, from -12000 to 12000 millivolts.
         * @param right_voltage The desired output voltage for the right motors, from -12000 to 12000 millivolts.
         */
        void tank(int left_voltage, int right_voltage);
        
        /**
         * @brief Wrapper around tank that takes in percentage values rather than raw voltage.
         * @param left_pct The desired percent output for the left motors, from -1.0 to 1.0.
         * @param right_pct The desired percent output for the right motors, from -1.0 to 1.0.
         */
        void tankPercentage(double left_pct, double right_pct);

        /**
         * @brief Driver control hook for split arcade control.
         * @param translation_axis_value The analog value of the designated translation (i.e. forwards/backwards) axis.
         * @param rotation_axis_value The analog value of the designated rotation axis.
         */
        void splitArcade(int translation_axis_value, int rotation_axis_value);
        
        /**
         * @brief Sets all motors to zero output voltage.
         */
        void brake();
        
        /**
         * @brief Get the average efficiency value of all motors in the drive. Useful for jam detection.
         * @return double The averaged efficiency of all motors.
         */
        double getAverageEfficiency();
};