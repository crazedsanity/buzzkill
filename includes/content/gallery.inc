<?

//see if there's a third section (i.e. if the uri is "content/gallery/ubk-smokey_skull.jpg", it's "ubk-smokey_skull.jpg.

$temp = explode('/', $_SERVER['REQUEST_URI']);
$pic = $temp[2];
$fileLoc = $GLOBALS['SITE_ROOT'] ."/public_html/images/gallery/". $pic;
if((strlen($pic) > 5) AND (file_exists($fileLoc))) {
	//looks like the pic exists...
	$page->change_content('<img src="/images/gallery/'. $pic .'" border="0">');
}
?>
