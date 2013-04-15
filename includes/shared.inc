<?
//--------------------------------------------------
new website($page);
//set the left & right selected icon thingies...
$page->add_template_var("lsel_". $finalSection, "&raquo;");
$page->add_template_var("rsel_". $finalSection, "&laquo;");
$page->add_template_var("finalSection", $finalSection);
//--------------------------------------------------
?>
