#include "robokauz/ROUTINES.hpp"

void selectRoutine(int side, int strategy)
{
    switch(side)
    {
        case Skills:
        {
            AutonomousRoutines::skills();
            return;
        }
        case None:
        {
            return;
        }
    }

    /*switch(side)
    {
        case Left:
        {
            switch(strategy)
            {
                case FullWinPoint:
                {
                    AutonomousRoutines::leftFullWP();
                    return;
                }
                case HalfWinPoint:
                {
                    AutonomousRoutines::leftHalfWP();
                    return;
                }
                case SingleGoalRush:
                {
                    AutonomousRoutines::leftSingleRush();
                    return;
                }
                case DoubleGoalRush:
                {
                    AutonomousRoutines::leftDoubleRush();
                    return;
                }
            }
        }
        case Right:
        {
            switch(strategy)
            {
                case FullWinPoint:
                {
                    AutonomousRoutines::rightFullWP();
                    return;
                }
                case HalfWinPoint:
                {
                    AutonomousRoutines::rightHalfWP();
                    return;
                }
                case SingleGoalRush:
                {
                    AutonomousRoutines::rightSingleRush();
                    return;
                }
                case DoubleGoalRush:
                {
                    AutonomousRoutines::rightDoubleRush();
                    return;
                }
            }
        }
    }*/
}