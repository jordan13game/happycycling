INCLUDES= -I$(top_srcdir)/lib/ys_log 				\
		  -I$(top_srcdir)/lib/heart_beat 			\
		  -I$(top_srcdir)/lib/mysql 				\
		  -I$(top_srcdir)/lib/db_service 			\
		  -I$(top_srcdir)/lib/perf 				\
		  -I$(top_srcdir)/lib/utility 			\
		  -I$(top_srcdir)/lib/rpc 				\
		  -I$(top_srcdir)/lib/thread_pool 		\
		  -I$(top_srcdir)/lib/package 			\
		  -I$(top_srcdir)/lib/ys_lua/include  	\
		  -I$(top_srcdir)/lib/ys_lua/lua-5.1.4 	\
		  -I$(top_srcdir)/lib/jpack 			\
		  -I../common/

CXXFLAGS = -Wall -g -O2 -fPIC -Wno-deprecated -std=c++0x -pipe -m64 -DDEBUG_LOG
LDFLAGS  = 

#LD_PRELOAD=/usr/lib/libtcmalloc_minimal4.so

gateway_LDADD =\
				../common/libcommon.a\
				$(top_srcdir)/lib/jpack/libys_jpack.a    \
				$(top_srcdir)/lib/rpc/libys_rpc.a            \
				$(top_srcdir)/lib/thread_pool/libys_thread_pool.a            \
				$(top_srcdir)/lib/ys_lua/libys_lua.a     	\
				$(top_srcdir)/lib/perf/libys_perf.a         \
				$(top_srcdir)/lib/db_service/libys_db_service.a \
				$(top_srcdir)/lib/mysql/libys_mysql.a        \
				$(top_srcdir)/lib/utility/libys_utility.a    \
				$(top_srcdir)/lib/ys_log/libys_log.a         \
				/usr/local/lib/libboost_system-mt.a\
				/usr/local/lib/libboost_thread-mt.a\
				-lreadline\
				-lcurl\
				-lpthread\
				-ltbb\
				-lmysqlclient\
				-ltbbmalloc\
				-lz\
				-ldl 

bin_PROGRAMS = gateway

noinst_PROGRAMS = 
noinst_LIBRARIES = 

gateway_SOURCES = 		\
		gw_public_service.cpp \
		gw_chat.cpp \
		gw_session.cpp \
		gw_team.cpp \
		gw_user.cpp \
		main.cpp
