FIND_PACKAGE( PkgConfig REQUIRED )
FIND_PATH( DBUS_INCLUDE_DIR dbus/dbus.h PATHS /usr/include/dbus-1.0 /usr/local/include/dbus-1.0 )
FIND_PATH( DBUS_INCLUDE_LIB_DIR dbus/dbus-arch-deps.h PATHS /usr/lib/dbus-1.0/include /usr/local/lib/dbus-1.0/include /usr/lib/x86_64-linux-gnu/dbus-1.0/include)
FIND_PATH( DBUS_GLIB_INCLUDE_DIR dbus/dbus-glib.h PATHS /usr/include/dbus-1.0 /usr/local/include/dbus-1.0 )
FIND_LIBRARY( DBUS_LIBRARY NAME dbus-1 PATHS /usr/lib /usr/local/lib )
FIND_LIBRARY( DBUS_GLIB_LIBRARY NAME dbus-glib-1 PATHS /usr/lib /usr/local/lib )
FIND_PROGRAM( GLIB_DBUS_BINDING_TOOL NAME dbus-binding-tool PATHS /usr/bin /usr/local/bin )

IF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR AND DBUS_LIBRARY )
   SET( DBUS_FOUND TRUE )
ENDIF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR AND DBUS_LIBRARY )

IF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR )
   SET( DBUS_INCLUDES ${DBUS_INCLUDE_DIR} ${DBUS_INCLUDE_LIB_DIR} )
ENDIF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR )

IF( DBUS_GLIB_INCLUDE_DIR AND DBUS_GLIB_LIBRARY )
   SET( DBUS_GLIB_FOUND TRUE )
ENDIF( DBUS_GLIB_INCLUDE_DIR AND DBUS_GLIB_LIBRARY )

IF( DBUS_GLIB_FOUND )
   IF( NOT dbus-glib_FIND_QUIETLY )
      MESSAGE( STATUS "Found dbus-glib: ${DBUS_GLIB_LIBRARY}" )
   ENDIF( NOT dbus-glib_FIND_QUIETLY )
ELSE( DBUS_GLIB__FOUND )
   IF( dbus-glib_FIND_REQUIRED )
      MESSAGE( FATAL_ERROR "Could not find dbus-glib" )
   ENDIF( dbus-glib_FIND_REQUIRED )
ENDIF( DBUS_GLIB_FOUND )

IF( DBUS_FOUND )
   IF( NOT dbus_FIND_QUIETLY )
      MESSAGE( STATUS "Found dbus: ${DBUS_LIBRARY}" )
   ENDIF( NOT dbus_FIND_QUIETLY )
ELSE( DBUS_FOUND )
   IF( dbus_FIND_REQUIRED )
      MESSAGE( FATAL_ERROR "Could not find dbus" )
   ENDIF( dbus_FIND_REQUIRED )
ENDIF( DBUS_FOUND )

IF( GLIB_DBUS_BINDING_TOOL )
	SET( GLIB_BIND_XML_SERVER "YES" )
	SET( GLIB_BIND_XML_CLIENT "YES" )
ENDIF( GLIB_DBUS_BINDING_TOOL )

MACRO( GLIB_BIND_XML_SERVER outfiles prefix )
	FOREACH( it ${ARGN} )
		GET_FILENAME_COMPONENT( outfile ${it} NAME_WE )
		GET_FILENAME_COMPONENT( in ${it} ABSOLUTE )
		SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}glue.h )
		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND ${GLIB_DBUS_BINDING_TOOL}
			ARGS --mode=glib-server --prefix=${prefix} --output=${outfile} ${in}
			DEPENDS ${infile} )
		SET( ${outfiles} "${${outfiles}}" ${outfile} )
	ENDFOREACH( it )
ENDMACRO( GLIB_BIND_XML_SERVER )

MACRO( GLIB_BIND_XML_CLIENT outfiles prefix )
	FOREACH( it ${ARGN} )
		GET_FILENAME_COMPONENT( outfile ${it} NAME_WE )
		GET_FILENAME_COMPONENT( in ${it} ABSOLUTE )
		SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}glue.h )
		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND ${GLIB_DBUS_BINDING_TOOL}
			ARGS --mode=glib-client --prefix=${prefix} --output=${outfile} ${in}
			DEPENDS ${infile} )
		SET( ${outfiles} "${${outfiles}}" ${outfile} )
	ENDFOREACH( it )
ENDMACRO( GLIB_BIND_XML_CLIENT )
