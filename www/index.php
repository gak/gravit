<html>

<title>Gravit - A gravity simulator</title>

<style>

body { background-color: black; color: white; font-family: verdana; font-size: 11px; }
.darkertext { color: #A0A0A0 }
a { color: yellow; text-decoration: none; }
a:hover { color: red; text-decoration: underline; }
.darkertext { color: #A0A0A0; }
.normtext { color: #E0E0E0; }

</style>

<body>

<h1>Gravit</h1>

<h2>About</h2>
Gravit is a gravity simulator which runs under Linux and Windows. It's released under the <a href="http://www.gnu.org/copyleft/gpl.html">GNU General Public License</a> which makes it free. It uses simple newtonian physics using the Barnes-Hut N-body algorithm. Although the main goal of Gravit is to be as accurate as possible, it also creates beautiful looking gravity patterns. It records the history of each particle so it can animate and display a path of its travels. At any stage you can rotate your view in 3D and zoom in and out. Gravit uses OpenGL with <a href="http://www.libsdl.org/">SDL</a>, <a href="http://www.libsdl.org/projects/SDL_ttf/">SDL_ttf</a> and <a href="http://www.libsdl.org/projects/SDL_image/">SDL_image</a>.

<h2>Screen Shots</h2>

<?

$from = 1;
$to = 12;
$cols = 4;
$colcount = 0;

echo '<table>';

for ($i = $from; $i <= $to; $i++) {

	$colcount ++;
	if ($colcount > $cols) {
		
		$colcount = 1;
		echo '<tr>';
		
	}

	$fn = sprintf( '%03u', $i);
	
	echo '<td>';
	echo '<a href="screenies/big/' . $fn . '.jpg">';
	echo '<img border="0" width="200" height="160" src="screenies/small/' . $fn . '.jpg">';
	echo '</a>';

}

echo '</table>';

?>

<h2>Demonstration Videos</h2>

<a href="video/gravit-001.avi">Zoomed in on a galaxy showing off v0.2 effects</a> - 9s, 1.1MB, 800x600<br>
<a href="video/gravit-002.avi">Many galaxies showing off v0.2 effects and the frameskip command</a> - 30s, 3.6MB, 1280x1024<br>
<a href="video/gravit-003.avi">Another v0.2 demo, draws an OctTree</a> - 36s, 4.0MB, 800x600<br>
<a href="video/gravit-004.avi">You must get this one!</a> - 22s, 2.5MB, 800x600<br>

<h2>Features</h2>
<li>It looks pretty :)</li>
<li>You can record, then play back at any speed</li>
<li>Stores every particle's position for every frame in memory</li>
<li>Compresses history when needed, for very long recordings</li>
<li>Mouse controllable rotation</li>
<li>Console with script execution</li>
<li>Colours can be based on mass, velocity or acceleration</li>

<h2>Details</h2>
<li>So far Gravit has been tested on Windows XP and Linux Slackware.</li>
<li>It is written in C and uses SDL, SDL_ttf, SDL_image and OpenGL.</li>
<li>It uses the <a href="http://www.amara.com/papers/nbody.html#tcu">Barnes-Hut Algorithm</a> (with room for optimisation).</li>
<li>Gravit was called Graviton, but i changed the name because someone already used Graviton for their project!</li>

<h2>Download</h2>

<!-- If you're unsure of things, get the stable version otherwise the development release shouldn't be too bad!<br> -->
Read the <a href="dist/ChangeLog">ChangeLog</a> for the latest release details.<br>

<?

function showFile($file, $desc) {

	echo "<a href=\"dist/$file\">$file</a> $desc (".number_format(filesize("dist/$file"))." bytes)<br>";

}

function showFiles($ver) {

	if ($ver == "0.1")
		showFile("gravit-$ver-win32-sdl.zip", "Gravit $ver for Windows with SDL and SDL_ttf");
	else
		showFile("gravit-$ver-win32-dll.zip", "Gravit $ver for Windows with SDL, SDL_ttf, SDL_image");
	showFile("gravit-$ver-win32.zip", "Gravit $ver for Windows");
	showFile("gravit-$ver-src.tgz", "Gravit $ver Source Code");
	
}

function showFileHeading($s) {

	echo "<h3>$s</h3>";

}


showFileHeading("Latest Stable Release");
showFiles("0.2");

showFileHeading("Older Releases");
showFiles("0.1");

?>

<h2>Some Features To Do</h2>
<li>Fix gimbal lock with 3d rotation</li>
<li>Better camera controls</li>
<li>Console tab-completition</li>
<li>Complete Save/Load functionality</li>
<li>Be more user friendly!</li>
<li>Create a GUI</li>
<li>Update display/input while rendering</li>
<li>Screenshot to jpg</li>
<li>Scriptable spawn locations</li>
<li>Usage with <a href="http://bima.astro.umd.edu/nemo/">NEMO</a></li>
<li>Create Videos</li>
<li>Tutorial Demonstration</li>
<li>Command line parameters</li>
<li>Multi-threading</li>
<li>Screensaver</li>
</body>

</html>
