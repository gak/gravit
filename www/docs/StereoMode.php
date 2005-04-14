<? header("Content-type: text/html; charset=iso-8859-1"); ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>Gravit Documentation - StereoMode</title>
<style type="text/css">
@import url(../gravit.css);
</style>
</head>
<body>
<h1>Stereoscopic Mode</h1>
<p>
<a class="ext-link" title="http:.." href="http:..">Gravit Home</a> / <a href="Index.php">Documentation Index</a>
</p>
<h2>Overview</h2>
<p>
Stereoscopy is a way to see a 3D image on a 2D screen. It is similar to the way the <a class="ext-link" title="http://www.magiceye.com/" href="http://www.magiceye.com/">Magic Eye</a> books do it.
</p>
<p>
There are two ways Gravit can display stereoscopic images. They are cross-eyed and parallel viewing. I'm not an expert in stereoscopy, so please don't take my word for this -- all I know is that it works!
</p>
<p>
To get cross-eyed viewing set up in Gravit, you'll need to get into the console using the ` key and typing in the following:
</p>
<pre class="wiki"> stereo 1
 stereoseparation -100
</pre><p>
The <i>stereoseparation</i> value is adjustable, the more negative this number is, the more 'depth' you will see.
</p>
<p>
To get parallel viewing going, the <i>stereoseparation</i> variable is positive:
</p>
<pre class="wiki"> stereo 1
 stereoseparation 100
</pre><p>
Increasing <i>stereoseparation</i> to a larger number will increse depth.
</p>
<p>
For more information on how to get your eyes to do all this, check out the following links:
</p>
<ul><li><a class="ext-link" title="http://www.angelfire.com/ca/erker/freeview.html" href="http://www.angelfire.com/ca/erker/freeview.html">http://www.angelfire.com/ca/erker/freeview.html</a>
</li><li><a class="ext-link" title="http://en.wikipedia.org/wiki/Stereoscopy" href="http://en.wikipedia.org/wiki/Stereoscopy">http://en.wikipedia.org/wiki/Stereoscopy</a>
</li></ul><h2>Example Images</h2>
<p>
Below are two examples of each method. The larger orange coloured galaxy should be infront. 
</p>
<h3>Cross-eyed</h3>
<div style="text-align: center"><img alt="Cross-eyed viewing" src="images/stereo-cross-eyed.jpg"></div>
<h3>Parallel</h3>
<div style="text-align: center"><img alt="Parallel viewing" src="images/stereo-parallel.jpg"></div>

</body>
</html>
