<?

$i = 0;
while (1) {

	$i++;
	$fn = sprintf( '%03u', $i);
	$fbig = "screenies/big/$fn.jpg";
	$fsmall = "screenies/small/$fn.jpg";
	
	if (!file_exists($fbig))
		break;

	if (file_exists($fsmall))
		continue;

	$cmd = "convert -scale 200x160 $fbig $fsmall";
	echo $cmd . "\n";
	exec($cmd);

}

?>