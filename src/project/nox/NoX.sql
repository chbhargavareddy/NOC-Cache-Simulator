-- MySQL dump 10.9
--
-- Host: localhost    Database: NoX 
-- ------------------------------------------------------
-- Server version	4.1.10-nt

--
-- Table structure for table `flit`
--

DROP TABLE IF EXISTS `flit`;
CREATE TABLE `flit` (
  `FNUM` int(10) NOT NULL default '0',
  `SRC` int(3) default NULL,
  `DEST` int(3) default NULL,
  `FTYPE` int(1) default NULL,
  PRIMARY KEY  (`FNUM`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `flow`
--

DROP TABLE IF EXISTS `flow`;
CREATE TABLE `flow` (
  `NUM` int(10) NOT NULL auto_increment,
  `FNUM` int(10) default NULL,
  `NODE` int(3) default NULL,
  `PC` int(2) default NULL,
  `VC` int(2) default NULL,
  `STAGE` int(3) default NULL,
  `CLOCK` int(10) default NULL,
  PRIMARY KEY  (`NUM`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

