<?php
$timings = array(
	'before'	=> time(),
	'after'		=> null
);


//store a database object in the page object so it is easy to get to.
$page->db = new cs_phpDB(constant('DB_DSN'), constant('DB_USERNAME'), constant('DB_PASSWORD'));

$page->config = $siteConfig;




function blog_display_entries(cs_genericPage $page, $blogName, $numEntries=null, $offset=null) {
	
	// LAST (X) ENTRIES FOR A BLOG
	if(!is_numeric($numEntries) || $numEntries < 1) {
		$numEntries = 5;
	}
	if(!is_numeric($offset) || $offset < 0) {
		$offset = 0;
	}
	$page->allow_invalid_urls(TRUE);
	$blog = new csb_blog($blogName);
	$myData = $blog->get_recent_blogs($numEntries, $offset);
	
	$addDisqus = false;
	if(count($myData) == 1) {
		$addDisqus = true;
	}
	foreach($myData as $entryId=>$entryData) {
		$myParsedRow .= blog_parse_single_entry($page, $entryData, true, true, $addDisqus);
		$myParsedRow .= "\n<HR><!-- END OF entry... -->";
	}
	$doNotRipRows = array();
	$permObj = new csb_permission();
	if($permObj->can_access_blog($blogName, $_SESSION['auth']['userInfo']['username'])) {
		$doNotRipRows[] = 'editLink';
	}
	
	$mainBlogData = $blog->get_blog_data_by_name($blogName);
	$page->add_template_var('html_title', $mainBlogData['blog_display_name'] .' -- Last '. $numEntries .' Entries [CrazedSanity.com]');
	$page->add_template_var('blogDisplayName', $mainBlogData['blog_display_name']);
	$page->add_template_var('blogName', $mainBlogData['blog_name']);
	$page->add_template_var('blogLocation', $mainBlogData['location'] .'/'. $mainBlogData['blog_name']);
	
	
	$page->add_template_var('content', $myParsedRow);
	$page->rip_all_block_rows('content', $doNotRipRows);
}//end blog_display_entries()


function blog_parse_single_entry(cs_genericPage $page, array $entryData, $setPageTitle=true, $setDisplayNamePrefix=true, $addDisqus=true) {
	
	$page->add_template_file('_singleEntry', '/content/blogEntry.tmpl');
#$page->gfObj->debug_print($page->templateVars['_singleEntry'],1);
#exit;
	$entryRow = $page->templateVars['_singleEntry'];
	foreach($entryData as $k=>$v) {
		if(!preg_match('/^blog_/', $k)) {
			$entryData['blog_'. $k] = $v;
			unset($entryData[$k]);
		}
	}
	
	$timeBits = explode(' ', $entryData['blog_formatted_post_timestamp']);
	$entryData['blog_formatted_post_timestamp_time'] = $timeBits[4] ." ". $timeBits[5];
	
	if(!$addDisqus) {
		$entryData['disqus'] = '';
	}
	$retval = $page->mini_parser($entryRow, $entryData, '{', '}');
	
	//add some stuff for the add_blog_submenu().
	$page->add_template_var('blogLocation', $entryData['blog_location'] .'/'. $entryData['blog_name']);
	$page->add_template_var('blogDisplayName', $entryData['blog_display_name']);
	
	return($retval);
}//end blog_parse_single_entry();


function add_blog_submenu(cs_genericPage $page, $useLink=true) {
	
	if($useLink) {
		$submenu = $page->templateRows['blockrow_subMenu'];
	}
	else {
		$submenu = $page->templateRows['blockrow_subMenuNoLink'];
	}
	$subMenuRepArr = array(
		'blockrow_subMenu_url'		=> $page->templateVars['blogLocation'],
		'blockrow_subMenu_display'	=> $page->templateVars['blogDisplayName']
	);
	$submenu = $page->mini_parser($submenu, $subMenuRepArr, '{', '}');
	$page->add_template_var('subMenu_blog', $submenu);
}//end add_blog_submenu();

?>
