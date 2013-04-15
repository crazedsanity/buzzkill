<?php


require_once(dirname(__FILE__) .'/cs-content/__autoload.php');

$gf = new cs_globalFunctions;
$siteConfig = new cs_siteConfig(dirname(__FILE__) .'/../rw/siteConfig.xml', 'website');




//set the domain we're on into the session...
$temp=explode(".",$_SERVER['HTTP_HOST']);
$GLOBALS['DOMAIN']=$temp[1];


?>
