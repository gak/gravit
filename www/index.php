<? header("Content-type: text/html; charset=iso-8859-1"); ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">

<html>

<head>

<title>Gravit - A gravity simulator</title>

<style type="text/css">

body { background-color: black; color: white; font-family: verdana; font-size: 11px; }
.darkertext { color: #A0A0A0 }
a { color: yellow; text-decoration: none; }
a:hover { color: red; text-decoration: underline; }
.darkertext { color: #A0A0A0; }
.normtext { color: #E0E0E0; }

img { border: none; }

</style>

<link rel="shortcut icon" href="favicon.ico">
<meta name="resource-type" content="document">
<meta name="description" content="Gravit is a gravity simulator that uses OpenGL and SDL">
<meta name="keywords" content="Gravit, Graviton, gravity, nbody, N-body, N body, simulate, simulation, simulator, octree, octtree, OpenGL, SDL, GL_ARB_point_parameters, GL_ARB_point_sprite">
</head>

<body>

<h1>Gravit</h1>

<h2>About</h2>
<p>Gravit is a gravity simulator which runs under Linux and Windows. It's released under the <a href="http://www.gnu.org/copyleft/gpl.html">GNU General Public License</a> which makes it free. It uses simple newtonian physics using the Barnes-Hut N-body algorithm. Although the main goal of Gravit is to be as accurate as possible, it also creates beautiful looking gravity patterns. It records the history of each particle so it can animate and display a path of its travels. At any stage you can rotate your view in 3D and zoom in and out. Gravit uses OpenGL with <a href="http://www.libsdl.org/">SDL</a>, <a href="http://www.libsdl.org/projects/SDL_ttf/">SDL_ttf</a> and <a href="http://www.libsdl.org/projects/SDL_image/">SDL_image</a>.</p>

<h2>Screen Shots</h2>

<?

$from = 1;
$to = 12;
$cols = 4;
$colcount = 0;

echo '<table>';
echo '<tr>';

for ($i = $from; $i <= $to; $i++) {

	$colcount ++;
	if ($colcount > $cols) {
		
		$colcount = 1;
		echo '<tr>';
		
	}

	$fn = sprintf( '%03u', $i);
	
	echo '<td>';
	echo '<a href="screenies/big/' . $fn . '.jpg">';
	echo '<img width="200" height="160" src="screenies/small/' . $fn . '.jpg" alt="Gravit Screenshot">';
	echo '</a>';

}

echo '</table>';

?>

<h2>Demonstration Videos</h2>
<p>Sorry, I can't afford the bandwidth!</p>

<h2>Features</h2>
<ul>
<li>It looks pretty :)</li>
<li>View the simulation in 3D using <a href="http://www.angelfire.com/ca/erker/freeview.html">stereoscopic imaging</a>!</li>
<li>Can be installed as a screen saver in Windows</li>
<li>You can record, then play back at any speed</li>
<li>Stores every particle's position for every frame in memory</li>
<li>Compresses history when needed, for very long recordings</li>
<li>Mouse controllable rotation</li>
<li>Console with script execution</li>
<li>Colours can be based on mass, velocity or acceleration</li>
<li>See an octtree being created in real-time</li>
<li>Save/Load functionality</li>
<li>Update display/input while recording a frame</li>
</ul>

<h2>Details</h2>
<ul>
<li>It is written in C and uses SDL, SDL_ttf, SDL_image and OpenGL.</li>
<li>It uses the <a href="http://www.amara.com/papers/nbody.html#tcu">Barnes-Hut Algorithm</a> (with room for optimisation).</li>
<li>Gravit was called Graviton, but i changed the name because someone already used Graviton for their project!</li>
</ul>

<h2>Download</h2>
<p>Gravit 0.3.0 now can be seen in 3D (without glasses) using <a href="http://www.angelfire.com/ca/erker/freeview.html">stereoscopic imaging</a>! Windows users now have a Windows installer and you can install Gravit as a screen saver. Also several new commands including stereo, stereoseparation, installscreensaver (win32), g, saveauto, autorotate, zoom, showcursor. Fixed a few bugs with mouse input.</p>
<p>Read the <a href="dist/ChangeLog">ChangeLog</a> for the latest release details.</p>
<?

function showFile($file, $desc) {

	echo "<li><a href=\"dist/$file\">$file</a> $desc (".number_format(filesize("dist/$file"))." bytes)</li>";

}

function showFiles($ver) {

	echo '<ul>';

	if ($ver == "0.1") {
		showFile("gravit-$ver-win32-sdl.zip", "Gravit $ver for Windows with SDL and SDL_ttf");
	} else if ($ver == "0.2" || $ver == "0.2.1" || $ver == "0.2.2") {
		showFile("gravit-$ver-win32-dll.zip", "Gravit $ver for Windows with SDL, SDL_ttf, SDL_image");
		showFile("gravit-$ver-win32.zip", "Gravit $ver for Windows");
		showFile("gravit-$ver-src.tgz", "Gravit $ver Source Code");
	} else {
		showFile("gravit-$ver-win32-installer.exe", "Gravit $ver Windows installer");
		showFile("gravit-$ver-win32.zip", "Gravit $ver for Windows");
		showFile("gravit-$ver-src.tgz", "Gravit $ver Source Code");
	}
	
	echo '</ul>';
	
}

function showFileHeading($s) {

	echo "<h3>$s</h3>";

}


showFileHeading("Latest Release");
showFiles("0.3.0");

showFileHeading("Older Releases");
showFiles("0.2.2");
showFiles("0.2.1");
showFiles("0.2");
showFiles("0.1");

?>

<h2>Some Features To Do</h2>
<ul>
<li>Fix gimbal lock with 3d rotation</li>
<li>Better camera controls</li>
<li>Console tab-completition</li>
<li>Be more user friendly!</li>
<li>Create a GUI</li>
<li>Screenshot to jpg</li>
<li>Scriptable spawn locations</li>
<li>Usage with <a href="http://bima.astro.umd.edu/nemo/">NEMO</a></li>
<li>Create Videos</li>
<li>Tutorial Demonstration</li>
<li>Multi-threading</li>
<li>Screensaver</li>
</ul>

</body>

</html>
