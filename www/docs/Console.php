<? header("Content-type: text/html; charset=iso-8859-1"); ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>Gravit Documentation - Console</title>
<style type="text/css">
@import url(../gravit.css);
</style>
</head>
<body>
<h1 id="ConsoleCommandsandVariables">Console Commands and Variables</h1>
<p>
<a class="ext-link" title=".." href="..">Gravit Home</a> / <a href="Index.php">Documentation Index</a>
</p>
<h2 id="Overview">Overview</h2>
<p>
Here's a list of commands and variables. The values in the <strong>Type</strong> column below are <strong>Cmd</strong> for command and <strong>Var</strong> for variable. Some variables are actually commands that act like variables like <i>g</i> and <i>autorotate</i>. Words that are in <i>italics</i> refer to a command or variable.
</p>
<h2 id="General">General</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>quit</td><td>Cmd</td><td>Will quit Gravit.
</td></tr><tr><td>exec</td><td>Cmd</td><td>Execute a Gravit script. The path can be absolute or relative. Scripts are just text files that contain console commands. Lines starting with a # are ignored.
</td></tr></table>
<h2 id="RecordingandPlayback">Recording and Playback</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>record</td><td>Cmd</td><td>Recording is when Gravit simulates gravity. The command will stop recording if it's already recording otherwise it will start recording.
</td></tr><tr><td>autorecord</td><td>Var</td><td>Setting this to 1 will automatically start recording after the <i>spawn</i> command.
</td></tr><tr><td>play</td><td>Cmd</td><td>Replay's a recording that is currently in memory. It will loop the simulation over and over.
</td></tr><tr><td>stop</td><td>Cmd</td><td>Simply stops recording or playing.
</td></tr><tr><td>status</td><td>Cmd</td><td>Will display some statistics
</td></tr><tr><td>g</td><td>Var</td><td>Set the accuracy and speed of a simulation. The default value is 5. Increase it for a slower, more accurate simulation. Decrease it for the opposite. A value too low (like 1 or 2) will create a chaotic simulation. Technically this creates a small value used for the gravity calculations. The forumula for this number is G = -10 ^ -g. This is used in F = Gm1m2/d^2.
</td></tr><tr><td>framecompression</td><td>Var</td><td>When Gravit uses up all memory for storing particles, it can 'compress' the history by removing every second frame. The simulation then will resume half way and only save every second frame. Turning this variable on will allow this, otherwise the simulation will stop.
</td></tr><tr><td>frameskip</td><td>Var</td><td>When playing a simulation, increase this number to skip played back frames for a faster playback.
</td></tr><tr><td>frame</td><td>Var</td><td>Jumps to a frame number in playback mode
</td></tr></table>
<h2 id="Spawning">Spawning</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>start</td><td>Cmd</td><td>An alias to <i>spawn</i>
</td></tr><tr><td>spawn</td><td>Cmd</td><td>Will create a new simulation with <i>particlecount</i> particles and will allocate <i>memoryavailable</i> memory.
</td></tr><tr><td>particlecount</td><td>Var</td><td>Next time <i>spawn</i> gets executed, this is how many particles will spawn
</td></tr><tr><td>memoryavailable</td><td>Var</td><td>This is the amount of memory (in MB) that Gravit will allocate when spawning a new simulation.
</td></tr><tr><td>spawngalcountmin</td><td>Var</td><td>Determines the minimum amount of galaxies to spawn.
</td></tr><tr><td>spawngalcountmax</td><td>Var</td><td>Determines the maximum amount of galaxies to spawn.
</td></tr><tr><td>spawngalmassmin</td><td>Var</td><td>The minimum mass of a particle. Each galaxy will pick a range between <i>spawngalmassmin</i> and <i>spawngalmassmax</i> to determine the mass of each particle within it.
</td></tr><tr><td>spawngalmassmax</td><td>Var</td><td>The maximum mass of a particle. Each galaxy will pick a range between <i>spawngalmassmin</i> and <i>spawngalmassmax</i> to determine the mass of each particle within it.
</td></tr><tr><td>spawngalsizemin</td><td>Var</td><td>The minimum radius of a galaxy.
</td></tr><tr><td>spawngalsizemax</td><td>Var</td><td>The maximum radius of a galaxy.
</td></tr><tr><td>spawngalvelmin</td><td>Var</td><td>The minimum velocity of a galaxy.
</td></tr><tr><td>spawngalvelmax</td><td>Var</td><td>The maximum velocity of a galaxy.
</td></tr><tr><td>spawnrangemin</td><td>Var</td><td>The minimum size of the universe where galaxies may spawn in.
</td></tr><tr><td>spawnrangemax</td><td>Var</td><td>The maximum size of the universe where galaxies may spawn in.
</td></tr></table>
<h2 id="SavingandLoading">Saving and Loading</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>load</td><td>Cmd</td><td>Load a previously saved simulation. Most simulation settings are saved except for <i>g</i>.
</td></tr><tr><td>save</td><td>Cmd</td><td>Saves the current simulation by the name you give it (eg. &#34;save mysimulation&#34;). If you have saved or loaded recently, you will have a &#34;simulation name&#34; which is shown on the top of your screen. If you have this, you don't need to specify a name to save -- it will automatically use the simulation name.
</td></tr><tr><td>saveauto</td><td>Var</td><td>When set to a number bigger then 0, it will automatically save every n frames.
</td></tr></table>
<h2 id="Video">Video</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>videorestart</td><td>Cmd</td><td>Restarts the video display with the new video settings. The settings that are applied by this command are <i>videowidth</i>, <i>videoheight</i>, <i>videobpp</i>, <i>videofullscreen</i>, <i>videoantialiasing</i>, <i>fontfile</i> and <i>fontsize</i>.
</td></tr><tr><td>videowidth</td><td>Var</td><td>Video resolution width Gravit will use when <i>videorestart</i> is executed or when the program starts. This usually needs to be used with <i>videoheight</i>. Good combos are 800x600, 1024x768, 1280x1024 and 1600x1200 -- depending on your video capabilities
</td></tr><tr><td>videoheight</td><td>Var</td><td>Video resolution height. See <i>videowidth</i>.
</td></tr><tr><td>videobpp</td><td>Var</td><td>Video bits per pixel. A good value for this is 16, 24, 32.
</td></tr><tr><td>videofullscreen</td><td>Var</td><td>Set this value to 1 if you want Gravit to take up the whole screen. 0 for windowed mode.
</td></tr><tr><td>videoantialiasing</td><td>Var</td><td>This turns on full screen anti-aliasing. Some hardware is not compatible with this.
</td></tr><tr><td>fontfile</td><td>Var</td><td>Specifies which true type font to use. The default is Vera.ttf which comes with Gravit.
</td></tr><tr><td>fontsize</td><td>Var</td><td>Used with <i>fontfile</i> to specify the font size in pixels.
</td></tr><tr><td>recordingvideorefreshtime</td><td>Var</td><td>This setting allows the screen to update while recording a frame. The value is in milliseconds. The higher you have this value, the less often your video will update (causing video and input lag) but recording will be faster. Set this to 0 to turn off video/input updating while recording completely.
</td></tr><tr><td>showcursor</td><td>Var</td><td>Set this to 0 to hide the mouse cursor, 1 to display it.
</td></tr><tr><td>blendmode</td><td>Var</td><td>There are 5 blend modes (0 to 4). 0 disables OpenGL blending. 1,2,3,4 use different combinations of blend settings. They can be pretty and wierd effects.
</td></tr><tr><td>fps</td><td>Var</td><td>Not implemented
</td></tr><tr><td>autocenter</td><td>Var</td><td>Tracks the center of the simulation when set to 1
</td></tr><tr><td>maxvertices</td><td>Var</td><td>Sets a limit to how many vertices to render. If it hits this value, <i>tailskip</i> will be doubled.
</td></tr><tr><td>zoom</td><td>Var</td><td>The current zoom. 0 means you're at the center of the simulation.
</td></tr><tr><td>autorotate</td><td>Var</td><td>This accepts 3 arguments for X Y and Z. Set these to 0 for no rotation.
</td></tr><tr><td>stereo</td><td>Var</td><td>When set to 1, Gravit will render the scene twice -- one on the left and one on the right. It is used for creating stereoscopic images to see Gravit in 3D.
</td></tr><tr><td>stereoseparation</td><td>Var</td><td>When <i>stereo</i> is set to 1, modify this value to determine the depth of the 3D image. Negative values are for parallel viewing, positive values are for cross-eyed viewing.
</td></tr><tr><td>screenshot</td><td>Cmd</td><td>Creates a bitmap (BMP) screenshot in the screenshots directory
</td></tr><tr><td>screenshotloop</td><td>Var</td><td>Every frame rendered to the screen will execute the screenshot command. This will easily fill up your hard drive with BMP files.
</td></tr></table>
<h2 id="ScreenSaver">Screen Saver</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>screensaver</td><td>Var</td><td>This is set to 1 when Gravit is running as a screensaver. It will quit on any mouse movement or keystroke.
</td></tr><tr><td>installscreensaver</td><td>Cmd</td><td>In windows, this will install Gravit as a screensaver.
</td></tr></table>
<h2 id="ParticlesandEffects">Particles and Effects</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>particlerendermode</td><td>Var</td><td>There are 3 rendering modes for particles. 0 for dots, 1 for fast and less compatible textured particles and 2 which looks like 1 except that it is more compatible and slower.
</td></tr><tr><td>particlerendertexture</td><td>Var</td><td>Rendering of the particle texture -- this was used for debugging.
</td></tr><tr><td>particlecolourmode</td><td>Var</td><td>There are 3 particle colour modes. 0 for colour based on mass, 1 is based on current velocity and 2 is based on acceleration (which needs at least 2 frames recorded to work).
</td></tr><tr><td>particlesizemin</td><td>Var</td><td>Minimum size of a particle. This has no effect in <i>particlerendermode 0</i>. These sizes differ between <i>particlerendermode</i> 1 and 2.
</td></tr><tr><td>particlesizemax</td><td>Var</td><td>Maximum size of a particle. This has no effect in <i>particlerendermode 0</i>. These sizes differ between <i>particlerendermode</i> 1 and 2.
</td></tr><tr><td>tailskip</td><td>Var</td><td>Changes the resolution of the particle tail, the higher the number the less accurate the tail is. Setting it to 1 will created the smoothest tails.
</td></tr><tr><td>tailfaded</td><td>Var</td><td>Setting this to 1 will make the tail increasingly transparent along the length of the tail. Set it to 0 to maintain a constant colour/opacity.
</td></tr><tr><td>tailopacity</td><td>Var</td><td>Sets the opacity of every tail. It ranges from 0 being invisible to 1 being opaque.
</td></tr><tr><td>taillength</td><td>Var</td><td>The length of the tail in frames. Set it to -1 to display every step in the tail.
</td></tr><tr><td>tailwidth</td><td>Var</td><td>Thinkness of each tail.
</td></tr><tr><td>drawaxis</td><td>Var</td><td>Not implemented
</td></tr><tr><td>drawtree</td><td>Var</td><td>Will display the octtree. 1 will show the tree using lines, 2 will show semi-transparent boxes. The tree will animate if <i>recordingvideorefreshtime</i> is bigger then 0.
</td></tr><tr><td>drawosd</td><td>Var</td><td>Setting this to 0 will hide all text on the screen.
</td></tr><tr><td>drawcolourscheme</td><td>Var</td><td>Will display or hide the colour key on the top right of the screen
</td></tr></table>
<h2 id="Colours">Colours</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>colourschemenew</td><td>Cmd</td><td>Clears out the current colour scheme.
</td></tr><tr><td>colourschemeadd</td><td>Cmd</td><td>Add a new colour to the colour scheme.
</td></tr></table>
<h2 id="Timers">Timers</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>timeradd</td><td>Cmd</td><td>Adds a timer
</td></tr><tr><td>timerdel</td><td>Cmd</td><td>Removes a timer
</td></tr><tr><td>timerlist</td><td>Cmd</td><td>Lists all timers
</td></tr></table>
<h2 id="Misc">Misc</h2>
<table class="wiki">
<tr><td style="width: 200px"><strong>Command</strong></td><td style="width: 50px"><strong>Type</strong></td><td><strong>Description</strong>
</td></tr><tr><td>popuptext</td><td>Cmd</td><td>Displays some text on the screen.
</td></tr><tr><td>cd</td><td>Cmd</td><td>Change directory. Not apparently useful!
</td></tr></table>

</body>
</html>
