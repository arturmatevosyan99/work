TEMPLATE = subdirs

!defined(SUBDIRS, var) {

if (ios | android) {
#	SUBDIRS += Enigma.pro # Reserved
	SUBDIRS += Regina.pro # Smartphone CAP Frontend (General Legacy Edition)
	SUBDIRS += Vanilla.pro # Smartphone CAP Frontend (General Edition)
#	SUBDIRS += Fantasia.pro # Reserved
	SUBDIRS += Eureka.pro # Smartphone CAP Frontend ([Mental|Fit]Gami Edition)
	SUBDIRS += Afrodita.pro # Smartphone CAP Frontend (DrOblozhko Legacy Edition)
#	SUBDIRS += Angelina.pro # Smartphone CAP Frontend (MetabolicPlate Edition)
	SUBDIRS += Ladybug.pro # Smartphone CAP Frontend (Test Cadabra Platform, CLI)
}

if (win32 | unix) {
#	SUBDIRS += Ladybug.pro # Desktop CAP Frontend (Test Cadabra Platform, CLI)
#	SUBDIRS += Vanilla.pro # Desktop CAP Frontend (General Edition, GUI, CLI, GraphQL?)
}

}
