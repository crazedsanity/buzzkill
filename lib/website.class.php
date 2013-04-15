<?php
/*
 * FILE INFORMATION: 
 * $HeadURL: https://svn.crazedsanity.com/svn/main/sites/crazedsanity.com/releases/5.3.12/lib/website.class.php $
 * $Id: website.class.php 2042 2012-01-13 04:23:48Z danf $
 * $LastChangedDate: 2012-01-12 22:23:48 -0600 (Thu, 12 Jan 2012) $
 * $LastChangedRevision: 2042 $
 * $LastChangedBy: danf $
 */


class website extends cs_versionAbstract {
	
	private $buildVersionFromSVN = '$LastChangedRevision: 2042 $';
	
	//----------------------------------------------------------------------------
	public function __construct(cs_genericPage $page) {
		$this->set_version_file_location(dirname(__FILE__) . '/../VERSION');
		$gf = new cs_globalFunctions;
		$page->add_template_var('WEBSITE_VERSION', $this->get_version());
	}//end __construct()
	//----------------------------------------------------------------------------
}
