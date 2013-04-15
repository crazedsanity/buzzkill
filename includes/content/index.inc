<?php
/*
 * Created on May 19, 2006
 *
 */

#// Preference stuff...
#{
#	$showBlogs = 'none';
#	//$showNews = 'inline';
#	if(isset($_SESSION['prefs']['newsAndBlogs'])) {
#		if(!isset($_SESSION['prefs']['newsAndBlogs']['showBlogs'])) {
#			$_SESSION['prefs']['newsAndBlogs']['showBlogs'] = 'inline';
#		}
#		$prefData = $_SESSION['prefs']['newsAndBlogs'];
#		$showBlogs = $prefData['showBlogs'];
#	}
#	else {
#		$_SESSION['prefs']['newsAndBlogs'] = array();
#	}
#	$prefData = $_SESSION['prefs']['newsAndBlogs'];
#	
#	// News will always be the inverse of blogs.
#	$showNews = $showBlogs;
#	$page->gfObj->swapValue($showNews, 'none', 'inline');
#	
#	$page->add_template_var("blogRecentEntries-shown-display", $showBlogs);
#	$page->add_template_var("rssArticles-shown-display", $showNews);
#	
#
#}

// BLOG STUFF
{
	#$page->rip_all_block_rows('content');
	$blog = new csb_blogLocation($page->db, '/blog');
	$page->add_template_var('blog_source_url', "http://www.crazedsanity.com");
	$myData = $blog->get_most_recent_blogs(1);
	
	//re-organize blog data so they show up chronologically...
	$sortByThis = array();
	foreach($myData as $blogName => $blogData)
	{
		$sortByThis[$blogName] = $page->gfObj->cleanString($blogData['post_timestamp'], 'float');
	}
	
	array_multisort($sortByThis, SORT_DESC);
	
	$newData = $myData;
	$myData = array();
	foreach($sortByThis as $blogName=>$garbage)
	{
		$myData[$blogName] = $newData[$blogName];
	}
	
	
	// Define a constant for the blog template file...
	define('CSBLOG_TEMPLATE', 'content/blogEntry.tmpl');
	$indexInfo = "";
	foreach($myData as $blogName => $blogData)
	{
		//blog_parse_single_entry(cs_genericPage $page, array $entryData, $setPageTitle=true, $setDisplayNamePrefix=true, $addDisqus=true)
		$indexInfo .= blog_parse_single_entry($page, $blogData, false, false, true);
	}
	
	$page->add_template_var('blogRecentEntries', $indexInfo);
}

$GLOBALS['rssArticleSources'] = array('slashdot', 'theregister');
if(!isset($_SESSION['rssArticleSources']) || !is_array($_SESSION['rssArticleSources'])) {
	$_SESSION['rssArticleSources'] = array('slashdot', 'theregister');
}

if($fullSectionArr[0] != 'ajax') {
	
	//handle users attempting to add/remove displayed feeds.
	if(isset($_GET['changeFeed'])) {
		$feedChangeResult = page_changeFeed($_GET['changeFeed']);
		if(is_array($feedChangeResult)) {
			$page->set_message_wrapper($feedChangeResult);
		}
		$page->gfObj->conditional_header($_SERVER['SCRIPT_NAME']);
	}
	$_SESSION['rssArticleSources'] = array_unique($_SESSION['rssArticleSources']);
	
	$displayNum = page_setFeedDisplayNum();
	
	$page->add_template_var("blog_list", page_createBlogListing($page));
	#$page->add_template_var('slashdot_article', page_buildRssFeeds($page, $_SESSION['rssArticleSources'], $displayNum));
		#exit(__FILE__ ." -- line #". __LINE__ ." -- more info::: ". $page->gfObj->debug_print($fullSectionArr,0));
}
else {
	#if(!isset($page->templateVars['content'])) {
		#exit(__FILE__ ." -- line #". __LINE__);
		$page->add_template_file('content', 'content/index.content.tmpl');
	#}
}
# */

function page_setFeedDisplayNum($useDisplayNum=null) {
	//Set the number of items to display.
	$displayNum = 20;
	if(isset($_GET['displayNum']) && is_numeric($_GET['displayNum']) && $_GET['displayNum'] > 0) {
		//got it.
		$displayNum = $_GET['displayNum'];
		$_SESSION['rssFeeds']['displayNum'] = $useDisplayNum;
	} elseif(isset($_SESSION['rssFeeds']['displayNum']) && is_numeric($_SESSION['rssFeeds']['displayNum']) && $_SESSION['rssFeeds']['displayNum'] > 0) {
		$displayNum = $_SESSION['rssFeeds']['displayNum'];
	}
	return($displayNum);
}//end page_setFeedDisplayNum()

function page_changeFeed($changeFeed) {

	$index = array_search($_GET['changeFeed'], $_SESSION['rssArticleSources']);
	$retval = true;
	if(!is_numeric($index)) {
		$_SESSION['rssArticleSources'][] = $changeFeed;
	}
	else {
		if(count($_SESSION['rssArticleSources']) > 1) {
			unset($_SESSION['rssArticleSources'][$index]);
		}
		else {
			$retval = array(
				'title'		=> "Cannot Remove Last Source",
				'message'	=> "You must leave at least one source!",
				'type'		=> "notice"
			);
		}
	}
	return($retval);
}//end page_changeFeed()


function page_createBlogListing(cs_genericPage $page) {
	$blog = new csb_blogLocation($page->db, '/blog');
	$recentBlogs = $blog->get_most_recent_blogs();
	
	//loop through the list & get modification times for each item.
	$blogUserRow = $page->set_block_row('content','blog_list');
	$finalBlogTmpl = "";
	if(is_array($recentBlogs)) {
		foreach($recentBlogs as $username => $data) {
			$blogTmplData = $data;
			$extraText = NULL;
			
			$repArr = $data;
			$repArr['user'] = $username;
			$repArr['display_title'] = $page->gfObj->truncate_string($repArr['title'], 35);
			if(strlen($repArr['age_hype'])) {
				$repArr['age_hype'] = " ". $repArr['age_hype'];
			}
			$finalBlogTmpl .= $page->mini_parser($blogUserRow, $repArr, "%%", "%%");
		}
	}
	return($finalBlogTmpl);
}//end page_createBlogListing()

function page_buildRssFeeds(cs_genericPage $page, array $rssArticleSources=null, $useDisplayNum=null) {
	if(is_null($rssArticleSources)) {
		$rssArticleSources = $_SESSION['rssArticleSources'];
	}
	if(is_null($useDisplayNum)) {
		$useDisplayNum = page_setFeedDisplayNum();
	}
	$rssObj = new rssDB(constant('RSS_FILES_DIR'), $page->db);
	$itemsArr = array();
	try {
		$itemsArr = $rssObj->get_most_recent_articles($rssArticleSources, $useDisplayNum);
	}
	catch(Exception $e) {
		$itemsArr = $rssObj->get_most_recent_articles($GLOBALS['rssArticleSources'], $useDisplayNum);
	}
	
	$replaceArr = array_flip(get_html_translation_table(HTML_ENTITIES));
	unset($replaceArr['&nbsp;'], $replaceArr['&shy;']);
	$findArr = array_keys($replaceArr);
	array_unshift($findArr, '&mdash;');
	$replaceArr = array_values($replaceArr);
	array_unshift($replaceArr, '&#151;');
	
	$_SESSION['rssFeeds']['displayNum'] = $useDisplayNum;
	$articleRow = $page->set_block_row("content", "slashdot_article");
	
	$allArticles = "";
	if(is_array($itemsArr)) {
		
		$authorDataSepArr = array
		( 
			"/ writes to mention[\W]/"	=> " wr<x>ites to mention",//TODO: fix so the "<x>" doesn't need to be added to make the second regex NOT pick it up.
			"/ writes[\W]/"				=> " writes ",
		);
		$articleDataSepArr = array(
			"/ From the article[\W]/"	=> " From the article:",
			"/ From that post[\W]/"		=> " From that post:"
		);
		
		#for($x=0; $x < $useDisplayNum; $x++) {
		foreach($itemsArr as $x=>$array) {
			
			$allArticles .= $page->mini_parser($articleRow, $array, '%%', '%%');
		}
	}
	if(defined('AJAX_CALL')) {
		$allArticles = microtime(true) . '<BR />'. $allArticles;
		$outerContainer = $page->set_block_row('content', 'rssArticles');
		$allArticles = $page->mini_parser($outerContainer, array('slashdot_article'=>$allArticles), '{', '}');
	}
	if(!strlen($allArticles)) {
		$allArticles = "Function is buggered, page=(". get_class($page) ."), useDisplayNum=(". $useDisplayNum ."), "
			. "count(rssArticleSources)=(". count($rssArticleSources) ."), count(itemsArr)=(". count($itemsArr) ."), "
			. "is_null(allArticles)=(". is_null($allArticles) ."), strlen(allArticles)=(". strlen($allArticles) .") "
			. "debug print of array::: ". $page->gfObj->debug_print($array,0);
	}
	return($allArticles);
}//end page_buildRssFeeds()
?>
