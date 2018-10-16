CREATE TABLE `sense_data` (
	  `ID` int(8) NOT NULL AUTO_INCREMENT,
	  `plant` varchar(8) DEFAULT NULL,
	  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
	  `moisture` varchar(8) DEFAULT NULL,
	  `temp` varchar(8) DEFAULT NULL,
	  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8mb4
