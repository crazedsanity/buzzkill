<?

/*
**	does all of the including of various files; each script should 
**		ONLY have to do require(../lib/includes.php) to work.
*/

require_once("siteConfig.php");

#=====================================================================================================
//all the standard template vars are below.  They're on the top to avoid any problems with them
//	overriding script-specific values.
#=====================================================================================================
//$templateVars["variableName"]="value";
//example: $templateVars["TITLE"]="Avsupport.com -- We like MUFFINS!!!";
//replaces all instances of {TITLE} in the pages with "Avsupport.com -- We like MUFFINS!!!!!"

//set the initial content for the site;

$templateVars["domain"]		= $GLOBALS['DOMAIN'];
$templateVars["PHP_SELF"]	= $_SERVER['SCRIPT_NAME'];
$templateVars["REQUEST_URI"]	= $_SERVER['REQUEST_URI'];
$templateVars["curDate"]	= date("F j, Y");
$templateVars["curYear"]	= date("Y");
$templateVars["curMonth"]	= date("m");
$templateVars["timezone"]	= date("T");

$templateVars["date"]		= date("F j, Y");
$templateVars["time"]		= date("h:i a");
$templateVars["page_section"]	= "Default";
$templateVars["body"]		= "<BODY bgcolor='white' text='#666666' link='#006666' vlink='#006666' alink='#006666' 
bgproperties='fixed'>";

$templateVars["html_title"]	= "::: BuzzKill - Accept No Limitations :::";

$templateVars["error_msg"]	= "";
$templateVars["status_msg"]	= "";

//magic to determine which template to use for content by default
$templateVars['content']	= "content/". ereg_replace("^/", "", str_replace(".php", "", $_SERVER['PHP_SELF'])) ."_content.tmpl";


//templateFiles["sectionName"]="filename";
//example: $templateFiles["contact"]="contact.tmpl";
//would read file contact.html into every place in the master template where it said {contact}

//set the intitial look and meta information for the site

$templateFiles["header"]	= "sections/main_header.tmpl";
$templateFiles["footer"]	= "sections/main_footer.tmpl";


?>
