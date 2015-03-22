INCLUDES= -I$(top_srcdir)/lib/ys_log 				\
		  -I$(top_srcdir)/lib/rpc 				\
		  -I$(top_srcdir)/lib/perf 				\
		  -I$(top_srcdir)/lib/jpack 			\
		  -I$(top_srcdir)/lib/ys_lua/include  	\
		  -I$(top_srcdir)/lib/ys_lua/lua-5.1.4 	\
		  -I$(top_srcdir)/lib/mysql 				\
		  -I$(top_srcdir)/lib/db_service\
		  -I$(top_srcdir)/lib/utility	

AM_CPPFLAGS = -Wall -O2  -fPIC -Wno-deprecated -std=c++0x -DDEBUG_LOG -m64 -pipe  

noinst_LIBRARIES = libcommon.a

libcommon_a_SOURCES = \
		config.cpp\
		db_helper.cpp\
		msg_dispatcher.cpp\
		db_def.cpp

