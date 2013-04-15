<?php


$page->rip_all_block_rows('content');
$blog = new csb_blogLocation('/blog');
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


$indexInfo = "";
foreach($myData as $blogName => $blogData)
{
	//blog_parse_single_entry(cs_genericPage $page, array $entryData, $setPageTitle=true, $setDisplayNamePrefix=true, $addDisqus=true)
	$indexInfo .= blog_parse_single_entry($page, $blogData, false, false, false);
}

$page->add_template_var('content', $indexInfo);
