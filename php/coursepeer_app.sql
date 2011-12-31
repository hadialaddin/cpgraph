-- CoursePeer Sample Application
-- --------------------------------------------------------
--
-- Table structure for table `cpapp_users`
--

CREATE TABLE IF NOT EXISTS `cpapp_users` (
  `userid` int(11) NOT NULL AUTO_INCREMENT,
  `cpuserid` int(11) NOT NULL,
  `cp_access_token` varchar(500) NOT NULL,
  `name` varchar(500) NOT NULL,
  `usertype` int(11) NOT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=8 ;