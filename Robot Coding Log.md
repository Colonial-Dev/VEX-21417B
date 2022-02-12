### Code Cleanup
My first order of bussiness after the Wawasee tournament was code cleanup. Up through the Wawasee tournament, all of the robot's code had been contained in a single monolithic source code file that was nearing a thousand lines in length, which made tweaking things an absolute chore. Compounding the issue was slapdash, hard-to-read and brittle code. 
- I was using a lot of "magic numbers" (unexplained integer values, like using "2" to mean "right side auton") 
- Brittle harcoding of some functionality made changing things a chore. The original iteration of the controller menu system was probably the worst offender, being at times prone to segmentation faults that would crash the entire program.
- The auton code was highly verbose - instructions were long, oft-repeated, and little-reused.

I had initially avoided splitting up my code due to how difficult C++ made it. Unlike modern languages, where code can more-or-less automatically interact across file boundaries, C++ requires you to manually use "include" markers to cobble together multiple files into one megafile during the compilation process.However, it had become unavoidable. I read up on the include system and managed to get it working after an excessive amount of googling, splitting my code into 8 files, each with a dedicated purpose. (One for driver control, one for auton setup, one for auton instructions...)

The magic numbers issue was next. Fortunately, C++ offers an easy solution: enums. These are dead simple; they basically let you use names in place of numbers. So instead of using "if autonSide == 2" to see if the right side was selected, I can say "if autonSide == Right," which is INFINITELY more readable.

### Fun with Classes
Now I had to deal with the hardcoding issue. In most places this was easy enough to resolve, but the controller menu was a whole different beast. The existing implementation was utterly insufficient, and I scrapped it wholly in favor of using a much more advanced paradigm: object orientation, aka the "++" part of C++.Object orientation (OO) is kind of hard to wrap your head around until you actually work with it, but I'll try to explain it as succintly as possible.

Basically, OO means your code is written to describe interactions between, well, objects. This differs from basic "procedural" programming, where code is more like a list of explicit instructions.

Objects are created by defining a "class." Classes are like an interactive box for data. Not only do they hold certain information in "fields" (for example, you could make a Car object that has fields for its name, model, etc), but they also define "methods" that are used to interact with these fields. (Continuing the car analogy, you could have a Fuel method that adds fuel to a gas level field.) 

Once you have a class, you can then *instantiate* it. This is where the true power of objects starts to show. Instantiation means you create an independent instance of that class, containing its own specific data - and you can instantiate as much as you want. You can make a whole showroom of Car objects, each of which is unique and compartamentalized.

So, to bring object orientation to the menu system, I created a tiered hierarchy of menu component classes.
- A MenuItem class, which holds its name and a list of arbitrary commands. It has a method to retrieve its name, and a method to execute the stored commands.
- A MenuLevel class, which holds its name and a *vector* of MenuItem instances. A vector is like an array - an ordered collection of a specific kind of thing - except it can shrink or grow on-the-fly. It has a method to retrieve its name, and it directly exposes its MenuItem vector for access by other objects/code.
- A MenuManager class, which holds a vector of MenuLevels (starting to see the pattern?) and two integer variables that point to the vector location of the current MenuLevel and the index of the current MenuItem within the current MenuLevel. The Manager has the most methods: a Page method for moving through the MenuItems in a MenuLevel, methods for retrieving the name of the current item and level, a method to swap to another level (based on its name) and finally a method to execute the currently selected MenuItem.

Once I had these classes, I had to instantiate the various items and levels and finally combine them in a single manager. This assembly happens dynamically on every boot-up, so adding or removing an item or level is as simple as editing a single line in the right file.

After this, all I had to do was hook the manager into my existing code for talking to the controller, and the menu system was reborn!

### Mercilessly Abusing the C++ Compiler with Macros
Now to the verbosity issue. The key here turned out to be a C++ feature known as macros.

Simplified, macros are a feature of the C++ compiler. (The compiler is the program responsible for translating human-written source code intomachine code that a computer like the Brain can use.) 

To be more specific, macros are a feature of the *preprocessor,* the first stage of the compilation process. The preprocessor cobbles together all your source code files into a single megablock of text (using the includes mentioned earlier), analyzes it to ensure it's syntactically correct (e.g. no mispellings or missing braces) and finally breaks it apart into little chunks ("tokens") for later stages of the compiler to optimize and finally translate into machine code.

Macros hijack the chunking stage of the preprocessor to replace (or "expand") programmer-defined shortcut phrases with actual code. This means you can have an easy-to-read and easy-to-remember phrase in your source code that equates to something much more complex when compiled.

For example, in my code, I have this macro definition:
```cpp
define MAIN_LIFT_HOVER \
frontLiftRight->setTarget(600);\
frontLiftLeft->setTarget(-600);\
frontLiftLeft->waitUntilSettled();
```
Without macros, if I want to raise the front lift from its starting position, I have to write or copy-paste in the same three fairly long lines of code, which makes auton instructions an ugly, hard-to-maintain mess.

With this macro, I can put `MAIN_LIFT_HOVER` wherever I want to get the same effect as writing the commands manually. Not only is this easier to write and read, it also makes changes easier, since I only have to edit the macro definition if the target values need tweaking.Macros can also take arguments. A good example is my `THROTTLE` macro:
```cpp
define THROTTLE(x) \
//The std::clamp just guards against setting the value to something 
//invalid.
driveTrain->setMaxVelocity(std::clamp(x, 0, 600)); 
\pathFinder->setMaxVelocity(std::clamp(x, 0, 600));
```
This macro sets the throttle (in RPMs) to whatever I put for x. So if I write `THROTTLE(300)` elsewhere, 300 will replace x wherever it appears in the macro definition.Using macros, I've effectively created a custom, simplified scripting language *within* C++ that cuts the length of my auton instructions by about two-thirds.

### Destination Calibration
Besides all that, I've spent a lot of time calibrating the values used in the drive train's PID controller, in order to reduce drift and increase turning accuracy. There's nothing really exciting to say here; it's just a lot of tweak and test, tweak and test, over and over and over.

### Features Bonus Round 
- Easy text output to brain screen in multiple places for diagnostics/debug purposes.
- Initialization scripting to take advantage of aformentioned brain output, including a POST (Power-On Self Test) that polls every motor and reports any that return a "not found" error. (As you saw, this came in handy at the most recent tournament.)
- Only generating necessary autonomous paths to save on processor time and memory.
- Cycling text on controller during launch holding that confirms the program is still running and informs the driver to press DOWN or plug into a comp switch to continue.

### Squiggles Overdrive (Future plans)
- Switch to the development version of OkapiLib, which has replaced the outdated Pathfinder library with Squiggles. Pathfinder is very limited - it can't calculate backwards motion or sharp turns, which severely hampers my capabilities. Squiggles does not have these issues, which means I'll be able to make much more versatile autonomous paths - think S, V and U turns.
- Skills auton of at least 120pts.
- Badger Theo into fixing lingering hardware issues to ensure auton reliability - so tipping/balance and front claw grip strength, at the very least. Replacing the standoffs and strips used to shield the wheel channel with plastic shielding is also an idea. (Putting them on the side of the bot like Liam's bot could also be nice - maybe around the brain area on back and behind the front lift as well?)
