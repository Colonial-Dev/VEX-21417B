### Worldsbound - March 15/16

This was unexpected, but I'm certainly not complaining and glad to be part of the team!

Because I'm just getting at the robot for the first time and Duncan and Colin were busy building, I didn't do too much this week. I spent most of the first day rewriting the operator control code to suit the bot and Duncan's preferences. I did make a small improvement by binding the ring conveyor to be toggle-activated - before, you had to hold down the trigger to keep the conveyor spinning, but now it turns on with one click and stays on with no further input. 

Besides that, I dabbled in LVGL, which is a graphics toolkit bundled with PROS that allows you to draw complex user interfaces on the brain screen. LVGL is a general-purpose toolkit for embedded systems like the brain, so it can do a TON of stuff, but for VEX purposes it's mainly useful for stuff like auton selection and debugging. I took home team C's brain so I could do the grunt work of writing a UI without cutting into proper practice time.

On Wednesday I dug through OkapiLib's documentation on odometry. OkapiLib is another PROS bundled toolkit that handles the nasty mathematical bits of stuff like odometry for you - simply plug in some measurements and other info about your robot, and you're off to the races. Unfortunately, a few details weren't properly explained, so I had to do some digging to verify certain measurements. 

Once that was done, I attempted some test runs, but had... less than stellar results. After inserting an instruction to print the encoder readings in the UI, I determined that the encoder on the middle wheel was unfortunately trashed. It didn't register rotation at all, instead fluctuating randomly in the 1-3 clicks range. (For reference, 360 clicks is a full rotation; the other two encoders were reading in the thousands of clicks after a few spins of the wheels.)

However, after falling back onto less accurate two-tracking-wheel odometry, things improved somewhat. Before I had to leave, it was handling straight-line movements with what appeared to be exceptional accuracy. Duncan says we've ordered new encoders and that they should arrive by the next time I'm at the room, so expect more news in this area soon.

I'm also toying with the idea of a record-replay system. At its simplest, record-replay just records the driver's inputs, then plays them back later in autonomous. Unfortunately, that naive version almost certainly won't work consistently, but if Voltage Strike's claim that they use a form of record-replay for their skills auton is true then there's definitely *some* way to make it happen... we'll see.