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
Gravit is a gravity simulator which runs under Linux and Windows. It's released under the <a href="http://www.gnu.org/copyleft/gpl.html">GNU General Public License</a> which makes it free.

<h2>Screen Shots</h2>

<?

$from = 1;
$to = 6;
$cols = 3;
$colcount = 0;

echo '<table>';

for ($i = $from; $i <= $to; $i++) {

	$colcount ++;
	if ($colcount > $cols) {
		
		$colcount = 0;
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

<h2>Features</h2>
<li>It looks pretty :)</li>
<li>You can record, then play back at any speed</li>
<li>Stores every particle's position for every frame in memory</li>
<li>Compresses history when needed, for very long recordings</li>
<li>Mouse controllable rotation</li>
<li>Console with script execution</li>
<li>Colours can be based on mass or velocity</li>

<h2>Details</h2>
<li>So far Gravit has been tested on Windows XP and Linux Slackware.</li>
<li>It is written in C and uses SDL, SDL_ttf and OpenGL.</li>
<li>It uses the <a href="http://www.amara.com/papers/nbody.html#tcu">Barnes-Hut Algorithm</a> (with room for optimisation).</li>

<h2>Download</h2>

<b>Note</b>: This version is for the most part un-tested. This means Gravit 0.1 may not even run on your computer! Read the README file included for information.<br><br>

<?

function showFile($file, $desc) {

	echo "<a href=\"dist/$file\">$desc</a> (".number_format(filesize("dist/$file"))." bytes)<br>";

}

function showFiles($ver) {

	showFile("gravit-$ver-win32-sdl.zip", "Gravit $ver for Windows with SDL (Recommended)");
	showFile("gravit-$ver-win32.zip", "Gravit $ver for Windows (If you have SDL already)");
	showFile("gravit-$ver-src.tgz", "Gravit $ver Source Code");
	
}

showFiles("0.1");

?>

<h2>Some Features To Do</h2>
<li>Fix gimbal lock with 3d rotation</li>
<li>Console tab-completition</li>
<li>Scriptable spawn locations</li>
<li>Complete Save/Load functionality</li>
<li>Replace GL_POINT with billboarding</li>
<li>Screenshot/Record Key</li>
<li>Be more user friendly!</li>
</body>

</html>
