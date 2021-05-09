TEMPLATE = app

#Executable Name
TARGET = plane
CONFIG = debug

#Destination
DESTDIR = .
OBJECTS_DIR = ./build/

HEADERS	+= 	../common/Shader.h	    		\	
		../common/Vector.h		        \	
		../common/Matrix.h		        \
		../common/Mesh.h		        \
        	../common/Texture.h             	\		
        	../common/Camera.h  			\
        	../common/Model.h  			\
		../common/Player.h  			\
		../common/Skybox.h  			\
		../common/HUD.h  			\
		../common/Map.h  			\
		../common/ShaderClass.h  		\
		../common/Hitbox.h  			\

			

#Sources
SOURCES += 	main.cpp			        \
  		../common/Shader.cpp		    	\
		../common/Vector.cpp		   	\
		../common/Matrix.cpp		    	\
		../common/Mesh.cpp		        \
        	../common/Texture.cpp           	\
        	../common/Camera.cpp 			\
        	../common/Model.cpp			\  
		../common/Player.cpp			\  
		../common/Skybox.cpp  			\
		../common/HUD.cpp  			\
		../common/Map.cpp  			\
		../common/ShaderClass.cpp  		\
		../common/Hitbox.cpp 			\


INCLUDEPATH += 	./ 				    	\
		../common/ 				\

		
#Library Libraries
LIBS +=	-lGLEW			    	    	        \
	-lglut			        		\
        -lGL             	                  	\  
