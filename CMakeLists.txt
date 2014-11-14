#Specify the version being used aswell as the language
cmake_minimum_required( VERSION 3.0 )
#Name your project here
project( AllProjects CXX )

option( WITH_SAMPLES "Add samples cmake files." OFF)

if ( WITH_SAMPLES )
    # include( Sample_01.cmake )
endif( WITH_SAMPLES )

include( PH_System.cmake )
include( PH_Register.cmake )
include( PH_Base.cmake ) # the name of solution is the same of the last included file.