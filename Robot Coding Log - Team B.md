This was unexpected, but I'm certainly not complaining and glad to be part of the team!

### Some Basics
I'm 99% sure you already know in a generic sense that I use PROS rather than VEXCode. But, in case you aren't sure what PROS is, a brief explanation: PROS is an alternative kernel for the V5 Brain, written and maintained by students at Purdue and officially endorsed by VRC/REC. A "kernel" is a special piece of software that acts as a middleman between user programs (like mine) and the actual hardware of the computer; the kernel is responsible for handling multitasking, managing memory, talking to attached devices (like a keyboard or a motor) and all that good stuff.

The main advantage of PROS is that it allows you to use third-party libraries in your programs. A third-party library is essentially a bunch of "helper code" written by other programmers to solve common problems in a reusable way. For example, PROS comes bundled with OkapiLib, a library that automates stuff like odometry - in theory, you just plug in some measurements and port numbers and you're off to the races, no understanding of higher-level mathematics required. (A secondary advantage is that I don't have to use the godawful VEXCode editor.)

### Operator Control
I spent most of my first day with the new robot stripping out all the code that was specific to team A's bot, then rewriting just the operator control code to suit the new design and Duncan's preferences. I did make one notable improvement by binding the conveyor controls to a small finite state machine, allowing it to be toggle-activated. Before, you had to hold down the trigger to keep the conveyor spinning, but now it turns on (or off, or reverses) with one click and keeps going with no further input.

A finite state machine sounds fancy, but it's just a way to represent a machine that can only be in one of a limited number of states at any given time. In our case, the conveyor can be represented by just three states: Idle, Forward and Reverse. By setting the motor's velocity depending on the state and binding some buttons to state-changing, we get the conveyor code:

```cpp
//R1 forward, R2 reverse
void conveyorControl()
{
	int conveyorStatus = Idle;

	while(true)
	{
        //get_digital_new_press is a special check that only goes through if the button is newly pressed.
        //If we just used get_digital_press, then holding down the button or even just tapping it slowly
        //would cause the conveyor to flail because the button is still pressed.
		if(master.get_digital_new_press(DIGITAL_R1))
		{
			if(conveyorStatus == Idle || conveyorStatus == Reverse) { conveyorStatus = Forward; }
			else { conveyorStatus = Idle; }
		}
		else if(master.get_digital_new_press(DIGITAL_R2))
		{
			if(conveyorStatus == Idle || conveyorStatus == Forward) { conveyorStatus = Reverse; }
			else { conveyorStatus = Idle; }
		}

		if(conveyorStatus == Idle) { conveyor_motor.move_velocity(0); }
		else if(conveyorStatus == Forward) { conveyor_motor.move_velocity(600); }
		else if(conveyorStatus == Reverse) { conveyor_motor.move_velocity(-600); }
		
		pros::delay(2);
	}
}
```

You probably noticed a few special keywords used throughout the snippet - `Idle`, `Forward`, and `Reverse`. These are essentially custom labels - known as "enums," short for "enumerated types" - that can be used in place of numbers when writing code. I use them a lot throughout my program - they really help make things readable, and it's much more intuitive to think in terms of "is this equal to Idle" than "is this equal to 0, the number I've picked to mean Idle."

This is what an enum looks like:

```cpp
enum ConveyorStatus
{
  Idle, //Equal to 0
  Forward, //Equal to 1
  Reverse //Equal to 2
};
```

Yes, it really is that braindead. But they're very handy!

### Oooo Shiny
One of the libraries bundled with PROS is LVGL, a graphics toolkit that handles all the heavy lifting of drawing complex GUIs (graphical user interfaces - buttons and stuff, basically.) LVGL is a general-purpose library for embedded systems (i.e. tiny computers like the Brain )so it can do a TON of stuff, but for VEX purposes you just need a few buttons and a text box or two for selecting autons and debugging. 

Unfortunately, a lot of the power (and therefore complexity) behind LVGL still bleeds through, so I took a spare Brain home over the weekend so I could handle all the grunt work of writing a UI without wasting proper practice time. I *would* put some code here, but it's mostly just a bunch of verbose commands specifying exactly where to put everything on the screen - nothing exciting or special.

![BrainUI_A](https://imgur.com/15Xsv2p.png) ![BrainUI_B](https://imgur.com/KFJGc7m.png)

Above are some pictures of the (mostly) final product.
- When you first start the program, the display defaults to showing the status readout and auton selector. The former shows live information about the current state of the robot and attached devices, including the output from odometry and the inertial sensor.
- You can switch the left pane to the control menu. This is primarily useful for test-running autons, although it can also be used to reset the program (i.e. recalibrate the odometry, inertial sensors etc so it's like the program was just started) or forcefully terminate it.
- The auton selector first prompts you for a side and then a strategy (if applicable), before finally showing a "ready" screen with your choice. A cancel button is available if you make a mistake, and the ready screen also has a "Help" button that will show any applicable info about the selection.

### Odometry Goes Brr
As aformentioned, OkapiLib is a library bundled with PROS that handles all the nasty mathematical bits of stuff like odometry for you. Since I lack the knowledge required to properly comprehend said nasty mathematical bits, this is obviously very helpful, and as such it's my first stop on the way to 290+ points.

Last Wednesday, I dug through OkapiLib's documentation on odometry and banged out the initialization code. Unfortunately, a few details in regards to the needed measurements weren't properly explained, so I ended up wasting some time digging around and doing trial-and-error tests to verify some things. The middle encoder breaking didn't help things either... but here's what the initialization command ultimately looked like:

```cpp
double GEAR_RATIO = 60.0/84.0;

auto drive_train = okapi::ChassisControllerBuilder()
  .withMotors({19, 6, 9}, {12, 15, 16})
  .withGains
  (
      {0.001, 0, 0.0001}, //Distance gains
      {0.0042, 0.0003, 0.0001}, //Turn gains
      {0.7, 0.001, 0.0001}  //Angle gains
  )
  .withSensors
  (
      ADIEncoder{'A', 'B', true}, //Left encoder (inverted)
      ADIEncoder{'C', 'D'},  //Right encoder
      ADIEncoder{'E', 'F', true}  //Middle encoder (inverted)
  )
  .withDimensions({okapi::AbstractMotor::gearset::green, GEAR_RATIO}, {{4_in, 11_in}, okapi::imev5GreenTPR * GEAR_RATIO})
  //Specify odometry dimensions and encoder type
  .withOdometry({{2.75_in, 5_in, 3.5_in, 2.75_in}, quadEncoderTPR})
  .buildOdometry(); //Build an odometry-enabled chassis
```

All this is doing is feeding a bunch of numbers into OkapiLib, which spits back out an `OdomChassisController` object that can be used to track and move the bot around the field. Properly explaining what an object is would take a while - if you want a basic overview, check out the "Fun with Classes" section of the "Robot Coding Log - Team A" document in this same Github repo - but in this case it can basically be treated like a magic box that drives the robot places based on the numbers I feed into it. If I want to go two feet forwards and to the right, I just have to say `drive_train->driveToPoint({2_ft, 0_ft});` and the robot will (hopefully) do it.

Once that was all written and the encoder was replaced, I was able to test it some more today, with *decent* results. The numbers in the `withGains` section will need dedicated tuning to ensure the robot drives straight and turns accurately, and turning in general is just kinda janky, but I should hopefully be able to figure it out tommorow.

### Future Plans
OkapiLib's odometry is a good starting point, but it's ultimately limited. Its primary issue is that it can't handle curvilinear motion - when it moves somewhere, it turns to face the destination and then drives STRAIGHT at it, no matter what might be in the way. This makes navigation tedious and time consuming, which is a death sentence for skills. I have several potential solutions lined up, in descending order of attractiveness:

- Writing a custom implementation of the [Pure Pursuit](https://www.chiefdelphi.com/uploads/default/original/3X/b/e/be0e06de00e07db66f97686505c3f4dde2e332dc.pdf) algorithm. Pure Pursuit works by taking in a simple chain of waypoints on the field, then injecting additional waypoints and smoothing the whole path into a curve. The robot then "pursues" the waypoints on this curve in order, using odometry to track and adjust its course on the fly. (That document I linked has excellent illustrations, if you're confused.) The advantages of Pure Pursuit are that it's accurate, self-correcting, and can be used for everything - not just curvilinear motion. The disadvantage is that I have to write the whole damn thing myself.
- Using OkapiLib's 2D motion profiling feature. I used this on team A's bot - it's essentially a weak sauce version of Pure Pursuit that works by mathematically generating the motor voltages needed to follow a path, then blindly following said curves to mixed results. While inaccurate, motion profiling is trivial to set up and should be useable for the occasional curved movement without issue. Motion profiling movements can't be chained together because it isn't self-correcting like odometry, so error compounds quickly.
- A record-replay system. The basic idea with one of these is simple; write some code to record the driver's actions, then play them back autonomously. In practice, implementing a consistent record-replay system is a complete nightmare - the robot drifts everywhere, you need to write a whole system to not just create recordings, but save and load them to/from an SD card, and you can't easily tweak a movement or whatever because the routine is stored as a bunch of numbers rather than human-readable commands. It's not *impossible* - Voltage Strike supposedly does this - but unless Duncan really pushes for it I don't think it'd be a good use of our time.

Other than that, I'll probably be spending most of my time between now and worlds tweaking and testing *ad nauseam*. That's where 80% of programming effort goes :p