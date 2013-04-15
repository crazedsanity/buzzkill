<?php

/*
**	does all of the including of various files; each script should 
**		ONLY have to do require(../lib/includes.php) to work.
*/

//Some things to make __autoload() go super fast...
define('AUTOLOAD_HINTS', dirname(__FILE__) .'/class.hints');
define('LIBDIR', dirname(__FILE__));

### Set the default timezone...
date_default_timezone_set("America/Chicago");


//set the domain we're on into the session...
if(isset($_SERVER['HTTP_HOST'])) {
	$temp=explode(".",$_SERVER['HTTP_HOST']);
	$GLOBALS['DOMAIN']=$temp[1];
}


require_once(dirname(__FILE__) .'/cs-content/__autoload.php');
$configFile = dirname(__FILE__) .'/../rw/siteConfig.xml';
if(file_exists(dirname(__FILE__) .'/../rw/test_siteConfig.xml')) {
	$configFile = dirname(__FILE__) .'/../rw/test_siteConfig.xml';
}
$siteConfig = new cs_siteConfig($configFile, 'website');


function exception_handler($exception) {
	if(isset($_SERVER['SERVER_PROTOCOL'])) {
		$showThis = file_get_contents(dirname(__FILE__) .'/../templates/system/404.shared.tmpl');
		$showThis = str_replace('{details}', $exception->getMessage(), $showThis);
	}
	else {
		$showThis = $exception->getMessage();
	}
	print $showThis;
	exit;
}

set_exception_handler('exception_handler');


function cs_debug_backtrace($printItForMe=NULL,$removeHR=NULL) {
	$gf = new cs_globalFunctions;
	if(is_null($printItForMe)) {
		if(defined('DEBUGPRINTOPT')) {
			$printItForMe = constant('DEBUGPRINTOPT');
		}
		elseif(isset($GLOBALS['DEBUGPRINTOPT'])) {
			$printItForMe = $GLOBALS['DEBUGPRINTOPT'];
		}
	}
	if(is_null($removeHR)) {
		if(defined('DEBUGREMOVEHR')) {
			$removeHR = constant('DEBUGREMOVEHR');
		}
		elseif(isset($GLOBALS['DEBUGREMOVEHR'])) {
			$removeHR = $GLOBALS['DEBUGREMOVEHR'];
		}
	}
//	if(function_exists("debug_print_backtrace")) {
//		//it's PHP5.  use output buffering to capture the data.
//		ob_start();
//		debug_print_backtrace();
//		
//		$myData = ob_get_contents();
//		ob_end_clean();
//	}
//	else {
		//create our own backtrace data.
		$stuff = debug_backtrace();
		if(is_array($stuff)) {
			$i=0;
			foreach($stuff as $num=>$arr) {
				if($arr['function'] !== "debug_print_backtrace") {
					
					$fromClass = '';
					if(isset($arr['class']) && strlen($arr['class'])) {
						$fromClass = $arr['class'] .'::';
					}
					
					$args = '';
					foreach($arr['args'] as $argData) {
						$args = $gf->create_list($args, $gf->truncate_string($gf->debug_print($argData, 0, 1, false), 600), ', ');
					}
					
					$fileDebug = "";
					if(isset($arr['file'])) {
						$fileDebug = " from file <u>". $arr['file'] ."</u>, line #". $arr['line'];
					}
					$tempArr[$num] = $fromClass . $arr['function'] .'('. $args .')'. $fileDebug;
					
				}
			}
			
			array_reverse($tempArr);
			$myData = null;
			foreach($tempArr as $num=>$func) {
				$myData = $gf->create_list($myData, "#". $i ." ". $func, "\n");
				$i++;
			}
		}
		else {
			//nothing available...
			$myData = $stuff;
		}
//	}
	
	$backTraceData = $gf->debug_print($myData, $printItForMe, $removeHR);
	return($backTraceData);
}//end cs_debug_backtrace()

function cs_get_where_called() {
	$stuff = debug_backtrace();
//	foreach($stuff as $num=>$arr) {
//		if($arr['function'] != __FUNCTION__ && $arr['function'] != 'debug_backtrace' && (!is_null($fromMethod) && $arr['function'] != $fromMethod)) {
//			#$retval = $arr['function'];
//			$fromClass = $arr['class'];
//			if(!$fromClass) {
//				$fromClass = '**GLOBAL**';
//			}
//			$retval = $arr['function'] .'{'. $fromClass .'}';
//			break;
//		} 
//	}
	$myData = $stuff[2];
	$fromClass = $myData['class'];
	if(!$fromClass) {
		$fromClass = '**GLOBAL**';
	}
	$retval = $fromClass .'::'. $myData['function'];
	return($retval);
}


?>
